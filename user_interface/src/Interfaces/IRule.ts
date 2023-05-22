/**
 * Represents a firewall rule.
 */
interface IRule {
    id: number;
    name: string;
    ingoingPackets: boolean;
    outgoingPackets: boolean;
    black: boolean;
    protocol: string;
    srcIP: string;
    dstIP: string;
    srcPort: number;
    dstPort: number;
    log: boolean;
}

export default IRule;
