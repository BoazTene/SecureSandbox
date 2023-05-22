import React from "react";
import { Title, Center } from "@mantine/core";
import PacketList from "./Components/PacketList";

const PacketViewer = () => {
    return (
        <div style={{ marginTop: "60px" }}>
            <Center>
                <Title>Blocked Packets</Title>
            </Center>
            <PacketList />
        </div>
    );
};

export default PacketViewer;
