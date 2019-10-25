#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include "registrostaff.h"
#include "registropaciente.h"
#include <QtSql/QSqlQueryModel>
#include "login.h"
#include <QMessageBox>
#include "agregaractividaddoctor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //titulo de la pagina principal del programa
    setWindowTitle("LOBO HOSPITAL");
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
    datosPac = new QSqlQuery;
    datosDoc = new QSqlQuery;
    datosStaff = new QSqlQuery;
    datosUser = new QSqlQuery;
    //La primera pagina que mostramos es la principal
    ui->stackedWidget_principal->setCurrentIndex(0);
    //Ocultamos el boton salir
    ui->pushButton_salir->setHidden(true);
    ui->pushButton_miPerfil->setHidden(true);
    id_staff=id_doctor=id_usuario=id_paciente="0";

    //Modo de contraseñas
    ui->lineEdit_password1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_passwordLogin->setEchoMode(QLineEdit::Password);
    id_usuario=id_staff=id_doctor=id_paciente="0";
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

    //Página con los datos de registro
    ui->stackedWidget_registros->setCurrentIndex(1);
    //Marcamos el radio button de paciente por default
    ui->lineEdit_nombre->setFocus();
    ui->radioButton_paciente->setChecked(true);

    //Quitamos los datos de doc
    ui->comboBox_especiDoc->setHidden(true);
    ui->label_especiDoc->setHidden(true);
    ui->lineEdit_cedula->setHidden(true);
    ui->label_cedula->setHidden(true);
    ui->lineEdit_universidad->setHidden(true);
    ui->label_universidad->setHidden(true);

    //Quitamos los datos del staff
    ui->comboBox_puesto->setHidden(true);
    ui->label_puesto->setHidden(true);

    //Cargar de la base de datos los puestos
    QSqlQueryModel *queryPuestos;
    queryPuestos= new QSqlQueryModel;
    queryPuestos->setQuery("SELECT nombre FROM puesto");
    ui->comboBox_puesto->setModel(queryPuestos);
}

//Cuando da click en el radio button para registrarse como doctor
void MainWindow::on_radioButton_doc_toggled(bool checked)
{
    if(checked){
        //Mostramos los datos de doc
        ui->comboBox_especiDoc->setHidden(false);
        ui->label_especiDoc->setHidden(false);
        ui->lineEdit_cedula->setHidden(false);
        ui->label_cedula->setHidden(false);
        ui->lineEdit_universidad->setHidden(false);
        ui->label_universidad->setHidden(false);

        //Cargar de la base de datos los puestos
        QSqlQueryModel *queryEspecialidades;
        queryEspecialidades= new QSqlQueryModel;
        queryEspecialidades->setQuery("SELECT nombre FROM especialidad");
        ui->comboBox_especiDoc->setModel(queryEspecialidades);

        //Quitamos los datos del staff
        ui->comboBox_puesto->setHidden(true);
        ui->label_puesto->setHidden(true);
    }
}

//Cuando da click en el radio button para registrarse como doctor
void MainWindow::on_radioButton_staff_toggled(bool checked)
{
    if(checked){
        //Mostramos los datos del staff
        ui->comboBox_puesto->setHidden(false);
        ui->label_puesto->setHidden(false);

        //Cargar de la base de datos los puestos
        QSqlQueryModel *queryPuestos;
        queryPuestos= new QSqlQueryModel;
        queryPuestos->setQuery("SELECT nombre FROM puesto");
        ui->comboBox_puesto->setModel(queryPuestos);

        //Si estaba marcado el radio button de doctor
        if(ui->comboBox_especiDoc->isVisible()){
            //Quitamos los datos de doc
            ui->comboBox_especiDoc->setHidden(true);
            ui->label_especiDoc->setHidden(true);
            ui->lineEdit_cedula->setHidden(true);
            ui->label_cedula->setHidden(true);
            ui->lineEdit_universidad->setHidden(true);
            ui->label_universidad->setHidden(true);
        }
    }
}

