#include "stringprovider.h"

QString StringProvider::HTML_TEMPLATE_ENGINE_ITEM;
QString StringProvider::HTML_TEMPLATE_IP_SOURCE_ITEM;
QString StringProvider::HTML_TEMPLATE_SCAN_FILE_RESULT;
QString StringProvider::HTML_TEMPLATE_SCAN_IP_RESULT;
QString StringProvider::HTML_TEMPLATE_SCAN_PENDING_RESULT;
QString StringProvider::HTML_TEMPLATE_SCAN_HASH_NOT_FOUND;
QString StringProvider::METASCAN_ONLINE_KEY;
QString StringProvider::APP_PATH;

StringProvider::StringProvider()
{

}

void StringProvider::loadTemplate() {
    // HTML template for engine item
    QString templateName(":/template/engine.html");
    QFile qfile(templateName);
    if(qfile.open(QIODevice::ReadOnly)) {
        StringProvider::HTML_TEMPLATE_ENGINE_ITEM = QString(qfile.readAll());
        qfile.close();
    }

    templateName = ":/template/ip_source.html";
    qfile.setFileName(templateName);
    if(qfile.open(QIODevice::ReadOnly)) {
        StringProvider::HTML_TEMPLATE_IP_SOURCE_ITEM = QString(qfile.readAll());
        qfile.close();
    }

    templateName = ":/template/scan_file_result.html";
    qfile.setFileName(templateName);
    if(qfile.open(QIODevice::ReadOnly)) {
        StringProvider::HTML_TEMPLATE_SCAN_FILE_RESULT = QString(qfile.readAll());
        qfile.close();
    }

    templateName = ":/template/scan_ip_result.html";
    qfile.setFileName(templateName);
    if(qfile.open(QIODevice::ReadOnly)) {
        StringProvider::HTML_TEMPLATE_SCAN_IP_RESULT = QString(qfile.readAll());
        qfile.close();
    }

    templateName = ":/template/scan_pending.html";
    qfile.setFileName(templateName);
    if(qfile.open(QIODevice::ReadOnly)) {
        StringProvider::HTML_TEMPLATE_SCAN_PENDING_RESULT = QString(qfile.readAll());
        qfile.close();
    }

    templateName = ":/template/hash_not_found.html";
    qfile.setFileName(templateName);
    if(qfile.open(QIODevice::ReadOnly)) {
        StringProvider::HTML_TEMPLATE_SCAN_HASH_NOT_FOUND = QString(qfile.readAll());
        qfile.close();
    }
}

QString StringProvider::formatFileSize(quint64 bytes)
{
    quint64 oneKB = 1024;
    quint64 oneMB = 1024 * 1024;
    quint64 oneGB = 1024 * 1024 * 1024;

    double result = 1.0;

    if(bytes > oneGB) {
        result = (bytes * 1.0) / oneGB;
        return QString("%1 %2").arg(QString::number(result, 'f', 2), "GB");
    } else if(bytes > oneMB) {
        result = (bytes * 1.0) / oneMB;
        return QString("%1 %2").arg(QString::number(result, 'f', 2), "MB");
    } else if(bytes > oneKB) {
        result = (bytes * 1.0) / oneKB;
        return QString("%1 %2").arg(QString::number(result, 'f', 2), "KB");
    }

    return "Unknown size";
}

QString StringProvider::formatDigit(int number, int digit)
{
    return QString("%1").arg(number, digit, 10, QChar('0'));
}

QString StringProvider::getFileName(QString filePath)
{
    return filePath.mid(filePath.lastIndexOf("/") + 1);
}

QString StringProvider::getStyleCustomProgressBar()
{
    QString style = "QProgressBar {border: none; text-align: center;}";
    style += "QProgressBar::chunk {background-color: #3C8CE4; width: 10px; margin: 6px 0px 6px 0px;}";
    return style;
}

bool StringProvider::isValidIPAddress(QString strIP)
{        
    if(!strIP.isEmpty()) {
        QStringList arr = strIP.split("\.");
        if(arr.length() == 4) {
            int correctCount = 0;
            for(int i = 0; i < 4; i ++) {
                if(arr.at(i).toInt() >= 0 && arr.at(i).toInt() <= 255) {
                    correctCount ++;
                }
            }
            if(correctCount == 4) {
                return true;
            }
        }
    }
    return false;
}
