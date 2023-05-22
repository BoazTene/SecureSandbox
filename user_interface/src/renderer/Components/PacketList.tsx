import React, { useState, useEffect } from "react";
import { useListState } from "@mantine/hooks";
import Packet from "./Packet";
import { Accordion, Button, Group } from "@mantine/core";

const PacketList = () => {
    const [load, setLoad] = useState(0);
    const [count, setCount] = useState<number>(0);
    const [packetOffset, setPacketOffset] = useState<number>(0);
    const [listening, setListening] = useState<boolean>(true);
    const [packets, handlePackets] = useListState([]);

    useEffect(() => {
        window.electronAPI.onBlockedPacket((packet) =>
            handleBlockedPackets(packet)
        );

        return () => {
            window.electronAPI.removeBlockedPacket();
        };
    }, [load, count]);

    const handleBlockedPackets = (packet) => {
        console.log(count);
        if (count > 10) {
            handlePackets.remove(0);
            setCount(count - 1);
            setPacketOffset(packetOffset + 1);
        }
        setCount(count + 1);
        handlePackets.append(packet);
    };

    const toggleListening = () => {
        if (listening) {
            window.electronAPI.removeBlockedPacket();
        } else {
            handlePackets.setState([]);
            setCount(0);
            setPacketOffset(0);
            window.electronAPI.onBlockedPacket((packet) =>
                handleBlockedPackets(packet)
            );
        }

        setListening(!listening);
    };

    return (
        <>
            <Group grow mt={10}>
                <Button color="gray" onClick={toggleListening}>
                    {listening ? "Stop" : "Start"}
                </Button>
            </Group>
                <Accordion mt={10} variant="separated">
                    {packets.map((packet, packetNumber) => {
                        return (
                            <Accordion.Item value={packetNumber + ""}>
                                <Accordion.Control>
                                    Packet Number: {packetNumber + packetOffset}
                                </Accordion.Control>
                                <Accordion.Panel>
                                    <Packet packet={packet} />
                                </Accordion.Panel>
                            </Accordion.Item>
                        );
                    })}
                </Accordion>

        </>
    );
};

export default PacketList;
