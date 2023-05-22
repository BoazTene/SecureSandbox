const crypto = require('crypto');

class RSA {
    #privateKey;
    #publicKey;

    constructor(privateKey, publicKey) {
        this.#privateKey = privateKey;
        this.#publicKey = publicKey;
    }

    encrypt(plaintext, publicKey) {
        const cypher = crypto.publicEncrypt({key: publicKey.getKeyPEM(), oaepLabel: "", oaepHash: "sha256", padding: crypto.constants.RSA_PKCS1_OAEP_PADDING}, Buffer.from(plaintext, 'utf-8'));

        return cypher.toString("base64");
    }

    decrypt(cypher) {
        let buffer = Buffer.from(cypher, 'base64');
        
        let plaintext = crypto.privateDecrypt({ key: this.#privateKey.getKeyPEM(), oaepLabel: "", oaepHash: "sha256", padding: crypto.constants.RSA_PKCS1_OAEP_PADDING}, buffer);

        return plaintext.toString('utf8');
    }

    getPublicKey() {
        return this.#publicKey;
    }
}

module.exports = RSA;
