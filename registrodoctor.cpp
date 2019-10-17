#include "registrodoctor.h"

registroDoctor::registroDoctor(){
     database=QSqlDatabase::database();
}

registroDoctor::registroDoctor(QString hostName, int port, QString dbName, QString userName, QString pass){
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(hostName);
    database.setPort(port);
    database.setDatabaseName(dbName);
    database.setUserName(userName);
    database.setPassword(pass);
    database.open();

}

QString registroDoctor::generarMatricula(){
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

QString registroDoctor::registroDoc(QString nombre,QString apeMaterno,QString apePaterno,QString fecha,QString email,QString telefono,QString especiDoc,
                               QString cedula,QString universidad,QString passwordRegistro2,QString foto,QString edad, QString idpregunta,QString respuesta){

    QString matricula,horario,idconsultorio;
    horario="Vespertino";
    idconsultorio="1";
    //metodo para generar la matricula
    matricula=generarMatricula();
    if(registrodoc.exec("insert into usuario(matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta)values('"+ matricula+"','"+passwordRegistro2+"','"+nombre+"','"+apeMaterno+"','"+apePaterno+"','"+fecha+"','"+edad+"','"+email+"','"+telefono+"',LOAD_FILE('"+foto+"'),'"+idpregunta+"','"+respuesta+"')")){
       //Si se creo correctamente, agregamos a la tabla doctor
            registrodoc.next();
        if(registrodoc.exec("insert into doctor(iddoctor,idUser,idEspecialidad,horario,idpuesto,cedulapro,universidad,idconsultorio,estado)values(NULL,'"+matricula+"','"+especiDoc+"','"+horario+"','1','"+cedula+"','"+universidad+"','"+idconsultorio+"','0')")){
                registrodoc.next();
            return matricula;

            }
                else qDebug()<<"1: "+registrodoc.lastError().text();

        }
             else qDebug()<<"2: "+registrodoc.lastError().text();
     //en caso de que no haya entrado a la sentencia correcta o errores de consultas.
   return  "0";

}


