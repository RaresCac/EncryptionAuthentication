#ifndef USERFILECREATOR_H
#define USERFILECREATOR_H

#include <QObject>
#include "encryptor.h"
#include "userfile.h"
#include "keyretriever.h"

class UserFileCreator : public QObject
{
    Q_OBJECT
public:
    explicit UserFileCreator(QObject *parent = nullptr);

signals:
    void errorMessage(QString message);

public slots:
    bool generateFile(QString username, QString password);
    bool loadFile(QString username, QString password);
    void error(int);
    void errorWithMessage(QString);

private:
    bool _saveFile(UserFile uf);
    bool _loadFile(UserFile &uf, QString username);
    void _handleError(int id);

    int _keySize = 32;
    Encryptor _encryptor;
    KeyRetriever _keyRetriever;

};

#endif // USERFILECREATOR_H
