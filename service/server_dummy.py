import socket

class Server:
    MY_USERNAME = "boaz_tene"
    MY_PASSWORD = "1234"
    SUPER_SECRET_FLAG = "Y@U_W!LL_N3veRC@tchME"

    def __init__(self, address: tuple[str, int]):
        self.sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        self.sk.bind(address)
        print(f"UDP server listening on {address[0]}:{address[1]}")

    def __parse_message(self, message: str):
        try:
            username = message.split(", ")[0].split("user: ")[1]
            password = message.split(", ")[1].split("password: ")[1]
        except Exception as e:
            return None, None

        return username, password

    def __authenticate(self, username: str, password: str) -> bool:
        return username == Server.MY_USERNAME and password == Server.MY_PASSWORD

        
    def run(self):
        while True:
            data, addr = self.sk.recvfrom(1024)
            message = data.decode('utf-8')

            username, password = self.__parse_message(message)

            if self.__authenticate(username, password):
                try:
                    self.sk.sendto(Server.SUPER_SECRET_FLAG.encode('utf-8'), addr)
                    print("[+] Someone got the flag", addr)
                except PermissionError:
                    print("[+] SECRET_FLAG was blocked by the kernel!?!?")
                    print("[+] I'm sure it's already been logged, oh no!!!")
            else:
                self.sk.sendto(b"Try again!", addr)


if __name__ == "__main__":
    addr = ("192.168.84.130", 1234)
    server = Server(addr)

    server.run()

