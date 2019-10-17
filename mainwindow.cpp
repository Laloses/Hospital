#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include "registrostaff.h"
#include "registropaciente.h"
#include <QtSql/QSqlQueryModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Conexion a la base de datos
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("lobohospital");
    database.setUserName("root");
    database.setPassword("");
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

    //Cargar de la base de datos los puestos
    QSqlQueryModel *queryPuestos;
    queryPuestos= new QSqlQueryModel;
    queryPuestos->setQuery("SELECT nombre FROM puesto");
    ui->comboBox_puesto->setModel(queryPuestos);
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

        //Cargar de la base de datos los puestos
        QSqlQueryModel *queryPuestos;
        queryPuestos= new QSqlQueryModel;
        queryPuestos->setQuery("SELECT nombre FROM puesto");
        ui->comboBox_puesto->setModel(queryPuestos);

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

//Cuando le da a agregar imagen
void MainWindow::on_pushButton_imgPerfil_clicked()
{
    //Abrimos el dialogo para obtener la ruta
     imgRoute = QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto = file.readAll();

    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto);
    int w=ui->label_imgPerfil->width();
    int h=ui->label_imgPerfil->height();
    ui->label_imgPerfil->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::KeepAspectRatio));
}

//Para verificar contraseñas iguales
bool MainWindow::verificarPasswordRegistro(){
    if(ui->lineEdit_password1->text() == ui->lineEdit_password2->text() && !ui->lineEdit_password1->text().isEmpty() ){
        return true;
    }
    QMessageBox::warning(this, "Contraseñas incorrectas", "Las contraseñas no coinciden.");
    return false;
}

//Verifica datos para todos los tipos de usuario
bool MainWindow::verificarDatosRegistro(){
    bool flag=false;
    QString estiloBueno, estiloMalo;
    estiloMalo="background-color: rgb(255,0,0);";
    estiloBueno="background-color: rgb(255,255,255);";
    QRegExp re("[a-zZ-A]"), re2("[0-9]");

    //Que el nombre y apellidos y contenga solo letras
   if( ui->lineEdit_nombre->text().contains(re) && ui->lineEdit_apePaterno->text().contains(re) &&
           ui->lineEdit_apeMaterno->text().contains(re) ){
       ui->lineEdit_nombre->setStyleSheet(estiloBueno);
       ui->lineEdit_apePaterno->setStyleSheet(estiloBueno);
       ui->lineEdit_apeMaterno->setStyleSheet(estiloBueno);
           if( !ui->lineEdit_email->text().isEmpty() ){
               ui->lineEdit_email->setStyleSheet(estiloBueno);
               if( ui->lineEdit_telefono->text().contains(re2) ){
                   ui->lineEdit_telefono->setStyleSheet(estiloBueno);
                   if ( !foto.isEmpty() ){
                       ui->label_imgPerfil->setStyleSheet(estiloBueno);
                       flag= true;
                   }
                   //Si no ha puesto la foto
                   else{
                       ui->label_imgPerfil->setStyleSheet(estiloMalo);
                   }
               }
               //Si no puso el telefono
               else{
                   ui->lineEdit_telefono->setStyleSheet(estiloMalo);
               }
           }
           //Si no puso el email
           else{
               ui->lineEdit_email->setStyleSheet(estiloMalo);
           }
   //En caso de que el nombre o apellido esté vacío
   }
   else{
       ui->lineEdit_nombre->setStyleSheet(estiloMalo);
       ui->lineEdit_apePaterno->setStyleSheet(estiloMalo);
       ui->lineEdit_apeMaterno->setStyleSheet(estiloMalo);
   }

    //Revision para los datos de doctor
    if(ui->radioButton_doc->isChecked()){
        if( !ui->lineEdit_cedula->text().isEmpty() ){
            ui->lineEdit_cedula->setStyleSheet(estiloBueno);
            if( !ui->lineEdit_universidad->text().isEmpty() ){
                ui->lineEdit_universidad->setStyleSheet(estiloBueno);
                flag= true;
            }
            //Si no puso universidad
            else{
                ui->lineEdit_universidad->setStyleSheet(estiloMalo);
            }
        }
        //Si no puso cedula
        else{
            ui->lineEdit_cedula->setStyleSheet(estiloMalo);
        }
    }

    if(!flag) QMessageBox::warning(this,"Faltan campos","Por favor complete todos los campos.");
    return flag;
}


