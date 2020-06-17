#include "encryptor.h"
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

Encryptor::Encryptor(QObject *parent) : QObject(parent)
{

}

bool Encryptor::generateFileQML(QString username, QString password)
{
    return _generateFile(username, password);
}


bool Encryptor::_generateFile(QString username, QString password){

    //Key to be used for encryption of other files
    uchar fileKey[_encKeySize];
    _generateRandomBytes(fileKey, _encKeySize);

    //Salt for PBKDF2-SHA-512
    uchar salt[_derivedKeySize];
    _generateRandomBytes(salt, _derivedKeySize);

    uchar derivedPass[_derivedKeySize];
    _derivePassword(username.toUtf8().data(), salt, _derivedKeySize, derivedPass);

    //To be hashed and stored in the user file
    //Used to authenticate the user from the password
    uchar storedHalf[_derivedKeySize/2];
    //Used to encrypt the fileKey
    uchar encHalf[_derivedKeySize/2];

    _splitArrayHalf(derivedPass, _derivedKeySize, storedHalf, encHalf);

    uchar iv[_ivSize];
    _generateRandomBytes(iv, _ivSize);

    uchar encryptedFileKey[_encKeySize];
    _encryptAES_256_CBC(fileKey, _encKeySize, encHalf, iv, encryptedFileKey);



}

bool Encryptor::_generateRandomBytes(uchar *array, size_t size){
    return RAND_bytes(array, size) == 1;
}

int Encryptor::_derivePassword(char *password, uchar *salt, size_t size, uchar *output)
{
    return PKCS5_PBKDF2_HMAC(password, size, salt, size, size, EVP_sha3_512(), size, output);
}

int Encryptor::_encryptAES_256_CBC(uchar *input, size_t inLen, uchar *key, uchar *iv, uchar *output)
{
    EVP_CIPHER_CTX* ctx;

    //Temporary length
    int len;
    //Output length of the resulting ciphertext
    int outLen;

    //Create contex
    if(!(ctx = EVP_CIPHER_CTX_new())) return false;

    //Initialize the algorithm to encrypt with the passed key and IV.
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) return false;

    //Encrypts the
    if(1 != EVP_EncryptUpdate(ctx, output, &len, input, inLen)) return false;
    outLen = len;

    //Encrypt last data that does not fill a block
    if(1 != EVP_EncryptFinal_ex(ctx, output + len, &len))
    outLen += len;

    EVP_CIPHER_CTX_free(ctx);

    return outLen;
}

void Encryptor::_splitArrayHalf(uchar *input, size_t inSize, uchar *half1, uchar *half2)
{
    size_t outLen = inSize / 2;
    memcpy(half1, input, outLen);
    memcpy(half2, input + outLen, outLen);
}

int Encryptor::_sha256(uchar *input, size_t inSize, uchar *output, uint* outSize)
{
    EVP_MD_CTX* ctx;

    if (!(ctx = EVP_MD_CTX_new())) return -1;

    if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr)) return -1;

    if (1 != EVP_DigestUpdate(ctx, input, inSize)) return -1;

    if (1 != EVP_DigestFinal_ex(ctx, output, outSize)) return -1;

    EVP_MD_CTX_free(ctx);
    return 0;
}

void Encryptor::_handleErrorMessage(int errorCode)
{
    switch (errorCode) {
        default: emit errorMessage("Unspecified error.");
    }
}
