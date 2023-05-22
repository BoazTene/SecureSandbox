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
} from "@mantine/core";
import Search from "./Components/Search";
import Title from "./Components/Title";
import RulesList from "./Components/RulesList";
import RuleEdit from "./Components/RuleEdit";
import IRule from "../Interfaces/IRule";
import ISearch from "../Interfaces/ISearch";
import AppHeader from "./AppHeader";
import { getFilterRules, defaultSearchFilter } from "./Helpers/Search";

const RuleViewer = () => {
    const [rules, setRules] = useState<IRule[]>([]);
    const [editOpened, { open, close }] = useDisclosure(false);
    const [searchOpened, setSearchOpened] = useState<boolean>(false);
    const [edittedRule, setEditRule] = useState<IRule | undefined>(undefined);
    const [searchFilter, setSearchFilter] =
        useState<ISearch>(defaultSearchFilter);

    useEffect(() => {
        updateRules();
    }, [editOpened]);

    const updateRules = () => {
        return new Promise((resolve) => {
            window.electronAPI.listRules().then((result) => {
                setRules(result);
                resolve(true);
            });
        });
    };

    const addRule = (rule: IRule) => {
        window.electronAPI.addRule(rule).then((success) => {
            if (success === true) {
                updateRules();
            }
        });
    };

    const deleteRule = (id: number) => {
        console.log("id", id);
        return new Promise((resolve, reject) => {
            window.electronAPI.deleteRule(id).then((success) => {
                console.log(success);
                if (success === true) {
                    updateRules().then(() => {
                        resolve(true);
                    });
                } else {
                    reject(false);
                }
            });
        });
    };

    const editRule = (rule: IRule) => {
        if (rule.id == -1) {
            addRule(rule);
            return;
        }

        deleteRule(rule.id).then((result) => {
            if (result == true) {
                addRule(rule);
            }
        });
    };

    return (
        <AppShell
            navbar={
                <MediaQuery smallerThan="sm" styles={{ display: "none" }}>
                    <Navbar hiddenBreakpoint="sm" width={{ sm: 350 }}>
                        <Navbar.Section grow component={ScrollArea}>
                            <Search
                                rules={rules}
                                setSearchFilter={setSearchFilter}
                            />
                        </Navbar.Section>
                    </Navbar>
                </MediaQuery>
            }
            styles={(theme) => ({
                main: {
                    backgroundColor:
                        theme.colorScheme === "dark"
                            ? theme.colors.dark[8]
                            : theme.colors.gray[0],
                },
            })}
        >
            <RulesList
                rules={getFilterRules(searchFilter, rules)}
                deleteRule={(id: number) => deleteRule(id)}
                refreshData={() => updateRules()}
                addRule={() => {
                    setEditRule(undefined);
                    open();
                }}
                editRule={(id: number) => {
                    setEditRule(
                        rules.filter((rule: IRule) => id == rule.id)[0]
                    );
                    open();
                }}
            />
            <RuleEdit
                opened={editOpened}
                defaultRule={edittedRule}
                setRule={editRule}
                setClose={close}
            />
        </AppShell>
    );
};

export default RuleViewer;
