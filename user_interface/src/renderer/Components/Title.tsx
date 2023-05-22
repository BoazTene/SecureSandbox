import React from "react";
import { Text, Center } from "@mantine/core";

const Title = () => {
    return (
        <Center>
        <Text
            fw={700}
            sx={{
                userSelect: "none",
                fontFamily: "Oswold, sans-serif",
            }}
        >
            Firewall Rules
        </Text>
        </Center>
    );
};

export default Title;
