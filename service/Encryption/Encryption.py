import base64

from .rsa import RSA
from .aes import AES


class Encryption:
    """
    Used To encrypt messages.

    Encryption process:
        1. Generating AES 256 key and iv on the fly.
        2. Encrypting the AES key with rsa encryption.
        3. Encrypting the data the AES key.

    Encrypted data:
        key: rsa_encrypted_aes_key (base64)
        iv: iv (base64)
        data: data aes encrypted (base64)

    Decryption is the other way around.
    """

    def __init__(self):
        self.__rsa = RSA()

    def encrypt(self, message: bytes) -> bytes:
        """
        Encryptes the message.
        """
        aes = AES()

        cipher_data, iv = aes.encrypt(message)
        cipher_key = self.__rsa.encrypt(aes.get_key())

        return self.__build_data(cipher_key, iv, cipher_data)

    def decrypt(self, cipher: bytes) -> bytes:
        """
        Decrypts a cipher message.

        Retuns plain text in utf8 encoding.
        """

        cipher_key, iv, cipher_data = self.__parse_data(cipher)

        aes_key = self.__rsa.decrypt(cipher_key)

        aes = AES(key=aes_key)

        data = aes.decrypt(cipher_data, iv)

        return data

    def get_rsa_public_key(self):
        """
        Returns the rsa public key in PEM format and in PKCS1 encoding
        """

        return self.__rsa.get_public_key()

    def set_rsa_encryption_key(self, key: bytes):
        self.__rsa.set_encryption_public_key(key)

    def __build_data(self, cipher_key: bytes, iv: bytes, cipher_data: bytes) -> bytes:
        """
        Builds an encrypted message.

        Generates the format of the message.

        Returns the message in bytes.
        """

        return b"key: %s\niv: %s\ndata: %s" % (cipher_key, iv, cipher_data)

    def __parse_data(self, cipher: bytes) -> tuple[bytes, bytes, bytes]:
        """
        Parses an encrypted message.

        Returns a tuple that includes the key, iv, data of the cipher.
        """

        try:
            key = cipher.splitlines()[0].split(b"key: ")[1]
            iv = base64.b64decode(cipher.splitlines()[1].split(b"iv: ")[1])
            data = cipher.split(b"data: ")[1]
        except Exception:
            raise Exception("Parsing failed")

        return key, iv, data
