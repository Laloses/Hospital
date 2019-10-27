#include "agregartipremedio.h"

agregarTipRemedio::agregarTipRemedio()
{
    database=QSqlDatabase::database();
}
agregarTipRemedio::agregarTipRemedio(QString hostName, int port, QString dbName, QString userName, QString pass){
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(hostName);
    database.setPort(port);
    database.setDatabaseName(dbName);
    database.setUserName(userName);
    database.setPassword(pass);
    database.open();
}

void agregarTipRemedio::agregaTip(QString  nomTip , QString texto,QString link, QString foto){
    QSqlQuery query;
    qDebug()<<"Agregar tip del dia";
    query.exec("insert into tip(nombreTip,descripcion,link,fotoTip)value('"+nomTip+"','"+texto+"','"+link+"',LOAD_FILE('"+foto+"'))");
    query.next();
}

void agregarTipRemedio::agregaRemedio(QString nomRemedio,QString ingredientes,QString procedimineto,QString foto,QString categoria){
    QSqlQuery query;
    qDebug()<<"Agregar remedio casero";
    query.exec("insert into remedios(nombreRemedio,ingredientes,procedimiento,fotoRemedio,idcategoria)value('"+nomRemedio+"','"+ingredientes+"','"+procedimineto+"',LOAD_FILE('"+foto+"'),'"+categoria+"')");
    query.next();

}
