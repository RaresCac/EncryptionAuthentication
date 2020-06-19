#include "userfile.h"

UserFile::UserFile()
{

}

QByteArray UserFile::saltArray() const
{
    return _saltArray;
}

void UserFile::setSaltArray(const QByteArray &saltArray)
{
    _saltArray = saltArray;
}

QByteArray UserFile::aesIVArray() const
{
    return _aesIVArray;
}

void UserFile::setAesIVArray(const QByteArray &aesIVArray)
{
    _aesIVArray = aesIVArray;
}

QByteArray UserFile::hashedPDKArray() const
{
    return _hashedPDKArray;
}

void UserFile::setHashedPDKArray(const QByteArray &hashedPDKArray)
{
    _hashedPDKArray = hashedPDKArray;
}

QByteArray UserFile::encKeyArray() const
{
    return _encKeyArray;
}

void UserFile::setEncKeyArray(const QByteArray &encKeyArray)
{
    _encKeyArray = encKeyArray;
}

QString UserFile::getFileName() const
{
    return fileName;
}

void UserFile::setFileName(const QString &value)
{
    fileName = value;
}
