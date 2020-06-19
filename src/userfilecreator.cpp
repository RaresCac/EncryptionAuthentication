#include "userfilecreator.h"
#include <QIODevice>
#include <QSaveFile>
#include <QDebug>

UserFileCreator::UserFileCreator(QObject *parent) : QObject(parent)
{
    //Connect errorOccured with handleError so that it is invoked automatically
    //connect(&_encryptor, SIGNAL(errorOccured(int id)), this, SLOT(error(int id)));
    connect(&_encryptor, &Encryptor::errorOccured, this, &UserFileCreator::error);
}

bool UserFileCreator::generateFile(QString username, QString password)
{
    UserFile userFile;

    int keySize = 32;
    int pdkSize = 64;
    int aesBlockSize = 16;

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

void UserFileCreator::error(int id)
{
    _handleError(id);
}

bool UserFileCreator::_saveFile(UserFile uf)
{
    QString fileName = "saves/" + uf.getFileName() + ".save";
    QSaveFile save(fileName);
    qDebug() << fileName;
    save.open(QIODevice::WriteOnly);
    save.write(uf.saltArray());
    save.write(uf.aesIVArray());
    save.write(uf.hashedPDKArray());
    save.write(uf.encKeyArray());
    return save.commit();
}

void UserFileCreator::_handleError(int id)
{
    emit errorMessage("Error");
}
