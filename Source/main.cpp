#include "mainwindow.h"
#include "Definition.h"
#include "stringprovider.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);

    StringProvider::APP_PATH = QCoreApplication::applicationDirPath();

    QIcon qicon(":image/opswat-logo.ico");

    MainWindow mainWindow;
    mainWindow.setFixedSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    mainWindow.setAcceptDrops(true);
    mainWindow.setWindowTitle(APP_NAME);
    mainWindow.setWindowIcon(qicon);
    mainWindow.show();

    return qApplication.exec();
}
