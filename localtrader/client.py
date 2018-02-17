class Client:
    def __init__(self, name):
        self.client_id = id(self)
        self.name = name
        self.orders = []
