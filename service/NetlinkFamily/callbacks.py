from netlink import Attribute
from dataclasses import dataclass
from typing import Callable


@dataclass
class Callbacks:
    list_rules_callback: Callable[[Attribute], any]
    blocked_packet_callback: Callable[[Attribute], any]
    add_rule_callback: Callable[[Attribute], any]



