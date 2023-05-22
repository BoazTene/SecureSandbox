import React from "react";
import { Checkbox, Group } from "@mantine/core";

export interface IRuleDirection {
    ingoingPackets: boolean;
    outgoingPackets: boolean;
}

interface SearchRuleDirectionProps {
    onChange: (value: IRuleDirection) => void;
    defaultValue?: IRuleDirection;
}

const SearchRuleDirection = ({ onChange, defaultValue }: SearchRuleDirectionProps) => {
    const onDirectionChange = (value: string[]) => {
        let ruleDirection = {
            ingoingPackets: value.includes("ingoing"),
            outgoingPackets: value.includes("outgoing"),
        };

        onChange(ruleDirection);
    };

    const getDefaultValueFromDirection = (value: IRuleDirection): string[] => {
        let result: string[] = [];
        if (value.ingoingPackets) {
            result.push("ingoing");
        }
        
        if (value.outgoingPackets) {
            result.push("outgoing");
        }

        return result;
    }

    return (
        <Checkbox.Group
            label="Rule's direction?"
            labelProps={{ style: { textAlign: "center" } }}
            onChange={onDirectionChange}
            defaultValue={defaultValue ? getDefaultValueFromDirection(defaultValue) : ["ingoing", "outgoing"]}
        >
            <Group>
                <Checkbox label="ingoing packets" value="ingoing" />
                <Checkbox label="outgoing packets" value="outgoing" />
            </Group>
        </Checkbox.Group>
    );
};

export default SearchRuleDirection;
