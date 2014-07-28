#include "mofile.h"

MoFile::MoFile(QString filePath) : QFile(filePath)
{
    qDebug() << "Init file with path: " << filePath;
    
    status = UNKNOWN_STATUS;
    state = STATE_QUEUED;
    isSavedOnDB = false;
}

MoFile::~MoFile()
{
    qDebug() << "Destroy MoFile " << fileName();
}

bool MoFile::getIsSavedOnDB() const
{
    return isSavedOnDB;
}

void MoFile::setIsSavedOnDB(bool value)
{
    isSavedOnDB = value;
}

QString MoFile::getDataID() const
{
    return dataID;
}

void MoFile::setDataID(const QString &value)
{
    dataID = value;
}

QString MoFile::getRestIP() const
{
    return restIP;
}

void MoFile::setRestIP(const QString &value)
{
    restIP = value;
}

QTableWidgetItem *MoFile::getLbMD5() const
{
    return lbMD5;
}

void MoFile::setLbMD5(QTableWidgetItem *value)
{
    lbMD5 = value;
}

QTableWidgetItem *MoFile::getLbStatus() const
{
    return lbStatus;
}

void MoFile::setLbStatus(QTableWidgetItem *value)
{
    lbStatus = value;
}

QTableWidgetItem *MoFile::getLbDetectedEngines() const
{
    return lbDetectedEngines;
}

void MoFile::setLbDetectedEngines(QTableWidgetItem *value)
{
    lbDetectedEngines = value;
}

QTableWidgetItem *MoFile::getLbLinkResult() const
{
    return lbLinkResult;
}

void MoFile::setLbLinkResult(QTableWidgetItem *value)
{
    lbLinkResult = value;
}

QProgressBar* MoFile::getProgProcess() const
{
    return progProcess;
}

void MoFile::setProgProcess(QProgressBar *value)
{
    progProcess = value;
}

int MoFile::getState() const
{
    return state;
}

void MoFile::setState(int value)
{
    state = value;
}

QString MoFile::getStatus() const
{
    return status;
}

void MoFile::setStatus(const QString &value)
{
    status = value;
}

QString MoFile::getLinkResult() const
{
    return linkResult;
}

void MoFile::setLinkResult(const QString &value)
{
    linkResult = value;
}

QByteArray MoFile::getMd5() const
{
    return md5;
}

void MoFile::setMd5(QByteArray value)
{
    md5 = value;
}

QByteArray MoFile::getSha1() const
{
    return sha1;
}

void MoFile::setSha1(QByteArray value)
{
    sha1 = value;
}

QByteArray MoFile::getSha256() const
{
    return sha256;
}

void MoFile::setSha256(QByteArray value)
{
    sha256 = value;
}

QNetworkAccessManager* MoFile::getNetworkAccessManager() const
{
    return networkAccessManager;
}

void MoFile::setNetworkAccessManager(QNetworkAccessManager *value)
{
    networkAccessManager = value;
}

void MoFile::doUpload()
{
    if(!fileName().isEmpty()) {
        if(open(QIODevice::ReadOnly)) {

            setState(STATE_UPLOADING);

            QNetworkRequest httpRequest(QUrl(URL_METASCAN_ONLINE_UPLOAD_FILE));

            httpRequest.setRawHeader("apikey", StringProvider::METASCAN_ONLINE_KEY.toStdString().c_str());
            httpRequest.setRawHeader("filename", fileName().toStdString().c_str());
            httpRequest.setRawHeader("content-type", "application/x-www-form-urlencoded");

            QSslConfiguration sslConfig = httpRequest.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            httpRequest.setSslConfiguration(sslConfig);

            QNetworkReply* networkReply = networkAccessManager->post(httpRequest, readAll());

            connect(networkReply, SIGNAL(uploadProgress(qint64,qint64)),
                    this, SLOT(slotUploadProgress(qint64,qint64)));

            connect(networkReply, SIGNAL(finished()),
                    this, SLOT(slotUploadFinished()));

            connect(networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                    this, SLOT(slotSslErrors(QNetworkReply*, QList<QSslError>)));

            close(); // Close file
        }
    }
}

