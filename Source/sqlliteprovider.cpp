#include "sqlliteprovider.h"
#include "Definition.h"
#include "stringprovider.h"
#include "eventlog.h"


SqlLiteProvider::SqlLiteProvider()
{
    if(db.open()) {
        db.close();
    }
}

SqlLiteProvider::~SqlLiteProvider()
{
    if(db.open()) {
        qDebug() << "Close database";
        db.close();
    }
}

bool SqlLiteProvider::loadDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(StringProvider::APP_PATH + DATABASE_NAME);
    if(db.open()) {

        QSqlQuery query(db);

        if(db.tables().contains(TABLE_EVENTLOG)) {
            qDebug() << "Load database";

            // Load the databse into the UI
            QList<EventLog> listEventlog;
            query.prepare(QString("Select * from %1").arg(TABLE_EVENTLOG));
            query.exec();
            while(query.next()) {
                QString date = query.value(0).toString();
                QString fileName = query.value(1).toString();
                QString status = query.value(2).toString();
                QString sha1 = query.value(3).toString();
                QString link = query.value(4).toString();

                listEventlog << EventLog(date, fileName, status, sha1, link);
            }
            if(listEventlog.size() > 0) {
                // Update UI on tab eventlog
                emit updateEventLogUI(listEventlog);
            }
        } else {
            // If there is no table EVENTLOG then create it
            QString strQuery = QString("drop table if exists %1");
            strQuery = strQuery.arg(TABLE_EVENTLOG);
            qDebug() << "[===1===]" << strQuery;
            query.prepare(strQuery);
            qDebug() << "Remove table EVENTLOG: " << query.exec() << " error = " << query.lastError();

            strQuery = QString("create table if not exists %1 (%2 TEXT, %3 TEXT, %4 TEXT, %5 TEXT, %6 TEXT)");
            strQuery = strQuery.arg(TABLE_EVENTLOG, COL_DATE, COL_FILE_NAME, COL_STATUS, COL_MD5, COL_LINK);
            qDebug() << "[===2===]" << strQuery;
            query.prepare(strQuery);
            qDebug() << "Create table EVENTLOG: " << query.exec() << " error = " << query.lastError();
        }
    }\

    return true;
}

bool SqlLiteProvider::clearLog()
{
    qDebug() << "Remove all log..."; //populate with some data
    if(db.open()) {
        QSqlQuery query(db);
        QString strQuery = QString("delete from %1").arg(TABLE_EVENTLOG);
        return query.exec(strQuery);
    }
    return false;
}

bool SqlLiteProvider::insertLog(EventLog eventlog) {
    qDebug() << "Insert log..."; //populate with some data
    if(db.open()) {
        QSqlQuery query(db);
        QString strQuery = QString("INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES (:date, :fileName, :status, :md5, :link)");
        strQuery = strQuery.arg(TABLE_EVENTLOG, COL_DATE, COL_FILE_NAME, COL_STATUS, COL_MD5, COL_LINK);
        query.prepare(strQuery);
        query.bindValue(":date", eventlog.getDate());
        query.bindValue(":fileName", eventlog.getFileName());
        query.bindValue(":status", eventlog.getStatus());
        query.bindValue(":md5", eventlog.getMD5());
        query.bindValue(":link", eventlog.getLink());
        return query.exec();
    }
    return false;
}
