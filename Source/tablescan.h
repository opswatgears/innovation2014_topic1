#ifndef TABLESCAN_H
#define TABLESCAN_H

#include "hashprovider.h"
#include "mofile.h"
#include "datetimeprovider.h"
#include "eventlog.h"

#include <QWidget>
#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QDragMoveEvent>
#include <QProgressBar>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QList>

class TableScan : public QTableWidget
{
    Q_OBJECT

private:
    int uploadRemain;
    QTimer* timerFetchReport;
    QNetworkAccessManager* networkAccessManager;
    QList<MoFile*> listFiles;
public:
    bool isDuplicatedFile(QString);
    explicit TableScan(QWidget *parent = 0);
    QString getScanResultAt(int row, int col);
    void addFile(QString filePath);
    QString getScanResultByMD5(QString md5);
    void clearAll();
    ~TableScan();
protected:
    void dragMoveEvent(QDragMoveEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
signals:
    void signalInsertLog(EventLog);
public slots:
    void fetchReport();
};

#endif // TABLESCAN_H
