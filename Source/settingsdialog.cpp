#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "Definition.h"
#include "stringprovider.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{

    ui->setupUi(this);

    setFixedSize(SETTINGS_DIALOG_WIDTH, SETTINGS_DIALOG_HEIGHT);

    QIcon qicon(":image/opswat-logo.ico");
    setWindowIcon(qicon);
    setWindowTitle(APP_NAME);

    QSettings qs(SETTINGS_DOMAIN, SETTINGS_USER);
    QString moKey = qs.value(SETTINGS_MO_KEY).toString();
    ui->edMoKey->setText(moKey);

    StringProvider::METASCAN_ONLINE_KEY = moKey;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_btnSave_clicked()
{
    if(ui->edMoKey->text().isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, APP_NAME, MSG_EMPTY_KEY,
                                                                 QMessageBox::Ok|QMessageBox::Cancel);
    } else {
        QString moKey = ui->edMoKey->text();
        qDebug() << "moKey = " << moKey.toStdString().c_str();

        QSettings qs(SETTINGS_DOMAIN, SETTINGS_USER);
        qs.setValue(SETTINGS_MO_KEY, moKey);

        StringProvider::METASCAN_ONLINE_KEY = moKey;
        this->close();
    }
}

void SettingsDialog::on_btnCancel_clicked()
{
    QSettings qs(SETTINGS_DOMAIN, SETTINGS_USER);
    QString moKey = qs.value(SETTINGS_MO_KEY).toString();

    if(moKey.isEmpty()) {
        QMessageBox::warning(this, APP_NAME, MSG_EMPTY_KEY, QMessageBox::Ok|QMessageBox::Cancel);
    } else {
        this->close();
    }
}
