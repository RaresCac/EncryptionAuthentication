#include "keyretriever.h"
#include <QDebug>

KeyRetriever::KeyRetriever(QObject *parent) : QObject(parent)
{

}

bool KeyRetriever::retrieveKey(UserFile uf, QString password)
{
    //Password derived key size
    int pdkSize = _keySize * 2;
    int saltSize;       //Will be overwritten by _QBAtoUchar
    uchar* salt = _QBAtoUchar(uf.saltArray(), saltSize);

    if (saltSize != pdkSize) {
        emit error("Error retrieving salt. Size does not match expected.");
        return false;
    }

    //First half of PBKDF2 output, used to authenticate the pass
    uchar authPDK[_keySize];

    //Second half of PBKDF2 output, used to encrypt/decrypt fileKey
    uchar encPDK[_keySize];
    _encryptor->derivePassword(password, salt, authPDK, encPDK);

    uchar hashAuthPDK[_keySize];
    uint hashSize;
    _encryptor->sha256(authPDK, sizeof (authPDK), hashAuthPDK, &hashSize);


    //Check if the stored authPDK is equal to the recently computed
    //If they are equal, it means that the password input by the user is correct
    if (!_compareQBAtoUchar(uf.hashedPDKArray().data(), hashAuthPDK, sizeof(hashAuthPDK))){
        emit error("Incorrect password.");
        return false;
    }

    int encKeySize, ivSize;
    uchar* encFileKey = _QBAtoUchar(uf.encKeyArray(), encKeySize);  //Encrypted file key
    uchar* iv = _QBAtoUchar(uf.aesIVArray(), ivSize);               //IV for AES

    uchar fileKey[_keySize];
    _encryptor->decryptAES_256_CBC(encFileKey, encKeySize, encPDK, iv, fileKey);

    _fileKey = QByteArray(reinterpret_cast<char*>(fileKey), _keySize);

    delete[] salt;
    delete[] encFileKey;
    delete[] iv;
    return true;
}

void KeyRetriever::printKey()
{
    qDebug() << _fileKey.toHex();
}

void KeyRetriever::removeFileKey()
{
    //Fills the _fileKey array with 0
    //memset(_fileKey, 0, _keySize);
    _fileKey.clear();
}

Encryptor *KeyRetriever::encryptor() const
{
    return _encryptor;
}

void KeyRetriever::setEncryptor(Encryptor *encryptor)
{
    _encryptor = encryptor;
}

int KeyRetriever::getKeySize() const
{
    return _keySize;
}

void KeyRetriever::setKeySize(int keySize)
{
    _keySize = keySize;
}

uchar* KeyRetriever::_QBAtoUchar(QByteArray qba, int& outSize)
{
    qDebug() << qba.size();
    outSize = qba.size();
    uchar* uca = new uchar[outSize];
    memcpy(uca, qba.data(), outSize);
    return uca;
}

bool KeyRetriever::_compareQBAtoUchar(QByteArray qba, uchar *uca, int inSize)
{
    return memcmp(qba.data(), uca, inSize) == 0;
}

