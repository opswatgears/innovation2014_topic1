#ifndef STRINGPROVIDER_H
#define STRINGPROVIDER_H

#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include <QRegExpValidator>
#include <QRegExp>
#include <QStringList>

class StringProvider
{
public:
    static QString HTML_TEMPLATE_ENGINE_ITEM;
    static QString HTML_TEMPLATE_IP_SOURCE_ITEM;
    static QString HTML_TEMPLATE_SCAN_FILE_RESULT;
    static QString HTML_TEMPLATE_SCAN_IP_RESULT;
    static QString HTML_TEMPLATE_SCAN_PENDING_RESULT;
    static QString HTML_TEMPLATE_SCAN_HASH_NOT_FOUND;
    static QString METASCAN_ONLINE_KEY;
    static void loadTemplate();
    static QString formatFileSize(quint64);
    static QString formatDigit(int, int digit = 5);
    static QString getFileName(QString);
    static QString getStyleCustomProgressBar();
    static bool isValidIPAddress(QString);
    StringProvider();
};

#endif // STRINGPROVIDER_H
