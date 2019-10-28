#ifndef AGREGARCITASPACIENTE_H
#define AGREGARCITASPACIENTE_H
#include "qstring.h"
#include "QVariant"
#include <QtSql/QSqlQuery>
#include <QSqlError>
class agregarCitasPaciente
{
private:
    QSqlQuery q;
    QSqlDatabase db;
    QSqlQuery queryRegistro,queryYear;
    QSqlDatabase database;
public:
    agregarCitasPaciente();
    agregarCitasPaciente(QString, int, QString, QString, QString);
    bool citasPaciente(QString,QString,QString,QString,QString,QString);
    bool citasExternas(QString,QString,QString,QString,QString,QString,QString,QString,QString);
};

#endif // AGREGARCITASPACIENTE_H
