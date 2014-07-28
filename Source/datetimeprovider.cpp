#include "datetimeprovider.h"

QString DateTimeProvider::getCurrentDate()
{
    return QDateTime::currentDateTime().toString();
}

QString DateTimeProvider::formatStringDate(QString str, QString format)
{
    if(!str.isEmpty()) {
        QDateTime datetime = QDateTime::fromString(str, format);
        QString year = QString::number(datetime.date().year());
        QString month = QString::number(datetime.date().month());
        QString day = QString::number(datetime.date().day());
        return QString("%1-%2-%3").arg(year, month, day);
    } else {
        return UNKNOWN_DATE;
    }
}

DateTimeProvider::DateTimeProvider()
{
}
