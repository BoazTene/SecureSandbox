import React from "react";
import { Text, Accordion } from "@mantine/core";

interface PacketProps {
    packet: Array<any>;
}

const Packet = ({ packet }: PacketProps) => {
    const renderLayer = (dict) => {
        return Object.values(dict).map((value, valueNum) => {
            if (typeof value === "string") {
                return (
                    <Text size="24px"
                                            sx={{
                                                userSelect: "none",
                                                fontFamily:
                                                    "Source Sans Pro, sans-serif",
                                                color: "#888888",
                                            }}>
                        {Object.keys(dict)[valueNum]}: {value}
                    </Text>
                );
            }

            return renderLayer(value);
        });
    };
    const renderPacket = () => {
        return (
            <Accordion variant="separated">
                {packet.map((layer, i) => {
                    return (
                        <Accordion.Item value={i + ""}>
                            <Accordion.Control>
                                {Object.keys(layer)[0]}
                            </Accordion.Control>
                            <Accordion.Panel>
                                {renderLayer(layer)}
                            </Accordion.Panel>
                        </Accordion.Item>
                    );
                })}
            </Accordion>
        );
    };

    return <>{renderPacket()}</>;
};

export default Packet;
