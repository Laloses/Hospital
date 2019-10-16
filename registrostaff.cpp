#include "registrostaff.h"
#include <QVariant>
#include <math.h>

registroStaff::registroStaff(){
}
QString registroStaff::generarMatricula(){
    QString matri;
    queryYear.exec("SELECT YEAR(CURDATE())");
    //Ponemos el año actual
    matri=queryYear.value(0).toString();
    //Generamos un numero aleatorio de 0 a 9999 y lo agreamos a la matricula
    matri+= QString::number( qrand() % 9999 );

    //Verificamos si ya existe en la base de datos
    //Si esta bien ejectuada, el next solo es true cuando encontró algun valor
    if( queryYear.exec("SELECT matricula FROM usuario WHERE matricula="+matri) && queryYear.next() )
        //Si ya hay una nueva volvemos a ejecutar la generación
        generarMatricula();
    //Si no hay una matricula, entonces guardamos esa
    return matri;
}
QString registroStaff::registrar(QString idPuesto,
                                 QString clave,
                                 QString nombre,
                                 QString apePaterno,
                                 QString apeMaterno,
                                 QString fechaN,
                                 QString edad,
                                 QString email,
                                 QString telefono,
                                 QByteArray fotop,
                                 QString idpregunta,
                                 QString respuesta){
        QString sql, matricula;

        //Generar matricula
        matricula=generarMatricula();

        //Guardamos los datos en orden como lo pide la base de datos
        sql=matricula+","+clave+","+nombre+","+apePaterno+","+apeMaterno+","+fechaN+","+edad+","+email+","+telefono+",?,"+idpregunta+","+respuesta;

        //No se ponen explicitamente todos los valores en la parte de "usuario(Valores)" porque sql acepta eso cuando se insertan TODOS los valores
        //Usamos prepare para poder añadir la foto
        queryRegistro.prepare("INSERET INTO usuario VALUES ("+sql+")");
        //Como le pusimos un signo de ?, la sentencia automaticamente lo reemplaza
        queryRegistro.addBindValue(QVariant(fotop));

        if(queryRegistro.exec()){
            //Si se creo correctamente, agregamos a la tabla staff
            //El estado es 0 porque se tiene que revisar
            sql=matricula+","+idPuesto+",0";
            //Si lo ejecuta correcto devolvemos su matricula o id de usuario
            if( queryRegistro.exec("insert into staff values ("+sql+")") ){
                return matricula;
            }
        }
        //en caso de que no haya entrado a la sentencia correcta o errores de consultas.
        return "0";
}
