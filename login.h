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
    QSqlDatabase recurso;
};

#endif // LOGIN_H