//Cuando da click en el radio button para registrarse como paciente
void MainWindow::on_radioButton_paciente_toggled(bool checked)
{
    if(checked){
        //Quitamos los datos que no sean de paciente
        //Quitamos los datos del staff
        ui->comboBox_puesto->setHidden(true);
        ui->label_puesto->setHidden(true);

        //Si estaba marcado el radio button de doctor
        if(ui->comboBox_especiDoc->isVisible()){
            //Quitamos los datos de doc
            ui->comboBox_especiDoc->setHidden(true);
            ui->label_especiDoc->setHidden(true);
            ui->lineEdit_cedula->setHidden(true);
            ui->label_cedula->setHidden(true);
            ui->lineEdit_universidad->setHidden(true);
            ui->label_universidad->setHidden(true);
        }
    }
}

//Iniciar sesion
void MainWindow::on_pushButton_iniciarSesion_clicked()
{
    QMessageBox informacion;
    int tipo;
    QString user=ui->lineEdit_idUsuario->text();
    QString clave=ui->lineEdit_passwordLogin->text();

    if(user==""||clave=="")
    {
        qDebug()<<"entre por que no escribi nada";
        informacion.setWindowTitle("Informacion");
        informacion.setText ("Usuario o clave incorrectos, intente de nuevo");
        informacion.setStandardButtons( QMessageBox::Ok) ;
        informacion.setDefaultButton (QMessageBox ::Ok ) ;
        informacion.setButtonText( QMessageBox::Ok,"Aceptar");
        informacion.exec();
    }else {

        qDebug()<<"entre por que no escribi nada 2";
        login lo;
        tipo=lo.ingresar(user,clave,database);

        if(tipo==5)
        {
         qDebug()<<"es un admi";
         //Lo mandamos a su pagina
         ui->stackedWidget_principal->setCurrentIndex(5);
         ui->pushButton_salir->setHidden(false);
         ui->pushButton_login->setHidden(true);
         ui->pushButton_registro->setHidden(true);
         //Ocultamos cosas del menu de arriba
         ocultarMenuP();
         ui->lineEdit_idUsuario->clear();
         ui->lineEdit_passwordLogin->clear();
        }
        else if(tipo==0)
        {
            qDebug()<<"no exite el usuario";
            informacion.setWindowTitle("Informacion");
            informacion.setText ("No existe la cuenta");
            informacion.setStandardButtons( QMessageBox::Ok) ;
            informacion.setDefaultButton (QMessageBox ::Ok ) ;
            informacion.setButtonText( QMessageBox::Ok,"Aceptar");
            informacion.exec();
        }
        else if(tipo==1)
        {
            qDebug()<<"tienes tu clave incorrecta";
            informacion.setWindowTitle("Informacion");
            informacion.setText ("Usuario o clave incorrectos, intente de nuevo");
            informacion.setStandardButtons( QMessageBox::Ok) ;
            informacion.setDefaultButton (QMessageBox ::Ok ) ;
            informacion.setButtonText( QMessageBox::Ok,"Aceptar");
            informacion.exec();
        }
        else if(tipo==2)
        {
            qDebug()<<"eres un paciente";
            ui->pushButton_salir->setHidden(false);
            ui->pushButton_login->setHidden(true);
            ui->pushButton_registro->setHidden(true);
            //Mostramos boton perfil
            ui->pushButton_miPerfil->setHidden(false);
            ui->lineEdit_idUsuario->clear();
            ui->lineEdit_passwordLogin->clear();

            id_paciente=lo.getIdPaciente();
            id_usuario=lo.getIdUser();
            on_pushButton_miPerfil_clicked();
        }
        else if(tipo==3)
        {
            qDebug()<<"eres un doctor";
            ui->pushButton_salir->setHidden(false);
            ui->pushButton_login->setHidden(true);
            ui->pushButton_registro->setHidden(true);
            //Mostramos boton perfil
            ui->pushButton_miPerfil->setHidden(false);
            ui->lineEdit_idUsuario->clear();
            ui->lineEdit_passwordLogin->clear();
            //Ocultamos cosas del menu de arriba
            ocultarMenuP();

            id_doctor=lo.getIdDoctor();
            id_usuario=lo.getIdUser();
            on_pushButton_miPerfil_clicked();
            cargarHorarioDoc();
        }
        else if(tipo==4)
        {
            qDebug()<<"eres de staff";
            ui->pushButton_salir->setHidden(false);
            ui->pushButton_login->setHidden(true);
            ui->pushButton_registro->setHidden(true);
            //Mostramos boton perfil
            ui->pushButton_miPerfil->setHidden(false);
            ui->lineEdit_idUsuario->clear();
            ui->lineEdit_passwordLogin->clear();
            //Ocultamos cosas del menu de arriba
            ocultarMenuP();

            id_staff=lo.getIdStaff();
            id_usuario=lo.getIdUser();
            on_pushButton_miPerfil_clicked();
        }
        else if(tipo==6)
        {
            qDebug()<<"no estas activado";
            informacion.setWindowTitle("Informacion");
            informacion.setText ("Tu cuenta aun no esta activada");
            informacion.setStandardButtons( QMessageBox::Ok) ;
            informacion.setDefaultButton (QMessageBox ::Ok ) ;
            informacion.setButtonText( QMessageBox::Ok,"Aceptar");
            informacion.exec();
        }

    }
}

