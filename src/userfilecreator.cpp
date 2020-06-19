#include "userfilecreator.h"
#include <QIODevice>
#include <QSaveFile>
#include <QDebug>
#include <QDir>

UserFileCreator::UserFileCreator(QObject *parent) : QObject(parent)
{
    connect(&_encryptor, &Encryptor::errorOccured, this, &UserFileCreator::error);
}

bool UserFileCreator::generateFile(QString username, QString password)
{
    UserFile userFile;

    int keySize = 32;

    _encryptor.setKeySize(keySize);

    int pdkSize = keySize * 2;
    int aesBlockSize = AES_BLOCK_SIZE;

    //First, generate key to be kept secret
    //Will be used to encrypt other files
    uchar* fileKey = _encryptor.getRandomKey(keySize);   //256 bit key, for AES 256 CBC
    uchar* pbkdf2Salt = _encryptor.getRandomKey(pdkSize);
    uchar* aesIV = _encryptor.getRandomKey(aesBlockSize);

    uchar storedPDKHalf[keySize];    //Will be stored hashed, used for authentication
    uchar encPDKHalf[keySize];       //Will encrypt the fileKey
    _encryptor.derivePassword(password, pbkdf2Salt, storedPDKHalf, encPDKHalf);

    uchar hashedStoredPDK[keySize];
    uint hashSize;
    _encryptor.sha256(storedPDKHalf, keySize, hashedStoredPDK, &hashSize);

    uchar encryptedFileKey[keySize];
    _encryptor.encryptAES_256_CBC(fileKey, keySize, encPDKHalf, aesIV, encryptedFileKey);

    //Everything needed is now computed, so save in userFile
    userFile.setSaltArray(QByteArray(reinterpret_cast<char*>(pbkdf2Salt), pdkSize));
    userFile.setAesIVArray(QByteArray(reinterpret_cast<char*>(aesIV), aesBlockSize));
    userFile.setHashedPDKArray(QByteArray(reinterpret_cast<char*>(&hashedStoredPDK), keySize));
    userFile.setEncKeyArray(QByteArray(reinterpret_cast<char*>(&encryptedFileKey), keySize));

    userFile.setFileName(username);
    _saveFile(userFile);

    delete[] fileKey;
    delete[] pbkdf2Salt;
    delete[] aesIV;

    return true;
}

bool UserFileCreator::loadFile(QString username, QString password)
{
    //todo
}

void UserFileCreator::error(int id)
{
    _handleError(id);
}

bool UserFileCreator::_saveFile(UserFile uf)
{
    QDir dir("saves");
    if (!dir.exists())
        dir.mkpath(".");

    QString fileName = "saves/" + uf.getFileName() + ".save";
    QSaveFile save(fileName);

    save.open(QIODevice::WriteOnly);
    save.write(uf.saltArray());
    save.write(uf.aesIVArray());
    save.write(uf.hashedPDKArray());
    save.write(uf.encKeyArray());
    return save.commit();
}

bool UserFileCreator::_loadFile(UserFile uf)
{
    //todo
}

void UserFileCreator::_handleError(int id)
{
    emit errorMessage("Error");
}
