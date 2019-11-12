#ifndef REGISTROPACIENTE_H
#define REGISTROPACIENTE_H
#include "qstring.h"
#include "QVariant"
#include <QtSql/QSqlQuery>

class registropaciente
{
private:
    QSqlQuery q;
    QSqlDatabase db;
    QSqlQuery queryRegistro,queryYear;
     QString generarMatricula();
      QSqlDatabase database;
      QString idUser,idPac;

public:
    registropaciente();
    registropaciente(QString, int, QString, QString, QString);
    QString registroPaciente(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    QString getIdPac();

};

#endif // REGISTROPACIENTE_H
