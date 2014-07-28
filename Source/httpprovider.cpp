#include "httpprovider.h"
#include "Definition.h"
#include "mainwindow.h"
#include "stringprovider.h"

HttpProvider::HttpProvider()
{
    connect(&qnetWorkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotHttpFinished(QNetworkReply*)));

    connect(&qnetWorkManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
            this, SLOT(sloSslErrors(QNetworkReply*, QList<QSslError>)));
}

HttpProvider::~HttpProvider()
{

}

void HttpProvider::doHttpGet(QString url)
{
    qDebug() << "doHttpGet = " << url;

    QNetworkRequest httpRequest(QUrl(url.toStdString().c_str()));

    httpRequest.setRawHeader("apikey", StringProvider::METASCAN_ONLINE_KEY.toStdString().c_str());
    httpRequest.setRawHeader("content-type", "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = httpRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    httpRequest.setSslConfiguration(sslConfig);

    qnetWorkManager.get(httpRequest);
}

void HttpProvider::doHttpPost(QString filePath)
{
    qDebug() << "FILE = " << filePath;


    QFile qfile(filePath);

    qfile.open(QIODevice::ReadOnly);

    if(!qfile.exists()) {
        qDebug() << "File ko ton tai";
        return;
    } else {
        qDebug() << "File = " + qfile.fileName();
    }

    QNetworkRequest httpRequest(QUrl(URL_METASCAN_ONLINE_UPLOAD_FILE));

    httpRequest.setRawHeader("apikey", StringProvider::METASCAN_ONLINE_KEY.toStdString().c_str());
    httpRequest.setRawHeader("filename", qfile.fileName().toStdString().c_str());
    httpRequest.setRawHeader("content-type", "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = httpRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    httpRequest.setSslConfiguration(sslConfig);

    QNetworkReply* qNetworkReply = qnetWorkManager.post(httpRequest, qfile.readAll());
    connect(qNetworkReply, SIGNAL(uploadProgress(qint64,qint64)),
            this, SLOT(sloUploadProgress(qint64,qint64)));

    qfile.close();

    qDebug() << "Sau khi close file";
}

void HttpProvider::slotHttpFinished(QNetworkReply* networkReply)
{
    qDebug() << "httpFinished with URL = " << networkReply->url().toString();

    networkReply->deleteLater();

    emit sigHttpFinished(networkReply);
}

void HttpProvider::sloSslErrors(QNetworkReply* networkReply, QList<QSslError> sslErrors) {
    qDebug() << "eventSslErrors";
    networkReply->ignoreSslErrors(sslErrors);
}

void HttpProvider::sloUploadProgress(qint64 bytesSent, qint64 bytesTotal) {


    QString status = QString("bytesSent = %1, bytesTotal = %2").arg(QString::number(bytesSent))
                                                               .arg(QString::number(bytesTotal));

    qDebug() << status;

    emit sigUploadProgress(bytesSent, bytesTotal);
}

void HttpProvider::doSearchHash(QString hash)
{
    QString url(URL_METASCAN_ONLINE_LOOKUP_HASH);
    url = url.arg(hash);

    qDebug() << "doSearchHash = " << url;

    QNetworkRequest httpRequest(QUrl(url.toStdString().c_str()));

    httpRequest.setRawHeader("apikey", StringProvider::METASCAN_ONLINE_KEY.toStdString().c_str());
    httpRequest.setRawHeader("content-type", "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = httpRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    httpRequest.setSslConfiguration(sslConfig);

    qnetWorkManager.get(httpRequest);
}

void HttpProvider::doScanIP(QString ipAddress)
{
    QString url(URL_METASCAN_ONLINE_SCAN_IP_ADDRESS);
    url = url.arg(ipAddress);

    qDebug() << "doScanIP = " << url;

    QNetworkRequest httpRequest(QUrl(url.toStdString().c_str()));

    httpRequest.setRawHeader("apikey", StringProvider::METASCAN_ONLINE_KEY.toStdString().c_str());
    httpRequest.setRawHeader("content-type", "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = httpRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    httpRequest.setSslConfiguration(sslConfig);

    qnetWorkManager.get(httpRequest);
}
