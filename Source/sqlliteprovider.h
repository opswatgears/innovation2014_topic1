#ifndef SQLLITEPROVIDER_H
#define SQLLITEPROVIDER_H

#include "eventlog.h"

#include <QList>
#include <QObject>
#include <QSqlDatabase>

class SqlLiteProvider : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase db;
signals:
    void updateEventLogUI(QList<EventLog>);
public:
    bool insertLog(EventLog);
    bool loadDatabase();
    bool clearLog();
    SqlLiteProvider();
    ~SqlLiteProvider();
};

#endif // SQLLITEPROVIDER_H
