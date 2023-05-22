const RSA = require("./RSA.js");
const AES = require("./AES.js");

class Encryption {
    #rsa;

    constructor(rsa) {
        this.#rsa = rsa;
    }

    encrypt(message, publicKey) {
        const aes = new AES();

        const encryptedKey = this.#rsa.encrypt(aes.getKey(), publicKey);
        let iv = aes.generateIV();

        const encryptedMessage = aes.encrypt(message, iv);

        iv = Buffer.from(iv).toString("base64");

        const data =
            "key: " +
            encryptedKey +
            "\niv:  " +
            iv +
            "\ndata: " +
            encryptedMessage;

        return data;
    }

    decrypt(ciphertext) {
        try {
            let key;
            let iv;
            let body;

            try {
                key = ciphertext.split("\n")[0].split("key: ")[1];
                iv = ciphertext.split("\n")[1].split("iv: ")[1];
                body = ciphertext.split("data: ")[1];
            } catch (e) {
                return;
            }

            const aes_key = this.#rsa.decrypt(key);
            iv = Buffer.from(iv, "base64").toString("utf-8");

            const aes = new AES(aes_key);
            let data = aes.decrypt(body.toString("utf-8"), iv);

            return data;
        } catch (e) {
            return undefined;
        }
    }

    getPublicKey() {
        return this.#rsa.getPublicKey();
    }
}

module.exports = Encryption;
