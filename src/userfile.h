#ifndef USERFILE_H
#define USERFILE_H

#include <QByteArray>
#include <QString>

//Represents the contents of a single user file
class UserFile
{
public:
    UserFile();

    QByteArray saltArray() const;
    QByteArray aesIVArray() const;
    QByteArray hashedPDKArray() const;
    QByteArray encKeyArray() const;
    QString getFileName() const;

    void setSaltArray(const QByteArray &saltArray);
    void setAesIVArray(const QByteArray &aesIVArray);
    void setHashedPDKArray(const QByteArray &hashedPDKArray);
    void setEncKeyArray(const QByteArray &encKeyArray);
    void setFileName(const QString &value);

private:
    QString fileName;
    QByteArray _saltArray;      //Salt for PBKDF2
    QByteArray _aesIVArray;     //IV for AES
    QByteArray _hashedPDKArray; //Hashed password derived key
    QByteArray _encKeyArray;    //Encrypted fileKey
};

#endif // USERFILE_H
