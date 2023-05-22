import React, { useState } from "react";
import { Radio, Group } from "@mantine/core";

interface SearchRuleTypeProps {
    onChange: (value: boolean | undefined) => void;
    value: boolean | undefined;
    withoutDefaultOption?: boolean;
}

const SearchRuleType = ({
    onChange,
    value,
    withoutDefaultOption,
}: SearchRuleTypeProps) => {
    const handleOnTypeChange = (newType: string) => {
        if (newType === "black") {
            onChange(true);
        } else if (newType === "white") {
            onChange(false);
        } else if (newType === "all") {
            onChange(undefined);
        }
    };

    const getValue = (): string => {
        if (value === undefined) {
            return "all";
        } else if (value === true) {
            return "black";
        } else if (value === false) {
            return "white";
        }
    };

    if (withoutDefaultOption) {
        return (
            <Radio.Group
                label="Black/White rule?"
                labelProps={{ style: { textAlign: "center" } }}
                defaultValue={getValue()}
                onChange={handleOnTypeChange}
                value={getValue()}
            >
                <Group>
                    <Radio ml="30px" label="black" value="black" />
                    <Radio label="white" value="white" />
                </Group>
            </Radio.Group>
        );
    } else {
        return (
            <Radio.Group
                label="Black/White rule?"
                labelProps={{ style: { textAlign: "center" } }}
                defaultValue="all"
                onChange={handleOnTypeChange}
                value={getValue()}
            >
                <Group>
                    <Radio ml="30px" label="black" value="black" />
                    <Radio label="white" value="white" />
                    <Radio label="all" value="all" />
                </Group>
            </Radio.Group>
        );
    }
};

export default SearchRuleType;
