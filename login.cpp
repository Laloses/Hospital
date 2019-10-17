#include "login.h"
#include "mainwindow.h"
#include <QVariant>
#include <QtSql>
#include <QDebug>

login::login()
{

}

int login::ingresar(QString user, QString clave,QSqlDatabase base)
{
    //valores de retorno
    //0 es que no se encontro el usuario
    //1 es que la clave es incorrecta
    //2 es que es un paciente
    //3 es que es un doctor
    //4 es que es staff
    //5 es un administrador
    recurso=base;
    recurso.open();

    QSqlQuery admi2,doctor,staff,paciente,usuarios;
    QString admi1,doctor1,staff1,paciente1,usuarios1;

    admi1="select matricula,clave from admi where matricula='"+user+"'; ";
    usuarios1="select matricula,clave from usuario where matricula='"+user+"';";
    usuarios.exec(usuarios1);
    usuarios.next();
    QString userval,claveval;

    //aqui hare los querys para encontrar el tipo de usuario
    doctor1="select iddoctor,idUser from doctor where idUser='"+user+"'; ";
    staff1="select idstaff,idUser from staff where idUser='"+user+"'; ";
    paciente1="select idpaciente,idUser from paciente where idUser='"+user+"'; ";

    doctor.exec(doctor1);
    staff.exec(staff1);
    paciente.exec(paciente1);


    //pasamos los valores donde se encuentre un usuario
    userval=usuarios.value(0).toString();
    claveval=usuarios.value(1).toString();

    admi2.exec(admi1);
    admi2.next();

    QString user1,clave1;
    user1=admi2.value(0).toString();
    clave1=admi2.value(1).toString();

    //variable para comparar
    QString aux;


    if(user==user1 && clave==clave1)
    {
       qDebug()<<"ingreaste correctamente ";
        return 5;
    }
    else if(user==user1 && clave!=clave1)
    {
        qDebug()<<"tienes la clave incorrecta";
        return 1;
    }
    else if(userval!=" ")
    {
        if(userval==user && claveval==clave)
        {
            bool tipo=true;
            //ya se sabe que es un usuario ahora avriguaremos que tipo es
            while(tipo)
            {
                if(doctor.next())
                {
                    aux=doctor.value(1).toString();
                    if(user==aux)
                    {
                        tipo=false;
                        return 3;
                    }
                }
                else
                {
                    if(paciente.next())
                    {
                        aux=paciente.value(1).toString();
                        if(user==aux)
                        {
                            tipo=false;
                            return 2;
                        }
                    }
                    else
                    {
                        if(staff.next())
                        {
                            aux=staff.value(1).toString();
                            if(user==aux)
                            {
                                tipo=false;
                                return 4;
                            }
                        }
                    }
                }
            }

        }
        else
        {
            return 1;
        }
    }
    return 0;
}
