import React, { CSSProperties, useState } from "react";
import {
    Box,
    Menu,
    ColorSwatch,
    Text,
    Paper,
    Group,
    Stack,
    Center,
    Grid,
} from "@mantine/core";
import { ReactSVG } from "react-svg";
import { IconTrash, IconArrowsLeftRight } from "@tabler/icons-react";
import IRule from "../../Interfaces/IRule";

interface RuleProps {
    rule: IRule;
    deleteRule: (id: number) => void;
    editRule: (id: number) => void;
}

const Rule = ({ rule, deleteRule, editRule }: RuleProps) => {
    const [menuOpened, setMenuOpened] = useState<boolean>(false);

    const handleContextMenu = (event) => {
        event.preventDefault();
        setMenuOpened(!menuOpened);
    };

    const handleEditRule = (event) => {
        setMenuOpened(false);
        editRule(rule.id);
    };

    const handleDeleteRule = (event) => {
        setMenuOpened(false);
        deleteRule(rule.id);
    };

    return (
        <Box onContextMenu={handleContextMenu}>
            <Menu opened={menuOpened} onClose={() => setMenuOpened(false)}>
                <Menu.Target>
                    <Paper radius="md" shadow="sm">
                        <Group grow ml="3vw">
                            <Stack>
                                <Text
                                    size="32px"
                                    fw={700}
                                    sx={{
                                        userSelect: "none",
                                        fontFamily: "Oswold, sans-serif",
                                    }}
                                >
                                    {rule.name}
                                </Text>
                                <Group ml="3vw">
                                    <Stack>
                                        <Text
                                            size="24px"
                                            sx={{
                                                userSelect: "none",
                                                fontFamily:
                                                    "Source Sans Pro, sans-serif",
                                                color: "#888888",
                                            }}
                                        >
                                            {rule.black
                                                ? "blocking"
                                                : "only allowed"}{" "}
                                        </Text>
                                        <Text
                                            size="24px"
                                            sx={{
                                                userSelect: "none",
                                                fontFamily:
                                                    "Source Sans Pro, sans-serif",
                                                color: "#888888",
                                            }}
                                        >
                                            {rule.srcIP} ----&gt; {rule.dstIP}
                                            {" over "}
                                            {rule.protocol}{" "}
                                        </Text>
                                        <Text
                                            size="24px"
                                            sx={{
                                                userSelect: "none",
                                                fontFamily:
                                                    "Source Sans Pro, sans-serif",
                                                color: "#888888",
                                            }}
                                        >
                                            source port: {rule.srcPort}
                                        </Text>
                                        <Text
                                            size="24px"
                                            sx={{
                                                userSelect: "none",
                                                fontFamily:
                                                    "Source Sans Pro, sans-serif",
                                                color: "#888888",
                                            }}
                                        >
                                            destination port: {rule.dstPort}
                                        </Text>
                                        <Text
                                            size="24px"
                                            sx={{
                                                userSelect: "none",
                                                fontFamily:
                                                    "Source Sans Pro, sans-serif",
                                                color: "#888888",
                                            }}
                                        >
                                            {rule.ingoingPackets &&
                                            rule.outgoingPackets
                                                ? "ingoing and outgoing packets"
                                                : rule.ingoingPackets
                                                ? "ingoing packets"
                                                : "outgoing packets"}
                                        </Text>
                                    </Stack>
                                </Group>
                            </Stack>
                        </Group>
                    </Paper>
                </Menu.Target>

                <Menu.Dropdown>
                    <Menu.Label>Rule Settings</Menu.Label>
                    <Menu.Item onClick={handleEditRule}>Edit rule</Menu.Item>
                    <Menu.Item
                        onClick={handleDeleteRule}
                        color="red"
                        icon={<IconTrash size={14} />}
                    >
                        Delete rule
                    </Menu.Item>
                </Menu.Dropdown>
            </Menu>
        </Box>
    );
};

export default Rule;
