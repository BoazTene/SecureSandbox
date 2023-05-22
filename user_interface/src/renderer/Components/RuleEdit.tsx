import React, { useState, useEffect } from "react";
import { Modal, TextInput, Button, Stack, NumberInput } from "@mantine/core";
import SearchRuleProtocol from "./SearchRuleProtocol";
import SearchRuleType from "./SearchRuleType";
import SearchRuleDirection from "./SearchRuleDirection";
import IRule from "../../Interfaces/IRule";

interface RuleEditProps {
    defaultRule?: IRule;
    opened: boolean;
    setClose: () => void;
    setRule: (rule: IRule) => void;
}

const RuleEdit = ({
    defaultRule,
    opened,
    setClose,
    setRule,
}: RuleEditProps) => {
    const [rule, setCurrentRule] = useState<IRule>(
        defaultRule !== undefined
            ? defaultRule
            : {
                  id: -1,
                  name: "",
                  srcIP: "",
                  dstIP: "",
                  ingoingPackets: true,
                  black: true,
                  outgoingPackets: true,
                  log: true,
                  srcPort: 0,
                  dstPort: 0,
                  protocol: "",
              }
    );

    useEffect(() => {
        console.log(rule.black);
        if (defaultRule !== undefined) setCurrentRule(defaultRule);
    }, [defaultRule]);

    const handleSubmit = (event) => {
        setRule(rule);
        setClose();
    };

    return (
        <Modal opened={opened} onClose={setClose} title="Rule Edit">
            <Stack>
                <TextInput
                    label="Rule Name:"
                    value={rule.name}
                    onChange={(event) =>
                        setCurrentRule({
                            ...rule,
                            name: event.currentTarget.value,
                        })
                    }
                />
                <TextInput
                    label="Source IP:"
                    value={rule.srcIP}
                    onChange={(event) =>
                        setCurrentRule({
                            ...rule,
                            srcIP: event.currentTarget.value,
                        })
                    }
                />
                <TextInput
                    label="Destination IP:"
                    value={rule.dstIP}
                    onChange={(event) =>
                        setCurrentRule({
                            ...rule,
                            dstIP: event.currentTarget.value,
                        })
                    }
                />
                <SearchRuleProtocol
                    defaultValue={rule.protocol}
                    onChange={(value: string) => {
                        setCurrentRule({ ...rule, protocol: value });
                    }}
                />
                <NumberInput label="Source Port" defaultValue={rule.srcPort} onChange={(value: number) => {
                    setCurrentRule({...rule, srcPort: value});  
                }}/>
                <NumberInput label="Destination Port" defaultValue={rule.dstPort} onChange={(value: number) => {
                    setCurrentRule({...rule, dstPort: value});  
                }}/>
                <SearchRuleType
                    withoutDefaultOption={true}
                    onChange={(value: boolean | undefined) => {
                        setCurrentRule({
                            ...rule,
                            black: value !== undefined ? value : rule.black,
                        });
                    }}
                    value={rule.black}
                />
                <SearchRuleDirection
                    defaultValue={{
                        ingoingPackets: rule.ingoingPackets,
                        outgoingPackets: rule.outgoingPackets,
                    }}
                    onChange={(value) => {
                        setCurrentRule({ ...rule, ...value });
                    }}
                />

                <Button onClick={handleSubmit} variant="outline">Submit</Button>
            </Stack>
        </Modal>
    );
};

export default RuleEdit;
