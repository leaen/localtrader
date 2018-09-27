import time
from enum import Enum

from .side import Side


class OrderStatus(Enum):
    ACTIVE = 0
    FILLED = 1
    PARTIALLY_FILLED = 2
    CANCELLED = 3


class Order:
    def __init__(self, price, size, side, client_id):
        """
        price: the execution price of the order
        size: the volume of the order
        side: whether the order is a BUY or SELL order
        client_id: the ID of the client who submitted the order
        """

        if not isinstance(size, int):
            raise ValueError('Size must be an integer')

        if not (isinstance(price, float) or isinstance(price, int)):
            raise ValueError('Price must be a numerical (float or int)')

        if price < 0:
            raise ValueError('Price must be non-negative')

        if size < 0:
            raise ValueError('Size must be non-negative')

        if not isinstance(client_id, int):
            raise ValueError('Client ID should be an integer')

        if not isinstance(side, Side):
            raise ValueError('Order side must be Side.BUY or Side.SELL')

        self.price = price
        self.total_size = size
        self.size = size
        self.side = side
        self.client_id = client_id
        self.status = OrderStatus.ACTIVE

        # Uses the system time with ms accuracy
        self.order_time = time.time()

        # This guarantees unique ID's during runtime, but not necessarily
        #   between runs (e.g. when backtesting). Perhaps there's a way
        #   to get better (deterministic) ID's?
        self.order_id = id(self)

    def __lt__(self, other):
        """
        We sort orders based on time and price.

        If a sell is a BUY then we sort prices highest to lowest (i.e.
            the most aggressive orders are at the front) and conversely
            we sort lowest to highest for SELL orders.

        In the case that prices for two orders are equal the order with
            the earlier order_time comes first.
        """
        if self.side != other.side:
            return True

        if self.side == Side.BUY:
            if self.price != other.price:
                return self.price > other.price
        else:
            if self.price != other.price:
                return self.price < other.price

        return self.order_time < other.order_time

    def set_status(self, status):
        """
        Set the current status of the order.
        """

        if not isinstance(status, OrderStatus):
            raise ValueError('Status must be of type OrderStatus')

        self.status = status

    def get_status(self):
        """
        Get the current status of the order.
        """

        return self.status

    def cancel(self):
        """
        Cancel the order.
        """

        self.status = OrderStatus.CANCELLED

    def fill(self, fill_size):
        """
        Updates the status of the order when some or all of it
        has been matched.
        """

        assert(self.status != OrderStatus.CANCELLED)

        self.size -= fill_size

        if self.size == 0:
            self.status = OrderStatus.FILLED
        elif 0 < self.size < self.total_size:
            self.status = OrderStatus.PARTIALLY_FILLED
