from collections import deque

from .order import Order
from .side import Side

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

        self.buy_orders = {}
        self.sell_orders = {}

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
            if order.price not in self.buy_orders:
                self.buy_orders[order.price] = deque()

            self.buy_orders[order.price].append(order)
        else:
            if order.price not in self.sell_orders:
                self.sell_orders[order.price] = deque()

            self.sell_orders[order.price].append(order)

        # TODO Check for order matches

    def cancel_order(self, order_id):
        """
        Takes the order_id of an order in the book and cancels it.

        Raises OrderNotFoundException if the order was not found in the book.
        """
        o = self.orders[order_id]

        book = self.buy_orders if o.side == Side.BUY else self.sell_orders

        try:
            book[o.price].remove(o)

            # If that was the last order at that price,
            # remove it from the dictionary
            if len(book[o.price]) == 0:
                del book[o.price]

        except ValueError:
            raise OrderNotFoundException

    def get_order_status(self, order_id):
        return self.orders[order_id].status

    def best_bid(self):
        """
        Returns the current best bid.

        Raises NoBidsException if there are no bids in the book.
        """

        if len(self.buy_orders.keys()) == 0:
            raise NoBidsException()

        return max(self.buy_orders.keys())

    def best_offer(self):
        """
        Returns the current best offer.

        Raises NoOffersException if there are no offers in the book.
        """
        if len(self.sell_orders.keys()) == 0:
            raise NoOffersException()

        return min(self.sell_orders.keys())
