#include "htmltemplate.h"
#include <QtGui>


QString HtmlTemplate::getTemplate(QString templateName)
{
    QString path(":/template/%1");
    path = path.arg(templateName);

    qDebug() << path.toStdString().c_str();

    QFile qfile(path);

    if(!qfile.exists()) {
        qDebug() << "File not exists";
    } else {
        qDebug() << "File co ton tai exists";
    }

    if(!qfile.open(QIODevice::ReadOnly)) {
        return "";
    }

    qfile.close();

    return QString(qfile.readAll());
}

HtmlTemplate::HtmlTemplate()
{
}