//Para calcular la edad de un usuario que se registra
QString MainWindow::calcularEdad(QString fechaN){
    QString edad="0";
    QSqlQuery fechaActual;
    //Restamos los años, pero comparamos si ya paso el mes de su fecha de nacimiento
    if( fechaActual.exec("select YEAR(CURDATE())-YEAR("+fechaN+") + IF(DATE_FORMAT(CURDATE(),'%m-%d') > DATE_FORMAT("+fechaN+",'%m-%d'), 0 , -1 )") ){
        fechaActual.next();
        edad=fechaActual.value(0).toString();
        return edad;
    }
    //Si no entra devuelve 0
    //Quitar este messageBox si funciona bien
    QMessageBox::warning(this,"","No se pudo calcular la edad, devolviendo 0");
    return edad;
}

//Cuando le da a registrarse depues de llenar los datos
void MainWindow::on_pushButton_registrarse_clicked()
{
   if(verificarDatosRegistro() && verificarPasswordRegistro()){
            //Pasamos a la pagina de preguntas
            ui->stackedWidget_registros->setCurrentIndex(2);
            //Cargamos las preguntas
            QSqlQueryModel *queryPreguntas;
            queryPreguntas = new QSqlQueryModel;
            queryPreguntas->setQuery("SELECT pregunta FROM pregunta");
            ui->comboBox_pregunta->setModel(queryPreguntas);
   }
}

//Cuando se elige una pregunta
void MainWindow::on_pushButton_respuesta_clicked()
{
    registroStaff regStaff;
    registropaciente regPaciente;
    registroDoctor regDoctor;
    QString correcto;
    QString idPuesto=QString::number( ui->comboBox_puesto->currentIndex()+1);
    QString idPregunta= QString::number(ui->comboBox_pregunta->currentIndex()+1);

    //si no ha escrito una respuesta
    if( ui->lineEdit_respuesta->text().isEmpty() ){
        QMessageBox::warning(this,"Completa el campo.", "Por favor escriba una respuesta.");
        //el return es para que no entre al siguiente pedazo de codigo
        return;
    }
       if(ui->radioButton_staff->isChecked()){
           //Guardamos la matricula que genere el metodo registrar
           correcto = regStaff.registrar(
                              //El index es el id de puesto
                               idPuesto,
                               //Cualquiera de las password funciona
                              ui->lineEdit_password1->text(),
                              ui->lineEdit_nombre->text(),
                              ui->lineEdit_apePaterno->text(),
                              ui->lineEdit_apeMaterno->text(),
                              ui->dateEdit_fNacimiento->text(),
                               //La edad
                              calcularEdad(ui->dateEdit_fNacimiento->text()),
                               //Juntamos el correo que ingresó
                              ui->lineEdit_email->text()+ui->comboBox_email->currentText(),
                              ui->lineEdit_telefono->text(),
                              imgRoute,
                             idPregunta,
                              ui->lineEdit_respuesta->text());
       }

       if(ui->radioButton_paciente->isChecked()){
           correcto = regPaciente.registroPaciente(

                               //Cualquiera de las password funciona
                              ui->lineEdit_password1->text(),
                              ui->lineEdit_nombre->text(),
                              ui->lineEdit_apePaterno->text(),
                              ui->lineEdit_apeMaterno->text(),
                              ui->dateEdit_fNacimiento->text(),
                               //La edad
                              calcularEdad(ui->dateEdit_fNacimiento->text()),
                               //Juntamos el correo que ingresó
                              ui->lineEdit_email->text()+ui->comboBox_email->currentText(),
                              ui->lineEdit_telefono->text(),
                              imgRoute,
                             idPregunta,
                              ui->lineEdit_respuesta->text());
       }
       if(ui->radioButton_doc->isChecked()){
           correcto = regDoctor.registroDoc(
                       ui->lineEdit_nombre->text(),
                       ui->lineEdit_apePaterno->text(),
                       ui->lineEdit_apeMaterno->text(),
                       ui->dateEdit_fNacimiento->text(),
                        ui->lineEdit_email->text()+ui->comboBox_email->currentText(),
                       ui->lineEdit_telefono->text(),
                       ui->comboBox_especiDoc->currentText(),
                       ui->lineEdit_cedula->text(),
                       ui->lineEdit_universidad->text(),
                       ui->lineEdit_password2->text(),
                       imgRoute,
                       calcularEdad(ui->dateEdit_fNacimiento->text()),
                       idPregunta,
                        ui->lineEdit_respuesta->text());

       }
    //Si el registro si se completó
    if(correcto != "0"){
        QMessageBox::information(this,"","Registrado con exito. Tu id de usuario es: "+correcto+".\n No pierdas esa información.");
    }
    else {
        QMessageBox::critical(this,"No se Registro", "Hay un error en el servidor, intente más tarde.");
    }



}
