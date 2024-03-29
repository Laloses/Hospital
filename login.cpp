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
    //6 doctor o staff sin permiso
    //4 es que es staff
    //5 es un administrador
    recurso=base;
    recurso.open();

    QSqlQuery admi2,doctor,staff,paciente,usuarios,p1;
    QString admi1,doctor1,staff1,paciente1,usuarios1,p2;

    admi1="select matricula,clave from admin where matricula='"+user+"'";
    usuarios1="select matricula,clave from usuario where matricula="+user;
    usuarios.exec(usuarios1);
    usuarios.next();
    QString userval,claveval;

    //aqui hare los querys para encontrar el tipo de usuario
    doctor1="select iddoctor,idUser,estado,idEspecialidad from doctor where idUser="+user;
    staff1="select idstaff,idUser,estado from staff where idUser="+user;
    p2="select idpuesto from staff where idUser="+user;
    paciente1="select idpaciente,idUser from paciente where idUser="+user;

    doctor.exec(doctor1);
    staff.exec(staff1);
    paciente.exec(paciente1);
    p1.exec(p2);


    //pasamos los valores donde se encuentre un usuario
    userval=usuarios.value(0).toString();
    claveval=usuarios.value(1).toString();

    QString est1,est2;
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
    else if(userval!="")
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
                        est1=doctor.value(2).toString();

                        if(est1=="1")
                        {
                            tipo=false;
                            idDoctor=doctor.value(0).toString();
                            idUser=userval;
                            especialidad=doctor.value(3).toString();
                            return 3;
                        }
                        else {
                            tipo=false;
                            return 6;
                        }

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
                            idPaciente=paciente.value(0).toString();
                            idUser=userval;
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
                                est2=staff.value(2).toString();
                                if(est2=="1")
                                {
                                    tipo=false;
                                    idStaff=staff.value(0).toString();
                                    idUser=userval;
                                    return 4;
                        }
                                else {
                                    tipo=false;
                                    return 6;
                        }
                            }
                        }
                    }
                }
            }

        }
        else
        {
            return 0;
        }
    }
    return 0;
}

QString login::getIdUser(){
    return idUser;
}

QString login::getIdStaff(){
    return idStaff;
}


QString login::getIdPaciente(){
    return idPaciente;
}

QString login::getIdDoctor(){
    return idDoctor;
}

QString login::getEspecialidad(){
    return especialidad;
}



