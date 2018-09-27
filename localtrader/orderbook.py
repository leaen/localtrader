import time

from .order import Order, OrderStatus
from .side import Side
from .trade import Trade

class NoBidsException(Exception):
    pass

class NoOffersException(Exception):
    pass

class OrderNotFoundException(Exception):
    pass

class Orderbook:
    def __init__(self, instrument):
        self.instrument = instrument

        self.orders = {}

        self.buy_orders = []
        self.sell_orders = []

        self.trades = []

    def submit_order(self, order):
        """
        Submits an order into orderbook to be processed.

        Returns a list of trades that occurred.
        """

        assert isinstance(order, Order), \
            'order must be of type Order instead have {}'.format(type(order))

        # Add order into the order pool
        self.orders[order.order_id] = order

        # Add into appropriate limit book
        if order.side == Side.BUY:
            self.insert_buy(order)
        elif order.side == Side.SELL:
            self.insert_sell(order)

        executed_trades = self.match_orders(order.side)

        return executed_trades

    def match_orders(self, action_side):
        executed_trades = []

        while self.is_matched():
            matched_buy = self.get_best_buy()
            matched_sell = self.get_best_sell()

            # Execute the trade
            if matched_buy.size >= matched_sell.size:
                size = matched_sell.size
            else:
                size = matched_buy.size

            if action_side == Side.BUY:
                side = Side.BUY
                taker = matched_buy.client_id
                maker = matched_sell.client_id
            elif action_side == Side.SELL:
                side = Side.SELL
                taker = matched_sell.client_id
                maker = matched_buy.client_id

            price = matched_buy.price
            execution_time = time.time()

            t = Trade(self.instrument, price, size, side,
                      maker, taker, execution_time)

            self.trades.append(t)
            executed_trades.append(t)

            matched_buy.fill(size)
            matched_sell.fill(size)

            if matched_buy.status == OrderStatus.FILLED:
                self.remove_buy(matched_buy)

            if matched_sell.status == OrderStatus.FILLED:
                self.remove_sell(matched_sell)

        return executed_trades

    def insert_buy(self, order):
        """
        Inserts an order into the buy book.
        """

        self.buy_orders.append(order)

    def insert_sell(self, order):
        """
        Inserts an order into the sell book.
        """

        self.sell_orders.append(order)

    def remove_buy(self, order):
        """
        Removes an order from the buy book.
        """

        self.buy_orders.remove(order)

    def remove_sell(self, order):
        """
        Removes an order from the sell book.
        """

        self.sell_orders.remove(order)

    def cancel_order(self, order_id):
        """
        Takes the order_id of an order in the book and cancels it.

        Raises OrderNotFoundException if the order was not found in the book.
        """
        o = self.orders[order_id]
        o.cancel()

        book = self.buy_orders if o.side == Side.BUY else self.sell_orders

        try:
            book.remove(o)
        except ValueError:
            raise OrderNotFoundException

    def get_order_status(self, order_id):
        """
        Gets the current status of an order by order id.
        """

        return self.orders[order_id].status

    def get_best_buy(self):
        """
        Returns the current best buy order.

        Raises NoBidsException if there are no bids in the book.
        """

        best_buy = None
        for buy_order in self.buy_orders:
            if best_buy is None:
                best_buy = buy_order
            elif buy_order < best_buy:
                best_buy = buy_order

        if best_buy is None:
            raise NoBidsException()

        return best_buy

    def get_best_sell(self):
        """
        Returns the current best sell order.

        Raises NoOffersException if there are no offers in the book.
        """

        best_sell = None
        for sell_order in self.sell_orders:
            if best_sell is None:
                best_sell = sell_order
            elif sell_order < best_sell:
                best_sell = sell_order

        if best_sell is None:
            raise NoOffersException()

        return best_sell

    def best_bid(self):
        """
        Returns the current best bid.

        Raises NoBidsException if there are no bids in the book.
        """

        return self.get_best_buy().price

    def best_offer(self):
        """
        Returns the current best offer.

        Raises NoOffersException if there are no offers in the book.
        """

        return self.get_best_sell().price

    def is_matched(self):
        """
        Returns True if there is a matching pair of orders and
            False otherwise.
        """

        try:
            return self.best_bid() >= self.best_offer()
        except (NoBidsException, NoOffersException) as e:
            return False

    def get_filled_by_client_id(self, client_id):
        """
        Given a client id, returns a list of all the trades that the
        client was involved in (i.e. all trades that the client was
        either a maker or a taker of).
        """

        req_t = []

        for t in self.trades:
            if t.maker == client_id or t.taker == client_id:
                req_t.append(t)

        return req_t

    def get_orders_by_client_id(self, client_id):
        """
        Given a client id, returns a list of all the non-cancelled orders
        that the client has open.
        """

        req_o = []

        for o in self.orders.values():
            if o.client_id == client_id:
                req_o.append(o)

        return req_o
