#include "registropaciente.h"
#include "mainwindow.cpp"
#include <QSqlQuery>

registropaciente::registropaciente(){
    database=QSqlDatabase::database();
}

QString registropaciente::generarMatricula(){
    QString matri="0";
    queryYear.exec("SELECT YEAR(CURDATE())");
    queryYear.next();
    //Ponemos el año actual
    matri=queryYear.value(0).toString();
    //Generamos un numero aleatorio de 0 a 9999 y lo agreamos a la matricula
    while(true){
        matri+= QString::number( qrand() % 9999 );
        qDebug()<<"Matricula: "+matri;

        //Verificamos si ya existe en la base de datos
        //el next solo es true cuando encontró algun valor
        queryYear.exec("SELECT matricula FROM usuario WHERE matricula='"+matri+"'");
        if( queryYear.next() ){
            matri=queryYear.value(0).toString();
        }
        else{
            break;
        }

    }
    //devolvemos la matricula encontrada
    return matri;
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

QString registropaciente::registroPaciente(QString clave,QString nombre, QString appaterno, QString apmaterno, QString fechaN, QString edad, QString email, QString telefono,QString fotop,QString idpregunta, QString respuesta, QString sexo){
    QString matricula;

    //Generar matricula
    matricula=generarMatricula();


    //query de insertado
    QSqlQuery insertar;
    if(
    insertar.exec("insert into usuario(matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta,sexo)values('"+ matricula +"','"+ clave +"','"+ nombre +"','"+ appaterno +"','"+ apmaterno +"','"+ fechaN +"','"+ edad +"','"+ email +"','"+ telefono +"',LOAD_FILE('"+fotop+"'),'"+ idpregunta +"','"+ respuesta +"','"+sexo+"')")){
    idUser=insertar.lastInsertId().toString();
        if( insertar.exec("insert into paciente() value (NULL,'"+matricula+"')") ){
            idPac=insertar.lastInsertId().toString();
            return matricula;
        }
        else qDebug()<< insertar.lastError().text();
    }
    else qDebug()<< insertar.lastError().text();
    //en caso de que no haya entrado a la sentencia correcta o errores de consultas.
    return "0";
}

QString registropaciente::getIdPac(){
    return idPac;
}
