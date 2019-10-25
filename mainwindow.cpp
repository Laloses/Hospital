#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    matric="";
    UserTipo=0;


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
    ui->comboBox_especiDoc->setHidden(true);
    ui->label_especiDoc->setHidden(true);
    ui->lineEdit_cedula->setHidden(true);
    ui->label_cedula->setHidden(true);
    ui->lineEdit_universidad->setHidden(true);
    ui->label_universidad->setHidden(true);

    //Quitamos los datos del staff
    ui->comboBox_puesto->setHidden(true);
    ui->label_puesto->setHidden(true);
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

//Cuando da click en el radio button para registrarse como Doctor
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

void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
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
         ui->stackedWidget_admin->setCurrentIndex(0);
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
                id_doctor=datosDoc->value(1).toString();
                //Pagina de doctor
                ui->stackedWidget_principal->setCurrentIndex(3);
            }
    }
}

//Cuando el usuario le da clic para ver su tip del día
void MainWindow::on_pushButton_tip_clicked()
{
      static tipdeldia tip;
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
}

void MainWindow::on_pushButton_datosPaciente_clicked()
{
    //Sus datos
    ui->stackedWidget_perfilPaciente->setCurrentIndex(0);
}



void MainWindow::on_pushButton_SolicitudesUsuarios_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(1);
    ui->pushButton_SolicitudesUsuarios->hide();
}



void MainWindow::PonerInfo(QString matri)
{
    ui->label_solicitud->clear();
    ui->lineEdit_nombre_solicitud->clear();
    ui->lineEdit_cedula_solicitud->clear();
    ui->lineEdit_Univercida_solicitud->clear();
    ui->lineEdit_especialidad_solicitud->clear();

    ui->label_solicitud->show();
    ui->lineEdit_nombre_solicitud->show();
    ui->lineEdit_cedula_solicitud->show();
    ui->lineEdit_Univercida_solicitud->show();
    ui->lineEdit_especialidad_solicitud->show();

    QString doc,staff,usua,imagen,nsol;
    QSqlQuery doc1,staff1,usuario;
    QPixmap pix;
    doc="select tipoUser from doctor where idUser='"+matri+"'; ";
    staff="select tipoUser from staff where idUser='"+matri+"'; ";
    doc1.exec(doc);
    staff1.exec(staff);
    usua="select nombre,fotop from usuario where matricula='"+matri+"'; ";
    usuario.exec(usua);
    usuario.next();
    imagen=pix.loadFromData(usuario.value(1).toByteArray());
    int w=ui->label_solicitud->width();
    int h=ui->label_solicitud->height();
     ui->label_solicitud->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
     nsol=usuario.value(0).toString();
     ui->lineEdit_nombre_solicitud->setText(nsol);

    if(doc1.next())
    {
       QString doc2,especialidad,nombreEsp,nombreFEsp;
       doc2="select idEspecialidad,cedulapro,universidad from doctor where idUser='"+matri+"'; ";
       QSqlQuery datos,datos2;
       datos.exec(doc2);
       datos.next();
       especialidad=datos.value(0).toString();
       nombreEsp="select nombre from especialidad where idEsp='"+especialidad+"'; ";
       datos2.exec(nombreEsp);
       datos2.next();
       nombreFEsp=datos2.value(0).toString();
       matric=matri;
       UserTipo=1;
       ui->lineEdit_especialidad_solicitud->setText(nombreFEsp);
       ui->lineEdit_cedula_solicitud->setText(datos.value(1).toString());
       ui->lineEdit_Univercida_solicitud->setText(datos.value(2).toString());


    }
    else if (staff1.next()) {
        QString staff2;
        staff2="select idpuesto from staff where idUser='"+matri+"'; ";
        QSqlQuery datos,datos2;
        datos.exec(staff2);
        datos.next();
        QString puesto,puestonombre;
        puesto=datos.value(0).toString();
        QString puesto1;
        matric=matri;
        UserTipo=2;
       //bloque de line edit
        ui->lineEdit_piso->hide();
        ui->lineEdit_numconsultorio->hide();
        ui->label_piso->hide();
        ui->label_consultorio->hide();
        puesto1="select nombre from puesto where idpuesto='"+puesto+"'; ";
        datos2.exec(puesto1);
        datos2.next();
        puestonombre=datos2.value(0).toString();
         ui->lineEdit_especialidad_solicitud->setText(puestonombre);
         ui->lineEdit_cedula_solicitud->hide();
         ui->lineEdit_Univercida_solicitud->hide();

    }


}


