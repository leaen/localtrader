import unittest

from localtrader.side import Side
from localtrader.order import Order, OrderStatus
from localtrader.client import Client
from localtrader.orderbook import Orderbook


class TestOrderbook(unittest.TestCase):
    def test_best_offer(self):
        # Orderbook for ABC
        book = Orderbook('ABC')
        bob = Client('Bob')

        o1 = Order(10.00, 1, Side.BUY, bob.client_id)
        book.submit_order(o1)
        self.assertEqual(book.best_offer(), 10.00)

        o2 = Order(11.00, 1, Side.BUY, bob.client_id)
        book.submit_order(o2)
        self.assertEqual(book.best_offer(), 11.00)

        # Cancelling the second order should bring the best offer back to 10.00
        book.cancel_order(o2.order_id)
        self.assertEqual(book.best_offer(), 10.00)

    def test_best_offer(self):
        # Orderbook for ABC
        book = Orderbook('ABC')
        bob = Client('Bob')

        o1 = Order(10.00, 1, Side.SELL, bob.client_id)
        book.submit_order(o1)
        self.assertEqual(book.best_offer(), 10.00)

        o2 = Order(11.00, 1, Side.SELL, bob.client_id)
        book.submit_order(o2)
        self.assertEqual(book.best_offer(), 10.00)

        # Cancelling the first order should bring the best ask up to 11.00
        book.cancel_order(o1.order_id)
        self.assertEqual(book.best_offer(), 11.00)

    def test_cancel_order(self):
        # Orderbook for ABC
        book = Orderbook('ABC')
        bob = Client('Bob')

        o1 = Order(10.00, 1, Side.BUY, bob.client_id)
        book.submit_order(o1)
        self.assertEqual(book.get_order_status(o1.order_id), OrderStatus.ACTIVE)
        book.cancel_order(o1.order_id)
        self.assertEqual(book.get_order_status(o1.order_id), OrderStatus.CANCELLED)

    def test_matching_order(self):
        # Orderbook for ABC
        book = Orderbook('ABC')

        bob = Client('Bob')
        alice = Client('Alice')

        # Bob buys at 10, Alice sells at 10
        book.submit_order(Order(10.00, 1, Side.BUY, bob.client_id))
        book.submit_order(Order(10.00, 1, Side.SELL, alice.client_id))

        # Check that the orders were matched
        bob_fills = book.get_filled_by_client_id(bob.client_id)
        alice_fills = book.get_filled_by_client_id(alice.client_id)

        self.assertEqual(len(bob_fills), 1)
        self.assertEqual(len(alice_fills), 1)

        # Check that the fills are correctly recorded
        bob_fill = bob_fills[0]
        alice_fill = bob_fills[0]
        self.assertEqual(bob_fill['maker'], bob.client_id)
        self.assertEqual(bob_fill['taker'], alice.client_id)
        self.assertEqual(bob_fill['size'], 1)
        self.assertEqual(bob_fill['side'], Side.SELL)
        self.assertEqual(bob_fill['instrument'], 'ABC')
        self.assertEqual(bob_fill, alice_fill)

    def test_price_time_priority(self):
        # Orderbook for ABC
        book = Orderbook('ABC')

        bob = Client('Bob')
        alice = Client('Alice')
        eve = Client('Eve')

        # Bob buys at 10, Alice sells at 10, Eve tries to sell at 10 as well
        book.submit_order(Order(10.00, 1, Side.BUY, bob.client_id))
        book.submit_order(Order(10.00, 1, Side.SELL, alice.client_id))
        book.submit_order(Order(10.00, 1, Side.SELL, eve.client_id))

        # Check that the orders were matched
        bob_fills = book.get_filled_by_client_id(bob.client_id)
        alice_fills = book.get_filled_by_client_id(alice.client_id)
        eve_fills = book.get_filled_by_client_id(alice.client_id)

        self.assertEqual(len(bob_fills), 1)
        self.assertEqual(len(alice_fills), 1)
        self.assertEqual(len(eve_fills), 0)

        # Check that the fills are correctly recorded
        bob_fill = bob_fills[0]
        alice_fill = bob_fills[0]
        self.assertEqual(bob_fill['maker'], bob.client_id)
        self.assertEqual(bob_fill['taker'], alice.client_id)
        self.assertEqual(bob_fill['size'], 1)
        self.assertEqual(bob_fill['side'], Side.SELL)
        self.assertEqual(bob_fill['instrument'], 'ABC')
        self.assertEqual(bob_fill, alice_fill)

        # Check that Eve's order is still on the book
        self.assertEqual(book.get_orders_by_client_id(eve.client_id), 1)

    def test_split_orders(self):
        # Orderbook for ABC
        book = Orderbook('ABC')

        bob = Client('Bob')
        alice = Client('Alice')
        eve = Client('Eve')

        # Bob buys 2 at 10, Alice sells at 10, Eve tries to sell at 10 as well
        book.submit_order(Order(10.00, 2, Side.BUY, bob.client_id))
        book.submit_order(Order(10.00, 1, Side.SELL, alice.client_id))
        book.submit_order(Order(10.00, 1, Side.SELL, eve.client_id))

        # Check that the orders were matched
        bob_fills = book.get_filled_by_client_id(bob.client_id)
        alice_fills = book.get_filled_by_client_id(alice.client_id)
        eve_fills = book.get_filled_by_client_id(eve.client_id)

        self.assertEqual(len(bob_fills), 2)
        self.assertEqual(len(alice_fills), 1)
        self.assertEqual(len(eve_fills), 1)

        # Check that the fills are correctly recorded
        bob_fill = bob_fills[0]
        alice_fill = alice_fills[0]
        self.assertEqual(bob_fill['maker'], bob.client_id)
        self.assertEqual(bob_fill['taker'], alice.client_id)
        self.assertEqual(bob_fill['size'], 1)
        self.assertEqual(bob_fill['side'], Side.SELL)
        self.assertEqual(bob_fill['instrument'], 'ABC')
        self.assertEqual(bob_fill, alice_fill)

        bob_fill = bob_fills[1]
        eve_fill = eve_fills[0]
        self.assertEqual(bob_fill['maker'], bob.client_id)
        self.assertEqual(bob_fill['taker'], eve.client_id)
        self.assertEqual(bob_fill['size'], 1)
        self.assertEqual(bob_fill['side'], Side.SELL)
        self.assertEqual(bob_fill['instrument'], 'ABC')
        self.assertEqual(bob_fill, eve_fill)

    def test_no_matching_orders(self):
        # Orderbook for ABC
        book = Orderbook('ABC')

        bob = Client('Bob')
        alice = Client('Alice')
        eve = Client('Eve')

        # Bob offers @ 9, alice asks @ 10, bob cancels then eve asks @ 8.5
        bob_order = Order(9.00, 2, Side.BUY, bob.client_id)
        book.submit_order(bob_order)
        book.submit_order(Order(10.00, 1, Side.SELL, alice.client_id))
        book.cancel_order(bob_order.order_id)
        book.submit_order(Order(8.50, 1, Side.SELL, eve.client_id))

        # Check that the orders were matched
        bob_fills = book.get_filled_by_client_id(bob.client_id)
        alice_fills = book.get_filled_by_client_id(alice.client_id)
        eve_fills = book.get_filled_by_client_id(eve.client_id)

        self.assertEqual(len(bob_fills), 0)
        self.assertEqual(len(alice_fills), 0)
        self.assertEqual(len(eve_fills), 0)
