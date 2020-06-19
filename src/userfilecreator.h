#ifndef USERFILECREATOR_H
#define USERFILECREATOR_H

#include <QObject>
#include "encryptor.h"
#include "userfile.h"

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

private:
    bool _saveFile(UserFile uf);
    bool _loadFile(UserFile uf);
    void _handleError(int id);

    Encryptor _encryptor;

};

#endif // USERFILECREATOR_H