void MoFile::doFetchReport()
{
    setState(STATE_FETCHING_REPORT);

    QString url = QString(URL_METASCAN_ONLINE_GET_SCAN_FILE_STATUS).arg(restIP, dataID);

    qDebug() << "doFetchReport = " << url;

    QNetworkRequest httpRequest(QUrl(url.toStdString().c_str()));

    httpRequest.setRawHeader("apikey", StringProvider::METASCAN_ONLINE_KEY.toStdString().c_str());
    httpRequest.setRawHeader("content-type", "application/x-www-form-urlencoded");

    QSslConfiguration sslConfig = httpRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    httpRequest.setSslConfiguration(sslConfig);

    QNetworkReply* qNetworkReply = networkAccessManager->get(httpRequest);

    connect(qNetworkReply, SIGNAL(finished()),
            this, SLOT(slotFetchReportFinished()));

    connect(networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
            this, SLOT(slotSslErrors(QNetworkReply*, QList<QSslError>)));
}

void MoFile::slotUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    int percentage = qRound(((bytesSent * 1.0 / bytesTotal) * 100));
    progProcess->setValue(percentage);
    lbStatus->setText(UPLOAD_FILE_STATUS_PENDING);
}

void MoFile::slotUploadFinished()
{

    QNetworkReply* networkReply = (QNetworkReply*) QObject::sender();

    int httpCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch (httpCode) {
    case 200: {
            QByteArray qba = networkReply->readAll();
            networkReply->deleteLater();

            QJsonDocument doc = QJsonDocument::fromJson(qba);
            QJsonObject jo = doc.object();
            dataID = jo["data_id"].toString();
            restIP = jo["rest_ip"].toString();

            lbStatus->setText(UPLOAD_FILE_STATUS_COMPLETED);
            lbLinkResult->setText(QString(URL_METASCAN_ONLINE_GET_SCAN_FILE_RESULT).arg(dataID));

            setState(STATE_UPLOAD_COMPLETED);
        }
        break;
    default: {
            updateHttpCode(httpCode);
            setState(STATE_QUEUED); // Prepare for re-upload
        }
        break;
    }
}

void MoFile::slotFetchReportFinished()
{
    qDebug() << "slotFetchReportFinished with " << fileName() << ", state = " << state;

    QNetworkReply* networkReply = (QNetworkReply*) QObject::sender();

    int httpCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch (httpCode) {
    case 200: {
            QByteArray qba = networkReply->readAll();
            networkReply->deleteLater();

            if(!qba.isEmpty()) {
                QJsonDocument doc = QJsonDocument::fromJson(qba);
                QJsonObject jo = doc.object();
                QJsonObject joScanResult = jo["scan_results"].toObject();
                int percentage = joScanResult["progress_percentage"].toInt();

                qDebug() << "slotFetchReportFinished with percent = " << percentage;

                if(percentage < 100) {
                    progProcess->setValue(percentage);
                    lbStatus->setText(FETCH_REPORT_STATUS_PENDING);
                    setState(STATE_UPLOAD_COMPLETED); // For re-fetch
                } else {
                    scanResult = QString(qba);

                    progProcess->setValue(0);
                    lbStatus->setText(SCAN_FILE_STATUS_COMPLETED);
                    updateStatusFile();
                    setState(STATE_SCAN_COMPLETED);
                }
            }
        }
        break;
    default:
        updateHttpCode(httpCode);
        setState(STATE_UPLOAD_COMPLETED); // Prepare for re-fetch report
        break;
    }
}

void MoFile::slotSslErrors(QNetworkReply* networkReply, QList<QSslError> sslErrors) {
    qDebug() << "sloSslErrors:: sloSslErrors";
    networkReply->ignoreSslErrors(sslErrors);
}

