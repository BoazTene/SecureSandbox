import base64
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import serialization, hashes


class RSA:
    """
    An implemention of RSA encryption with OAEP padding.
    """

    def __init__(self):
        self.__private_key, self.__public_key = self.__generate_keys()
        self.__encryption_public_key = self.__public_key

    def encrypt(self, message: bytes) -> bytes:
        """
        Encryptes the message using setted public key.

        Returns the encrypted message in base64 encoding.
        """

        cipher_text = self.__encryption_public_key.encrypt(
            message,
            padding.OAEP(
                mgf=padding.MGF1(algorithm=hashes.SHA256()),
                algorithm=hashes.SHA256(),
                label=None,
            ),
        )
        cipher_text = base64.b64encode(cipher_text)

        return cipher_text

    def decrypt(self, cipher: bytes) -> bytes:
        """
        Decryptes the cipher using the private key.

        The cipher should be in base64 encoding.

        Returns the message in bytes (readable).
        """

        cipher = base64.b64decode(cipher)

        plain_text = self.__private_key.decrypt(
            cipher,
            padding.OAEP(
                mgf=padding.MGF1(algorithm=hashes.SHA256()),
                algorithm=hashes.SHA256(),
                label=None,
            ),
        )

        return plain_text

    def get_public_key(self) -> bytes:
        """
        Returns the rsa public key in PEM encoding and PKCS1 format.
        """

        return self.__public_key.public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.PKCS1,
        )

    def set_encryption_public_key(self, public_key: bytes) -> None:
        """
        Sets the rsa public key that will be used on encryption.
        """

        self.__encryption_public_key = serialization.load_pem_public_key(
            public_key)

    def __generate_keys(self) -> tuple[rsa.RSAPrivateKey, rsa.RSAPublicKey]:
        """
        Generates an rsa (2048) private and public keys.
        """

        private_key = rsa.generate_private_key(
            public_exponent=65537, key_size=2048)

        return private_key, private_key.public_key()