void MainWindow::ocultarMenuP(){
    ui->comboBox_servicios->setHidden(true);
    ui->pushButton_especialidades->setHidden(true);
    ui->pushButton_infoHospital->setHidden(true);
    ui->pushButton__dirMedico->setHidden(true);
    ui->line->setHidden(true);
    ui->line_2->setHidden(true);
    ui->line_3->setHidden(true);
}

void MainWindow::mostrarMenuP(){
    ui->comboBox_servicios->setHidden(false);
    ui->pushButton_especialidades->setHidden(false);
    ui->pushButton_infoHospital->setHidden(false);
    ui->pushButton__dirMedico->setHidden(false);
    ui->line->setHidden(false);
    ui->line_2->setHidden(false);
    ui->line_3->setHidden(false);
}

//Salir
void MainWindow::on_pushButton_salir_clicked()
{
    id_usuario=id_staff=id_doctor=id_paciente="0";
    //Pagina principal
    ui->stackedWidget_principal->setCurrentIndex(0);
    //ocultar boton salir y mi perfil
    ui->pushButton_salir->setHidden(true);
    ui->pushButton_miPerfil->setHidden(true);
    //Mostrar botones de login y registrar
    ui->pushButton_login->setHidden(false);
    ui->pushButton_registro->setHidden(false);

    //mostramos cosas del menu de arriba
    mostrarMenuP();
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
    ui->label_imgPerfil->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatioByExpanding));
}

//Para verificar contraseñas iguales
bool MainWindow::verificarPasswordRegistro(){
    QString estiloBueno,estiloMalo;
    estiloBueno="border:1px black; border-style:solid";
    estiloMalo="border:2px red; border-style:solid";
    if(ui->lineEdit_password1->text() == ui->lineEdit_password2->text() && !ui->lineEdit_password1->text().isEmpty() ){
        ui->lineEdit_password1->setStyleSheet(estiloBueno);
        ui->lineEdit_password2->setStyleSheet(estiloBueno);
        return true;
    }
    ui->lineEdit_password1->setStyleSheet(estiloMalo);
    ui->lineEdit_password2->setStyleSheet(estiloMalo);
    QMessageBox::warning(this, "Contraseñas incorrectas", "Las contraseñas no coinciden.");
    return false;
}

