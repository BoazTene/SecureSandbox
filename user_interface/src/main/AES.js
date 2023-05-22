const { Buffer } = require("node:buffer");
const crypto = require("crypto");
const {
    scryptSync,
    randomFill,
    createCipheriv,
    createDecipheriv,
} = require("node:crypto");

const algorithm = "aes-256-cbc";
const password = ";asdfkjDKj#@#Kjij!";

class AES {
    #key;

    constructor(key) {
        if (key === undefined) {
            this.#key = this.#generateKey();
        } else {
            this.#key = key;
        }
    }

    encrypt(message, iv) {
        if (!this.#key) {
            this.#key = this.#generateKey();
        }

        const cipher = createCipheriv(algorithm, this.#key, iv);
        let ciphertext = cipher.update(message, "utf8", "base64");
        ciphertext += cipher.final("base64");

        return ciphertext;
    }

    decrypt(ciphertext, iv) {
        if (!this.#key) {
            this.#key = this.#generateKey();
        }
        ciphertext = Buffer.from(ciphertext, "base64").toString("binary")

        const decipher = createDecipheriv(algorithm, this.#key, iv);

        let decoded = decipher.update(ciphertext, "binary", "utf8");
        decoded += decipher.final("utf8");

        return decoded;
    }

    generateIV() {
        const allowedChars =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!)";

        const ivBytes = new Uint8Array(16);
        crypto.getRandomValues(ivBytes);

        const ivStr = Array.from(ivBytes)
            .map((byte) => allowedChars[byte % allowedChars.length])
            .join("");
        const iv = new TextEncoder().encode(ivStr);

        return iv;
    }

    #generateKey() {
        return scryptSync(password, "salt", 32);
    }

    getKey() {
        return this.#key;
    }
}

module.exports = AES;
