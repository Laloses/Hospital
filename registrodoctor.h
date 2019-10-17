#ifndef REGISTRODOCTOR_H
#define REGISTRODOCTOR_H
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtSql>
class registroDoctor
{
public:
    registroDoctor();
    QString registroDoc(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    registroDoctor(QString ,int,QString,QString,QString);
    QString generarMatricula();
private:
     QSqlQuery registrodoc,queryYear;
     QSqlDatabase database;

};

#endif // REGISTRODOCTOR_H
