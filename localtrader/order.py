import time

from .side import Side


class Order:
    def __init__(self, price, size, side, client_id):
        assert price >= 0, "Price must be non-negative"
        assert size >= 0, "Size must be non-negative"

        self.price = price
        self.size = size
        self.client_id = client_id

        assert isinstance(side, Side), 'Order side must be Side.BUY or Side.SELL'
        self.side = side

        self.order_time = time.time()
        self.order_id = id(self) # This guarantees unique ID's for a single runtime,
                                 #   but perhaps there's a better way to get ID's?
