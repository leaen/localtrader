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

        self.price = price
        self.size = size
        self.side = side
        self.maker = maker
        self.taker = taker
        self.trade_time = trade_time

    def __str__(self):
        return f'{self.instrument}: {self.side} {self.size} @ {self.price} {self.taker} {self.maker}'
