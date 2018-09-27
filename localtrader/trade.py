from .side import Side

class Trade:
    def __init__(self, instrument, price, size, side, maker, taker, trade_time):
        """
        instrument: the instrument that was traded
        price: the execution price of the trade
        size: the volume of the trade
        side: whether the trade was a buy or sell
        maker: the client ID of the maker
        taker: the client ID of the taker
        """

        # TODO Sanitisation

        self.instrument = instrument
        self.price = price
        self.size = size
        self.side = side
        self.maker = maker
        self.taker = taker
        self.trade_time = trade_time

    def __str__(self):
        return f"{self.instrument}: {self.side} {self.size} @ {self.price} {self.taker} {self.maker}"

    @staticmethod
    def serialize(t):
        side = "BUY" if t.side == Side.BUY else "SELL"
        return f"t|{t.instrument}|{t.price:.4f}|{t.size}|{side}|{t.maker}|{t.taker}|{t.trade_time}"

    @staticmethod
    def deserialize(t_serialized):
        sections = t_serialized.split('|')

        if sections[0] != 't':
            raise ValueError("Mismatched message type. '{t_serialized}' does not appear to be a trade.")

        try:
            instrument = sections[1]
            price = float(sections[2])
            size = int(sections[3])
            side = sections[4]
            if side not in ["BUY", "SELL"]:
                raise ValueError()
            side = Side.BUY if side == "BUY" else Side.SELL
            maker = int(sections[5])
            taker = int(sections[6])
            trade_time = int(sections[7])
        except ValueError as e:
            raise ValueError(f"Failed to deserialize trade message '{o_serialized}'.")

        return Trade(instrument, price, size, side, maker, taker, trade_time)
