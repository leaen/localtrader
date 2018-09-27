import asyncio
import websockets
import time
import random
import sys

import numpy as np

from websockets.exceptions import ConnectionClosed

from localtrader.order import Order
from localtrader.side import Side


SERVER_ADDRESS = "ws://localhost:8765"

WAIT_MEAN = 150
WAIT_SD = 75

PRICE_SD = 0.25

SIZE_MEAN = 5
SIZE_SD = 2

RANDOM_WALK_COEFF = 1

async def usleep(t):
    await asyncio.sleep(max(t/1000000.0, 0))

def generate_order(instrument, client_id):
    size = int(np.random.normal(SIZE_MEAN, SIZE_SD))
    size = max(1, size)

    if random.random() > 0.5:
        price = np.random.normal(99.00, PRICE_SD)
        return Order(instrument, price, size, Side.BUY, client_id)
    else:
        price = np.random.normal(101.00, PRICE_SD)
        return Order(instrument, price, size, Side.SELL, client_id)

async def spam_orders(instrument, client_id):
    while True:
        o = generate_order(instrument, client_id)
        o_serialized = Order.serialize(o)
        msg = o_serialized
        async with websockets.connect(SERVER_ADDRESS) as websocket:
            await websocket.send(msg)
            print(f"> {msg}")

            resp = await websocket.recv()
            print(f"< {resp}")

        delay = np.random.normal(WAIT_MEAN, WAIT_SD)
        await usleep(delay)

def main():
    client_id = None
    instrument = None

    if len(sys.argv) == 2:
        client_id = int(sys.argv[1])
        instrument = 'ABC'
    elif len(sys.argv) == 3:
        client_id = int(sys.argv[1])
        instrument = sys.argv[2]
    else:
        client_id = id(client_id)
        instrument = 'ABC'

    while True:
        try:
            asyncio.get_event_loop().run_until_complete(spam_orders(instrument, client_id))
        except (OSError, ConnectionClosed) as e:
            print("Failed to connect to exchange. Waiting 3 seconds ...")
            time.sleep(3)

if __name__ == "__main__":
    main()
