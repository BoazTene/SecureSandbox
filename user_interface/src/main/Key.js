const crypto = require('crypto');

class Key {
    #key;

    /**
     * Builds a new key (private/public).
     *
     * The key is saved as PEM (always).
     *
     * @param key the key in PEM format.
     */
    constructor(key) {
        this.#key = key;
    }

    getKeyPEM() {
        return this.#key;
    }

    static fromPEM(key) {
        return new Key(key);
    }

    static generateKeyPair() {
        const { publicKey, privateKey } = crypto.generateKeyPairSync("rsa", {
            modulusLength: 2048,
            publicKeyEncoding: {
                type: "pkcs1",
                format: "pem",
            },
            privateKeyEncoding: {
                type: "pkcs1",
                format: "pem",
            },
        });

        return {
            publicKey: Key.fromPEM(publicKey),
            privateKey: Key.fromPEM(privateKey),
        };
    }
}

module.exports = Key;