void MainWindow::on_pushButton_AceptarSoli_clicked()
{
   if(ui->radioButton_doctors->isChecked()){
       ui->lineEdit_numconsultorio->show();
       ui->lineEdit_piso->show();
       ui->label_piso->show();
       ui->label_consultorio->show();
   }else if(ui->radioButton_staffs->isChecked()){
       ui->lineEdit_numconsultorio->hide();
       ui->lineEdit_piso->hide();
       ui->label_piso->hide();
       ui->label_consultorio->hide();
}

    if(ui->lineEdit_nombre_solicitud->text()=="" && ui->lineEdit_especialidad_solicitud->text()==""){
        QMessageBox messageBox(QMessageBox::Warning,
                                         tr("Warning"), tr("Por favor,selecione algun candito."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 if (messageBox.exec() == QMessageBox::Yes){
                  }
            }
       else { if (ui->lineEdit_nombre_solicitud->text()!="" && ui->lineEdit_especialidad_solicitud->text()!="") {
        mostrarZonas();
        ui->stackedWidget_admin->setCurrentIndex(2);

        }
    }
}

//----------------------------------------------------------------------------
void MainWindow::mostrarZonas(){

    QSqlQueryModel *queryPuestos;
    queryPuestos= new QSqlQueryModel;
    queryPuestos->setQuery("SELECT nombreArea FROM areah");
    ui->comboBox_area->setModel(queryPuestos);


}


void MainWindow::infoConsultorio(QString idConsultorio){

    qDebug()<<"hola:"<<idConsultorio;
    QString consulta,nAre,numPiso,numConsultorio;
    QSqlQuery query;
    consulta="select are.idarea,are.nombreArea,con.idConsultorio,con.numConsultorio,con.ocupado,are.ubicacion as piso"
               " from areah as are inner join consultorio as con on are.idarea=con.idarea where idConsultorio='"+idConsultorio+"'";
    query.exec(consulta);
    query.next();
    nAre=query.value(1).toString();
    numPiso=query.value(2).toString();
    numConsultorio=query.value(3).toString();
    ui->lineEdit_area->setText(nAre);
    ui->lineEdit_piso->setText(numPiso);
    ui->lineEdit_numconsultorio->setText(numConsultorio);

}

void MainWindow::mostrasConsultorios(){

    QString areConsultorio,consulta,numconsultorio,idconsultorio,ocupado;
    QSqlQuery query;
    ocupado="false";
    areConsultorio=ui->comboBox_area->currentText();
    qDebug()<<areConsultorio;
    consulta="select are.idarea,are.nombreArea,con.idConsultorio,con.numConsultorio,con.ocupado"
               " from areah as are inner join consultorio as con on are.idarea=con.idarea where are.nombreArea='"+areConsultorio+"' and con.ocupado='"+ocupado+"'";
    query.exec(consulta);

    int contF=0;
    int contC=0;
    while(query.next())
    {
        idconsultorio=query.value(2).toString();
        numconsultorio=query.value(3).toString();
        QPushButton *s=new QPushButton();
        s->setAccessibleName(idconsultorio);
        s->setText(numconsultorio);
        ui->lista2->addWidget(s,contF,contC,1,1);
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(s,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(s,idconsultorio);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(infoConsultorio(QString)));
        s->setFixedSize(50,40);
        if(contC==2)
        {
            contF++;
            contC=0;
        }
        contC++;
    }


}


void MainWindow::on_pushButton_regresarSolicitudes_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(1);
}

void MainWindow::on_comboBox_area_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_area->clear();
    ui->lineEdit_piso->clear();
    ui->lineEdit_numconsultorio->clear();
    mostrasConsultorios();
}

