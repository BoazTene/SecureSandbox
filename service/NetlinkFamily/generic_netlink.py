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

from netlink import NetLink, Message, CB_Kind, CB_Type, AttributePolicy, Attribute
from .errors import AttributeNotExists
import struct


class GenericMessage(Message):
    """
        Defines a generic message.

        This message is sendable and will add automatically the generic header.
    """

    def __init__(self, family_id: int, flags: int, cmd: int, version: int):
        super().__init__(family_id, 0, flags)

        self.cmd = cmd
        self.version = version
        
        self.append(self.__generate_header(), GenericNetLink.HEADER_LEN) # appends the generic header

    def __generate_header(self) -> bytes:
        """
            Generates generic netlink header.
            
            Generic Netlink header is defined as followes:
                cmd - 8 bits
                version - 8 bits
                not_used/reserved - 16 bits

            @return the header in bytes.
        """

        return struct.pack("BBH", self.cmd, self.version, 0)
    
    @staticmethod
    def from_message(message: Message):
        """
            Creates an generic message from an existing netlink message (Base Message).
            
            @param messsage src message.
            @return Generic Message.
        """

        length, family_id, flags, seq, pid = message.parse_header()
        cmd, version, _ = GenericMessage.parse_genl_header(message.get_bytes())
        
        genl_message = GenericMessage(family_id, flags, cmd, version)

        genl_message.append(message.get_bytes()[NetLink.HEADER_LEN + GenericNetLink.HEADER_LEN:], 0) # adding payload.

        return genl_message
        
    @staticmethod
    def parse_genl_header(data: bytes) -> tuple[int, int, int]:
        """
            Parses the generic message's header (both parent and generic header).

            Generic Netlink message:
                16 bytes - is the netlink header, includes (length, type, flags, seq, pid).
                4 bytes - is the generic netlink header, includes (cmd, version, reserved bytes/not used).
                payload

            @param data bytes message data.
            @return a tuple that contains cmd, version, reserved bytes (you can ignore).

        """

        header = data[NetLink.HEADER_LEN:NetLink.HEADER_LEN + GenericNetLink.HEADER_LEN]
        return struct.unpack("BBH", header)


class GenericNetLink(NetLink):
    """
        Simple and base implementation of the generic netlink.
    """

    HEADER_LEN = 4
    PROTOCOL = 16

    def __init__(self, family_name: str, policies: list[AttributePolicy]):
        super().__init__(NetLink.resolve_genl_family_id(family_name), GenericNetLink.PROTOCOL, GenericNetLink.HEADER_LEN, policies)

    def parse_message(self, msg: Message) -> tuple[list[Attribute], int, int]:
        """
            Parses a message (attributes + header).
            For conviency later its receiving Message type.

            @param msg message to parse
            @return tuple of list of attributes, cmd and version.
        """
        message = GenericMessage.from_message(msg)
        
        attributes = self.parse_message_attributes(message)
        cmd, version = message.cmd, message.version

        return attributes, cmd, version 

    def find_attribute(self, attributes: list[Attribute], attr_type: int):
        """
            Finds an attribute by type.

            If the attribute type doesn't exists the AttributeNotExists error will be raised.

            @param attributes the searched attributes.
            @param attr_type the type of the wanted attribute.
            @return the index of the attribute in the list.
        """
        for i, attribute in enumerate(attributes):
            if attribute.type == attr_type:
                return i

        raise AttributeNotExists()
        
