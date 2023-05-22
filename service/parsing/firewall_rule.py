from netlink import Attribute
from .Utils import parse_protocol, parse_protocol_name
from NetlinkFamily import NestedAttribute, Attributes
from dataclasses import dataclass
import struct
import json


class SizeNotMatch(Exception):
    pass


@dataclass
class FirewallRule:
    """
        Represents a FirewallRule struct.
    """

    FORMAT = "50sIIIi15s15sIII" 

    id: int
    name: str
    outgoingPackets: bool
    ingoingPackets: bool
    black: bool
    protocol: str 
    srcIP: str
    dstIP: str
    srcPort: int
    dstPort: int
    log: bool

    @classmethod
    def from_raw_data(cls, id: int, raw_data: bytes):
        """
            Creates a firewall rule from a raw_data of the struct.

            @return the firewall rule.
        """

        if len(raw_data) != FirewallRule.get_size():
            raise SizeNotMatch()

        struct_values = struct.unpack(FirewallRule.FORMAT, raw_data)
        
        return cls(id, *FirewallRule.format_struct_values(struct_values))

    @staticmethod
    def format_struct_values(struct_values: tuple) -> tuple:
        """
            Formants a tuple of a raw struct memebers.
            The method will trim src_ip's remaining \x00 (null bytes).

            @param struct_values a tuple containg the struct values.
            @return the formatted struct values.

        """

        return struct_values[0].decode("utf-8").rstrip('\x00'), bool(struct_values[1]), \
                    bool(struct_values[2]), bool(struct_values[3]), parse_protocol(int(struct_values[4])), \
                    struct_values[5].decode('utf-8').rstrip('\x00'), \
                    struct_values[6].decode('utf-8').rstrip('\x00'), int(struct_values[7]), int(struct_values[8]), bool(struct_values[9])

    def unformat_struct_values(self, struct_values: list) -> list:
        return struct_values[0].encode("utf-8"), int(struct_values[1]), \
                    int(struct_values[2]), int(struct_values[3]), parse_protocol_name(struct_values[4]), \
                    struct_values[5].encode('utf-8'), \
                    struct_values[6].encode('utf-8'), int(struct_values[7])


    def to_json(self) -> str:
        """
            Serializes the struct into json format.
        """
        return json.dumps(self.__dict__)

    def to_struct(self) -> bytes:
        values = list({key: self.__dict__[key] for key in self.__dict__ if key != "id"}.values())
        values = self.unformat_struct_values(values)
             
        return struct.pack(FirewallRule.FORMAT, *values)

    def as_attribute(self) -> list[NestedAttribute]:
        nested = []

        nested.append(NestedAttribute(struct.pack("50s", self.name.encode('utf-8')), Attributes.RULE_NESTED["name"]))
        nested.append(NestedAttribute(struct.pack("q", int(self.outgoingPackets)), Attributes.RULE_NESTED["outgoingPackets"]))
        nested.append(NestedAttribute(struct.pack("q", int(self.ingoingPackets)), Attributes.RULE_NESTED["ingoingPackets"]))
        nested.append(NestedAttribute(struct.pack("q", int(self.black)), Attributes.RULE_NESTED["black"]))
        nested.append(NestedAttribute(struct.pack("q", parse_protocol_name(self.protocol)), Attributes.RULE_NESTED["protocol"]))
        nested.append(NestedAttribute(struct.pack("15s", self.srcIP.encode('utf-8')), Attributes.RULE_NESTED["srcIP"]))
        nested.append(NestedAttribute(struct.pack("15s", self.dstIP.encode('utf-8')), Attributes.RULE_NESTED["dstIP"]))
        nested.append(NestedAttribute(struct.pack("i", int(self.dstPort)), Attributes.RULE_NESTED["dstPort"]))
        nested.append(NestedAttribute(struct.pack("i", int(self.srcPort)), Attributes.RULE_NESTED["srcPort"]))
        nested.append(NestedAttribute(struct.pack("q", int(self.log)), Attributes.RULE_NESTED["log"]))
        print(self.srcPort, self.dstPort)

        return nested


    @staticmethod
    def get_size() -> int:
        """
            Returns the the size of the struct.
        """

        return struct.calcsize(FirewallRule.FORMAT)


@dataclass
class FirewallRules:
    rules: list[FirewallRule]


    @classmethod
    def from_raw_data(cls, raw_data: bytes):
        """
            Creates a FirewallRule list from a raw_data.
            
            @param raw_data the list raw data.
            @return list of firewallRule
        """
        elements = [raw_data[i:i+FirewallRule.get_size()] for i in range(0, len(raw_data), FirewallRule.get_size())]

        return cls([FirewallRule.from_raw_data(i, element) for i, element in enumerate(elements)])

    def to_json(self):
        return json.dumps([rule.__dict__ for rule in self.rules])

