"""
    Python client for the Netlink interface.
    Copyright (C) 2023 Boaz Tene

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

from netlink import AttributePolicy, CB_Kind, CB_Type, Attribute, Message, NetLink
from .nested_attribute import NestedAttribute
from .attributes import Attributes
from .commands import Commands
from .multicast import Multicast
from .callbacks import Callbacks
from .generic_netlink import GenericNetLink, GenericMessage
from .errors import AttributeNotExists

import struct


class FireWallFamily(GenericNetLink):
    """
    A custom generic netlink family.
    """

    FAMILY_NAME = "FIREWALL_LOGER"
    VERSION = 1
    POLICY = [
        AttributePolicy(
            0, 0, 100000
        ),  # policy for attribute 0. type: 1 (string), min length: 0, max length: 100
        AttributePolicy(1, 1, 8642),
    ]

    def __init__(self, callbacks: Callbacks):
        super().__init__(FireWallFamily.FAMILY_NAME, FireWallFamily.POLICY)

        self.__callbacks = callbacks

        # multicast groups requires this to be disabled.
        self.disable_seq_check()

        self.__configure_mc()
        self.__configure_cb()

    def __configure_mc(self):
        self.add_membership(
            NetLink.resolve_genl_group_id(
                FireWallFamily.FAMILY_NAME, Multicast.LOGGING)
        )

    def __configure_cb(self):
        self.modify_cb(CB_Kind.CB_CUSTOM, CB_Type.CB_VALID, self.recv_messages)

    def recv_messages(self, message: Message):
        """
        The callback for valid messages.

        @param message the new message
        """
        attributes, cmd, version = self.parse_message(message)

        if cmd == Commands.BLOCKED_PACKET_ARRIVED:
            try:
                attribute_id = self.find_attribute(
                    attributes, Attributes.PACKET_BUFFER)
            except AttributeNotExists:
                return

            self.__callbacks.blocked_packet_callback(attributes[attribute_id])
        elif cmd == Commands.LIST_RULES:
            try:
                attribute_id = self.find_attribute(
                    attributes, Attributes.RULES)
            except AttributeNotExists:
                self.__callbacks.list_rules_callback(Attribute(b'', 0, Attributes.RULES))
                return
            self.__callbacks.list_rules_callback(attributes[attribute_id])
        elif cmd == Commands.ADD_RULE:
            try:
                attribute_id = self.find_attribute(
                    attributes, Attributes.RULE)
            except AttributeNotExists:
                return

            self.__callbacks.add_rule_callback(attributes[attribute_id])
        else:
            pass

    def list_rules(self, kwargs={}):
        message = GenericMessage(
            self.get_family_id(),
            0x300,
            Commands.LIST_RULES,
            FireWallFamily.VERSION,
        )

        self.send(message)

    def constract_nested_attribute(self, base_message: GenericMessage, attribute_type: int, nested_attributes: list[NestedAttribute]):
        start_pointer = base_message.nla_nest_start(attribute_type)

        for attr in nested_attributes:
            base_message.nla_put(attr.data, attr.attribute_type)

        base_message.nla_nest_end(start_pointer)
        


    def add_rule(self, rule_data: list[NestedAttribute]):
       for attr in rule_data:
           print(attr.data, attr.attribute_type)
       message = GenericMessage(
               self.get_family_id(),
               0,
               Commands.ADD_RULE,
               FireWallFamily.VERSION)

       self.constract_nested_attribute(message, Attributes.RULE, rule_data)
              
       self.send(message)

    def remove_rule(self, rule_id: int):
        message = GenericMessage(
               self.get_family_id(),
               0,
               Commands.REMOVE_RULE,
               FireWallFamily.VERSION)

        self.constract_nested_attribute(message, Attributes.RULE_ID, [NestedAttribute(struct.pack('q', rule_id), Attribute.U8)])
        self.send(message)


    def handle_blocked_packet_arrived(self, attributes):
        try:
            attribute_id = self.find_attribute(
                attributes, Attributes.PACKET_BUFFER)
        except AttributeNotExists:
            print("Recieved an attribute that doesn't exists")
            return

        packet_attribute = attributes[attribute_id]

        log_data_buffer = packet_attribute.get_data_bytes()
        packet_buffer_len = packet_attribute.len - (
            4 * 2
        )  # removing the int, int = 4 bytes so 8.
        protocol, packet_len, packet_buffer = struct.unpack(
            "II%ds" % packet_buffer_len, log_data_buffer
        )

        parsed_packet = parse_packet(packet_buffer)

        if self.blocked_packet_callback is not None:
            self.blocked_packet_callback(parsed_packet)


if __name__ == "__main__":
    family = FireWallFamily()
    family.list_rules()
    family.recv_messages()
