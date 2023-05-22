/**
 * This file is used to declare the main process api.
 * It should be imported only on the main renderer file.
 */

import IRule from "../Interfaces/IRule";

declare global {
    interface Window {
        electronAPI: {
            listRules: () => Promise<IRule[]>;
            deleteRule: (id: number) => Promise<boolean>;
            addRule: (rule: IRule) => Promise<boolean>;
            onConnect: (callback: (isConnected: boolean) => void) => Promise<void>;
            onBlockedPacket: (callback: (packet: any) => void) => Promise<void>;
            removeBlockedPacket: () => Promise<void>;
            removeConnect: () => Promise<void>;
            isConnected: () => boolean;
        };
    }
}
