#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    StringProvider::loadTemplate();

    ui->setupUi(this);

    initLayout();

    sqlLiteProvider = new SqlLiteProvider();
    connect(sqlLiteProvider, SIGNAL(updateEventLogUI(QList<EventLog>)),
            this, SLOT(sloUpdateEventLogUI(QList<EventLog>)));
    sqlLiteProvider->loadDatabase();

    httpProvider = new HttpProvider();
    connect(httpProvider, SIGNAL(sigHttpFinished(QNetworkReply*)),
            this, SLOT(sloHttpFinished(QNetworkReply*)));

    connect(ui->tableScan, SIGNAL(signalInsertLog(EventLog)),
            this, SLOT(slotInsertLog(EventLog)));

    // Load metascan online key
    QSettings qs(SETTINGS_DOMAIN, SETTINGS_USER);
    QString moKey = qs.value(SETTINGS_MO_KEY).toString();
    StringProvider::METASCAN_ONLINE_KEY = moKey;
    if(moKey.isEmpty()) {
        qDebug() << "moKey = NULLLLL";
        SettingsDialog* dialogSettings = new SettingsDialog(this);
        dialogSettings->setModal(true);
        dialogSettings->show();
    } else {
        qDebug() << "moKey = " << moKey;
    }
}

MainWindow::~MainWindow()
{
    delete sqlLiteProvider;
    delete httpProvider;
    delete ui;
}

void MainWindow::sloHttpFinished(QNetworkReply* networkReply) {
    QString url = networkReply->url().toString();
    int httpCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray qba = networkReply->readAll();

    qDebug() << "======> URL = " << url;
    qDebug() << "======> HTTP CODE = " << httpCode;
    // qDebug() << "[sloHttpFinished]: response = " << qba;

    QJsonDocument doc;
    QJsonObject jo;
    if(!qba.isEmpty()) {
        doc = QJsonDocument::fromJson(qba);
        jo = doc.object();
    }

    if(url.contains("http://hashlookup.metascan-online.com/v2/hash/")) {
        // Get look up hash response

        switch (httpCode) {
        case 200:
            updateLookUpHashResultUI(jo, url);
            break;
        default:
            qDebug() << "[LOI 400|401|403|500]";
            break;
        }
    }
    else if(url.contains("https://ipscan.metascan-online.com/v1/scan/")) {
        // Get scan IP response

        switch (httpCode) {
        case 200:
            updateScanIPResultUI(jo, url);
            break;
        default:
            qDebug() << "[LOI 400|401|403|500]";
            break;
        }
    }
    else if(url.contains(URL_METASCAN_ONLINE_GET_SCAN_FILE_RESULT)) {

    }

    if(networkReply->error() == QNetworkReply::NoError) {

    }
    else {
        ui->btnScanIP->setEnabled(true);
        ui->btnLookupHash->setEnabled(true);
    }
}

void MainWindow::initLayout() {

    ui->tabContainer->setCurrentIndex(0);
    ui->tabContainer->setTabText(0, "Scan File");
    ui->tabContainer->setTabText(1, "Look Up Hash");
    ui->tabContainer->setTabText(2, "Scan IP");
    ui->tabContainer->setTabText(3, "Event Log");

    ui->tableLog->setColumnWidth(0, 100);
    ui->tableLog->setColumnWidth(1, 100);
    ui->tableLog->setColumnWidth(2, 75);
    ui->tableLog->setColumnWidth(3, 275);
    ui->tableLog->setColumnWidth(4, 300);
    ui->tableLog->setSortingEnabled(true);

    ui->tableScan->setColumnWidth(0, 100);
    ui->tableScan->setColumnWidth(1, 100);
    ui->tableScan->setColumnWidth(2, 150);
    ui->tableScan->setColumnWidth(3, 100);
    ui->tableScan->setColumnWidth(4, 150);
    ui->tableScan->setColumnWidth(5, 100);
    ui->tableScan->setColumnWidth(6, 400);
    ui->tableScan->setSortingEnabled(true);

    ui->wvScanIP->setAcceptDrops(false);
    ui->wvLookUpHash->setAcceptDrops(false);
}

void MainWindow::sleep(int nSeconds) {
    QTime dieTime= QTime::currentTime().addSecs(nSeconds);
    while(QTime::currentTime() < dieTime) {
        // qDebug() << "Sleeping...";
    };
}

