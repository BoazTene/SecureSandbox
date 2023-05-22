import base64
import os
from cryptography.hazmat.primitives import (
    serialization,
    hashes,
    padding as prim_padding,
)
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend


class AES:
    """
    An implemention of AES-256-cbc encryption.
    """

    # allowed characters on key generation
    ALLOWED_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!)"

    def __init__(self, key=None):
        if key is None:
            self.__key = self.__generate_key()
        else:
            self.__key = key

    def encrypt(self, message: bytes) -> tuple[bytes, bytes]:
        """
        Encryptes the message with the aes-256 key.

        Returns a tuple of the encrypted aes message with
        base64 encoding and the iv used with base64 encoding.
        """

        iv = self.__generate_aes_iv()

        padder = prim_padding.PKCS7(algorithms.AES.block_size).padder()
        padded_data = padder.update(message) + padder.finalize()
        cipher = Cipher(
            algorithms.AES(self.__key), modes.CBC(iv), backend=default_backend()
        )
        encryptor = cipher.encryptor()
        aes_ciphertext = encryptor.update(padded_data) + encryptor.finalize()

        return base64.b64encode(aes_ciphertext), base64.b64encode(iv)

    def decrypt(self, ciphertext: bytes, iv: bytes) -> bytes:
        """
        Decryptes the cipher text with the aes 256 key and the iv.

        Returns text in utf-8 format (readable).
        """

        ciphertext = base64.b64decode(ciphertext)

        cipher = Cipher(
            algorithms.AES(self.__key), modes.CBC(iv), backend=default_backend()
        )

        decryptor = cipher.decryptor()
        plain_text = decryptor.update(ciphertext) + decryptor.finalize()
        unpadder = prim_padding.PKCS7(algorithms.AES.block_size).unpadder()
        plain_text = unpadder.update(plain_text)
        plain_text += unpadder.finalize()

        return plain_text

    def get_key(self) -> bytes:
        """
        Returns the aes key in bytes.
        """

        return self.__key

    def __generate_key(self) -> bytes:
        """
        Generates a new aes key (32 bytes).
        """

        key_bytes = os.urandom(32)
        key_str = "".join(
            [AES.ALLOWED_CHARS[b % len(AES.ALLOWED_CHARS)] for b in key_bytes]
        )
        key = key_str.encode("utf-8")

        return key

    def __generate_aes_iv(self) -> bytes:
        """
        Generates a new aes iv (16 bytes).
        """

        key_bytes = os.urandom(16)
        iv_str = "".join(
            [AES.ALLOWED_CHARS[b % len(AES.ALLOWED_CHARS)] for b in key_bytes]
        )
        iv = iv_str.encode("utf-8")

        return iv