void MainWindow::on_pushButton_guardar_clicked()
{
    // update de doctor
    QString actualizacion,turno,numConsul,area,estado;
    QSqlQuery actual;
    estado="1";
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de asignar la area.?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    turno=ui->comboBox_turno->currentText();
    numConsul=ui->lineEdit_numconsultorio->text();
    if(ui->lineEdit_area->text()==""){
        QMessageBox messageBox(QMessageBox::Warning,
                                         tr("Warning"), tr("Por favor,Asigne una area."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 if (messageBox.exec() == QMessageBox::Yes){

                  }
    }else{
        if (message.exec() == QMessageBox::Yes){
    if(userTipo==1){
        actualizacion="update doctor set horario='"+turno+"',idconsultorio='"+numConsul+"',estado='"+estado+"'where idUser='"+matric+"'";
        actual.exec(actualizacion);
        qDebug()<<"Actualizando datos del doctor";
        ui->stackedWidget_admin->setCurrentIndex(1);
        }
    }
        else if (userTipo==2){
        //update de staff
         area=ui->lineEdit_area->text();
        actualizacion="update staff set estado='"+estado+"',idArea='"+area+"',turno='"+turno+"' where idUser='"+matric+"'";
        actual.exec(actualizacion);
        qDebug()<<"Actualizando datos del staff";
        ui->stackedWidget_admin->setCurrentIndex(1);
        }
    }

}



void MainWindow::on_pushButton_rechazarsoli_clicked()
{
    QString consulta,matri;
    QSqlQuery query;
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de rechazar solicitud?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    if(ui->lineEdit_nombre_solicitud->text()=="" && ui->lineEdit_especialidad_solicitud->text()==""){
        QMessageBox messageBox(QMessageBox::Warning,
                                         tr("Warning"), tr("Por favor,selecione algun candito."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Actividad"));
                 if (messageBox.exec() == QMessageBox::Yes){

                  }

    }else  if (message.exec() == QMessageBox::Yes){
        qDebug()<<"matricula:"<<matric;
    if(userTipo==1){
        consulta=" delete from doctor where idUser='"+matric+"'";
        query.exec(consulta);
        consulta="delete from usuario where matricula='"+matric+"'";
        query.exec(consulta);
    }
        else{
        if (userTipo==2) {
          consulta=" delete from staff where idUser='"+matric+"'";
          query.exec(consulta);
          consulta="delete from usuario where matricula='"+matric+"'";
          query.exec(consulta);
          clearLayout(ui->lista);
        }
      }
    }

}

void MainWindow::on_radioButton_doctors_clicked()
{
    qDebug()<<"entre:";

    ui->lineEdit_especialidad_solicitud->clear();
    ui->lineEdit_nombre_solicitud->clear();
    ui->lineEdit_cedula_solicitud->clear();
    ui->lineEdit_Univercida_solicitud->clear();
    ui->lineEdit_Univercida_solicitud->show();
    ui->lineEdit_cedula_solicitud->show();
    ui->label_solicitud->setText(" ");

    clearLayout(ui->lista);
    int cont=0;
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryDoc,queryStaff,userEst;
    consultaDoc="select *from Doctores";
    queryDoc.exec(consultaDoc);
            while(queryDoc.next())
            {
                if(queryDoc.value(5).toString()=="1")
                {

                }
                else {
                nombre=queryDoc.value(0).toString();
                espec=queryDoc.value(4).toString();
                espera="en espera";

                matricula=queryDoc.value(3).toString();

                QPushButton *b=new QPushButton();
                b->setText("Ver Solicitud");
                QLabel *l=new QLabel;
                l->setText(nombre);
                b->setFixedSize(QSize(120,40));
                QSignalMapper *mapper=new QSignalMapper(this);
                connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
                mapper->setMapping(b,matricula);
                connect(mapper,SIGNAL(mapped(QString)),this,SLOT(PonerInfo(QString)));
                QLabel *espacio=new QLabel();
                QLabel *esp=new QLabel();
                esp->setText(espec);
                QLabel *estado=new QLabel();
                estado->setText(espera);
                ui->lista->addWidget(b,cont,0,1,1);
                ui->lista->addWidget(l,cont,1,1,1);
                ui->lista->addWidget(esp,cont,2,1,1);
                ui->lista->addWidget(estado,cont,3,1,1);

                cont++;
                }
            }

}

void MainWindow::on_radioButton_staffs_clicked()
{
    clearLayout(ui->lista);
    ui->lineEdit_especialidad_solicitud->clear();
    ui->lineEdit_nombre_solicitud->clear();
    ui->lineEdit_Univercida_solicitud->clear();
    ui->lineEdit_Univercida_solicitud->hide();
    ui->lineEdit_cedula_solicitud->hide();
    ui->lineEdit_cedula_solicitud->clear();
    ui->label_solicitud->setText(" ");
    int cont=0;
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryDoc,queryStaff,userEst;
    consultaStaff="select *from Staffs";
    queryStaff.exec(consultaStaff);

    while(queryStaff.next())
    {
        if(queryStaff.value(5).toString()=="1")
        {

        }
        else {


        //QString nombre,espec,espera,matricula;
        nombre=queryStaff.value(0).toString();
        espec=queryStaff.value(4).toString();
        espera="En espera.";

        matricula=queryStaff.value(3).toString();

        QPushButton *b=new QPushButton();
        b->setText("Ver Solicitud");
        QLabel *l=new QLabel;
        l->setText(nombre);
        b->setFixedSize(QSize(120,40));
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(b,matricula);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(PonerInfo(QString)));
        QLabel *espacio=new QLabel();
        QLabel *esp=new QLabel();
        esp->setText(espec);
        QLabel *estado=new QLabel();
        estado->setText(espera);
        ui->lista->addWidget(b,cont,0,1,1);
        ui->lista->addWidget(l,cont,1,1,1);

        ui->lista->addWidget(esp,cont,2,1,1);
        ui->lista->addWidget(estado,cont,3,1,1);
        //   ui->lista->addWidget(esp,cont,3,1,1);
        // ui->lista->addWidget(m,cont,4,1,1);
        // ui->gridLayout.add;
        cont++;

        }
    }

}

void MainWindow::on_pushButton_menu_Pincipal_Adm_clicked()
{
    ui->pushButton_SolicitudesUsuarios->show();
    ui->stackedWidget_admin->setCurrentIndex(0);
}
