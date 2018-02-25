import unittest

from localtrader.side import Side
from localtrader.order import Order


class TestOrder(unittest.TestCase):
    def test_invalid_side(self):
        # Try making an order with side of 0
        with self.assertRaises(ValueError):
            o = Order(10.00, 5, 0, 123)

        # Try making an order with side of 'BUY'
        with self.assertRaises(ValueError):
            o = Order(10.00, 5, 'BUY', 123)

    def test_invalid_client_id(self):
        # Try making an order with client_id 'abc'
        with self.assertRaises(ValueError):
            o = Order(10.00, 5, Side.BUY, 'abc')

    def test_negative_price(self):
        # Try making an order at price of -10.00
        with self.assertRaises(ValueError):
            o = Order(-10.00, 5, Side.BUY, 123)

    def test_negative_size(self):
        # Try making an order with size -5
        with self.assertRaises(ValueError):
            o = Order(10.00, -5, Side.BUY, 123)

    def test_non_integer_size(self):
        # Try making an order with size 'a'
        with self.assertRaises(ValueError):
            o = Order(10.00, 'a', Side.BUY, 123)

        # Try making an order with size 1.5
        with self.assertRaises(ValueError):
            o = Order(10.00, 1.5, Side.BUY, 123)

    def test_price_time_order(self):
        """
        Test the price-time ordering of orders.

        Orders should be sorted by price first (highest to lowest
            for buy orders and reverse for sell orders) and then
            by time.
        """
        early_order = Order(10.00, 1, Side.BUY, 123)
        late_order = Order(10.00, 1, Side.BUY, 123)

        self.assertTrue(early_order < late_order, 'Early orders should come first')

        low_order = Order(10.00, 1, Side.BUY, 123)
        high_order = Order(11.00, 1, Side.BUY, 123)

        self.assertTrue(high_order < low_order, 'Buy orders with highest price come first')

        low_order = Order(10.00, 1, Side.SELL, 123)
        high_order = Order(11.00, 1, Side.SELL, 123)

        self.assertTrue(low_order < high_order, 'Sell orders with lowest price come first')

        high_order = Order(11.00, 1, Side.SELL, 123)
        low_order = Order(10.00, 1, Side.SELL, 123)

        self.assertTrue(low_order < high_order, 'Sell orders with lowest price come first, time is second')
