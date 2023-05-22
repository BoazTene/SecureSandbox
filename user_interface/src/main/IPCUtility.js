const { ipcMain } = require("electron");
const RPC = require("./RPC.js");

class IPCUtility {
    #rpc;
    constructor() {
        this.#rpc = new RPC();
    }

    registerHooks(mainWindow) {
        ipcMain.handle("list-rules", (event) => this.#rpc.list_rules());
        ipcMain.handle("delete-rule", (event, id) => this.#rpc.delete_rule(id));
        ipcMain.handle("add-rule", (event, rule) => this.#rpc.add_rule(rule));

        this.#rpc.onConnect((isConnected) =>
            this.#onConnect(mainWindow, isConnected)
        );

        ipcMain.on("is-connected", (event) => {
            event.returnValue = this.#rpc.isConnected();
        });

        this.#rpc.on("blocked_packet", (packet) => this.#onBlockedPacket(mainWindow, packet));
    }

    #onBlockedPacket(mainWindow, packet) {
        mainWindow.webContents.send("blocked_packet", packet);
    }

    #onConnect(mainWindow, isConnected) {
        mainWindow.webContents.send("is-connected", isConnected);
    }
}

module.exports = IPCUtility;
