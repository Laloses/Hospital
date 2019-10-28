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

bool agregarCitasPaciente::citasPaciente(QString matricula,QString fecha, QString hora, QString doctor, QString sintomas){

    bool res=false;
    //query de insertado
    QSqlQuery insertar;

        if(insertar.exec("insert into cita(matricula,fecha,hora,doctor,sintomas,idcitaExterna,estado) values("+ matricula +",'"+ fecha +"','"+ hora +"',"+ doctor +",'"+ sintomas +"',NULL,false)") ){
            res= true;
        }
        else {
            qDebug()<<insertar.lastError().text();
         }
    return res;
}




bool agregarCitasPaciente::citasExternas(QString matricula, QString nombreCompleto, QString lugarOrigen, QString edad, QString sexo, QString fecha, QString hora , QString doctor, QString sintomas)
{
    bool res=false;
    QSqlQuery insertar;
    if(insertar.exec("insert into citaExterna(matricula,nombreCompleto,lugarOrigen,edad,sexo) values("+ matricula +",'"+ nombreCompleto +"','"+ lugarOrigen +"',"+ edad +",'"+ sexo+"')") )
    {

        QString idcitaExterna;
        idcitaExterna = insertar.lastInsertId().toString();

        if(insertar.exec("insert into cita(matricula,fecha,hora,doctor,sintomas,idcitaExterna,estado) values("+ matricula +",'"+ fecha +"','"+ hora +"',"+ doctor +",'"+ sintomas +"',"+ idcitaExterna +",false)") ){
            res=true;
        }
        else qDebug()<<"Error en cita"<<insertar.lastError().text();
    } else qDebug()<<"Error en cita externa"<<insertar.lastError().text();
    return res;
}

