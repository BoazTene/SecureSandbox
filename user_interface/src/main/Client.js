const { EventEmitter } = require("node:events");
const net = require("net");
const jsonrpc = require("jsonrpc-lite");

const Key = require("./Key.js");
const RSA = require("./RSA.js");
const Encryption = require("./Encryption.js");

const IP = "192.168.84.130";
const PORT = 9001;

class Client extends EventEmitter {
    #encryption;
    #client;
    #serverPublicKey;
    #reconnecting;

    constructor() {
        super();

        const { publicKey, privateKey } = Key.generateKeyPair();

        this.#encryption = new Encryption(new RSA(privateKey, publicKey));
        this.#client = new net.Socket();
        this.#client.on("close", () => this.#onClose());
        this.#client.on("error", () => this.#onClose());
        this.#client.on("data", (data) => this.emit("data", data));
        this.#client.on("connect", () => this.#initializeHandShake());
        this.#reconnecting = false;

        this.#connect();
    }

    #connect() {
      this.#client.connect(PORT, IP);
    }

    #onClose() {
        if (this.#reconnecting === true) return;
        this.#reconnecting = true;
        this.emit("close");
        this.#client.destroy();

        setTimeout(() => {
            this.#connect();
            this.#reconnecting = false;
        }, 1000);
    }

    async #initializeHandShake() {
        this.#serverPublicKey = undefined;
        const hello = await this.#recvEncrypted();

        if (hello !== "Hello Client") {
            this.#sendDecrypted("Handshake Failed");
            this.#client.close();
            return;
        }

        this.#sendDecrypted("Hello Server");

        const serverPublicKey = await this.#recvEncrypted();
        this.#serverPublicKey = Key.fromPEM(serverPublicKey);

        this.#sendDecrypted(this.#encryption.getPublicKey().getKeyPEM());

        this.emit("connect");
    }

    #recvEncrypted() {
        return new Promise((resolve) => {
            this.#client.once("data", (cipher) => {
                if (cipher.toString() === "Alive Check") {
                    this.#recvEncrypted();
                }

                resolve(cipher.toString());
            });
        });
    }

    #sendDecrypted(message) {
        this.#client.write(message.toString());
    }

    send(data) {
        const ciphertext = this.#encryption.encrypt(
            data,
            this.#serverPublicKey
        );

        this.#sendDecrypted(ciphertext);
    }

    decrypt(data) {
        const text = this.#encryption.decrypt(data.toString());

        if (text === undefined) {
            return text;
        }

        return text.toString();
    }

    async recv() {
        const cipher = await this.#recvEncrypted();

        const message = this.#encryption.decrypt(cipher.toString());

        return message;
    }
}

module.exports = Client;
