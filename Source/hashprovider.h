#ifndef HASHPROVIDER_H
#define HASHPROVIDER_H

#include "mofile.h"
#include <QObject>
#include <QRunnable>
#include <QFile>
#include <QCryptographicHash>
#include <QThreadPool>

class HashProvider : public QObject, public QRunnable
{
    Q_OBJECT
signals:
public slots:
public:
    HashProvider(MoFile* file);
    explicit HashProvider(QObject *parent = 0);
    void run(void);
private:
    MoFile* mofile;

};

#endif // HASHPROVIDER_H
