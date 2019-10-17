#include "registropaciente.h"
#include "mainwindow.cpp"
#include <QSqlQuery>

registropaciente::registropaciente(){
    database=QSqlDatabase::database();
}

QString registropaciente::generarMatricula(){
    QString matri;
    queryYear.exec("SELECT YEAR(CURDATE())");
    queryYear.next();
    //Ponemos el año actual
    matri=queryYear.value(0).toString();
    //Generamos un numero aleatorio de 0 a 9999 y lo agreamos a la matricula
    matri+= QString::number( qrand() % 9999 );
    qDebug()<<"Matricula: "+matri;

    //Verificamos si ya existe en la base de datos
    //Si esta bien ejectuada, el next solo es true cuando encontró algun valor
    if( queryYear.exec("SELECT matricula FROM usuario WHERE matricula='"+matri+"'") )
        if( queryYear.next() )
        //Si ya hay una nueva volvemos a ejecutar la generación
        generarMatricula();
    //Si no hay una matricula, entonces guardamos esa
    return matri;
    //comentario
}

registropaciente::registropaciente(QString hostName, int port, QString dbName, QString userName, QString pass){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(pass);
    db.open();
}

QString registropaciente::registroPaciente(QString clave,QString nombre, QString appaterno, QString apmaterno, QString fechaN, QString edad, QString email, QString telefono,QString fotop,QString idpregunta, QString respuesta){
    QString matricula;

    //Generar matricula
    matricula=generarMatricula();


    //query de insertado
    QSqlQuery insertar;
    if(
    insertar.exec("insert into usuario(matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta)values('"+ matricula +"','"+ clave +"','"+ nombre +"','"+ appaterno +"','"+ apmaterno +"','"+ fechaN +"','"+ edad +"','"+ email +"','"+ telefono +"',LOAD_FILE('"+fotop+"'),'"+ idpregunta +"','"+ respuesta +"')")){

        if( insertar.exec("insert into paciente() value (NULL,'"+matricula+"')") ){
            return matricula;
        }
        else qDebug()<< queryRegistro.lastError().text();
    }
    else qDebug()<< queryRegistro.lastError().text();
    //en caso de que no haya entrado a la sentencia correcta o errores de consultas.
    return "0";

}
