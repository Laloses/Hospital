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
                      QByteArray fotop);
private:
    QSqlQuery queryRegistro;
};

#endif // REGISTROSTAFF_H
