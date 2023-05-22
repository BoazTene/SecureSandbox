/**
 * An interface that represents a search query.
 */
interface ISearch {
    name: string | undefined;
    srcIP: string | undefined;
    dstIP: string | undefined;
    protocol: string | undefined;
    ingoingPackets: boolean;
    outgoingPackets: boolean;
    black: boolean | undefined;
}

export default ISearch;
