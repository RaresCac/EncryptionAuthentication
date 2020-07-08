#ifndef KEYRETRIEVER_H
#define KEYRETRIEVER_H

#include <QObject>
#include "encryptor.h"
#include "userfile.h"

/*
 * Retrieves fileKey from a UserFile object
 * Key is stored in memory inside this object
 *
 */
class KeyRetriever : public QObject
{
    Q_OBJECT
public:
    explicit KeyRetriever(QObject *parent = nullptr);

    bool retrieveKey(UserFile uf, QString password);

    Encryptor *encryptor() const;
    uchar* getFileKey();
    int getKeySize() const;

    void setEncryptor(Encryptor *encryptor);
    void setKeySize(int keySize);

public slots:
    void printKey();
    void removeFileKey();
    void updateKeySize();

signals:
    void keyRetrieved();
    void error(QString message);

private:
    int _decryptAES(uchar* input, size_t inSize, uchar* key, uchar* iv, uchar* output);
    uchar* _QBAtoUchar(QByteArray qba, int &outSize);
    bool _compareQBAtoUchar(QByteArray qba, uchar* uca, int inSize);

    int _keySize;
    QByteArray _fileKey;
    Encryptor* _encryptor = nullptr;

};

#endif // KEYRETRIEVER_H
