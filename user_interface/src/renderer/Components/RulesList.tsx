import React, { useRef } from "react";
import {
    Stack,
    Group,
    Text,
    Center,
    ScrollArea,
    Button,
    useMantineTheme,
} from "@mantine/core";
import IRule from "../../Interfaces/IRule";
import Rule from "./Rule";

interface RulesListProps {
    rules: IRule[];
    deleteRule: (id: number) => void;
    addRule: () => void;
    editRule: (id: number) => void;
    refreshData: () => void;
}

const RulesList = ({
    rules,
    deleteRule,
    addRule,
    editRule,
    refreshData,
}: RulesListProps) => {
    const theme = useMantineTheme();

    return (
        <Stack>
            <Center>
                <Stack>
                    <Text
                        fw={700}
                        size="48px"
                        sx={{
                            userSelect: "none",
                            fontFamily: "Oswold, sans-serif",
                        }}
                    >
                        Firewall Rules
                    </Text>
                    <Group grow>
                        <Button
                            variant="outline"
                            color="grey"
                            onClick={refreshData}
                        >
                            Refresh data
                        </Button>
                        <Button
                            variant="outline"
                            color="gray"
                            onClick={addRule}
                        >
                            New Rule
                        </Button>
                    </Group>
                </Stack>
            </Center>
            {rules.map((rule) => (
                <Rule
                    key={rule.id}
                    rule={rule}
                    deleteRule={deleteRule}
                    editRule={editRule}
                />
            ))}
        </Stack>
    );
};

export default RulesList;
