#ifndef LOGIN_H
#define LOGIN_H
#include <QMainWindow>
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlDatabase>

class login
{
public:
    login();
    int ingresar(QString,QString,QSqlDatabase);
    QString getIdUser();
    QString getIdStaff();
    QString getIdPaciente();
    QString getIdDoctor();
    QSqlDatabase recurso;
    QString idUser, idStaff, idPaciente, idDoctor;
};

#endif // LOGIN_H
