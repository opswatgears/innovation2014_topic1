#include "eventlog.h"


EventLog::EventLog(QString pdate, QString pfileName, QString pstatus, QString psha1, QString plink)
{
    date = pdate;
    fileName = pfileName;
    status = pstatus;
    md5 = psha1;
    link = plink;
}

QString EventLog::getStatus() const
{
    return status;
}

void EventLog::setStatus(const QString &value)
{
    status = value;
}

QString EventLog::getFileName() const
{
    return fileName;
}

void EventLog::setFileName(const QString &value)
{
    fileName = value;
}

QString EventLog::getDate() const
{
    return date;
}

void EventLog::setDate(const QString &value)
{
    date = value;
}

QString EventLog::getMD5() const
{
    return md5;
}

void EventLog::setMD5(const QString &value)
{
    md5 = value;
}

QString EventLog::getLink() const
{
    return link;
}

void EventLog::setLink(const QString &value)
{
    link = value;
}