void MainWindow::on_tabContainer_tabBarClicked(int index)
{

//    QFile qfile("C:\\Users\\ble.INTL\\Desktop\\tetttt.html");
//    if(!qfile.open(QIODevice::ReadOnly)) {
//        qDebug() << "Cannot open template file";
//    }
//    QString htmlTemplate = qfile.readAll();
//    qfile.close();

//    ui->wvScanFileResult->setHtml(htmlTemplate);
}

void MainWindow::on_btnLookupHash_clicked()
{
    QString md5 = ui->edHash->text();

    if(md5.length() > 0 && !md5.contains(' ')) {
        ui->btnLookupHash->setEnabled(false);
        ui->wvLookUpHash->setHtml("");

        QString search = ui->tableScan->getScanResultByMD5(md5);

        if(search.length() > 0) {
            ui->wvLookUpHash->setHtml(search);
            ui->btnLookupHash->setEnabled(true);
        } else {
            httpProvider->doSearchHash(md5);
        }

    } else {
        QMessageBox::warning(this, QString("Invalid hash"),
                                   QString("Please input a valid hash"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
    }
}

void MainWindow::updateLookUpHashResultUI(QJsonObject jo, QString url) {

    QString hash = url.mid(url.lastIndexOf("/") + 1).toUpper();

    if(jo.contains(hash) && jo[hash].toString().toUpper().compare(HASH_NOT_FOUND) == 0) {
        // Hash not found
        QString htmlResult = StringProvider::HTML_TEMPLATE_SCAN_HASH_NOT_FOUND;
        htmlResult = htmlResult.replace("@hash", hash);

        // Update UI webview
        ui->wvLookUpHash->setHtml(htmlResult);
    } else {
        // Found a result
        QJsonObject joScanResult = jo["scan_results"].toObject();

        // Found hash result ==> bind result data to webview

        QJsonObject joFileInfo = jo["file_info"].toObject();

        QString md5 = joFileInfo["md5"].toString();
        QString sha1 = joFileInfo["sha1"].toString();
        QString sha256 = joFileInfo["sha256"].toString();
        QString fileType = joFileInfo["file_type_extension"].toString();
        QString fileDescription = joFileInfo["file_type_description"].toString();
        QString displayName = joFileInfo["display_name"].toString();
        QString uploadTimestamp = joFileInfo["upload_timestamp"].toString();
        qint64 fileSize = joFileInfo["file_size"].toInt();
        QString startTime = joScanResult["start_time"].toString();

        // Bind data for list engines

        QString htmlEnginesResult;
        QJsonObject joScanDetails = joScanResult["scan_details"].toObject();
        QStringList engineNames = joScanDetails.keys();
        int totalEngines = joScanResult["total_avs"].toInt();
        int detectedEngines = 0;
        foreach (QString engineName, engineNames) {
            QJsonObject joEngine = joScanDetails[engineName].toObject();
            QString status = "normal";
            if(joEngine["scan_result_i"].toInt() == 1) {
                status = "issue";
                detectedEngines ++;
            }
            QString threat = joEngine["threat_found"].toString();
            QString definitionDate = joEngine["def_time"].toString();
            int scanTime = joEngine["scan_time"].toInt();



            QString temp(StringProvider::HTML_TEMPLATE_ENGINE_ITEM);
            temp.replace("@scanTime", QString::number(scanTime));
            temp.replace("@definitionDate", DateTimeProvider::formatStringDate(definitionDate, DATE_TIME_FORMAT_2));
            temp.replace("@threat", threat);
            temp.replace("@status", status);
            temp.replace("@engine", engineName);

            htmlEnginesResult += temp;
        }

        // Bind data for file info
        QString htmlResult = StringProvider::HTML_TEMPLATE_SCAN_FILE_RESULT;

        qDebug() << "totalEngines = " << totalEngines << ", detectedEngines = " << detectedEngines;

        // Format the detected engines 2 digit
        if(detectedEngines >= 0 && detectedEngines <= 9) {
            htmlResult.replace("@number", QString("%1%2").arg("&nbsp;", QString::number(detectedEngines)));
        } else {
            htmlResult.replace("@number", QString::number(detectedEngines));
        }

        if(detectedEngines == 0) {
            htmlResult.replace("@imageName", "engine_not_found_threat.png");
        } else {
            htmlResult.replace("@imageName", "engine_found_threat.png");
        }

        htmlResult.replace("@firstUploaded", uploadTimestamp);
        htmlResult.replace("@lastScanned", fileDescription);
        htmlResult.replace("@fileName", displayName.mid(displayName.lastIndexOf("/") + 1));
        htmlResult.replace("@fileType", fileType);
        htmlResult.replace("@fileSize", QString::number(fileSize));

        htmlResult.replace("@number", QString::number(detectedEngines));
        htmlResult.replace("@total", QString::number(totalEngines));
        htmlResult.replace("@date", startTime);
        htmlResult.replace("@md5", md5);
        htmlResult.replace("@sha1", sha1);
        htmlResult.replace("@sha256", sha256);
        htmlResult.replace("@listEngines", htmlEnginesResult);

        // Update UI webview
        ui->wvLookUpHash->setHtml(htmlResult);

    }

    // Lookup hash complete
    ui->btnLookupHash->setEnabled(true);
}

void MainWindow::on_btnScanIP_clicked()
{
    QString strIP = ui->edScanIP->text();
    if(StringProvider::isValidIPAddress(strIP)) {
        ui->btnScanIP->setEnabled(false);
        ui->wvScanIP->setHtml("");
        httpProvider->doScanIP(strIP);
    } else {
        QMessageBox::warning(this, QString("Invalid IP Address"),
                                   QString("Please input a valid IP"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
    }
}

void MainWindow::updateScanIPResultUI(QJsonObject jo, QString url) {

    if(jo.contains("err") && QString::compare(jo["err"].toString(), "IP/Domain is null or empty")) {
        // IP not found
    } else {
        // Found IP result ==> bind result data to webview

        QString ipAddress = jo["address"].toString();

        QJsonObject geoInfo = jo["geo_info"].toObject();
        QString countryCode = geoInfo["country_code"].toString();
        QString countryName = geoInfo["country_name"].toString();
        QString regionCode = geoInfo["region_code"].toString();
        QString regionName = geoInfo["region_name"].toString();
        QString city = geoInfo["city"].toString();

        // Bind data for list engines
        QString htmlIPResult;
        QJsonArray joScanResult = jo["scan_results"].toArray();

        int totalSources = 0;
        int detectedSources = 0;
        for (int i = 0; i < joScanResult.size(); i ++) {
            totalSources ++;

            QJsonObject joItem = joScanResult.at(i).toObject();
            QString source = joItem["source"].toString();

            QJsonObject joSubItem = joItem["results"].toArray().at(0).toObject();

            QString referenceLink = joSubItem["alternativeid"].toString();
            // QString source = joResults["assessment"].toString();
            QString detectTime = joSubItem["detecttime"].toString();
            int score = joSubItem["confident"].toInt();
            QString result = joSubItem["result"].toString();
            QString rowEffect = "";
            if(QString::compare(result, "dirty") == 0) {
                result = "issue";
                rowEffect = "row-virus";
                detectedSources ++;
            } else {
                result = "normal";
            }

            QString temp(StringProvider::HTML_TEMPLATE_IP_SOURCE_ITEM);
            temp.replace("@referenceLink", referenceLink);
            temp.replace("@source", source);
            temp.replace("@score", QString::number(score));

            qDebug() << "detectTime === " << detectTime;

            temp.replace("@lastUpdate", DateTimeProvider::formatStringDate(detectTime, DATE_TIME_FORMAT));
            temp.replace("@status", result);
            temp.replace("@class", rowEffect);

            htmlIPResult += temp;
        }

        // Bind data for file info
        QString htmlResult = StringProvider::HTML_TEMPLATE_SCAN_IP_RESULT;

        qDebug() << "totalEngines = " << totalSources << ", detectedEngines = " << detectedSources;

        if(detectedSources == 0) {
            htmlResult.replace("@imageName", "source_not_found_threat.png");
        } else {
            htmlResult.replace("@imageName", "source_found_threat.png");
        }

        htmlResult.replace("@ipAddress", ipAddress);
        if(!countryName.isEmpty()) {
            htmlResult.replace("@countryName", countryName);
        } else {
            htmlResult.replace("@countryName", STATUS_NOT_AVAILABLE);
        }
        if(!countryCode.isEmpty()) {
            htmlResult.replace("@countryCode", countryCode);
        } else {
            htmlResult.replace("@countryCode", STATUS_NOT_AVAILABLE);
        }
        if(!regionName.isEmpty()) {
            htmlResult.replace("@regionName", regionName);
        } else {
            htmlResult.replace("@regionName", STATUS_NOT_AVAILABLE);
        }
        if(!regionCode.isEmpty()) {
            htmlResult.replace("@regionCode", regionCode);
        } else {
            htmlResult.replace("@regionCode", STATUS_NOT_AVAILABLE);
        }
        if(!city.isEmpty()) {
            htmlResult.replace("@city", city);
        } else {
            htmlResult.replace("@city", STATUS_NOT_AVAILABLE);
        }

        htmlResult.replace("@number", QString::number(detectedSources));
        htmlResult.replace("@total", QString::number(totalSources));
        htmlResult.replace("@listSources", htmlIPResult);

        // Update UI webview
        ui->wvScanIP->setHtml(htmlResult);

        // Lookup hash complete
        ui->btnScanIP->setEnabled(true);
        qDebug() << "Set enable = true";
    }
}

void MainWindow::appendLog(EventLog item)
{
    QTableWidgetItem* colDate = new QTableWidgetItem(item.getDate());
    QTableWidgetItem* colFileName = new QTableWidgetItem(item.getFileName());
    QTableWidgetItem* colStatus = new QTableWidgetItem(item.getStatus());
    QTableWidgetItem* colSHA1 = new QTableWidgetItem(item.getMD5());
    QTableWidgetItem* colLink = new QTableWidgetItem(item.getLink());

    int newRow = ui->tableLog->rowCount();
    ui->tableLog->insertRow(newRow);
    ui->tableLog->setItem(newRow, 0, colDate);
    ui->tableLog->setItem(newRow, 1, colFileName);
    ui->tableLog->setItem(newRow, 2, colStatus);
    ui->tableLog->setItem(newRow, 3, colSHA1);
    ui->tableLog->setItem(newRow, 4, colLink);
}

void MainWindow::clearTableLog() {
    while(ui->tableLog->rowCount() > 0) {
        ui->tableLog->removeRow(0);
    }
}

void MainWindow::on_actionAddFile_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if(fileNames.length() > 0) {
        ui->tableScan->addFile(fileNames.at(0));
    }
}

void MainWindow::on_actionSettings_triggered()
{
     SettingsDialog* dialogSettings = new SettingsDialog(this);
     dialogSettings->setModal(true);
     dialogSettings->show();
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              APP_NAME,
                                                              MSG_QUIT_APP,
                                                              QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog* dialogAbout = new AboutDialog(this);
    dialogAbout->setModal(true);
    dialogAbout->show();
}

void MainWindow::sloUpdateEventLogUI(QList<EventLog> logs)
{
    // Remove all rows
    ui->tableLog->setRowCount(0);

    if(!logs.empty()) {
        int size = logs.size();
        for(int i = 0; i < size; i ++) {
            EventLog item = logs.at(i);

            QTableWidgetItem* colDate = new QTableWidgetItem(item.getDate());
            QTableWidgetItem* colFileName = new QTableWidgetItem(item.getFileName());
            QTableWidgetItem* colStatus = new QTableWidgetItem(item.getStatus());
            QTableWidgetItem* colSHA1 = new QTableWidgetItem(item.getMD5());
            QTableWidgetItem* colLink = new QTableWidgetItem(item.getLink());

            // ui->tabEventlog->inse
            ui->tableLog->insertRow(i);
            ui->tableLog->setItem(i, 0, colDate);
            ui->tableLog->setItem(i, 1, colFileName);
            ui->tableLog->setItem(i, 2, colStatus);
            ui->tableLog->setItem(i, 3, colSHA1);
            ui->tableLog->setItem(i, 4, colLink);
        }
    }
}

void MainWindow::loadEventLogFromDB() {

}

void MainWindow::on_tableScan_cellClicked(int row, int column)
{
    QString resultHTML = ui->tableScan->getScanResultAt(row, column);
    ui->wvTableResult->setHtml(resultHTML);
}


void MainWindow::slotInsertLog(EventLog log)
{
    qDebug() << "slotInsertLog tu main window";

    if(sqlLiteProvider->insertLog(log)) {
        appendLog(log);
    }
}

void MainWindow::on_btnClearLog_clicked()
{
    if(sqlLiteProvider->clearLog()) {
        clearTableLog();
    }
}

void MainWindow::on_btnClearTableScan_clicked()
{
    while(ui->tableScan->rowCount() > 0) {
        ui->tableScan->removeRow(0);
        ui->wvTableResult->setHtml("");
        ui->tableScan->clearAll();
    }
}
