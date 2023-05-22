from dataclasses import dataclass


@dataclass
class NestedAttribute:
    data: bytes
    attribute_type: int
