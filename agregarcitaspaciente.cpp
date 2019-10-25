#include "agregarcitaspaciente.h"
#include <QDebug>
#include <QSqlQuery>

agregarCitasPaciente::agregarCitasPaciente()
{
    database=QSqlDatabase::database();
}

agregarCitasPaciente::agregarCitasPaciente(QString hostName, int port, QString dbName, QString userName, QString pass){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(pass);
    db.open();
}

bool agregarCitasPaciente::citasPaciente(QString matricula,QString fecha, QString hora, QString doctor, QString sintomas,QString idcitaExterna){


    //query de insertado
    QSqlQuery insertar;

        if(insertar.exec("insert into cita(matricula,fecha,hora,doctor,sintomas)values("+ matricula +",'"+ fecha +"','"+ hora +"',"+ doctor +",'"+ sintomas +"',"+ idcitaExterna +")") ){
            return true;
        }
        else {
            qDebug()<<insertar.lastError().text();
            return false;
         }

    }




bool agregarCitasPaciente::citasExternas(QString matricula, QString nombreCompleto, QString lugarOrigen, QString edad, QString sexo, QString fecha, QString hora , QString doctor, QString sintomas)
{
    QSqlQuery insertar;
    if(insertar.exec("insert into citaExterna() values("+ matricula +",'"+ nombreCompleto +"','"+ lugarOrigen +"',"+ edad +",'"+ sexo +"','"+ fecha +"','"+ hora +"',"+ doctor +",'"+ sintomas +"')"))
    {

        QString idcitaExterna=
        insertar.lastInsertId().toString();

        if(insertar.exec("insert into cita(matricula,fecha,hora,doctor,sintomas,idcitaExterna)values("+ matricula +",'"+ fecha +"','"+ hora +"',"+ doctor +",'"+ sintomas +"',"+ idcitaExterna +")") ){
            return true;
        }else
        {

        }
    }
}

