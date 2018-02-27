### Orderbook
- Orderbook(instrument)
    - Should do regular initialisation
    - Read from a configuration file for tick size etc
- submit\_order(Order order)
    - Returns dictionary with whether submission was successful
- validate_order(Order order)
    - Check to make sure order has the correct instrument,
        price, size is valid (positive, respects tick size)
- get\_fills\_by\_client\_id(int client\_id, since = timestamp)
- get\_fills\_by\_client(Client client, since = timestamp)
- best\_bid()
- best\_ask()
- get\_buys()
- get\_sells()
- get\_trades(since = timestamp)
    - since is an optional parameter that can be used to
        get only 'fresh' trades

### Exchange
- submit\_order(Order order)
    - push on order onto the appropriate orderbook
- get\_balance\_by\_client\_id(int client\_id)
- get\_balance\_by\_client(Client client)
- get\_trades(since = timestamp, instrument=instrument)
    - get trades since the timestamp for all orderbooks or
        only for the specific instrument
- get\_instruments()
    - return a list of all instruments
- get\_instrument\_details(string instrument)
    - read from the instrument configuration file

### Order
- get_status(), set_status(Status status)
- cancel()
