import React, { useState, useEffect } from "react";
import { Tooltip, ColorSwatch, useMantineTheme, Tabs } from "@mantine/core";

const AppHeader = () => {
    const [render, _] = useState<boolean>(true); // used for renderering purposes (ignore).
    const [isConnected, setIsConnected] = useState<boolean>(
        window.electronAPI.isConnected()
    );

    const theme = useMantineTheme();

    useEffect(() => {
        window.electronAPI.onConnect(setIsConnected);

        return () => {
            window.electronAPI.removeConnect();
        };
    }, [render]);

    return (
        <>
            <Tabs.List>
                <Tabs.Tab value="rule-editor">Rule Editor</Tabs.Tab>
                <Tabs.Tab value="packet-viewer">Packet Viewer</Tabs.Tab>
            </Tabs.List>

            <Tooltip
                label={isConnected === true ? "Connected" : "Disconnected"}
            >
                <ColorSwatch
                    color={
                        isConnected === true
                            ? theme.colors["green"][6]
                            : theme.colors["red"][6]
                    }
                    pos="absolute"
                    right={0}
                    mr={10}
                />
            </Tooltip>
        </>
    );
};

export default AppHeader;
