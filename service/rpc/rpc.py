from .parsed_rpc import ParsedRPC
from .rpc_method import RPCMethod
from tinyrpc.protocols.jsonrpc import JSONRPCProtocol
from tinyrpc import BadRequestError, RPCBatchRequest
from queue import Queue
import json


class RPC:
    """
    A JSONRPC implmentation.
    Handles rpc requests.
    """

    def __init__(self, methods: RPCMethod):
        self.__methods = methods
        self.__requests = Queue()
        self.__rpc = JSONRPCProtocol()

    def logging_request(self, packet: dict):
        return self.__rpc.create_request(
            "recv_blocked_packet", {"packet": packet}
        )

    def list_rules(self, rules: str):
        return self.__requests.get().respond({"rules": rules})

    def parse_message(self, data: bytes) -> ParsedRPC:
        """
        Parses the message (method, args, ...),
        and calls the right method.

        Returns method response or error message
        """

        try:
            request = self.__rpc.parse_request(data)
        except BadRequestError as error:
            print(error)
            return None
        
        self.__requests.put(request)
        
        return ParsedRPC(method=request.method, kwargs=request.kwargs)

    def handle_request(self, request: ParsedRPC):
        """
            Handles an rpc request will call the asked method.
        """
        for method in self.__methods:
            if method.name == request.method:
                method.callback(kwargs=request.kwargs)

