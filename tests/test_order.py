import unittest

from localtrader.side import Side
from localtrader.order import Order, OrderStatus


class TestOrder(unittest.TestCase):
    def test_order_status(self):
        """
        Order status should be OrderStatus.ACTIVE when the order is first
            created, then change to OrderStatus.FILLED if the order is
            successfully matched, or OrderStatus.CANCELLED if cancelled.
        """
        o = Order("ABC", 50.00, 2, Side.BUY, 123)

        self.assertEqual(o.get_status(), OrderStatus.ACTIVE)

        o.cancel()

        self.assertEqual(o.get_status(), OrderStatus.CANCELLED)

    def test_invalid_side(self):
        # Try making an order with side of 0
        with self.assertRaises(ValueError):
            o = Order("ABC", 10.00, 5, 0, 123)

        # Try making an order with side of 'BUY'
        with self.assertRaises(ValueError):
            o = Order("ABC", 10.00, 5, 'BUY', 123)

    def test_invalid_client_id(self):
        # Try making an order with client_id 'abc'
        with self.assertRaises(ValueError):
            o = Order("ABC", 10.00, 5, Side.BUY, 'abc')

    def test_negative_price(self):
        # Try making an order at price of -10.00
        with self.assertRaises(ValueError):
            o = Order("ABC", -10.00, 5, Side.BUY, 123)

    def test_negative_size(self):
        # Try making an order with size -5
        with self.assertRaises(ValueError):
            o = Order("ABC", 10.00, -5, Side.BUY, 123)

    def test_non_integer_size(self):
        # Try making an order with size 'a'
        with self.assertRaises(ValueError):
            o = Order("ABC", 10.00, 'a', Side.BUY, 123)

        # Try making an order with size 1.5
        with self.assertRaises(ValueError):
            o = Order("ABC", 10.00, 1.5, Side.BUY, 123)

    def test_price_time_order(self):
        """
        Test the price-time ordering of orders.

        Orders should be sorted by price first (highest to lowest
            for buy orders and reverse for sell orders) and then
            by time.
        """
        early_order = Order("ABC", 10.00, 1, Side.BUY, 123)
        late_order = Order("ABC", 10.00, 1, Side.BUY, 123)

        self.assertTrue(early_order < late_order, 'Early orders should come first')

        low_order = Order("ABC", 10.00, 1, Side.BUY, 123)
        high_order = Order("ABC", 11.00, 1, Side.BUY, 123)

        self.assertTrue(high_order < low_order, 'Buy orders with highest price come first')

        low_order = Order("ABC", 10.00, 1, Side.SELL, 123)
        high_order = Order("ABC", 11.00, 1, Side.SELL, 123)

        self.assertTrue(low_order < high_order, 'Sell orders with lowest price come first')

        high_order = Order("ABC", 11.00, 1, Side.SELL, 123)
        low_order = Order("ABC", 10.00, 1, Side.SELL, 123)

        self.assertTrue(low_order < high_order, 'Sell orders with lowest price come first, time second')

    def test_serialize_order(self):
        o = Order("ABC", 10.00, 10, Side.BUY, 123)
        o_serialized = Order.serialize(o)

        self.assertEqual(o_serialized, "o|ABC|10.0000|10|BUY|123")

        o = Order("CBA", 12.333, 432, Side.SELL, 4242)
        o_serialized = Order.serialize(o)

        self.assertEqual(o_serialized, "o|CBA|12.3330|432|SELL|4242")

    def test_deserialize_order(self):
        o_serialized = "o|ABC|10.0000|10|BUY|123"
        o = Order.deserialize(o_serialized)

        self.assertEqual(o.instrument, "ABC")
        self.assertEqual(o.price, 10.00)
        self.assertEqual(o.size, 10)
        self.assertEqual(o.side, Side.BUY)
        self.assertEqual(o.client_id, 123)

        o_serialized = "o|CBA|12.3330|432|SELL|4242"

        o = Order.deserialize(o_serialized)

        self.assertEqual(o.instrument, "CBA")
        self.assertEqual(o.price, 12.333)
        self.assertEqual(o.size, 432)
        self.assertEqual(o.side, Side.SELL)
        self.assertEqual(o.client_id, 4242)
