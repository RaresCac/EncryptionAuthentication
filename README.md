# EncryptionAuthentication

This application will allow to generate and securely store 256 bit keys behind a username and password for convenience.

256-bit keys, along with the salt and IV, are generated randomly at the time of creation of the UserFile.

The Encryptor class uses OpenSSL 1.1.1g to implement the cryptographic functionality.

## UserFile structure

* **First 64 bytes**: Salt for PBDKF2-SHA-512 (randomly generated at registration).
* **Next 32 bytes**: Hashed first 256 bits of the password derived key, using SHA-256. 
* **Next 16 bytes**: IV for AES-256-CBC (randomly generated at registration).
* **Rest of the bytes**: Encrypted file key. The file key is generated randomly at registration, and it is encrypted with the second half of the password derived key.

The key size of fileKey is defined in the Encryptor class, and changes to it will be propagated through UserFileCreator and KeyRetriever.

While the structure of the application allows for the sizes of fileKey to be modified and propagated through the other classes, it will most likely not work at the moment as PBKDF2 is hardcoded to use SHA3-512.