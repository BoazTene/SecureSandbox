from dataclasses import dataclass
from typing import Callable


@dataclass
class RPCMethod:
    name: str
    callback: Callable

