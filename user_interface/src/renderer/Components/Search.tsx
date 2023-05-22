import IRule from "../../Interfaces/IRule";
import { TextInput, Stack, Text, Center  } from "@mantine/core";
import React, { useState, useEffect } from "react";
import SearchRuleType from "./SearchRuleType";
import SearchRuleProtocol from "./SearchRuleProtocol";
import SearchRuleDirection, { IRuleDirection } from "./SearchRuleDirection";
import ISearch from "../../Interfaces/ISearch";

interface SearchProps {
    rules: IRule[];
    setSearchFilter: (filter: ISearch) => void;
}

const Search = ({ rules, setSearchFilter }: SearchProps) => {
    const defaultQuery = {
        name: "",
        srcIP: "",
        dstIP: "",
        protocol: undefined,
        black: undefined,
        ingoingPackets: true,
        outgoingPackets: true,
    };

    const [query, setQuery] = useState<ISearch>(defaultQuery);

    useEffect(() => {
        setSearchFilter(query);
    }, [query]);

    return (
        <Stack>
            <Center mt="30px">
                <Text fw={700} fz="xl">
                    Search Rules
                </Text>
            </Center>
            <TextInput
                label="Name"
                placeholder="Firewall name..."
                value={query.name}
                onChange={(event) => {
                    let copiedQuery = { ...query };
                    copiedQuery.name = event.currentTarget.value;
                    setQuery(copiedQuery);
                }}
            />
            <TextInput
                label="Source IP"
                placeholder="192.168.0.1"
                value={query.srcIP}
                onChange={(event) => {
                    let copiedQuery = { ...query };
                    copiedQuery.srcIP = event.currentTarget.value;
                    setQuery(copiedQuery);
                }}
            />
            <TextInput
                label="Destination IP"
                placeholder="192.168.0.1"
                value={query.dstIP}
                onChange={(event) => {
                    let copiedQuery = { ...query };
                    copiedQuery.dstIP = event.currentTarget.value;
                    setQuery(copiedQuery);
                }}
            />

            <SearchRuleProtocol
                onChange={(protocol: string) => {
                    let copiedQuery = { ...query };
                    copiedQuery.protocol = protocol;
                    setQuery(copiedQuery);
                }}
            />
            <SearchRuleDirection
                onChange={(direction: IRuleDirection) => {
                    let copiedQuery = { ...query };
                    copiedQuery.ingoingPackets = direction.ingoingPackets;
                    copiedQuery.outgoingPackets = direction.outgoingPackets;
                    setQuery(copiedQuery);
                }}
            />

            <SearchRuleType
                value={query.black}
                onChange={(black: boolean) => {
                    let copiedQuery = { ...query };
                    copiedQuery.black = black;
                    setQuery(copiedQuery);
                }}
            />
        </Stack>
    );
};

export default Search;
