import React from "react";
import { protocolList } from "ip-protocols";
import { Select } from "@mantine/core";

interface SearchRuleProtocolProps {
    onChange: (value: string) => void;
    defaultValue?: string;
}

const SearchRuleProtocol = ({ onChange, defaultValue }: SearchRuleProtocolProps) => {
    const generateProtocolList = (): { value: string; label: string }[] => {
        let protocols: { value: string; label: string }[] = [];

        protocolList.slice(0, -3).map((protocol) => {
            if (protocol.Name !== "" && protocol.Name !== undefined) {
                protocols.push({
                    value: protocol.Name.toLowerCase(),
                    label:
                        protocol.Name.toLowerCase() + " - " + protocol.Number,
                });
            }
        });

        return protocols;
    };

    return (
        <Select
            label="Choose a Protocol"
            defaultValue={defaultValue}
            dropdownPosition="bottom"
            data={generateProtocolList()}
            clearable
            searchable
            onChange={(value) => value === null? onChange(undefined): onChange(value)}
            placeholder="Pick one"
        />
    );
};

export default SearchRuleProtocol;
