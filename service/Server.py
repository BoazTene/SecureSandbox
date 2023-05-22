import socket

from Client import Client


class Server:
    """
    The main server.
    """

    ADDRESS = ("0.0.0.0", 9001)

    def __init__(self, address=ADDRESS):
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.__socket.bind(address)
        self.__socket.listen(1)
        print("[*] Server is listening")

        self.__clients = []
        
    def run(self):  
        while True:
            c, addr = self.__socket.accept()
            
            client_handler = Client(c, addr)
            self.__clients.append(client_handler)
            client_handler.start()
