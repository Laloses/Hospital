#ifndef REGISTROSTAFF_H
#define REGISTROSTAFF_H
#include "qstring.h"
#include <QtSql/QSqlQuery>
#include <QByteArray>


class registroStaff
{
public:
    registroStaff();
    QString registrar(QString idPuesto,
                      QString clave,
                      QString nombre,
                      QString apePaterno,
                      QString apeMaterno,
                      QString fechaN,
                      QString edad,
                      QString email,
                      QString telefono,
                      QString fotop,
                      QString idpregunta,
                      QString respuesta,
                      QString sexo);
private:
    QSqlQuery queryRegistro,queryYear;
    QSqlDatabase database;
    QString generarMatricula();
    //comentario
};

#endif // REGISTROSTAFF_H
