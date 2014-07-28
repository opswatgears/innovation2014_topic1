#ifndef HTTPPROVIDER_H
#define HTTPPROVIDER_H

#include <QString>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>

class HttpProvider : public QObject
{
    Q_OBJECT
signals:
    void sigUploadProgress(qint64, qint64);
    void sigHttpFinished(QNetworkReply*);
public slots:
    void slotHttpFinished(QNetworkReply*);
    void sloSslErrors(QNetworkReply*, QList<QSslError>);
    void sloUploadProgress(qint64, qint64);
public:
    void doSearchHash(QString);
    void doScanIP(QString);
    void doHttpGet(QString);
    void doHttpPost(QString);
    HttpProvider();
    ~HttpProvider();
private:
    QNetworkAccessManager qnetWorkManager;
};

#endif // HTTPPROVIDER_H
