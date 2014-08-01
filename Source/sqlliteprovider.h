#ifndef SQLLITEPROVIDER_H
#define SQLLITEPROVIDER_H

#include "eventlog.h"

#include <QDebug>
#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