QString MoFile::getResultHTML() {

    // Scan completed ==> bind result data to webview

    QJsonDocument doc = QJsonDocument::fromJson(scanResult.toUtf8());
    QJsonObject jo = doc.object();
    QJsonObject joScanResult = jo["scan_results"].toObject();

    QJsonObject joFileInfo = jo["file_info"].toObject();

    QString md5 = joFileInfo["md5"].toString();
    QString sha1 = joFileInfo["sha1"].toString();
    QString sha256 = joFileInfo["sha256"].toString();
    QString fileType = joFileInfo["file_type_extension"].toString();
    QString fileDescription = joFileInfo["file_type_description"].toString();
    QString displayName = joFileInfo["display_name"].toString();
    QString fileName = StringProvider::getFileName(displayName);
    QString uploadTimestamp = joFileInfo["upload_timestamp"].toString();
    qint64 fileSize = joFileInfo["file_size"].toInt();
    QString startTime = joScanResult["start_time"].toString();


    if(!joScanResult.contains("scan_details")) {
        // SCAN PENDING
        // Only bind data for file info
        QString htmlResult = StringProvider::HTML_TEMPLATE_SCAN_PENDING_RESULT;
        htmlResult.replace("@fileName", this->fileName());
        htmlResult.replace("@date", DateTimeProvider::getCurrentDate());
        htmlResult.replace("@image", "scan_pending.png");
        return htmlResult;
    } else {
        // SCAN COMPLETED
        // Bind data for list engines
        QString htmlEnginesResult;
        QJsonObject joScanDetails = joScanResult["scan_details"].toObject();
        QStringList engineNames = joScanDetails.keys();
        int totalEngines = joScanResult["total_avs"].toInt();
        int detectedEngines = 0;
        foreach (QString engineName, engineNames) {
            QJsonObject joEngine = joScanDetails[engineName].toObject();
            QString status = "normal";
            QString rowEffect = "";
            if(joEngine["scan_result_i"].toInt() == 1) {
                status = "issue";
                detectedEngines ++;
                rowEffect = "row-virus";
            }
            QString threat = joEngine["threat_found"].toString();
            QString definitionDate = joEngine["def_time"].toString();
            int scanTime = joEngine["scan_time"].toInt();

            QString temp(StringProvider::HTML_TEMPLATE_ENGINE_ITEM);
            temp.replace("@scanTime", QString::number(scanTime));
            temp.replace("@definitionDate", DateTimeProvider::formatStringDate(definitionDate, DATE_TIME_FORMAT));
            temp.replace("@threat", threat);
            temp.replace("@status", status);
            temp.replace("@engine", engineName);
            temp.replace("@class", rowEffect);

            htmlEnginesResult += temp;
        }

        // Bind data for file info
        QString htmlResult = StringProvider::HTML_TEMPLATE_SCAN_FILE_RESULT;

        if(detectedEngines == 0) {
            htmlResult.replace("@imageName", "engine_not_found_threat.png");
        } else {
            htmlResult.replace("@imageName", "engine_found_threat.png");
        }

        htmlResult.replace("@firstUploaded", uploadTimestamp);
        htmlResult.replace("@lastScanned", fileDescription);
        htmlResult.replace("@fileName", displayName);
        htmlResult.replace("@fileType", fileType);
        htmlResult.replace("@fileSize", StringProvider::formatFileSize(fileSize));

        htmlResult.replace("@number", QString::number(detectedEngines));
        htmlResult.replace("@total", QString::number(totalEngines));
        htmlResult.replace("@date", DateTimeProvider::formatStringDate(startTime, DATE_TIME_FORMAT));
        htmlResult.replace("@md5", md5);
        htmlResult.replace("@sha1", sha1);
        htmlResult.replace("@sha256", sha256);
        htmlResult.replace("@listEngines", htmlEnginesResult);

        // qDebug() << htmlResult;

        // Return HTML result
        return htmlResult;
    }
}

void MoFile::updateStatusFile()
{
    QJsonDocument doc = QJsonDocument::fromJson(scanResult.toUtf8());
    QJsonObject jo = doc.object();
    QJsonObject joScanResult = jo["scan_results"].toObject();
    QJsonObject joScanDetails = joScanResult["scan_details"].toObject();
    QStringList engineNames = joScanDetails.keys();
    int totalEngines = joScanResult["total_avs"].toInt();
    int detectedEngines = 0;
    foreach (QString engineName, engineNames) {
        QJsonObject joEngine = joScanDetails[engineName].toObject();
        if(joEngine["scan_result_i"].toInt() == 1) {
            detectedEngines ++;
        }
    }
    lbDetectedEngines->setText(QString("%1/%2").arg(QString::number(detectedEngines),
                                                    QString::number(totalEngines)));


    // Update file status and save to databse eventlog
    status = "Clean";
    if(detectedEngines > 0) {
        status = "Infected";
    }
    linkResult = QString(URL_METASCAN_ONLINE_GET_SCAN_FILE_RESULT).arg(dataID);
}

void MoFile::updateHttpCode(int httpCode)
{
    switch (httpCode) {
    case 0:
        lbStatus->setText(HTTP_MSG_ERROR_0);
        break;
    case 400:
        lbStatus->setText(HTTP_MSG_ERROR_400);
        break;
    case 401:
        lbStatus->setText(HTTP_MSG_ERROR_401);
        break;
    case 403:
        lbStatus->setText(HTTP_MSG_ERROR_403);
        break;
    case 404:
        lbStatus->setText(HTTP_MSG_ERROR_404);
        break;
    case 500:
        lbStatus->setText(HTTP_MSG_ERROR_500);
        break;
    default:
        lbStatus->setText(QString(HTTP_RESPONSE_CODE).arg(httpCode));
        break;
    }
}
