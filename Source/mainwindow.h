#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Definition.h"
#include "htmltemplate.h"
#include "sqlliteprovider.h"
#include "eventlog.h"
#include "ui_mainwindow.h"
#include "httpprovider.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "stringprovider.h"
#include "datetimeprovider.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include <QTableWidget>
#include <QTableWidgetItem>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>

#include <QNetworkRequest>

#include <QList>

#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void loadEventLogFromDB();
    void initLayout();
    void updateLookUpHashResultUI(QJsonObject, QString);
    void updateScanIPResultUI(QJsonObject, QString);
    void appendLog(EventLog);
    void clearTableLog();
    void sleep(int);
    Ui::MainWindow *ui;
    HttpProvider* httpProvider;
    SqlLiteProvider* sqlLiteProvider;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void sloHttpFinished(QNetworkReply*);
    void on_tabContainer_tabBarClicked(int index);
    void on_btnLookupHash_clicked();
    void on_btnScanIP_clicked();
    void on_actionSettings_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void sloUpdateEventLogUI(QList<EventLog>);
    void slotInsertLog(EventLog);
    void on_tableScan_cellClicked(int row, int column);
    void on_actionAddFile_triggered();


private slots:
    void on_btnClearLog_clicked();
    void on_btnClearTableScan_clicked();
};

#endif // MAINWINDOW_H
