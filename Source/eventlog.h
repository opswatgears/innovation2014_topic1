#ifndef EVENTLOG_H
#define EVENTLOG_H
#include <QString>

class EventLog
{
private:
    QString fileName;
    QString date;
    QString status;
    QString md5;
    QString link;
public:
    EventLog(QString,QString,QString,QString,QString);
    QString getStatus() const;
    void setStatus(const QString &value);
    QString getFileName() const;
    void setFileName(const QString &value);
    QString getDate() const;
    void setDate(const QString &value);
    QString getMD5() const;
    void setMD5(const QString &value);
    QString getLink() const;
    void setLink(const QString &value);
};

#endif // EVENTLOG_H
