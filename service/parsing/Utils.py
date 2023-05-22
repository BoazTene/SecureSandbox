import socket
import subprocess
import json

prefix = "IPPROTO_"
proto_table = {
    num: name[len(prefix):]
    for name, num in vars(socket).items()
    if name.startswith(prefix)
}


def parse_protocol(protocol: int):
    if protocol == -1:
        return "*"

    try:
        return proto_table[protocol].lower()
    except Exception:
        return "*"

def parse_protocol_name(protocol: str) -> int:
    if protocol == '*':
        return -1

    try:
        for num, name in proto_table.items():
            if name.lower() == protocol:
                return num

        return -1
    except Exception:
        return -1

