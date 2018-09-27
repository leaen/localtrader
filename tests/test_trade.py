import time
import unittest

from localtrader.side import Side
from localtrader.trade import Trade


class TestOrder(unittest.TestCase):
    def test_serialize_trade(self):
        t_time = time.time_ns()
        t = Trade('ABC', 100.0, 50, Side.BUY, 123, 456, t_time)
        t_serialized = Trade.serialize(t)

        self.assertEqual(t_serialized, f"t|ABC|100.0000|50|BUY|123|456|{t_time}")

    def test_deserialize_trade(self):
        t_serialized = "t|ABC|100.0000|50|BUY|123|456|1538028038976563587"
        t = Trade.deserialize(t_serialized)

        self.assertEqual(t.instrument, "ABC")
        self.assertEqual(t.price, 100.0000)
        self.assertEqual(t.size, 50)
        self.assertEqual(t.side, Side.BUY)
        self.assertEqual(t.maker, 123)
        self.assertEqual(t.taker, 456)
        self.assertEqual(t.trade_time, 1538028038976563587)
