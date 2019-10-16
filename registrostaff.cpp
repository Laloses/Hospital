#include "registrostaff.h"
#include <QVariant>

registroStaff::registroStaff(){
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
                                 QByteArray fotop){

        QString sql, matricula;
        //Generar matricula


        //Guardamos los datos en orden como lo pide la base de datos
        sql=matricula+","+clave+","+nombre+","+apePaterno+","+apeMaterno+","+fechaN+","+edad+","+email+","+telefono+",?";

        //No se ponen explicitamente todos los valores en la parte de "usuario(Valores)" porque sql acepta eso cuando se insertan TODOS los valores
        //Usamos prepare para poder añadir la foto
        queryRegistro.prepare("insert into usuario values ("+sql+")");
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
