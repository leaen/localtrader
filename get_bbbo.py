import asyncio
import websockets
import time
import sys


SERVER_ADDRESS = "ws://localhost:8765"

async def get_bbbo(instrument):
    async with websockets.connect(SERVER_ADDRESS) as websocket:
        await websocket.send("bbbo")
        bbbo = await websocket.recv()
        bb = float(bbbo.split('|')[1])
        bo = float(bbbo.split('|')[2])

    return (bb, bo)

def main():
    instrument = None

    if len(sys.argv) == 2:
        instrument = sys.argv[1]
    else:
        instrument = 'ABC'

    try:
        bb, bo = asyncio.get_event_loop().run_until_complete(get_bbbo(instrument))
        print(f"Best bid  : {bb:.4f}")
        print(f"Best offer: {bo:.4f}")
    except OSError as e:
        print("Failed to connect to exchange. Waiting 3 seconds ...")
        time.sleep(3)

if __name__ == "__main__":
    main()
