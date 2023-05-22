import ISearch from "../../Interfaces/ISearch";
import IRule from "../../Interfaces/IRule";


export const getFilterRules = (
    searchFilter: ISearch,
    rules: IRule[]
): IRule[] => {
    console.log("Rules", rules);
    return rules.filter((rule) => {
        let valid: boolean = true;
        if (searchFilter.srcIP !== undefined) {
            valid = valid && rule.srcIP.includes(searchFilter.srcIP);
        }

        if (searchFilter.dstIP !== undefined) {
            valid = valid && rule.dstIP.includes(searchFilter.dstIP);
        }

        if (searchFilter.protocol !== undefined) {
            valid = valid && rule.protocol === searchFilter.protocol;
        }

        if (searchFilter.black !== undefined) {
            valid = valid && rule.black === searchFilter.black;
        }

        if (searchFilter.name !== undefined) {
            valid = valid && rule.name.includes(searchFilter.name);
        }

        valid = valid && searchFilter.ingoingPackets === rule.ingoingPackets;
        valid = valid && searchFilter.outgoingPackets === rule.outgoingPackets;

        return valid;
    });
};

export const defaultSearchFilter: ISearch = {
        name: undefined,
        srcIP: undefined,
        dstIP: undefined,
        protocol: undefined,
        ingoingPackets: false,
        outgoingPackets: false,
        black: undefined,
}
