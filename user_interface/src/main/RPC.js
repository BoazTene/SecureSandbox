const { EventEmitter } = require("node:events");

const jsonrpc = require("jsonrpc-lite");
const Client = require("./Client.js");

class RPC extends EventEmitter {
    #client;
    #id;
    #isConnected;

    constructor() {
        super();
        this.#client = new Client();
        this.#id = 1;
        this.#isConnected = false;

        this.#client.on("connect", () => (this.#isConnected = true));
        this.#client.on("close", () => (this.#isConnected = false));
        this.#client.on("data", (data) => this.#onRecv(data));

        this.#client.setMaxListeners(1);
    }

    #onRecv(data) {
        if (!this.#isConnected) return;
        if (data === undefined) return;
        const decrypt_data = this.#client.decrypt(data.toString());
        if (decrypt_data === undefined) return;
        const rpc_packet = jsonrpc.parse(decrypt_data);

        if (rpc_packet.type === "request") {
            if (rpc_packet.payload.method === "recv_blocked_packet") {
                if (rpc_packet.payload.params.packet !== undefined) {
                    this.#handleBlockedPacket(rpc_packet.payload.params.packet);
                }
            }
        }
    }

    #ByteArrayToString(byteArray) {
        let output = "";
        for (let i = 0; i < byteArray.length; i++) {
            const byte = byteArray[i];
            if (byte >= 32 && byte <= 126) {
                output += String.fromCharCode(byte);
            } else {
                output += "\\x" + byte.toString(16).padStart(2, "0");
            }
        }

        return output;
    }

    #dictToString(dict) {
        Object.values(dict).map((value, valueNum) => {
            if (typeof value === "string") {
                dict[Object.keys(dict)[valueNum]] = this.#ByteArrayToString(
                    Uint8Array.from(Buffer.from(value, "base64"))
                );
            } else {
                console.log(typeof value);
                this.#dictToString(value);
            }
        });
    }

    #parseLogPacket(packet) {
        packet.map(layer => {
            this.#dictToString(layer);
        });

        this.emit("blocked_packet", packet);
    }

    #handleBlockedPacket(packet) {
        packet = JSON.parse(packet);
        this.#parseLogPacket(packet);
    }

    async #recvResponse(uniqueId) {
        const rpc_result = await this.#client.recv();
        const result = jsonrpc.parse(rpc_result);

        if (result.payload.id === uniqueId) {
            return result;
        } else {
            return await this.#recvResponse(uniqueId);
        }
    }

    async list_rules() {
        if (this.#isConnected === false) {
            return new Promise((resolve) => {
                this.#client.once("connect", async () =>
                    resolve(await this.list_rules())
                );
            });
        }

        const requestId = this.#id;
        this.#id++;

        const request = JSON.stringify(
            jsonrpc.request(requestId, "list_rules", {})
        );

        this.#client.send(request);
        const result = await this.#recvResponse(requestId);
        console.log(result)
        if (result.type == "success") {
            return JSON.parse(result.payload.result.rules);
        }

        throw Exception("List_rules failed");
    }

    async delete_rule(id) {
        if (this.#isConnected === false) {
            console.log("Failed because not connected");
            throw Exception("Not Connected");
        }

        const requestId = this.#id;
        this.#id++;
        const request = JSON.stringify(
            jsonrpc.request(requestId, "delete_rule", { id: id })
        );

        this.#client.send(request);
        const result = await this.#recvResponse(requestId);
        if (result.type == "success") {
            return JSON.stringify(result.payload.result);
        }

        throw Exception("delete rule failed");
    }

    async add_rule(rule) {
        if (this.#isConnected === false) {
            throw Exception("Not Connected");
        }

        const requestId = this.#id;
        this.#id++;

        const request = JSON.stringify(
            jsonrpc.request(requestId, "add_rule", rule)
        );

        this.#client.send(request);
        const result = await this.#recvResponse(requestId);
        if (result.type == "success") {
            return result.payload.result;
        }

        throw Exception("add rule failed");
    }

    onConnect(func) {
        this.#client.prependListener("connect", () => func(true));
        this.#client.prependListener("close", () => {
            if (this.#isConnected === true) {
                func(false);
            }
        });
    }

    isConnected() {
        return this.#isConnected;
    }
}

module.exports = RPC;
