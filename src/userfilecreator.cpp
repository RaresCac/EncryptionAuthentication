#include "userfilecreator.h"
#include <QIODevice>
#include <QSaveFile>
#include <QDebug>
#include <QDir>
#include <QtGlobal>

UserFileCreator::UserFileCreator(QObject *parent) : QObject(parent)
{
    _keyRetriever.setEncryptor(&_encryptor);
    connect(&_encryptor, &Encryptor::errorOccured, this, &UserFileCreator::error);
    connect(&_keyRetriever, &KeyRetriever::error, this, &UserFileCreator::errorWithMessage);
}

bool UserFileCreator::generateFile(QString username, QString password)
{
    UserFile userFile;

    _encryptor.setKeySize(_keySize);

    //Password derived key size
    int pdkSize = _keySize * 2;
    int aesBlockSize = AES_BLOCK_SIZE;

    //First, generate key to be kept secret
    //Will be used to encrypt other files
    uchar* fileKey = _encryptor.getRandomKey(_keySize);   //256 bit key, for AES 256 CBC
    uchar* pbkdf2Salt = _encryptor.getRandomKey(pdkSize);
    uchar* aesIV = _encryptor.getRandomKey(aesBlockSize);

    uchar storedPDKHalf[_keySize];    //Will be stored hashed, used for authentication
    uchar encPDKHalf[_keySize];       //Will encrypt the fileKey
    _encryptor.derivePassword(password, pbkdf2Salt, storedPDKHalf, encPDKHalf);

    uchar hashedStoredPDK[_keySize];
    uint hashSize;
    _encryptor.sha256(storedPDKHalf, _keySize, hashedStoredPDK, &hashSize);

    uchar encryptedFileKey[_keySize];
    _encryptor.encryptAES_256_CBC(fileKey, _keySize, encPDKHalf, aesIV, encryptedFileKey);

    //Everything needed is now computed, so save in userFile
    userFile.setSaltArray(QByteArray(reinterpret_cast<char*>(pbkdf2Salt), pdkSize));
    userFile.setAesIVArray(QByteArray(reinterpret_cast<char*>(aesIV), aesBlockSize));
    userFile.setHashedPDKArray(QByteArray(reinterpret_cast<char*>(&hashedStoredPDK), _keySize));
    userFile.setEncKeyArray(QByteArray(reinterpret_cast<char*>(&encryptedFileKey), _keySize));

    userFile.setFileName(username);
    _saveFile(userFile);

    delete[] fileKey;
    delete[] pbkdf2Salt;
    delete[] aesIV;

    return true;
}

bool UserFileCreator::loadFile(QString username, QString password)
{
    UserFile uf;
    if (!_loadFile(uf, username)) return false;
    if (!_keyRetriever.retrieveKey(uf, password)) return false;    //Key is decrypted and stored in KeyRetriever
    return true;
}

void UserFileCreator::error(int id)
{
    _handleError(id);
}

void UserFileCreator::errorWithMessage(QString msg)
{
    emit errorMessage(msg);
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

bool UserFileCreator::_loadFile(UserFile &uf, QString username)
{    
    QString fileName = "saves/" + username + ".save";

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    uf.setSaltArray(file.read(_keySize));
    uf.setAesIVArray(file.read(AES_BLOCK_SIZE));
    uf.setHashedPDKArray(file.read(_keySize));
    uf.setEncKeyArray(file.read(_keySize));
    file.close();
    return true;
}

void UserFileCreator::_handleError(int id)
{
    Q_UNUSED(id); //todo
    emit errorMessage("Error");
}
