#ifndef AGREGARTIPREMEDIO_H
#define AGREGARTIPREMEDIO_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtSql>

class agregarTipRemedio
{
 public:
    agregarTipRemedio();
    agregarTipRemedio(QString ,int,QString,QString,QString);
    void agregaTip(QString,QString,QString,QString);
    void agregaRemedio(QString,QString,QString,QString,QString);
    void editarRemedio(QString,QString,QString,QString,QString);

 private:
    QSqlDatabase database;

};

#endif // AGREGARTIPREMEDIO_H
