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
    QSqlDatabase recurso;
    QString id;
};

#endif // LOGIN_H
