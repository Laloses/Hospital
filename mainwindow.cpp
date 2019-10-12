#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Conexion a la base de datos
    database = QSqlDatabase::addDatabase("QODBC");
    database.setDatabaseName("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=lobohospital;");
    database.setUserName("root");
    if(!database.open()){
        qDebug()<<database.lastError().text();
    }
    else {
        qDebug()<<"Base de datos conectada";
    }

    //La primera pagina que mostramos es la principal
    ui->stackedWidget_principal->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Cuando da clic en el boton para mostrar los datos de registro
void MainWindow::on_pushButton_verRegistros_clicked()
{
    //Página con los datos de registro
    ui->stackedWidget_registros->setCurrentIndex(1);
    //Marcamos el radio button de paciente por default
    ui->radioButton_paciente->setChecked(true);

    //Quitamos los datos de doc
    ui->comboBox_especiDoc->setVisible(false);
    ui->label_especiDoc->setVisible(false);
    ui->lineEdit_cedula->setVisible(false);
    ui->label_cedula->setVisible(false);
    ui->lineEdit_universidad->setVisible(false);
    ui->label_universidad->setVisible(false);

    //Quitamos los datos del staff
    ui->comboBox_puesto->setVisible(false);
    ui->label_puesto->setVisible(false);
}

//Cuando da clic en el logo lo manda a la principal
void MainWindow::on_pushButton_logo_clicked()
{
    //Página principal
    ui->stackedWidget_principal->setCurrentIndex(0);
}

//Cuando da clic en el boton de login
void MainWindow::on_pushButton_login_clicked()
{
    //Página de login
    ui->stackedWidget_principal->setCurrentIndex(1);
    //No ver lo de registros
    ui->stackedWidget_registros->setCurrentIndex(0);
}

//Cuando da clic en el boton de registro
void MainWindow::on_pushButton_registro_clicked()
{
    //Página de login
    ui->stackedWidget_principal->setCurrentIndex(1);
    //Ver lo de registros
    on_pushButton_verRegistros_clicked();
}

//Cuando da click en el radio button para registrarse como doctor
void MainWindow::on_radioButton_doc_toggled(bool checked)
{
    if(checked){
        //Mostramos los datos de doc
        ui->comboBox_especiDoc->setVisible(true);
        ui->label_especiDoc->setVisible(true);
        ui->lineEdit_cedula->setVisible(true);
        ui->label_cedula->setVisible(true);
        ui->lineEdit_universidad->setVisible(true);
        ui->label_universidad->setVisible(true);

        //Quitamos los datos del staff
        ui->comboBox_puesto->setVisible(false);
        ui->label_puesto->setVisible(false);
    }
}

//Cuando da click en el radio button para registrarse como doctor
void MainWindow::on_radioButton_staff_toggled(bool checked)
{
    if(checked){
        //Mostramos los datos del staff
        ui->comboBox_puesto->setVisible(true);
        ui->label_puesto->setVisible(true);

        //Si estaba marcado el radio button de doctor
        if(ui->comboBox_especiDoc->isVisible()){
            //Quitamos los datos de doc
            ui->comboBox_especiDoc->setVisible(false);
            ui->label_especiDoc->setVisible(false);
            ui->lineEdit_cedula->setVisible(false);
            ui->label_cedula->setVisible(false);
            ui->lineEdit_universidad->setVisible(false);
            ui->label_universidad->setVisible(false);
        }
    }
}

//Cuando da click en el radio button para registrarse como paciente
void MainWindow::on_radioButton_paciente_toggled(bool checked)
{
    if(checked){
        //Quitamos los datos que no sean de paciente
        //Quitamos los datos del staff
        ui->comboBox_puesto->setVisible(false);
        ui->label_puesto->setVisible(false);

        //Si estaba marcado el radio button de doctor
        if(ui->comboBox_especiDoc->isVisible()){
            //Quitamos los datos de doc
            ui->comboBox_especiDoc->setVisible(false);
            ui->label_especiDoc->setVisible(false);
            ui->lineEdit_cedula->setVisible(false);
            ui->label_cedula->setVisible(false);
            ui->lineEdit_universidad->setVisible(false);
            ui->label_universidad->setVisible(false);
        }
    }
}
