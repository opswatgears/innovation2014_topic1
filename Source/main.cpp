#include "mainwindow.h"
#include "Definition.h"

#include <QApplication>
#include <QIcon>

#include <QThread>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);

    QIcon qicon(":image/opswat-logo.ico");

    MainWindow mainWindow;
    mainWindow.setFixedSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    mainWindow.setAcceptDrops(true);
    mainWindow.setWindowTitle(APP_NAME);
    mainWindow.setWindowIcon(qicon);
    mainWindow.show();

    return qApplication.exec();
}
