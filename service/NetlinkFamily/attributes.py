class Attributes:
    """
    The family's possible attributes with ids.
    """

    PACKET_BUFFER = 0
    RULES = 1 # a list of struct rule
    RULE = 2 # a single rule firewall_rule.h struct
    RULE_NESTED = {
                "name": 10,
                "ingoingPackets": 11,
                "outgoingPackets": 12,
                "black": 13,
                "protocol": 14,
                "srcIP": 15,
                "dstIP": 16,
                "log": 17,
                "srcPort": 18,
                "dstPort": 19,
            }
    RULE_ID = 3 
