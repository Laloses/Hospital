#include "agregartipremedio.h"

agregarTipRemedio::agregarTipRemedio()
{
    database=QSqlDatabase::database();
}
//METODO PARA CONEXION DE LA BASE DE DATOS
agregarTipRemedio::agregarTipRemedio(QString hostName, int port, QString dbName, QString userName, QString pass){
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(hostName);
    database.setPort(port);
    database.setDatabaseName(dbName);
    database.setUserName(userName);
    database.setPassword(pass);
    database.open();
}
//METODO PARA AGREGRA TIPS DEL DIA
void agregarTipRemedio::agregaTip(QString  nomTip , QString texto,QString link, QString foto){
    QSqlQuery query;
    qDebug()<<"Agregar tip del dia";
    query.exec("insert into tip(nombreTip,descripcion,link,fotoTip)value('"+nomTip+"','"+texto+"','"+link+"',LOAD_FILE('"+foto+"'))");
    query.next();
}
//METODO PARA AGREGRA REMEDIOS CASEROS
void agregarTipRemedio::agregaRemedio(QString nomRemedio,QString ingredientes,QString procedimineto,QString foto,QString categoria){
    QSqlQuery query;
    qDebug()<<"Agregar remedio casero";
    query.exec("insert into remedios(nombreRemedio,ingredientes,procedimiento,fotoRemedio,idcategoria)value('"+nomRemedio+"','"+ingredientes+"','"+procedimineto+"',LOAD_FILE('"+foto+"'),'"+categoria+"')");
    query.next();

}

//METODO PARA EDITAR LOS REMEDIOS CASEROS
void agregarTipRemedio::editarRemedio(QString idremedio ,QString nomRemedio,QString ingredientes,QString procedimineto,QString categoria){

    QSqlQuery query;
    qDebug()<<"editar remedio casero";
    //nombreRemedio,ingredientes,procedimiento,fotoRemedio,idcategoria
    query.exec("update remedios set nombreRemedio='"+nomRemedio+"',ingredientes='"+ingredientes+"',procedimiento='"+procedimineto+"',idcategoria='"+categoria+"' where idremedio='"+idremedio+"'");
    query.next();

}

//METODO PARA EDITAR LOS TIPS DEL DIA
void agregarTipRemedio::editarTips(QString idtip, QString nomTip, QString descrip, QString link){

    QSqlQuery query;
    qDebug()<<"editar tip del dia";
    //idtip,nombreTip,descripcion,link
    query.exec("update tip set nombreTip='"+nomTip+"',descripcion='"+descrip+"',link='"+link+"' where idtip='"+idtip+"'");
    query.next();

}
