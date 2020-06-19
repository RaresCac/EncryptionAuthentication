# EncryptionAuthentication

This application will allow to generate and securely store 256 bit keys behind a username and password for convenience.

256-bit keys are generated randomly at the time of creation of the UserFile.

The Encryptor class uses OpenSSL 1.1.1g to implement the cryptographic functionality.

## UserFile structure

First 64 bytes: Salt for PBDKF2-SHA-512 (randomly generated at registration).
Next 16 bytes: IV for AES-256-CBC (randomly generated at registration).
Next 32 bytes: Hashed (using SHA-256) first 256 bits of the password derived key.
Rest of the bytes: Encrypted file key. The file key is generated randomly at registration, and it is encrypted with the second half of the password derived key.
