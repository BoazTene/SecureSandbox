from __future__ import annotations
from dataclasses import dataclass

@dataclass
class ParsedRPC:
    method: str
    kwargs: dict[any]

    @classmethod
    def from_parsed_rpc(cls, parsed_rpc: ParsedRPC):
        return cls(method=parsed_rpc.method, kwargs=parsed_rpc.kwargs)