//Verifica datos para todos los tipos de usuario
bool MainWindow::verificarDatosRegistro(){
    bool flag=false;
    QString estiloBueno, estiloMalo;
    int tamMax=8;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";
    QRegExp re("^[a-zZ-A]*$"), re2("^[0-9]*$");

    //Que el nombre y apellidos y contenga solo letras
   if( ui->lineEdit_nombre->text().contains(re) ){
           ui->lineEdit_nombre->setStyleSheet(estiloBueno);
           if( ui->lineEdit_apePaterno->text().contains(re)){
                ui->lineEdit_apePaterno->setStyleSheet(estiloBueno);
                if( ui->lineEdit_apeMaterno->text().contains(re) ){
                    ui->lineEdit_apeMaterno->setStyleSheet(estiloBueno);
                    if( !ui->lineEdit_email->text().isEmpty() ){
                        ui->lineEdit_email->setStyleSheet(estiloBueno);
                        if( ui->lineEdit_telefono->text().contains(re2) && ui->lineEdit_telefono->text().size()<=tamMax+2){
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
           //En caso de que el apellido paterno
           }
           else{
               ui->lineEdit_apeMaterno->setStyleSheet(estiloMalo);
           }
        //En caso de que el apellido materno este mal
        }
        else{
            ui->lineEdit_apePaterno->setStyleSheet(estiloMalo);
        }
    //En caso de que esta mal el nombre
    }
    else{
        ui->lineEdit_nombre->setStyleSheet(estiloMalo);
    }

    //Revision para los datos de doctor
    if(ui->radioButton_doc->isChecked()){
        if( ui->lineEdit_cedula->text().contains(re2) && ui->lineEdit_cedula->text().size()<=tamMax ){
            ui->lineEdit_cedula->setStyleSheet(estiloBueno);
            if( ui->lineEdit_universidad->text().contains(re) ){
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

    if(!flag) QMessageBox::warning(this,"Faltan campos","Por favor verifique los campos.");
    return flag;
}


//Para calcular la edad de un usuario que se registra
QString MainWindow::calcularEdad(QString fechaN){
    QString edad="0";
    QSqlQuery fechaActual;
    qDebug()<<"fecha: "+fechaN;
    //Restamos los años, pero comparamos si ya paso el mes de su fecha de nacimiento
    if( fechaActual.exec("SELECT YEAR(CURDATE())-YEAR("+fechaN+") + IF( DATE_FORMAT(CURDATE(),'%m-%d') > DATE_FORMAT("+fechaN+",'%m-%d'),0, -1)") ){
        fechaActual.next();
        edad=fechaActual.value(0).toString();
        qDebug()<<"Edad: "+edad;
        return edad;
    }
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
    QString edad =calcularEdad(ui->dateEdit_fNacimiento->text());
    QString idEspecialidad = QString::number(ui->comboBox_especiDoc->currentIndex()+1);

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
                              edad,
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
                              edad,
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
                       idEspecialidad,
                       ui->lineEdit_cedula->text(),
                       ui->lineEdit_universidad->text(),
                       ui->lineEdit_password2->text(),
                       imgRoute,
                       edad,
                       idPregunta,
                        ui->lineEdit_respuesta->text());

       }
    //Si el registro si se completó
    if(correcto != "0"){
        QMessageBox::information(this,"","Registrado con exito. Tu id de usuario es: "+correcto+".\n No pierdas esa información.");
        //Vaciamos las variables
            ui->lineEdit_password1->setText("");
            ui->lineEdit_nombre->setText("");
            ui->lineEdit_apePaterno->setText("");
            ui->lineEdit_apeMaterno->setText("");
            QDate date;
            date.setDate(2009,9,9);
            ui->dateEdit_fNacimiento->setDate(date);
            ui->lineEdit_email->setText("");
            ui->comboBox_email->setCurrentIndex(0);
            ui->lineEdit_telefono->setText("");
            ui->label_imgPerfil->setText("Imagen perfil");
            ui->label_imgPerfil->setPixmap(QPixmap());
            ui->comboBox_pregunta->setCurrentIndex(0);
            ui->lineEdit_respuesta->setText("");
            ui->comboBox_puesto->setCurrentIndex(0);
            ui->comboBox_especiDoc->setCurrentIndex(0);
            ui->lineEdit_cedula->setText("");
            ui->lineEdit_universidad->setText("");
            ui->lineEdit_password2->setText("");
            ui->lineEdit_password1->setText("");

        //movemos al usuario al inicio
        on_pushButton_logo_clicked();
    }
    else {
        QMessageBox::critical(this,"No se Registro", "Hay un error en el servidor, intente más tarde.");
    }

}


//Cuando el usuario ya inicio sesión y quiere ver si perfil
void MainWindow::on_pushButton_miPerfil_clicked()
{
    //Si ya inicio sesión
    if(id_usuario!="0"){
        datosUser->exec("SELECT * FROM usuario WHERE matricula="+id_usuario);
        datosUser->next();
        datosPac->exec("SELECT * FROM paciente WHERE idUser="+id_usuario);
        datosStaff->exec("SELECT * FROM staff WHERE idUser="+id_usuario);
        datosDoc->exec("SELECT * FROM doctor WHERE idUser="+id_usuario);
            if(datosPac->next()){
                cargarDatosUsuarios();
                //Pagina de paciente
                ui->stackedWidget_principal->setCurrentIndex(2);
                //Pagina de sus datos
                ui->stackedWidget_perfilPaciente->setCurrentIndex(0);
                id_paciente=datosPac->value(1).toString();
            }
            if(datosStaff->next()){
                cargarDatosUsuarios();
                id_staff=datosStaff->value(1).toString();
                //Pagina de staff
                ui->stackedWidget_principal->setCurrentIndex(4);
            }
            if(datosDoc->next()){
                cargarDatosUsuarios();
                cargarHorarioDoc();
                id_doctor=datosDoc->value(1).toString();
                //Pagina de doctor
                ui->stackedWidget_principal->setCurrentIndex(3);
            }
    }
}

//Cuando el usuario le da clic para ver su tip del día
void MainWindow::on_pushButton_tip_clicked()
{
      tipdeldia tip(this);
      tip.mostrarTip();
      tip.show();
}

//Funcion para cargar los datos en el perfil del usuario
void MainWindow::cargarDatosUsuarios(){
        if(id_paciente!="0"){
            QPixmap img;
            //Ponemos su imagen
            img.loadFromData(datosUser->value(9).toByteArray());
            //Imagen
            img.scaled(ui->label_imgPerfilPaciente->width(),ui->label_imgPerfilPaciente->height(),Qt::KeepAspectRatio);
            ui->label_imgPerfilPaciente->setPixmap(img);
            //Nombre
            ui->label_pagPaciente->setText(datosUser->value(2).toString()+" "+datosUser->value(3).toString()+" "+datosUser->value(4).toString());
        }
        if(id_doctor!="0"){
            //datosUser.value()
            //datosDoc.value()
            //Nombre
            ui->label_pagDoc->setText(datosUser->value(2).toString()+" "+datosUser->value(3).toString()+" "+datosUser->value(4).toString());
        }
        if(id_staff!="0"){
            //datosUser.value()
            //datosStaff.value()
            //Nombre
            ui->label_pagPersonal->setText(datosUser->value(2).toString()+" "+datosUser->value(3).toString()+" "+datosUser->value(4).toString());
        }
}

void MainWindow::on_lineEdit_passwordLogin_returnPressed()
{
    on_pushButton_iniciarSesion_clicked();
}

void MainWindow::on_pushButton_citasDoc_clicked()
{
    //pag citas doc
    ui->stackedWidget_perfilDoctor->setCurrentIndex(2);
}

void MainWindow::on_pushButton_horarioDoc_clicked()
{
    //Mostrar su horario
    ui->stackedWidget_perfilDoctor->setCurrentIndex(1);
    cargarHorarioDoc();
}

void MainWindow::on_pushButton_datosPaciente_clicked()
{
    //Sus datos
    ui->stackedWidget_perfilPaciente->setCurrentIndex(0);
}

void MainWindow::on_pb_agregarActividadDoc_clicked()
{
    //Mostramos el dialog de actividades
    agregarActividadDoctor *agreAct = new agregarActividadDoctor(this,id_doctor);
    agreAct->exec();
    on_pushButton_horarioDoc_clicked();
}

void MainWindow::cargarHorarioDoc(){
    QString turno;
    QStringList dias, horas;
    turno=datosDoc->value(3).toString();
    int wTable = ui->tableHorario->width();
    int hTable = ui->tableHorario->height();
    ui->tableHorario->setColumnWidth(0,(wTable -ui->tableHorario->verticalHeader()->width())/7);
    ui->tableHorario->setColumnWidth(1,(wTable-ui->tableHorario->verticalHeader()->width())/7);
    ui->tableHorario->setColumnWidth(2,(wTable-ui->tableHorario->verticalHeader()->width())/7);
    ui->tableHorario->setColumnWidth(3,(wTable-ui->tableHorario->verticalHeader()->width())/7);
    ui->tableHorario->setColumnWidth(4,(wTable-ui->tableHorario->verticalHeader()->width())/7);
    ui->tableHorario->setColumnWidth(5,(wTable-ui->tableHorario->verticalHeader()->width())/7);
    ui->tableHorario->setColumnWidth(6,(wTable-ui->tableHorario->verticalHeader()->width())/7);

    ui->tableHorario->setRowHeight(0,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(1,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(2,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(3,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(4,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(5,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(6,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    ui->tableHorario->setRowHeight(7,(hTable-ui->tableHorario->horizontalHeader()->height())/9);
    dias<< "Lunes" << "Martes" << "Miércoles" << "Jueves" << "Viernes" << "Sábado" << "Domingo";
    ui->tableHorario->setColumnCount(7);
    ui->tableHorario->setRowCount(9);
    //-------------------------------------- MATUTINO ----------------------------------
    if(turno=="Matutino"){
        //prueba tablaHorario
        horas<< "05:00" << "06:00" << "07:00" << "08:00" << "09:00" << "10:00" << "11:00" << "12:00" << "13:00";
        ui->tableHorario->setHorizontalHeaderLabels(dias);
        ui->tableHorario->setVerticalHeaderLabels(horas);
    }
    //-------------------------------------- Vespertino ----------------------------------
    else if(turno=="Vespertino"){
        //prueba tablaHorario
        horas<< "13:00" << "14:00" << "15:00" << "16:00" << "17:00" << "18:00" << "19:00" << "20:00" << "21:00";
        ui->tableHorario->setHorizontalHeaderLabels(dias);
        ui->tableHorario->setVerticalHeaderLabels(horas);
    }
    //-------------------------------------- Nocturno ----------------------------------
    else if(turno=="Nocturno"){
        //prueba tablaHorario
        horas<< "21:00" << "22:00" << "23:00" << "00:00" << "01:00" << "02:00" << "03:00" << "04:00" << "05:00";
        ui->tableHorario->setHorizontalHeaderLabels(dias);
        ui->tableHorario->setVerticalHeaderLabels(horas);
    }
    QSqlQuery q;
    int i=0;
    q.exec("SELECT nombreAct,hora,dia FROM horarioDoc WHERE idDoc="+id_doctor);
        //insertamos en la tabla (fila, columna, elemento a insertar)
        while(q.next()){
            QLabel *prueba = new QLabel();
            prueba->setAlignment(Qt::AlignCenter);
            prueba->setStyleSheet("color:white;");
            prueba->setText(q.value(0).toString());

            QWidget *waux = new QWidget();

            QVBoxLayout *layout1 = new QVBoxLayout(waux);
            layout1->addWidget(prueba);
            waux->setLayout(layout1);

            //hora
            int col=0,fil=0;
            for(int j=0; j<8; j++){
                if (horas.at(j) == q.value(1).toString()){
                    fil=j;
                    break;
                }
            }
            //dia
            for(int j=0; j<7; j++){
                if (dias.at(j) == q.value(2).toString()){
                    col=j;
                    break;
                }
            }

            ui->tableHorario->setCellWidget(fil, col, waux);
            ui->tableHorario->setItem(fil, col, new QTableWidgetItem);

            if(q.value(0).toString()=="Consulta"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::blue);
            }
            else
            if(q.value(0).toString()=="Descanso"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::green);
            }
            else{
                ui->tableHorario->item(fil, col)->setBackground(Qt::red);
            }
            ui->tableHorario->setEditTriggers(QAbstractItemView::NoEditTriggers);
            i++;
        }
}

void MainWindow::on_radioCitaPersonal_clicked()
{
    ui->labelNombreComp->setHidden(true);
    ui->lineNombreCompleto->setHidden(true);
    ui->labelOrigen->setHidden(true);
    ui->lineOrigen->setHidden(true);
    ui->labelEdad->setHidden(true);
    ui->lineEdad->setHidden(true);
}

void MainWindow::on_radioCitaExterna_clicked()
{
    ui->labelNombreComp->setHidden(false);
    ui->lineNombreCompleto->setHidden(false);
    ui->labelOrigen->setHidden(false);
    ui->lineOrigen->setHidden(false);
    ui->labelEdad->setHidden(false);
    ui->lineEdad->setHidden(false);
}
