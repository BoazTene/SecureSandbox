from scapy.all import IP
import struct
import json
import base64

class PacketParser:
   NATIVE_VALUES = (int, float, str, bytes, bool, list, tuple, set, dict, type(None)) 

   def __init__(self, raw_data: bytes):
        protocol, len = struct.unpack("II", raw_data[0:8])
        packet_raw = raw_data[8:]
        self.__packet = IP(packet_raw)

   def __serialize_layer(self, layer):
        layer_dict = {}

        if not getattr(layer, 'fields_desc', None):
            return

        for field in layer.fields_desc:
            field_value = getattr(layer, field.name)
            if field_value is type(None):
                field_value = None

            if not isinstance(field_value, PacketParser.NATIVE_VALUES):
                field_value = self.__serialize_layer(field_value)
            if not isinstance(field_value, bytes):
                field_value = str(field_value).encode("utf-8")

            layer_dict[field.name] = base64.b64encode(field_value).decode('utf-8')

        return {layer.name: layer_dict}
                            
   def serialize(self) -> bytes:
        """
        Turn every layer to dict, store in ChainMap type.
        :return: ChainMaq
        """
        layers = list()
        layer_number = 0

        while True:
            layer = self.__packet.getlayer(layer_number)
            if not layer:
                break
            print(layer.name)
            layers.append(self.__serialize_layer(layer))

            layer_number += 1

        return json.dumps(layers)


