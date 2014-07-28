#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "Definition.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setFixedSize(ABOUT_DIALOG_WIDTH, ABOUT_DIALOG_HEIGHT);

    QIcon qicon(":image/opswat-logo.ico");
    setWindowIcon(qicon);
    setWindowTitle(APP_NAME);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
