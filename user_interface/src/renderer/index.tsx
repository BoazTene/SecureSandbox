import "./global";
import React, { useState, useEffect } from "react";
import { useDisclosure } from "@mantine/hooks";
import ReactDOM from "react-dom";
import {
    AppShell,
    Navbar,
    Header,
    MediaQuery,
    Burger,
    MantineProvider,
    ScrollArea,
    Tabs,
} from "@mantine/core";
import RuleViewer from "./RuleViewer";
import PacketViewer from "./PacketViewer";
import AppHeader from "./AppHeader";
import { getFilterRules, defaultSearchFilter } from "./Helpers/Search";

const App = () => {
    return (
        <MantineProvider
            theme={{ colorScheme: "dark" }}
            withGlobalStyles
            withNormalizeCSS
        >
            <Tabs defaultValue="rule-editor">
                <div style={{ position: "fixed", top: 0, width: "100%", zIndex: 9999 }}>
                    <Header height={{ base: 50 }} p="md">
                        <div
                            style={{
                                display: "flex",
                                alignItems: "center",
                                height: "100%",
                            }}
                        >
                            <AppHeader />
                        </div>
                    </Header>
                </div>
                <Tabs.Panel value="rule-editor" pt="xs">
                    <RuleViewer />
                </Tabs.Panel>
                <Tabs.Panel value="packet-viewer" pt="xs">
                    <PacketViewer/>
                </Tabs.Panel>
            </Tabs>
        </MantineProvider>
    );
};

ReactDOM.render(<App />, document.getElementById("root"));
