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

WAIT_MEAN = 15
WAIT_SD = 75

PRICE_MEAN = 100
PRICE_SD = 2

SIZE_MEAN = 100
SIZE_SD = 20

RANDOM_WALK_COEFF = 0.1

async def usleep(t):
    await asyncio.sleep(max(t/1000000.0, 0))

def generate_order(instrument, client_id):
    global PRICE_MEAN
    price = round(np.random.normal(PRICE_MEAN, PRICE_SD), 2)
    PRICE_MEAN += (random.random() - 0.5) * RANDOM_WALK_COEFF

    size = int(25 *random.random())
    size = max(1, size)

    side = Side.BUY if random.random() > 0.5 else Side.SELL

    return Order(instrument, price, size, side, client_id)

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
