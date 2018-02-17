from queue import Queue

from .order import Order
from .side import Side

class Orderbook:
    def __init__(self, instrument):
        self.instrument = instrument
        self.orders = {}
        self.buy_orders = {}
        self.sell_orders = {}

        self.trades = []

    def submit_order(order):
        """
        Submits an order into orderbook to be processed.
        """

        assert isinstance(order, Order), \
            'order must be of type Order instead have {}'.format(type(order))

        # Add order into the order pool
        self.orders[orders.order_id] = order

        # Add into appropriate limit book
        if order.side == Side.BUY:
            if order.price not in self.buy_orders:
                self.buy_orders[order.price] = Queue()

            self.buy_orders[order.price].put(order)
        else:
            if order.price not in self.sell_orders:
                self.sell_orders[order.price] = Queue()

            self.sell_orders[order.price].put(order)


