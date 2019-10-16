#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSql/QSqlError>

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
    //Ocultamos el boton salir
    ui->pushButton_salir->setHidden(true);
    id_staff=id_doctor=id_usuario=id_paciente="0";
}

MainWindow::~MainWindow()
{
    delete ui;
}
//********************** SPRINT 1 ***************************
//********************** SPRINT 1 ***************************
//********************** SPRINT 1 ***************************

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
    //Que ya este seleccionado para meter info
    ui->lineEdit_idUsuario->setFocus();
}

//Cuando da clic en el boton de registro
void MainWindow::on_pushButton_registro_clicked()
{
    //Página de login
    ui->stackedWidget_principal->setCurrentIndex(1);
    //Ver lo de registros
    on_pushButton_verRegistros_clicked();
    ui->lineEdit_nombre->setFocus();
}

//Cuando da clic en el boton para mostrar los datos de registro
void MainWindow::on_pushButton_verRegistros_clicked()
{
    //Página con los datos de registro
    ui->stackedWidget_registros->setCurrentIndex(1);
    //Marcamos el radio button de paciente por default
    ui->lineEdit_nombre->setFocus();
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

//Iniciar sesion
void MainWindow::on_pushButton_iniciarSesion_clicked()
{
    //Mostrar boton salir
    ui->pushButton_salir->setHidden(false);
    //Pagina paciente
    ui->stackedWidget_principal->setCurrentIndex(2);
    ui->pushButton_login->setHidden(true);
    ui->pushButton_registro->setHidden(true);
}

//Salir
void MainWindow::on_pushButton_salir_clicked()
{
    id_usuario=id_staff=id_doctor=id_paciente="0";
    //Pagina principal
    ui->stackedWidget_principal->setCurrentIndex(0);
    //ocultar boton salir
    ui->pushButton_salir->setHidden(true);
    //Mostrar botones de login y registrar
    ui->pushButton_login->setHidden(false);
    ui->pushButton_registro->setHidden(false);
}
