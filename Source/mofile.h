#ifndef MOFILE_H
#define MOFILE_H

#include "Definition.h"
#include "stringprovider.h"
#include "eventlog.h"
#include "datetimeprovider.h"
#include "sqlliteprovider.h"

#include <QObject>
#include <QString>
#include <QDate>
#include <QByteArray>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>

#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QTableWidgetItem>
#include <QProgressBar>
#include <QTableWidgetItem>

#include <QSslError>
#include <QSslConfiguration>

class MoFile : public QFile
{
    Q_OBJECT
private:
    bool isSavedOnDB;
    int state;
    QString status;
    QString linkResult;
    QString dataID;
    QString restIP;
    QString scanResult;
    QByteArray md5;
    QByteArray sha1;
    QByteArray sha256;
    QTableWidgetItem* lbStatus;
    QTableWidgetItem* lbMD5;
    QTableWidgetItem* lbDetectedEngines;
    QTableWidgetItem* lbLinkResult;
    QProgressBar* progProcess;
    QNetworkAccessManager* networkAccessManager;
public:
    MoFile(QString filePath);
    ~MoFile();

    QByteArray getMd5() const;
    void setMd5(QByteArray value);

    QByteArray getSha1() const;
    void setSha1(QByteArray value);

    QByteArray getSha256() const;
    void setSha256(QByteArray value);

    bool getIsSavedOnDB() const;
    void setIsSavedOnDB(bool value);

    int getState() const;
    void setState(int value);

    QTableWidgetItem *getLbMD5() const;
    void setLbMD5(QTableWidgetItem *value);

    QTableWidgetItem *getLbStatus() const;
    void setLbStatus(QTableWidgetItem *value);

    QTableWidgetItem *getLbDetectedEngines() const;
    void setLbDetectedEngines(QTableWidgetItem *value);

    QTableWidgetItem *getLbLinkResult() const;
    void setLbLinkResult(QTableWidgetItem *value);

    QProgressBar *getProgProcess() const;
    void setProgProcess(QProgressBar *value);

    QNetworkAccessManager *getNetworkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager *value);

    QString getDataID() const;
    void setDataID(const QString &value);

    QString getRestIP() const;
    void setRestIP(const QString &value);

    QString getStatus() const;
    void setStatus(const QString &value);

    QString getLinkResult() const;
    void setLinkResult(const QString &value);

    void doUpload();
    void doFetchReport();
    QString getResultHTML();
    void updateStatusFile();
    void updateHttpCode(int);

signals:

public slots:
    void slotUploadProgress(qint64 byteSent, qint64 bytesReceived);
    void slotUploadFinished();
    void slotFetchReportFinished();
    void slotSslErrors(QNetworkReply* networkReply, QList<QSslError> sslErrors);
};

#endif // MOFILE_H
