#ifndef DATETIMEPROVIDER_H
#define DATETIMEPROVIDER_H

#include "Definition.h"

#include <QString>
#include <QDateTime>

class DateTimeProvider
{
public:
    static QString getCurrentDate();
    static QString formatStringDate(QString,QString);
    DateTimeProvider();
};

#endif // DATETIMEPROVIDER_H
