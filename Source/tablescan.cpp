#include "tablescan.h"

TableScan::TableScan(QWidget *parent) :
    QTableWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropOverwriteMode(true);
    setDropIndicatorShown(true);

    networkAccessManager = new QNetworkAccessManager(this);
    timerFetchReport = new QTimer(this);

    connect(timerFetchReport, SIGNAL(timeout()), this, SLOT(fetchReport()));
    timerFetchReport->start(TIMER_FETCH_REPORT);

    uploadRemain = MAX_CONCURENT_UPLOAD;
}

TableScan::~TableScan()
{
    qDebug() << "=====> Destroy TableScan";
    timerFetchReport->stop();
    delete networkAccessManager;

    int nFiles = listFiles.count();
    for(int i = 0; i < nFiles; i ++) {
        MoFile* moFile = listFiles.at(i);
        if(moFile != NULL) {
            delete moFile;
        }
    }
}

void TableScan::dragEnterEvent(QDragEnterEvent *qDragEnterEvent)
{
    if (qDragEnterEvent->mimeData()->hasUrls()) {
        qDragEnterEvent->acceptProposedAction();
    }
}

void TableScan::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void TableScan::dropEvent(QDropEvent *qDropEvent) {

    const QMimeData* mimeData = qDropEvent->mimeData();

    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();

        // Maximum is 32 files
        int maximumFiles = 32;
        for (int i = 0; i < urlList.size() && i < maximumFiles; ++i)
        {
            qDebug() << "Drop file" << urlList.at(i).toLocalFile();

            // Add file to table
            addFile(urlList.at(i).toLocalFile());
        }
    }
}

void TableScan::fetchReport()
{
    if(!listFiles.isEmpty()) {
        for(int i = 0; i < listFiles.count(); i ++) {
            MoFile* moFile = listFiles.at(i);

            if(moFile->getStatus().compare(UNKNOWN_STATUS) != 0 && !moFile->getIsSavedOnDB()) {

                moFile->setIsSavedOnDB(true);
                QString timestamp = DateTimeProvider::getCurrentDate();
                EventLog log(timestamp,
                             moFile->fileName(),
                             moFile->getStatus(),
                             moFile->getMd5().toHex().toUpper(),
                             moFile->getLinkResult());
                emit signalInsertLog(log);
            }

            switch (moFile->getState()) {
            case STATE_QUEUED: {
                // if(uploadRemain > 0) {
                    moFile->doUpload();
                    // uploadRemain --;
                // }
                }
                break;
            case STATE_UPLOADING:
                break;
            case STATE_UPLOAD_COMPLETED:
                moFile->doFetchReport();
                break;
            case STATE_FETCHING_REPORT:
                break;
            case STATE_SCAN_COMPLETED:
                break;
            }
        }
    }
}

bool TableScan::isDuplicatedFile(QString mFilePath) {
    if(!listFiles.isEmpty()) {
        int nFiles = listFiles.count();
        for(int i = 0; i < nFiles; i ++) {
            MoFile* moFile = listFiles.at(i);
            if(moFile->fileName().compare(mFilePath) == 0) {
                return true;
            }
        }
    }
    return false;
}

QString TableScan::getScanResultAt(int row, int col)
{
    if(!listFiles.empty()) {
        return listFiles.at(row)->getResultHTML();
    }

    return "";
}


QString TableScan::getScanResultByMD5(QString md5)
{
    if(!listFiles.empty()) {
        for(int i = 0; i < listFiles.size(); i ++) {
            QString str1 = listFiles.at(i)->getMd5().toHex().toUpper();
            QString str2 = md5.toUpper();

            if(str1.compare(str2) == 0) {
                return listFiles.at(i)->getResultHTML();
            }
        }
    }

    return "";
}

void TableScan::clearAll()
{
    if(!listFiles.empty()) {
        int nFiles = listFiles.count();
        for(int i = 0; i < nFiles; i ++) {
            MoFile* moFile = listFiles.at(i);
            moFile->setState(STATE_SCAN_COMPLETED);
            if(moFile != NULL) {
                delete moFile;
            }
        }
        listFiles.clear();
    }
}

void TableScan::addFile(QString filePath) {
    if(!isDuplicatedFile(filePath)) {

        // Control for a ROW
        QTableWidgetItem* lbDate = new QTableWidgetItem(DateTimeProvider::getCurrentDate());
        QTableWidgetItem* lbFile = new QTableWidgetItem(StringProvider::getFileName(filePath));
        QTableWidgetItem* lbStatus = new QTableWidgetItem(MESSAGE_QUEUED);
        QProgressBar* progProcess = new QProgressBar(this);
        QTableWidgetItem* lbMD5 = new QTableWidgetItem(MESSAGE_CALCULATE_MD5);
        QTableWidgetItem* lbDetectedEngines = new QTableWidgetItem();
        QTableWidgetItem* lbLinkResult = new QTableWidgetItem();

        lbFile->setToolTip(filePath);

        // Assuming objectName is 'progressBar'..
        progProcess->setStyleSheet(StringProvider::getStyleCustomProgressBar());

        // File to upload
        MoFile* moFile = new MoFile(filePath);
        // Set control to update UI
        moFile->setLbStatus(lbStatus);
        moFile->setLbMD5(lbMD5);
        moFile->setLbLinkResult(lbLinkResult);
        moFile->setLbDetectedEngines(lbDetectedEngines);
        moFile->setProgProcess(progProcess);
        moFile->setNetworkAccessManager(networkAccessManager);
        listFiles.append(moFile); // Add file to queue


        // Calculate md5, sha1, sha256
        HashProvider* hashProvider = new HashProvider(moFile);
        QThreadPool::globalInstance()->start(hashProvider);

        // Append to last row
        int rowInsert = rowCount();
        insertRow(rowInsert);
        setItem(rowInsert, 0, lbDate);
        setItem(rowInsert, 1, lbFile);
        setItem(rowInsert, 2, lbStatus);
        setCellWidget(rowInsert, 3, progProcess);
        setItem(rowInsert, 4, lbMD5);
        setItem(rowInsert, 5, lbDetectedEngines);
        setItem(rowInsert, 6, lbLinkResult);
    } else {
        qDebug() << "Duplicated File!!!!!!!!!!!!!!!!!!!!!!";
    }
}
