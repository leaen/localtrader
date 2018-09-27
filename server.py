import asyncio
import websockets
import time

from localtrader.orderbook import Orderbook
from localtrader.order import Order
from localtrader.trade import Trade


since_update = 0
book = Orderbook("ABC")

async def msgs_per_sec():
    while True:
        await asyncio.sleep(1)
        global since_update

        print(f"Processing {since_update} msgs/s")
        since_update = 0

async def process_message(websocket, path):
    global messages, i, since_update

    msg = await websocket.recv()
    since_update += 1

    message_type = msg.split('|', maxsplit = 1)[0]

    if message_type == 'o':
        await websocket.send("ACK")
        o = Order.deserialize(msg)
        if o.size != 0:
            trades = book.submit_order(o)

            """
            if len(trades) > 0:
                print('\n'.join(map(str, trades)))
            """

    elif message_type == 'bb':
        await websocket.send(f"bb|{book.best_bid():.4f}")

    elif message_type == 'bo':
        await websocket.send(f"bo|{book.best_offer():.4f}")

    elif message_type == 'bbbo':
        await websocket.send(f"bbbo|{book.best_bid():.4f}|{book.best_offer():.4f}|{time.time_ns()}")

start_server = websockets.serve(process_message, 'localhost', 8765)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().create_task(msgs_per_sec())

asyncio.get_event_loop().run_forever()
