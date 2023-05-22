from netlink import Attribute
from threading import Thread
import socket
import json
import time

from errors import EmptyMessageError
from NetlinkFamily import FireWallFamily, Callbacks
from Encryption import Encryption
from rpc import RPC, RPCMethod
from parsing import PacketParser, FirewallRule, FirewallRules


class Client(Thread):
    def __init__(self, client: socket.socket, address: tuple[str, int]):
        Thread.__init__(self)

        self.__client = client
        self.__address = address
        self.__netlink_family = FireWallFamily(self.__build_netlink_callbacks())

        self.__encryption = Encryption()
        self.__rpc = RPC(methods=self.__build_rpc_methods())

    def __build_rpc_methods(self) -> list[RPCMethod]:
        """
        Builds the rpc methods.
        """

        return [
            RPCMethod("list_rules", self.__netlink_family.list_rules),
            RPCMethod("add_rule", self.add_rule_rpc_callback),
            RPCMethod("delete_rule", self.remove_rule_rpc_callback),
        ]

    def __build_netlink_callbacks(self) -> Callbacks:
        """
            Builds the netlink callbacks(commands callbacks).
        """

        return Callbacks(
            list_rules_callback=self.list_rules_callback,
            blocked_packet_callback=self.blocked_packet_callback,
            add_rule_callback=None,
        )


    def add_rule_rpc_callback(self, kwargs={}):
         print(kwargs)
         rule = FirewallRule(**kwargs)
         self.__netlink_family.add_rule(rule.as_attribute())

    def remove_rule_rpc_callback(self, kwargs={}):
         self.__netlink_family.remove_rule(kwargs['id'])

    def list_rules_callback(self, raw_rules: Attribute):
        rules = FirewallRules.from_raw_data(raw_rules.get_data_bytes())
        serialized_rules = rules.to_json()

        request = self.__rpc.list_rules(serialized_rules).serialize().decode("utf-8")
        self.send(request)

    def blocked_packet_callback(self, blocked_packet: Attribute):
        print(blocked_packet.get_data_bytes())
        parsed_packet = PacketParser(blocked_packet.get_data_bytes()).serialize()

        request = self.__rpc.logging_request(parsed_packet).serialize().decode('utf-8')
        self.send(request)

    def initialize_handshake(self):
        """
        Performs handshake with the client
        """

        self.__client.send(b"Hello Client")
        client_hello = self.__client.recv(1024)

        if client_hello != b"Hello Server":
            self.__client.send(b"Handshake failed")
            self.__client.close()

            raise Exception("Handshake failed")

        self.__client.send(self.__encryption.get_rsa_public_key())

        key = self.__client.recv(4096)
        self.__encryption.set_rsa_encryption_key(key)

    def send(self, message: str):
        """
        Encrypts the message and sends it to the client.
        """

        data = self.__encryption.encrypt(message.encode("utf-8"))
        self.__client.send(data)

    def recv(self) -> str:
        """
        Receives a message and decrypts it.
        Returns the decrypted text.
        """

        cipher_text = self.__client.recv(2048)
        
        if len(cipher_text) == 0:
            raise EmptyMessageError()

        message = self.__encryption.decrypt(cipher_text)

        return message.decode("utf-8")

    def run(self):
        """
        The main handler code.
        After initializing handshake it will completely handle the client.
        """
        print("[*] (%s:%d) just connected" % self.__address)

        self.initialize_handshake()
        self.__client.setblocking(False)

        while True:
            self.__netlink_family.recv()
            try:
                data = self.recv()
            except BlockingIOError:
                continue
            except EmptyMessageError:
                continue
            except Exception:
                continue

            request = self.__rpc.parse_message(data.encode("ascii"))
            self.__rpc.handle_request(request)

        print("[*] (%s:%d) just disconnected" % self.__address)
