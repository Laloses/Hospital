#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagarcitaspaciente.h"
#include "ordenarintervencion.h"
#include "ordenarestudios.h"
#include "pagointervenciones.h"
#include "editarusuario.h"
#include "cambiarcontrasenia.h"
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QTimer>
#include <QDate>
#include <QFileDialog>
#include <QFile>
#include <QFileSystemModel>
#include <QPrinter>
#include "remedios.h"
#include "dialogdoctor.h"
#include "permisoLaboral.h"
#include "verPermisosLaborales.h"
#include "infohospital.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //titulo de la pagina principal del programa1
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
    model = new QSqlQueryModel;
    datosPac = new QSqlQuery;
    datosDoc = new QSqlQuery;
    datosStaff = new QSqlQuery;
    datosUser = new QSqlQuery;
    //La primera pagina que mostramos es la principal
    ui->stackedWidget_principal->setCurrentIndex(0);
    //Ocultamos el boton salir
    ui->pushButton_salir->setHidden(true);
    ui->pushButton_miPerfil->setHidden(true);

    //Modo de contraseñas
    ui->lineEdit_password1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_passwordLogin->setEchoMode(QLineEdit::Password);
    ui->tv_listaDocCitas->setHidden(true);
    id_usuario=id_staff=id_doctor=id_paciente="0";
    matric="";
    UserTipo=0;
    toggleVision = 0;
    toggleVision1 = 0;
    contMedicinas=0;
    ui->tv_listaDocCitas->horizontalHeader()->setHidden(false);
    ui->pb_urg->setHidden(true);
    ui->nofi_2->hide();
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
    if(id_paciente.trimmed() != "0" || id_usuario.trimmed()=="0"){
        //Página principal
        ui->stackedWidget_principal->setCurrentIndex(0);
    }
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

        qDebug()<<"escribi algo";
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
         cargarCategoria();
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
            clearLayout(ui->barraDeNoti);

            ui->pushButton_salir->setHidden(false);
            ui->pushButton_login->setHidden(true);
            ui->pushButton_registro->setHidden(true);
            //Mostramos boton perfil
            ui->pushButton_miPerfil->setHidden(false);
            ui->lineEdit_idUsuario->clear();
            ui->lineEdit_passwordLogin->clear();

            id_paciente=lo.getIdPaciente();
            id_usuario=lo.getIdUser();
            ui->pb_remedios->setHidden(false);

            //esto lo puse para habilitar las notificaciones
            verNoti=1;
            ui->nofi->hide();

            QString busca;
            busca="select *from notificacion where UserP='"+id_usuario+"'";
            QSqlQuery buscarNoti;
            buscarNoti.exec(busca);

            int contadorNoti=0;
            int filas=0;

            while(buscarNoti.next())
            {
                QString nueva;
                nueva=buscarNoti.value(4).toString();
                qDebug()<<"ya se vio: "<<nueva;
                if(nueva=="0")
                {
                    contadorNoti++;
              }
            }

            while(buscarNoti.previous()){
                if(buscarNoti.value(1).toString()=="1")
                {
                    QPlainTextEdit *b=new QPlainTextEdit();
                   // QPushButton *b=new QPushButton();
                    b->setPlainText(buscarNoti.value(2).toString());
                    b->setFixedSize(QSize(200,50));
                    b->setStyleSheet("background-color: rgb(151,240,104); ");
                    ui->barraDeNoti->addWidget(b,filas,0,Qt::AlignTop);
                    filas++;
                }else
                {

                 QPlainTextEdit *b=new QPlainTextEdit();
               //QPushButton *b=new QPushButton();
                b->setPlainText(buscarNoti.value(2).toString());
                b->setFixedSize(QSize(200,70));
                b->setStyleSheet("background-color: rgb(243,173,106); ");
                ui->barraDeNoti->addWidget(b,filas,0,Qt::AlignTop);
                filas++;
                }
            }

            QString num=QString::number(contadorNoti);
            qDebug()<<"este numero conte"<<num;
            if(num=="0")
            {
                qDebug()<<"no encontre nada";
            ui->notificacionL->hide();
            }
            else
            {
                qDebug()<<"encontre algo";
                QPixmap pixmap(":/imgs/notification2.png");
                ui->notificacionL->setPixmap(pixmap);
                ui->notificacionL->setStyleSheet("background-color:red; border:solid 1px red; border-radius:500px; color: white;");
                ui->notificacionL->setText(num);
                ui->notificacionL->show();
            }

            on_pushButton_miPerfil_clicked();

        }
        else if(tipo==3)
        {
            especialidad=lo.getEspecialidad();
            qDebug()<<"especialidad"<<especialidad;
            if(especialidad=="19"){
             qDebug()<<"eres un doctor pero tu eres el triar";
             ui->lineEdit_idUsuario->clear();
             ui->lineEdit_passwordLogin->clear();
             ui->stackedWidget_principal->setCurrentIndex(3);
             ui->stackedWidget_perfilDoctor->setCurrentIndex(9);
             ui->pushButton_salir->setHidden(false);
             ui->pushButton_login->setHidden(true);
             ui->pushButton_registro->setHidden(true);

             ui->pushButton_horarioDoc->hide();
             ui->pb_rechazarCitas->hide();
             ui->pb_realizarConsulta->hide();
             ui->pushButton_historial->hide();
             ui->pb_inter->hide();
             ui->pushButton_4->hide();
             ui->butonNotifi_4->hide();
             ui->notificacionL_4->hide();
             ui->line_10->hide();
             ui->line_7->hide();
             ui->line_12->hide();
             ui->line_14->hide();

             ui->pushButton_miPerfil->setHidden(false);
             ui->pb_urg->setHidden(false);
             id_usuario=lo.getIdUser();
             id_doctor=lo.getIdDoctor();
             ocultarMenuP();

            }else {
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
            ui->stackedWidget_principal->setCurrentIndex(3);
            ui->stackedWidget_perfilDoctor->setCurrentIndex(0);

            //esto lo puse para habilitar las notificaciones
            verNoti=1;
            ui->nofi_2->hide();

            QString busca;
            busca="select *from notificacion where UserP='"+id_doctor+"'";
            QSqlQuery buscarNoti;
            buscarNoti.exec(busca);

            int contadorNoti=0;
            int filas=0;

            while(buscarNoti.next())
            {
                QString nueva;
                nueva=buscarNoti.value(4).toString();
                qDebug()<<"ya se vio: "<<nueva;
                if(nueva=="0")
                {

                    contadorNoti++;
                }
            }

            while(buscarNoti.previous()){
                if(buscarNoti.value(1).toString()=="1")
                {

                    QPlainTextEdit *b=new QPlainTextEdit();
                    // QPushButton *b=new QPushButton();
                    b->setPlainText(buscarNoti.value(2).toString());
                    b->setFixedSize(QSize(200,50));
                    b->setStyleSheet("background-color: rgb(151,240,104); ");
                    ui->barraDeNoti_2->addWidget(b,filas,0,Qt::AlignTop);
                    filas++;
                }else
                {

                    QPlainTextEdit *b=new QPlainTextEdit();
                    //QPushButton *b=new QPushButton();
                    b->setPlainText(buscarNoti.value(2).toString());
                    b->setFixedSize(QSize(200,50));
                    b->setStyleSheet("background-color: rgb(243,173,106); ");
                    ui->barraDeNoti_2->addWidget(b,filas,0,Qt::AlignTop);
                    filas++;
                }
            }

            QString num=QString::number(contadorNoti);
            qDebug()<<"este numero conte"<<num;
            if(num=="0")
            {
                qDebug()<<"no encontre nada";
                ui->notificacionL_4->hide();
            }
            else
            {
                qDebug()<<"encontre algo";
                ui->notificacionL_4->setStyleSheet("background-color:red; border:solid 1px red; border-radius:500px; color: white;");
                ui->notificacionL_4->setText(num);
                ui->notificacionL_4->show();
            }

          }
            on_pushButton_miPerfil_clicked();
       }
        else if(tipo==4)
        {
            qDebug()<<"eres de staff";
            ui->pushButton_salir->setHidden(false);
            ui->pushButton_login->setHidden(true);
            ui->pushButton_registro->setHidden(true);
            ui->btnCancelarEditarStaff->setHidden(true);
            ui->btnGuardarEditarStaff->setHidden(true);
            //Mostramos boton perfil
            ui->pushButton_miPerfil->setHidden(false);
            ui->lineEdit_idUsuario->clear();
            ui->lineEdit_passwordLogin->clear();
            //Ocultamos cosas del menu de arriba
            ocultarMenuP();

            id_staff=lo.getIdStaff();
            id_usuario=lo.getIdUser();

            //esto lo puse para habilitar las notificaciones
            verNoti=1;
            ui->nofi_staff->hide();

            QString busca;
            busca="select *from notificacion where UserP='"+id_usuario+"'";
            QSqlQuery buscarNoti;
            buscarNoti.exec(busca);

            int contadorNoti=0;
            int filas=0;

            while(buscarNoti.next())
            {
                QString nueva;
                nueva=buscarNoti.value(4).toString();
                qDebug()<<"ya se vio: "<<nueva;
                if(nueva=="0")
                {

                    contadorNoti++;
                }
            }

            while(buscarNoti.previous()){
                if(buscarNoti.value(1).toString()=="1")
                {

                    QPlainTextEdit *b=new QPlainTextEdit();
                    // QPushButton *b=new QPushButton();
                    b->setPlainText(buscarNoti.value(2).toString());
                    b->setFixedSize(QSize(200,50));
                    b->setStyleSheet("background-color: rgb(151,240,104); ");
                    ui->barraDeNoti_5->addWidget(b,filas,0,Qt::AlignTop);
                    filas++;
                }else
                {

                    QPlainTextEdit *b=new QPlainTextEdit();
                    //QPushButton *b=new QPushButton();
                    b->setPlainText(buscarNoti.value(2).toString());
                    b->setFixedSize(QSize(200,50));
                    b->setStyleSheet("background-color: rgb(243,173,106); ");
                    ui->barraDeNoti_5->addWidget(b,filas,0,Qt::AlignTop);
                    filas++;
                }
            }

            QString num=QString::number(contadorNoti);
            qDebug()<<"este numero conte"<<num;
            if(num=="0")
            {
                qDebug()<<"no encontre nada";
                ui->notiStaff->hide();
            }
            else
            {
                qDebug()<<"encontre algo";
                ui->notiStaff->setStyleSheet("background-color:red; border:solid 1px red; border-radius:500px; color: white;");
                ui->notiStaff->setText(num);
                ui->notiStaff->show();
            }

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
    //ui->pb_servicios->setHidden(true);
    ui->pushButton_especialidades->setHidden(true);
    ui->pushButton_infoHospital->setHidden(true);
    ui->pushButton__dirMedico->setHidden(true);
    ui->line->setHidden(true);
    ui->line_2->setHidden(true);
    ui->line_3->setHidden(true);
    ui->line_8->setHidden(true);
    ui->cb_servicios->hide();
    ui->pb_remedios->hide();
}

void MainWindow::mostrarMenuP(){
    //ui->pb_servicios->setHidden(false);
    ui->pushButton_especialidades->setHidden(false);
    ui->pushButton_infoHospital->setHidden(false);
    ui->pushButton__dirMedico->setHidden(false);
    ui->line->setHidden(false);
    ui->line_2->setHidden(false);
    ui->line_3->setHidden(false);
    ui->line_8->setHidden(false);
    ui->cb_servicios->show();
    ui->pb_remedios->show();
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
    ui->tableHorario->clear();
    //mostramos cosas del menu de arriba
    ui->pb_remedios->setHidden(true);
    ui->pushButton_horarioDoc->show();
    ui->pb_rechazarCitas->show();
    ui->pb_realizarConsulta->show();
    ui->pushButton_historial->show();
    ui->pb_inter->show();
    ui->pushButton_4->show();
    ui->butonNotifi_4->show();
    ui->notificacionL_4->show();
    ui->line_10->show();
    ui->line_7->show();
    ui->line_12->show();
    ui->line_14->show();
    ui->pb_urg->hide();
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
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
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
    if( fechaActual.exec("SELECT YEAR(CURDATE())-YEAR('"+fechaN+"') + IF( DATE_FORMAT(CURDATE(),'%m-%d') > DATE_FORMAT('"+fechaN+"','%m-%d'),0, -1)") ){
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
                              ui->lineEdit_respuesta->text(),
                                ui->le_sexo->text());

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
        on_pushButton_login_clicked();
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
                id_paciente=datosPac->value(0).toString();
                cargarDatosUsuarios();
                //Pagina de paciente
                ui->stackedWidget_principal->setCurrentIndex(2);
                //Pagina de sus datos
                ui->stackedWidget_perfilPaciente->setCurrentIndex(0);
            }

            if(datosStaff->next()){
                id_staff=datosStaff->value(0).toString();
                
                if(datosStaff->value(2).toInt()==7){
                    //Pagina de staff
                    ui->pushButton_horarioDoc_2->setHidden(false);
                }else
                {
                    ui->pushButton_horarioDoc_2->setHidden(true);
                }
                cargarDatosUsuarios();
                ui->stackedWidget_principal->setCurrentIndex(4);
                ui->stackedWidget_PerfilStaff->setCurrentIndex(0);

            }

            if(datosDoc->next()){
                id_doctor=datosDoc->value(0).toString();
                cargarDatosUsuarios();
                //cargarHorarioDoc();
                //Pagina de doctor
                ui->stackedWidget_principal->setCurrentIndex(3);
                ui->stackedWidget_perfilDoctor->setCurrentIndex(0);
            }
    }
}

//Cuando el usuario le da clic para ver su tip del día
void MainWindow::on_pushButton_tip_clicked()
{
    static tipdeldia tip(this);
      tip.mostrarTip();
      tip.setModal(true);
      tip.show();
}

//Funcion para cargar los datos en el perfil del usuario
void MainWindow::cargarDatosUsuarios(){
        QPixmap img;
        if(id_paciente!="0"){
            //Ponemos su imagen
            img.loadFromData(datosUser->value(9).toByteArray());
            //Imagen
            img.scaled(ui->label_imgPerfilPaciente->width(),ui->label_imgPerfilPaciente->height(),Qt::KeepAspectRatio);
            ui->label_imgPerfilPaciente->setPixmap(img);
            //Nombre
            ui->label_pagPaciente->setText(datosUser->value(2).toString()+" "+datosUser->value(3).toString()+" "+datosUser->value(4).toString());
            ui->lineCorreoPaciente->setText(datosUser->value(7).toString());
            ui->lineTelPaciente->setText(datosUser->value(8).toString());
            ui->lineFechaNacPaciente->setText(datosUser->value(5).toString());
            ui->lineContraseniaPaciente->setText(datosUser->value(1).toString());
            ui->lineConfirmaContraseniaPaciente->setText(datosUser->value(1).toString());

            ui->imgCon1_3->setHidden(true);
            ui->imgCon2_3->setHidden(true);
            ui->lineContraseniaPaciente->setHidden(true);
            ui->lineConfirmaContraseniaPaciente->setHidden(true);
            ui->btnMostrarContrasena_5->setHidden(true);
            ui->btnMostrarContrasena_6->setHidden(true);
            ui->btnCancelarEditarPaciente->setHidden(true);
            ui->btnGuardarEditarPaciente->setHidden(true);
            ui->pb_bajaPaciente->hide();
        }
        if(id_doctor!="0"){
            //Nombre
            ui->lineNombreDoctor->setText(datosUser->value(2).toString()+" "+datosUser->value(3).toString()+" "+datosUser->value(4).toString());
            //cedula
            ui->lineCedProfDoctor->setText(datosDoc->value(5).toString());
            //turno
            ui->lineTurnoDoctor->setText(datosDoc->value(3).toString());
            //consultorio
            QSqlQuery consul;
            consul.exec("SELECT numConsultorio FROM consultorio WHERE idconsultorio="+datosDoc->value(7).toString());
            consul.next();
            ui->lineConsultorioDoctor->setText(consul.value(0).toString());
            //Correo
            ui->lineCorreoDoctor->setText(datosUser->value(7).toString());
            //telefono
            ui->lineTelDoctor->setText(datosUser->value(8).toString());
            //FachaN
            ui->lineFechaNacDoctor->setText(datosUser->value(5).toString());
            //Contraseña1
            ui->lineContraseniaDoc->setText(datosUser->value(1).toString());
            //Contraseña2
            ui->lineConfirmaContraseniaDoc->setText(datosUser->value(1).toString());
            //Foto
            img.loadFromData(datosUser->value(9).toByteArray());
            //Imagen
            img.scaled(ui->lblFotoDoctor->width(),ui->lblFotoDoctor->height(),Qt::KeepAspectRatio);
            ui->lblFotoDoctor->setPixmap(img);


            ui->imgCon1_2->setHidden(true);
            ui->imgCon2_2->setHidden(true);
            ui->lineContraseniaDoc->setHidden(true);
            ui->lineConfirmaContraseniaDoc->setHidden(true);
            ui->btnMostrarContrasena_3->setHidden(true);
            ui->btnMostrarContrasena_4->setHidden(true);
            ui->btnCancelarEditarDoctor->setHidden(true);
            ui->btnGuardarEditarDoctor->setHidden(true);
            ui->pb_bajaDoctor->hide();
        }
        if(id_staff!="0"){
            //datos staff ----------------------------
            //Ponemos su imagen
            img.loadFromData(datosUser->value(9).toByteArray());
            //Imagen
            img.scaled(ui->label_imgPerfilPaciente->width(),ui->label_imgPerfilPaciente->height(),Qt::KeepAspectRatio);
            ui->lblFotoStaff->setPixmap(img);
            //Nombre
            ui->lineNombreStaff->setText(datosUser->value(2).toString()+" "+datosUser->value(3).toString()+" "+datosUser->value(4).toString());
            ui->lineCorreoStaff->setText(datosUser->value(7).toString());
            ui->lineTelStaff->setText(datosUser->value(8).toString());
            ui->lineFechaNacStaff->setText(datosUser->value(5).toString());
            ui->lineContrasenia->setText(datosUser->value(1).toString());
            ui->lineConfirmaContrasenia->setText(datosUser->value(1).toString());

            ui->imgCon1->setHidden(true);
            ui->imgCon2->setHidden(true);
            ui->lineContrasenia->setHidden(true);
            ui->lineConfirmaContrasenia->setHidden(true);
            ui->btnMostrarContrasena->setHidden(true);
            ui->btnMostrarContrasena_2->setHidden(true);
            ui->btnCancelarEditarStaff->setHidden(true);
            ui->btnGuardarEditarStaff->setHidden(true);
            ui->pushButton_5->hide();
            ui->pushButton_6->show();

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
    ui->lineEdit_matriculaMedico->clear();
    ui->label_mensajePaciente->hide();
    ui->label_espacioBlanco->hide();
    SolicitudCitas();


}

void MainWindow::on_pushButton_horarioDoc_clicked()
{
    //Mostrar su horario
    ui->stackedWidget_perfilDoctor->setCurrentIndex(1);
    ui->lineEdit_matriculaMedico->clear();
    ui->label_mensajePaciente->hide();
    ui->label_espacioBlanco->hide();
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
    cargarHorarioDoc_2();
}

void MainWindow::cargarHorarioDoc_2(){
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
            for(int j=0; j<=8; j++){
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

            if(q.value("nombreAct").toString()=="Consulta"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::darkBlue);
            }
            else
            if(q.value("nombreAct").toString()=="Descanso"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::green);
            }
            else if(q.value("nombreAct").toString()=="Coordinación"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::magenta);
            }
            else if(q.value("nombreAct").toString()=="Investigación"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::darkCyan);
            }
            else if(q.value("nombreAct").toString()=="Administración"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::black);
            }
            else{
                ui->tableHorario->item(fil, col)->setBackground(Qt::red);
            }
            ui->tableHorario->setEditTriggers(QAbstractItemView::NoEditTriggers);
            i++;
        }//Fin while
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
    int i=0, numConsultas=0;
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
            for(int j=0; j<=8; j++){
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

            if(q.value("nombreAct").toString()=="Consulta"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::darkBlue);
                numConsultas++;
            }
            else
            if(q.value("nombreAct").toString()=="Descanso"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::green);
            }
            else if(q.value("nombreAct").toString()=="Coordinación"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::magenta);
            }
            else if(q.value("nombreAct").toString()=="Investigación"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::darkCyan);
            }
            else if(q.value("nombreAct").toString()=="Administración"){
                ui->tableHorario->item(fil, col)->setBackground(Qt::black);
            }
            else{
                ui->tableHorario->item(fil, col)->setBackground(Qt::red);
            }
            ui->tableHorario->setEditTriggers(QAbstractItemView::NoEditTriggers);
            i++;
        }//Fin while

        if(numConsultas<25){
            QMessageBox res(QMessageBox::Information,"Actividades faltantes","Por favor organice su horario para tener un mínimo de 25 horas de consulta.");
            if(res.exec() == QMessageBox::Yes ){
                ui->stackedWidget_principal->setCurrentIndex(3);
                ui->stackedWidget_perfilDoctor->setCurrentIndex(1);
            }
            ui->pb_rechazarCitas->setEnabled(0);
            ui->pushButton_historial->setEnabled(0);
            ui->pb_inter->setEnabled(0);
            ui->pushButton_4->setEnabled(0);
            ui->pb_urg->setEnabled(0);
            ui->pushButton_logo->setEnabled(0);
            ui->pushButton_horarioDoc->setEnabled(0);
            ui->butonNotifi_4->setEnabled(0);
            ui->pushButton_miPerfil->setEnabled(0);
            ui->pushButton_salir->setEnabled(0);

            QPushButton* pb = new QPushButton(" Finalizar ");
            pb->setStyleSheet("background-color:red; color:white;");
            ui->layHorario->addWidget(pb);
            connect(pb,&QPushButton::clicked,this,&MainWindow::verficarHorario);
        }
}

void MainWindow::verficarHorario(){
    QSqlQuery* q = new QSqlQuery;
    int horas=0;
    q->exec("SELECT nombreAct FROM horarioDoc WHERE idDoc="+id_doctor+" AND nombreAct='Consulta'");
        while(q->next()){
            horas++;
        }
        if(horas<25){
            QMessageBox::information(this,"Actividades faltantes","Por favor organice su horario para tener un mínimo de 25 horas de consulta.");
        }
        else{
            ui->pb_rechazarCitas->setEnabled(1);
            ui->pushButton_historial->setEnabled(1);
            ui->pb_inter->setEnabled(1);
            ui->pushButton_4->setEnabled(1);
            ui->pb_urg->setEnabled(1);
            ui->pushButton_logo->setEnabled(1);
            ui->pushButton_horarioDoc->setEnabled(1);
            ui->butonNotifi_4->setEnabled(1);
            ui->pushButton_miPerfil->setEnabled(1);
            ui->pushButton_salir->setEnabled(1);

            //Castear una señal y convertirla a un objeto
            QPushButton* pb = qobject_cast<QPushButton*>(QObject::sender());
            delete pb;
        }
}


void MainWindow::on_pushButton_SolicitudesUsuarios_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(1);
    ui->pushButton_SolicitudesUsuarios->hide();
    ui->pushButton_tip_2->hide();
    ui->pushButton_agregar_remedio->hide();
}


//metodo para asignar en el menu dinamico de solicitdes
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

//
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
//metodo para mostrar areas de trabajo
void MainWindow::mostrarZonas(){
    QSqlQueryModel *queryPuestos;
    queryPuestos= new QSqlQueryModel;
    queryPuestos->setQuery("SELECT nombreArea FROM areah");
    ui->comboBox_area->setModel(queryPuestos);
}

// metodo para asignar en el menu dinamico de consultorios por area
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
//menu dinamico de mostrar consultorios para asignar un consultorio a un doctor nuevo
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

//para regresas donde se encuentra las solicitudes de los doctores o staff en espera
void MainWindow::on_pushButton_regresarSolicitudes_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(1);
}

//metodo para hacer la busqueda por area a la hora de asignar un consultorio
void MainWindow::on_comboBox_area_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_area->clear();
    ui->lineEdit_piso->clear();
    ui->lineEdit_numconsultorio->clear();
    mostrasConsultorios();
}

//metodo para guardad los
void MainWindow::on_pushButton_guardar_clicked()
{
    // update de doctor
    QString actualizacion,turno,numConsul,area,estado,buscaId,idCondultorio,idarea,areas,idareas;
    QSqlQuery actual;
    estado="1";
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Estas seguro de asignar el área.?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    turno=ui->comboBox_turno->currentText();
    numConsul=ui->lineEdit_numconsultorio->text();
    buscaId="select idconsultorio from consultorio where numConsultorio='"+numConsul+"'";
    actual.exec(buscaId);
    actual.next();
    idCondultorio=actual.value(0).toString();

    if(ui->lineEdit_area->text()==""){
        QMessageBox messageBox(QMessageBox::Warning,
                                         tr("Warning"), tr("Por favor,Asigne una area."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 if (messageBox.exec() == QMessageBox::Yes){

                  }
    }else{

    if(UserTipo==1){
        if (message.exec() == QMessageBox::Yes){
        qDebug()<<"entre update doctor";
        actualizacion="update doctor set horario='"+turno+"',idconsultorio='"+idCondultorio+"',estado='"+estado+"'where idUser='"+matric+"'";
        qDebug()<<actualizacion;
        actual.exec(actualizacion);
        qDebug()<<"Actualizando datos del doctor";
         clearLayout(ui->lista);
        ui->stackedWidget_admin->setCurrentIndex(1);
        }
    }
        else if (UserTipo==2){
        if (message.exec() == QMessageBox::Yes){
            areas=ui->lineEdit_area->text();
            idarea="select idarea from areah where nombreArea='"+areas+"'";
            idareas=actual.value(0).toString();
        //update de staff
         area=ui->lineEdit_area->text();
         qDebug()<<"Actualizando datos del staff";
        actualizacion="update staff set estado='"+estado+"',idArea='"+idareas+"',turno='"+turno+"' where idUser='"+matric+"'";
        actual.exec(actualizacion);
         clearLayout(ui->lista);
        ui->stackedWidget_admin->setCurrentIndex(1);
             }
        }
    }
    on_radioButton_doctors_clicked();

}


//rechazar solicitudes
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
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 if (messageBox.exec() == QMessageBox::Yes){

                  }

    }else  if (message.exec() == QMessageBox::Yes){
        qDebug()<<"matricula:"<<matric;
        qDebug()<<"tipo:"<<UserTipo;
    if(UserTipo==1){
        consulta=" delete from doctor where idUser='"+matric+"'";
        query.exec(consulta);
        consulta="delete from usuario where matricula='"+matric+"'";
        query.exec(consulta);
    }
        else{
        if (UserTipo==2) {
            qDebug()<<"entre staff";
          consulta=" delete from staff where idUser='"+matric+"'";
          query.exec(consulta);
          consulta="delete from usuario where matricula='"+matric+"'";
          query.exec(consulta);
          clearLayout(ui->lista);
        }
      }
    }

}

//cunsulta solo muetras doctores
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
//solo mostamos solicitudes de staff en espera
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
        cont++;

        }
    }

}
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-////////////////////////////////////PARTE DE TIPS DEL DIA /////////////////////////////////////////
//-///////////////////////////////////ELIMINAR/////EDITAR/////////AGREGAR/////////////////////////////////////////////////

// metodo para guardar la imagen del tip
void MainWindow::on_pushButton_agregarImagen_clicked()
{
    ui->pushButton_agregarImagen->setText("");
    //Abrimos el dialogo para obtener la ruta
     imgRoute = QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto = file.readAll();
    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto);

    QIcon l(pix);
    ui->pushButton_agregarImagen->setIcon(l);
}


void MainWindow::on_pushButton_menu_Pincipal_Adm_clicked()
{
    ui->pushButton_SolicitudesUsuarios->show();
    ui->pushButton_agregar_remedio->show();
    ui->pushButton_tip_2->show();
    ui->stackedWidget_admin->setCurrentIndex(0);
}



void MainWindow::on_pushButton_tip_2_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(3);
    ui->stackedWidget_tips->setCurrentIndex(0);
    ui->pushButton_SolicitudesUsuarios->hide();
    ui->pushButton_tip_2->hide();
    ui->pushButton_agregar_remedio->hide();

}
//metodo paguardad el tip del dia
void MainWindow::on_pushButton_agregar_tip_clicked()
{
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    QString texto,nomTip,link;
    nomTip=ui->lineEdit_nombreTip->text();
    link=ui->lineEdit_link->text();
    texto=ui->plainTextEdit_descripcion->toPlainText();
    agregarTipRemedio tip;

    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de agregar el tip de salud?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

       if(nomTip==""|| link=="" || texto=="" || foto==""){
           QMessageBox messageBox(QMessageBox::Warning,
                                            tr("Warning"), tr("Por favor,ingrese los datos necesarios."), QMessageBox::Yes);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    if (messageBox.exec() == QMessageBox::Yes){

                     }
       }

        if(nomTip==""){
        ui->lineEdit_nombreTip->setStyleSheet(estiloMalo);
        } else {
      ui->lineEdit_nombreTip->setStyleSheet(estiloBueno);

      }

        if(link==""){
         ui->lineEdit_link->setStyleSheet(estiloMalo);
        }else{
        ui->lineEdit_link->setStyleSheet(estiloBueno);

        }

         if(texto==""){
          ui->plainTextEdit_descripcion->setStyleSheet(estiloMalo);
            }else {
           ui->plainTextEdit_descripcion->setStyleSheet(estiloBueno);

          }

          if(foto==""){
            qDebug()<<"mensaje de eerro";
           ui->pushButton_agregarImagen->setStyleSheet(estiloMalo);
          }


      if(nomTip!=""&& link!=""&& texto!="" && foto!=""){
          if (message.exec() == QMessageBox::Yes){
        qDebug()<<"mensaje de confirmacion";
        tip.agregaTip(nomTip,texto,link,imgRoute);
        ui->lineEdit_nombreTip->clear();
        ui->lineEdit_link->clear();
        ui->plainTextEdit_descripcion->clear();
        QString concolor =":/ /imgs/fondo.png";
        QIcon color(concolor);
        ui->pushButton_agregarImagen->setIcon(color);
          }
      }
}



void MainWindow::on_pushButton_menu_admin_clicked()
{
    QString estiloBueno;
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    ui->stackedWidget_admin->setCurrentIndex(0);
    ui->pushButton_SolicitudesUsuarios->show();
    ui->pushButton_tip_2->show();
    ui->pushButton_agregar_remedio->show();
    ui->lineEdit_nombreTip->clear();
    ui->lineEdit_link->clear();
    ui->plainTextEdit_descripcion->clear();
    QString concolor =":/ /imgs/fondo.png";
    QPixmap c=concolor;
    QIcon color(concolor);
    ui->pushButton_agregarImagen->setIcon(color);
    ui->lineEdit_nombretip->clear();
    ui->lineEdit_links->clear();
    ui->plainTextEdit_descritip->clear();
    ui->label_imagenTip->setPixmap(c);
    //limpia si se queda en error los campos de agregar nuevo tip
    //ui->pushButton_agregarImagen->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripcion->setStyleSheet(estiloBueno);
    ui->lineEdit_link->setStyleSheet(estiloBueno);
    ui->lineEdit_nombreTip->setStyleSheet(estiloBueno);
    //poner en colores normal los tips
    ui->lineEdit_nombretip->setStyleSheet(estiloBueno);
    ui->lineEdit_links->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descritip->setStyleSheet(estiloBueno);
    ui->label_imagenTip->setPixmap(c);
    //editar tips
    ui->lineEdit_nombretipEdit->clear();
    ui->lineEdit_liksEdit->clear();
    ui->plainTextEdit_descripEdit->clear();
    ui->lineEdit_nombretipEdit->setStyleSheet(estiloBueno);
    ui->lineEdit_liksEdit->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripEdit->setStyleSheet(estiloBueno);
    ui->label_imageneditartip->setPixmap(c);
}

//METODO PARA AGREGAR TIP
void MainWindow::on_pushButton_agregartips_clicked()
{
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";

    ui->lineEdit_nombreTip->setStyleSheet(estiloBueno);
    ui->lineEdit_link->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripcion->setStyleSheet(estiloBueno);
    //ui->pushButton_agregarImagen->setStyleSheet(estiloBueno);
    QString concolor =":/ /imgs/fondo.png";
    QIcon color(concolor);
    ui->pushButton_agregarImagen->setIcon(color);
    ui->stackedWidget_tips->setCurrentIndex(1);
    ui->lineEdit_nombretip->clear();
    ui->lineEdit_links->clear();
    ui->plainTextEdit_descritip->clear();
    QPixmap c=concolor;
    ui->label_imagenTip->setPixmap(c);
    //poner en colores normal los tips
    ui->lineEdit_nombretip->setStyleSheet(estiloBueno);
    ui->lineEdit_links->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descritip->setStyleSheet(estiloBueno);
    ui->label_imagenTip->setPixmap(c);
    //editar tips
    ui->lineEdit_nombretipEdit->clear();
    ui->lineEdit_liksEdit->clear();
    ui->plainTextEdit_descripEdit->clear();
    ui->lineEdit_nombretipEdit->setStyleSheet(estiloBueno);
    ui->lineEdit_liksEdit->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripEdit->setStyleSheet(estiloBueno);
    ui->label_imageneditartip->setPixmap(c);

}
//BOTON QUE TE LLEVA AL MENU DE ELIMINAR TIPS
void MainWindow::on_pushButton_eliminarTips_clicked()
{
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    QString concolor =":/ /imgs/fondo.png";
    QPixmap c=concolor;

    ui->lineEdit_nombreTip->setStyleSheet(estiloBueno);
    ui->lineEdit_link->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripcion->setStyleSheet(estiloBueno);
    //ui->pushButton_agregarImagen->setStyleSheet(estiloBueno);
    QIcon color(concolor);
    ui->pushButton_agregarImagen->setIcon(color);
    mostrarTipsEliminar();
    ui->stackedWidget_tips->setCurrentIndex(2);

    //limpia si se queda en error los campos de agregar nuevo tip
    //ui->pushButton_agregarImagen->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripcion->setStyleSheet(estiloBueno);
    ui->lineEdit_link->setStyleSheet(estiloBueno);
    ui->lineEdit_nombreTip->setStyleSheet(estiloBueno);

    //editar tips
    ui->lineEdit_nombretipEdit->clear();
    ui->lineEdit_liksEdit->clear();
    ui->plainTextEdit_descripEdit->clear();
    ui->lineEdit_nombretipEdit->setStyleSheet(estiloBueno);
    ui->lineEdit_liksEdit->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripEdit->setStyleSheet(estiloBueno);
    ui->label_imageneditartip->setPixmap(c);
}

//METODO PARA EDITAR EL TIP QUE ANTES YA SE HAYA SELECIONADO
void MainWindow::on_pushButton_editarTips_clicked()
{
    qDebug()<<"idtip para editar:"<<idtipeditar;

    QString nombre,link,descrip,image;
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    nombre=ui->lineEdit_nombretipEdit->text();
    link=ui->lineEdit_liksEdit->text();
    descrip=ui->plainTextEdit_descripEdit->toPlainText();
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de editar el tip del dia?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));


    if(nombre==""){
    QMessageBox messageBox(QMessageBox::Warning,
                                     tr("Warning"), tr("Por favor,seleccione tip a editar."), QMessageBox::Yes);
             messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
             if (messageBox.exec() == QMessageBox::Yes){

              }
       }
    if(nombre==""){
        ui->lineEdit_nombretipEdit->setStyleSheet(estiloMalo);
    }else {
        ui->lineEdit_nombretipEdit->setStyleSheet(estiloBueno);
    }
    if(link==""){
        ui->lineEdit_liksEdit->setStyleSheet(estiloMalo);
    }else {
       ui->lineEdit_liksEdit->setStyleSheet(estiloBueno);
    }
    if(descrip==""){
        ui->plainTextEdit_descripEdit->setStyleSheet(estiloMalo);
    }else {
       ui->plainTextEdit_descripEdit->setStyleSheet(estiloBueno);
    }
    if(nombre!=""){
if (message.exec() == QMessageBox::Yes){
       agregarTipRemedio editarTip;
       editarTip.editarTips(idtipeditar,nombre,descrip,link);
       QString concolor =":/ /imgs/fondo.png";
       ui->label_imageneditartip->setPixmap(concolor);
        ui->lineEdit_nombretipEdit->clear();
        ui->lineEdit_liksEdit->clear();
        ui->plainTextEdit_descripEdit->clear();
        ui->lineEdit_nombretipEdit->setStyleSheet(estiloBueno);
        ui->lineEdit_liksEdit->setStyleSheet(estiloBueno);
        ui->plainTextEdit_descripEdit->setStyleSheet(estiloBueno);
        mostrarTipsEditar();
       }
    }

}

//METODO PARA ELIMINAR UN TIP SELECCIONADO
void MainWindow::on_pushButton_eliminartip_clicked()
{
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";

    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de eliminar el tip del dia?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    qDebug()<<"entre:"<<idtip;
     QSqlQuery query;
     QMessageBox messageBox(QMessageBox::Warning,
                                      tr("Warning"), tr("Por favor,seleccione tip a eliminar."), QMessageBox::Yes);
              messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

              if(ui->lineEdit_nombretip->text()==""){
              if (messageBox.exec() == QMessageBox::Yes){

               }
     }

      if(ui->lineEdit_nombretip->text()==""){
          ui->lineEdit_nombretip->setStyleSheet(estiloMalo);
      } else {
        ui->lineEdit_nombretip->setStyleSheet(estiloBueno);

      }

      if(ui->lineEdit_links->text()==""){
          ui->lineEdit_links->setStyleSheet(estiloMalo);
      }else {
          ui->lineEdit_links->setStyleSheet(estiloBueno);
    }

      if(ui->plainTextEdit_descritip->toPlainText()==""){
          ui->plainTextEdit_descritip->setStyleSheet(estiloMalo);
      }else {
        ui->plainTextEdit_descritip->setStyleSheet(estiloBueno);
       }



     if(ui->lineEdit_nombretip->text()!=""){
     if (message.exec() == QMessageBox::Yes){
     query.exec("delete from tip where idtip='"+idtip+"'");
     query.next();
     ui->lineEdit_nombretip->clear();
     ui->lineEdit_links->clear();
     ui->plainTextEdit_descritip->clear();
     QString concolor =":/ /imgs/fondo.png";
     QPixmap c=concolor;
     mostrarTipsEliminar();
     ui->label_imagenTip->setPixmap(c);
        }
     }

}
//METODO PARA PONER LA INFORMACION EN CADA BOTON DEL MENU DINAMICO para editar
void MainWindow::Ponertips(QString idtipedi){

    QString consultatips,nombre,descrip,link;
    QSqlQuery querytips;
    consultatips="select *from tip where idtip='"+idtipedi+"'";
    querytips.exec(consultatips);
    querytips.next();
    qDebug()<<"soy el idtip del metodo ponertips:"<<idtipedi;
    idtipeditar=idtipedi;
    nombre=querytips.value(1).toString();
    descrip=querytips.value(2).toString();
    link=querytips.value(3).toString();
    QPixmap pix;
    if(!pix.loadFromData(querytips.value(4).toByteArray())){
        ui->label_imageneditartip->setText("<b>Error de imagen</b>");

    }

    int widWidth = this->ui->label_imageneditartip->width();
    int widHeight = this->ui->label_imageneditartip->height();
    ui->label_imageneditartip->setPixmap(pix.scaled(widWidth, widHeight, Qt::KeepAspectRatioByExpanding));
    ui->lineEdit_nombretipEdit->setText(nombre);
    ui->lineEdit_liksEdit->setText(link);
    ui->plainTextEdit_descripEdit->setPlainText(descrip);


}
//METODO PARA PONER LA INFORMACION EN CADA BOTON DEL MENU DINAMICO PARA ELIMINAR TIP
void MainWindow::ponerTipseliminar(QString idtip){

    QString consultatips,nombre,descrip,link;
    QSqlQuery querytips;
    consultatips="select *from tip where idtip='"+idtip+"'";
    querytips.exec(consultatips);
    querytips.next();
    qDebug()<<consultatips;
    nombre=querytips.value(1).toString();
    descrip=querytips.value(2).toString();
    link=querytips.value(3).toString();
    QPixmap pix;
    if(!pix.loadFromData(querytips.value(4).toByteArray())){
        ui->label_imagenTip->setText("<b>Error de imagen</b>");

    }
    //eliminar
    int w = this->ui->label_imagenTip->width();
    int wi= this->ui->label_imagenTip->height();
    ui->label_imagenTip->setPixmap(pix.scaled(w,wi, Qt::KeepAspectRatioByExpanding));
    ui->lineEdit_nombretip->setText(nombre);
    ui->lineEdit_links->setText(link);
    ui->plainTextEdit_descritip->setPlainText(descrip);

}

//MENU PARA MOSTRAR LOS TIPS PARA SU EDICION EN EL MENU DINAMICO
void MainWindow::mostrarTipsEditar(){
    clearLayout(ui->listatips);
    QString consultatips,nombre;
    QSqlQuery querytips;
    int cont=0;
    consultatips="select *from tip";
    querytips.exec(consultatips);
    while(querytips.next())
    {
        idtipeditar=querytips.value(0).toString();
        qDebug()<<"sdfnfjb.kjdfbl_:"<<idtipeditar;
        nombre=querytips.value(1).toString();
        QPushButton *b=new QPushButton();
        b->setText(nombre);
        b->setFixedSize(QSize(200,40));
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(b,idtipeditar);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(Ponertips(QString)));
        ui->listatips->addWidget(b,cont,0,1,1);
        cont++;
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
    ui->lb_sexo->setHidden(true);
    ui->cb_sexo->setHidden(true);
}

void MainWindow::on_radioCitaExterna_clicked()
{
    ui->labelNombreComp->setHidden(false);
    ui->lineNombreCompleto->setHidden(false);
    ui->labelOrigen->setHidden(false);
    ui->lineOrigen->setHidden(false);
    ui->labelEdad->setHidden(false);
    ui->lineEdad->setHidden(false);
    ui->lb_sexo->setHidden(false);
    ui->cb_sexo->setHidden(false);
}

//Boton citas perfil paciente
void MainWindow::on_pushButton_clicked()
{
    ui->btnBuscarDoctor->setHidden(true);
    ui->stackedWidget_perfilPaciente->setCurrentIndex(1);
    QSqlQuery fecha;
    QStringList f;
    int anio, mes, dia;
    fecha.exec("SELECT CURDATE()");
    fecha.next();
    f = fecha.value(0).toString().split("-");
    anio = f.at(0).toInt();
    mes = f.at(1).toInt();
    dia= f.at(2).toInt();

    ui->fechaCita->setMinimumDate(QDate(anio,mes,dia));

    QStringList horas;
    horas<<  "05:00" << "06:00" << "07:00" << "08:00" << "09:00" << "10:00" << "11:00" << "12:00" << "13:00"
    << "14:00" << "15:00" << "16:00" << "17:00" << "18:00" << "19:00" << "20:00" << "21:00"
    << "22:00" << "23:00" << "00:00" << "01:00" << "02:00" << "03:00" << "04:00" << "05:00";

    ui->lb_noHayDocs->setHidden(true);
    ui->lb_selDoc->setHidden(true);
    ui->lb_maxchar->setHidden(true);
    ui->lb_charAct->setHidden(true);
    ui->horaCita->addItems(horas);
    on_radioCitaPersonal_clicked();
}

//Buscar un doctor para una cita
void MainWindow::on_btnBuscarDoctor_clicked()
{
    QSqlQuery horarios, doc;
    QString hora,fecha;
    int diaNum, idDoc;
    diaNum = ui->fechaCita->date().dayOfWeek();
    hora = ui->horaCita->currentText();

    QStringList dias;
    dias<< "Lunes" << "Martes" << "Miércoles" << "Jueves" << "Viernes" << "Sábado" << "Domingo";

    horarios.exec("SELECT idDoc FROM horariodoc WHERE hora='"+hora+"' AND dia='"+dias.at(diaNum-1)+"' AND nombreAct='Consulta'");
    if(ui->le_nombreDoc->text().isEmpty() && !horarios.next()){
        ui->lb_noHayDocs->setHidden(false);
        ui->tv_listaDocCitas->setModel(nullptr);
    }
    else{
        ui->lb_selDoc->setHidden(false);
        ui->lb_noHayDocs->setHidden(true);
        idDoc=horarios.value(0).toInt();

        //Si movio la fecha pero aun no pone el nombre del doctor
        if(!ui->le_nombreDoc->text().isEmpty()){
            QStringList nombreC;
            QString nombre, apeM, apeP;
            nombreC=ui->le_nombreDoc->text().split(" ");

            //Si solo ingreso una palabra
            if(nombreC.size()==1) {
                //Buscamos por nombre o apellido paterno
                nombre = nombreC.at(0);
                apeP = nombreC.at(0);

                model->setQuery("SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor "
                              "FROM doctor as d , especialidad as e , usuario as u "
                              "WHERE d.iddoctor = "+QString::number((idDoc))+" "
                              "AND u.matricula = d.idUser "
                              "AND d.idEspecialidad = e.idEsp "
                              "OR u.nombre='"+nombre+"' "
                              "OR u.appaterno ='"+apeP+"' ");
            }
            //Si puso mas de su nombre o apellido
            else {
                nombre = nombreC.at(0);
                apeP = nombreC.at(1);
                apeM = nombreC.at(2);

                model->setQuery("SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor "
                              "FROM doctor as d , especialidad as e , usuario as u "
                              "WHERE d.iddoctor = "+QString::number((idDoc))+" "
                              "AND u.matricula = d.idUser "
                              "AND d.idEspecialidad = e.idEsp "
                              "OR u.nombre='"+nombre+"' "
                              "OR u.appaterno ='"+apeP+"' "
                              "OR u.apmaterno ='"+apeM+"'");
            }
        }
        //Si está vacio el campo del nombre del doctor
        //Mostramos todos los doctores disponibles
        else{
            model->setQuery("SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor "
                          "FROM doctor as d , especialidad as e , usuario as u "
                          "WHERE d.iddoctor = "+QString::number((idDoc))+" "
                          "AND u.matricula = d.idUser "
                          "AND d.idEspecialidad = e.idEsp");
        qDebug()<<idDoc;
        ui->tv_listaDocCitas->setModel(model);
        ui->tv_listaDocCitas->setHidden(false);
        ui->tv_listaDocCitas->hideColumn(2);
        ui->tv_listaDocCitas->setColumnWidth(0,ui->tv_listaDocCitas->width()/2);
        ui->tv_listaDocCitas->setColumnWidth(1,ui->tv_listaDocCitas->width()/2);
    }
    }
}

void MainWindow::on_btnAgendarCita_clicked()
{
    QRegExp letras("^[a-zZ-A ]*$"), numeros("^[0-9]*$");
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";

    //Si es una cita del mismo usuario
    if (ui->radioCitaPersonal->isChecked()){
        if(id_doctor!="0"){
            ui->tv_listaDocCitas->setStyleSheet(estiloBueno);
                   if(!ui->le_nombreDoc->text().isEmpty()){
                       ui->le_nombreDoc->setStyleSheet(estiloBueno);
                       if(!ui->sintomasCitas->toPlainText().isEmpty() && ui->sintomasCitas->toPlainText().size()<=500){
                           ui->sintomasCitas->setStyleSheet(estiloBueno);
                           ui->lb_maxchar->setHidden(true);
                           ui->lb_charAct->setHidden(true);

                           agregarCitasPaciente cita;
                           if ( cita.citasPaciente(id_usuario,ui->fechaCita->date().toString("yyyy-MM-dd"),ui->horaCita->currentText(),id_doctor,ui->sintomasCitas->toPlainText()) ){
                               QMessageBox::information(this,"Correcto", "Cita agendada correctamente");
                           }else{
                               QMessageBox::information(this,"Error", "Hubo un problema al agendar su cita, intente de nuevo más tarde.");
                           }
                           //Vaciamos variables
                           ui->lb_selDoc->setHidden(true);
                           ui->tv_listaDocCitas->setModel(nullptr);
                           ui->tv_listaDocCitas->setHidden(true);
                           ui->le_nombreDoc->setText("");
                           ui->sintomasCitas->setText("");
                           id_doctor="0";

                       }else{
                           ui->sintomasCitas->setStyleSheet(estiloMalo);
                           ui->lb_charAct->setText( QString::number( ui->sintomasCitas->toPlainText().size() ) );
                           ui->lb_charAct->setHidden(false);
                           ui->lb_maxchar->setHidden(false);
                       }

                   }
                   else{
                         ui->le_nombreDoc->setStyleSheet(estiloMalo);
                   }
        }
        else{
            ui->tv_listaDocCitas->setStyleSheet(estiloMalo);
        }
    }

    //Si es una cita de un menor
    if(ui->radioCitaExterna->isChecked()){
        if(id_doctor!="0"){
            ui->tv_listaDocCitas->setStyleSheet(estiloBueno);
           if(ui->lineNombreCompleto->text().contains(letras)){
               ui->lineNombreCompleto->setStyleSheet(estiloBueno);
               if(ui->lineOrigen->text().contains(letras)){
                   ui->lineOrigen->setStyleSheet(estiloBueno);
                   if(ui->lineEdad->text().contains(numeros) && ui->lineEdad->text().toInt()<18){
                       ui->lineEdad->setStyleSheet(estiloBueno);
                               if(!ui->le_nombreDoc->text().isEmpty()){
                                   ui->le_nombreDoc->setStyleSheet(estiloBueno);
                                   if(!ui->sintomasCitas->toPlainText().isEmpty()){
                                       ui->sintomasCitas->setStyleSheet(estiloBueno);

                                       agregarCitasPaciente cita;
                                       if( cita.citasExternas(id_paciente, ui->lineNombreCompleto->text(),ui->lineOrigen->text(),ui->lineEdad->text(),ui->cb_sexo->currentText(),ui->fechaCita->date().toString("yyyy-MM-dd"),ui->horaCita->currentText(),id_doctor,ui->sintomasCitas->toPlainText() ) ) {
                                            QMessageBox::information(this,"Correcto", "Cita agendada correctamente");
                                       }else{
                                           QMessageBox::information(this,"Error", "Hubo un problema al agendar su cita, intente de nuevo más tarde.");
                                       }
                                       //Vaciamos variables
                                       ui->lb_selDoc->setHidden(true);
                                       ui->tv_listaDocCitas->setModel(nullptr);
                                       ui->tv_listaDocCitas->setHidden(true);
                                       ui->le_nombreDoc->setText("");
                                       ui->sintomasCitas->setText("");
                                       id_doctor="0";

                                   }else{
                                       ui->sintomasCitas->setStyleSheet(estiloMalo);
                                   }

                               }
                               else{ ui->le_nombreDoc->setStyleSheet(estiloMalo);}


                                   }
                                       else{
                                             ui->lineEdad->setStyleSheet(estiloMalo);
                                       }
                                           }
                                           else{
                                                 ui->lineOrigen->setStyleSheet(estiloMalo);
                                           }
                                                   }
                                                   else{
                                                         ui->lineNombreCompleto->setStyleSheet(estiloMalo);
                                                   }
        }
        else{
              ui->tv_listaDocCitas->setStyleSheet(estiloMalo);
        }
    }
}

//Cuando le da click a la tabla de citas
void MainWindow::on_tv_listaDocCitas_clicked(const QModelIndex &index)
{
    id_doctor = model->index(index.row(),2).data().toString();
}

void MainWindow::on_tv_listaDocCitas_doubleClicked(const QModelIndex &index)
{
    ui->le_nombreDoc->setText( model->index(index.row(),0).data().toString() );
}

void MainWindow::on_fechaCita_userDateChanged(const QDate &date)
{
    QSqlQuery fecha;
    fecha.exec("SELECT CURDATE()");
    QString fechaUI = date.toString("yyyy-MM-dd");
    QString fechaReal = fecha.value(0).toString();
    if(fechaUI!=fechaReal){
        on_btnBuscarDoctor_clicked();
    }
}

void MainWindow::on_horaCita_activated(const QString &arg1)
{
        on_btnBuscarDoctor_clicked();
}

void MainWindow::SolicitudCitas()
{
    QString citas,est,idD,idD2;
    idD="select iddoctor from doctor where idUser='"+id_usuario+"'; ";
    QSqlQuery busId;
    busId.exec(idD);
    busId.next();
    idD2=busId.value(0).toString();;
    est="0";
    citas="select *from cita where doctor='"+idD2+"' and estado='"+est+"'; ";
    QSqlQuery citas1;
    citas1.exec(citas);

    int f=0;
    int fi=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";


    QLabel *l=new QLabel;
    l->setText("folio de Cita");
    l->setFixedSize(QSize(100,25));
    l->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas->addWidget(l,0,0,Qt::AlignLeft);

    QLabel *l1=new QLabel;
    l1->setText("Matricula");
    l1->setFixedSize(QSize(100,25));
    l1->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas->addWidget(l1,0,1,Qt::AlignLeft);

    QLabel *l2=new QLabel;
    l2->setText("Fecha");
    l2->setFixedSize(QSize(100,25));
    l2->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas->addWidget(l2,0,2,Qt::AlignLeft);

    QLabel *l3=new QLabel;
    l3->setText("Hora");
    l3->setFixedSize(QSize(100,25));
    l3->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas->addWidget(l3,0,3,Qt::AlignLeft);


    QString folio,matricu,fecha,hora;
    int i=0;

    while(citas1.next())
    {
        folio=citas1.value(0).toString();
        matricu=citas1.value(1).toString();
        fecha=citas1.value(2).toString();
        hora=citas1.value(3).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }


        QLabel *l=new QLabel;
        l->setText(folio);
        l->setFixedSize(QSize(100,25));
        l->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay->addWidget(l,i,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(matricu);
        m->setFixedSize(QSize(100,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay->addWidget(m,i,1,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->citasLay->addWidget(r,i,2,Qt::AlignTop);

        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay->addWidget(h,i,3,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(40,25));
       ui->citasLay->addWidget(ss,i,4,Qt::AlignTop);


       QPushButton *b=new QPushButton();
       b->setText("Revisar");
       b->setFixedSize(QSize(100,25));
       b->setStyleSheet("background-color: rgb(138,222,242);border: 1px solid rgb(60,200,234)");
       QSignalMapper *mapper2=new QSignalMapper(this);
       connect(b,SIGNAL(clicked(bool)),mapper2,SLOT(map()));
       mapper2->setMapping(b,folio);
       connect(mapper2,SIGNAL(mapped(QString)),this,SLOT(verCita(QString)));
       ui->citasLay->addWidget(b,i,5,Qt::AlignTop);

       QPushButton *s=new QPushButton();
       s->setText("Aceptar");
       s->setFixedSize(QSize(100,25));
       s->setStyleSheet("background-color: rgb(60,160,234)");
       QSignalMapper *mapper=new QSignalMapper(this);
       connect(s,SIGNAL(clicked(bool)),mapper,SLOT(map()));
       mapper->setMapping(s,folio);
       connect(mapper,SIGNAL(mapped(QString)),this,SLOT(aceptarCita(QString)));
       ui->citasLay->addWidget(s,i,6,Qt::AlignTop);

       QPushButton *q=new QPushButton();
       q->setText("Rechazar");
       q->setFixedSize(QSize(100,25));
       q->setStyleSheet("background-color: rgb(138,198,242)");
       QSignalMapper *mapper1=new QSignalMapper(this);
       connect(q,SIGNAL(clicked(bool)),mapper1,SLOT(map()));
       mapper1->setMapping(q,folio);
       connect(mapper1,SIGNAL(mapped(QString)),this,SLOT(rechazarCita(QString)));
       ui->citasLay->addWidget(q,i,7,Qt::AlignTop);
    i++;
    }

}

void MainWindow::aceptarCita(QString folio)
{
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("Deseas aceptar la solicitud?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        QString update,valor;
        valor="1";
        update="update cita set estado='"+valor+"' where idCita='"+folio+"' ";
        QSqlQuery aceptar;
        qDebug()<<update;
        aceptar.exec(update);


        //aqui mando la notificacion:
        QString fech,hor,tipo;
        QString cita,user1;
            cita="select *from  cita where idCita='"+folio+"';  ";
            QSqlQuery cita1;
            cita1.exec(cita);
            cita1.next();
            user1=cita1.value(1).toString();

        fech=cita1.value(2).toString();
        hor=cita1.value(3).toString();

        QString mensaj;
        mensaj="Se le informa que su cita para el dia: "+fech+" y con horario de: "+hor+" fue ACEPTADA.";
        tipo="1";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"');";
        QSqlQuery mandarNoti;
        mandarNoti.exec(notificacion);

        clearLayout(ui->citasLay);
        SolicitudCitas();
    }
    else
    {

    }

}
//MENSAJE SI EL DOCTOR RECHAZA UNA CITA DE PACIENTE
void MainWindow::rechazarCita(QString folio)
{
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de RECHAZAR la solicitud?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

        //aqui mando la notificacion:
        QString fech,hor,tipo;
        QString cita,user1;
            cita="select * from  cita where idCita='"+folio+"';  ";
            QSqlQuery cita1;
            cita1.exec(cita);
            cita1.next();
            user1=cita1.value(1).toString();

        fech=cita1.value(2).toString();
        hor=cita1.value(3).toString();

        QString mensaj;
        mensaj="Se le informa que su cita para el dia: "+fech+" y con horario de: "+hor+" fue RECHAZADA.";
        tipo="0";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"');";
        QSqlQuery mandarNoti;
        mandarNoti.exec(notificacion);

        QString delete1;
        delete1="delete from cita where idCita='"+folio+"' ";
        QSqlQuery eliminar;
        eliminar.exec(delete1);

        clearLayout(ui->citasLay);
        SolicitudCitas();
    }
    else
    {
    }
}

void MainWindow::verCita(QString folio)
{
    ui->stackedWidget_perfilDoctor->setCurrentIndex(3);
    ui->pushButton_horarioDoc->hide();
    ui->pushButton_citasDoc->hide();

    QString cita,user1;
    cita="select *from  cita where idCita='"+folio+"';  ";
    QSqlQuery cita1;
    cita1.exec(cita);
    cita1.next();
    user1=cita1.value(1).toString();

    QString CitaExtra,respons;
    respons="select *from usuario where matricula='"+user1+"';  ";
    QSqlQuery datos;
    datos.exec(respons);
    datos.next();

    QString nombreU;
    nombreU=datos.value(2).toString()+" "+datos.value(3).toString()+" "+datos.value(3).toString();

    int actual=QDate::currentDate().year();
    int yearUSuario=0;
    int edad=0;
    QString edad1;

    CitaExtra=cita1.value(6).toString();
    if(CitaExtra==""||CitaExtra=="0")
    {
        ui->tipoCita->setText("--Personal--");
        ui->Responsable->hide();
        ui->paciente->setText(nombreU);
        ui->telelefono->setText(datos.value(8).toString());
        ui->correo->setText(datos.value(7).toString());
        yearUSuario=datos.value(5).toDate().year();
        edad=actual-yearUSuario;
        edad1=QString::number(edad);
        ui->edad->setText(edad1);
        ui->fecha->setText(cita1.value(2).toString());
        ui->hora->setText(cita1.value(3).toString());
        ui->lugar->hide();
        ui->lugartext->hide();
        ui->label_5->hide();
        ui->sintomas->setPlainText(cita1.value(5).toString());
    }
    else
    {
        QString citaExterna,nombreU2;
        citaExterna="select *from citaExterna where idcitaExterna='"+CitaExtra+"'; ";
        QSqlQuery datosCita;
        datosCita.exec(citaExterna);
        datosCita.next();
        nombreU2=datosCita.value(2).toString();

        ui->tipoCita->setText("--Externa--");
        ui->Responsable->setText(nombreU);
        ui->paciente->setText(nombreU2);
        ui->telelefono->setText(datos.value(8).toString());
        ui->correo->setText(datos.value(7).toString());
        ui->edad->setText(datosCita.value(4).toString());
        ui->fecha->setText(cita1.value(2).toString());
        ui->hora->setText(cita1.value(3).toString());
        ui->lugar->setText(datosCita.value(3).toString());
        ui->sintomas->setPlainText(cita1.value(5).toString());

    }
}

void MainWindow::on_regresar_citasDoc_clicked()
{
    ui->stackedWidget_perfilDoctor->setCurrentIndex(2);
    ui->pushButton_horarioDoc->show();
    ui->pushButton_citasDoc->show();
}
//BOTON DE NOTIFICACIONES QUE TIENE EL PACIENTE
void MainWindow::on_butonNotifi_clicked()
{
    ui->stackedWidget_perfilPaciente->setCurrentIndex(0);
    if(verNoti==1)
    {
        if(ui->notificacionL->text()!="")
        {
            ui->notificacionL->hide();
        }
        ui->nofi->show();
        verNoti=0;
    }
    else
    {
      ui->nofi->hide();

      QString idNoti,id,update1;
      idNoti="select idNoti from notificacion;";
      QSqlQuery upNoti,upNoti1;
      upNoti.exec(idNoti);

      while(upNoti.next())
      {
          id=upNoti.value(0).toString();
          update1="update notificacion set vista=true where idNoti='"+id+"';";
          upNoti1.exec(update1);
            qDebug()<<id;
            qDebug()<<update1;
      }
      verNoti=1;
    }
}


void MainWindow::on_btnEditarStaff_clicked()
{
    ui->lineCorreoStaff->setReadOnly(false);
    ui->lineCorreoStaff->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->lineTelStaff->setReadOnly(false);
    ui->lineTelStaff->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->btnEditarStaff->setHidden(true);
    ui->btnCancelarEditarStaff->setHidden(false);
    ui->btnGuardarEditarStaff->setHidden(false);
    ui->imgCon1->setHidden(false);
    ui->imgCon2->setHidden(false);
    ui->lineContrasenia->setHidden(false);
    ui->lineConfirmaContrasenia->setHidden(false);
    ui->lineContrasenia->setReadOnly(false);
    ui->lineConfirmaContrasenia->setReadOnly(false);
    ui->btnMostrarContrasena->setHidden(false);
    ui->btnMostrarContrasena_2->setHidden(false);
    ui->lineContrasenia->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->lineConfirmaContrasenia->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->pushButton_5->show();
}

void MainWindow::on_btnCancelarEditarStaff_clicked()
{
    ui->lineCorreoStaff->setReadOnly(true);
    ui->lineCorreoStaff->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->lineTelStaff->setReadOnly(true);
    ui->lineTelStaff->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none; background-color:transparent");
    ui->btnEditarStaff->setHidden(false);
    ui->btnCancelarEditarStaff->setHidden(true);
    ui->btnGuardarEditarStaff->setHidden(true);
    ui->imgCon1->setHidden(true);
    ui->imgCon2->setHidden(true);
    ui->lineContrasenia->setHidden(true);
    ui->lineConfirmaContrasenia->setHidden(true);
    ui->btnMostrarContrasena->setHidden(true);
    ui->btnMostrarContrasena_2->setHidden(true);
    ui->lineContrasenia->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->lineConfirmaContrasenia->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->pushButton_5->hide();
}

void MainWindow::on_btnGuardarEditarStaff_clicked()
{
    QString actualizacion;
    QSqlQuery perfilStaff;
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea guardar los cambios?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        qDebug() << "Dijo que aye";
        //datos de edición
        QString correoNuevo = ui->lineCorreoStaff->text();
        QString telefonoNuevo = ui->lineTelStaff->text();
        QString passwordNueva = ui->lineContrasenia->text();
        QString passwordNuevaConf = ui->lineConfirmaContrasenia->text();
        if(passwordNueva == "" || passwordNuevaConf == "" || correoNuevo == "" || telefonoNuevo == ""){
            QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Los campos no pueden estar vacíos."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));messageBox.exec();
        }
        else{
            if(passwordNueva != passwordNuevaConf){
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Verifique que las contraseñas coincidan."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else if(telefonoNuevo.size() < 10){
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Telefono debe ser 10 dígitos"), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else {
                qDebug() << "------------------------";
                qDebug() << "Id del usuario (staff)";
                qDebug(id_usuario.toLatin1());
                actualizacion="update usuario set clave='"+passwordNueva+"',email='"+correoNuevo+"',telefono="+telefonoNuevo+" WHERE matricula='"+id_usuario+"'";
                perfilStaff.exec(actualizacion);
                QMessageBox messageBox(QMessageBox::Information,tr("Éxito"), tr("Los datos se han actualizado correctamente."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
                on_pushButton_miPerfil_clicked();
                ui->lineCorreoStaff->setReadOnly(true);
                ui->lineCorreoStaff->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->lineTelStaff->setReadOnly(true);
                ui->lineTelStaff->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none; background-color:transparent");
                ui->btnEditarStaff->setHidden(false);
                ui->imgCon1->setHidden(true);
                ui->imgCon2->setHidden(true);
                ui->lineContrasenia->setHidden(true);
                ui->lineConfirmaContrasenia->setHidden(true);
                ui->btnMostrarContrasena->setHidden(true);
                ui->btnMostrarContrasena_2->setHidden(true);
                ui->btnCancelarEditarStaff->setHidden(true);
                ui->btnGuardarEditarStaff->setHidden(true);
                ui->lineContrasenia->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->lineConfirmaContrasenia->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->pushButton_5->hide();
            }
        }
    }
    else{
        qDebug() << "Dijo que nel";
    }

}

void MainWindow::on_btnMostrarContrasena_clicked()
{
    if(toggleVision==0){
        QPixmap pixmap(":/imgs/view.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena->setIcon(ButtonIcon);
        ui->lineContrasenia->setEchoMode(QLineEdit::Normal);
        toggleVision = 1;
    }
    else if (toggleVision==1) {
        QPixmap pixmap(":/imgs/NoView.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena->setIcon(ButtonIcon);
        ui->lineContrasenia->setEchoMode(QLineEdit::Password);
        toggleVision = 0;
    }
}

void MainWindow::on_btnMostrarContrasena_2_clicked()
{
    if(toggleVision1==0){
        QPixmap pixmap(":/imgs/view.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_2->setIcon(ButtonIcon);
        ui->lineConfirmaContrasenia->setEchoMode(QLineEdit::Normal);
        toggleVision1 = 1;
    }
    else if (toggleVision1==1) {
        QPixmap pixmap(":/imgs/NoView.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_2->setIcon(ButtonIcon);
        ui->lineConfirmaContrasenia->setEchoMode(QLineEdit::Password);
        toggleVision1 = 0;
    }
}

//MENU DINAMICO QUE PONE LOS TIPS PARA PODER ELIMINAR
void MainWindow::mostrarTipsEliminar(){

    clearLayout(ui->listatips_3);
    QString consultatips,nombre;
    QSqlQuery querytips;
    int cont=0;
    consultatips="select *from tip";
    querytips.exec(consultatips);

    while(querytips.next())
    {
        idtip=querytips.value(0).toString();
        nombre=querytips.value(1).toString();
        QPushButton *b=new QPushButton();
        b->setText(nombre);
        b->setFixedSize(QSize(200,40));
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(b,idtip);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(ponerTipseliminar(QString)));
        ui->listatips_3->addWidget(b,cont,0,1,1);
        cont++;
    }


}

//BOTON QUE TE MUEVE A LA INTERFAZ PARA EDITAR UN TIP
void MainWindow::on_pushButton_editarTip_clicked()
{
    QString estiloBueno;
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    QString concolor =":/ /imgs/fondo.png";
    QPixmap c=concolor;
    //limpia si se queda en error los campos de agregar nuevo tip
    //ui->pushButton_agregarImagen->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descripcion->setStyleSheet(estiloBueno);
    ui->lineEdit_link->setStyleSheet(estiloBueno);
    ui->lineEdit_nombreTip->setStyleSheet(estiloBueno);
    mostrarTipsEditar();
    ui->stackedWidget_tips->setCurrentIndex(3);
    //poner en colores normal los tips
    ui->lineEdit_nombretip->setStyleSheet(estiloBueno);
    ui->lineEdit_links->setStyleSheet(estiloBueno);
    ui->plainTextEdit_descritip->setStyleSheet(estiloBueno);
    ui->label_imagenTip->setPixmap(c);
}




//-////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-////////////////////////////////////PARTE DE REMEDIOS CASEROS /////////////////////////////////////////
//-///////////////////////////////////ELIMINAR/////EDITAR/////////AGREGAR/////////////////////////////////////////////////
//-------------gregar remido casero
//carga las categorias que existen en remedios caseros
void MainWindow::cargarCategoria(){
    QSqlQueryModel *queryRemedios;
    queryRemedios= new QSqlQueryModel;
    queryRemedios->setQuery("SELECT  nombreCategoria FROM tipoCategoriaRem");
    ui->comboBox_remedios->setModel(queryRemedios);
    ui->comboBox_eliminarREmedios->setModel(queryRemedios);
    ui->comboBox_categoriaEdit->setModel(queryRemedios);
    ui->comboBox_catEditar->setModel(queryRemedios);
}

//boton que te llava al apartodo del remedios caseros
void MainWindow::on_pushButton_agregar_remedio_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(4);
    ui->stackedWidget_remedio->setCurrentIndex(0);
    ui->pushButton_SolicitudesUsuarios->hide();
    ui->pushButton_tip_2->hide();
    ui->pushButton_agregar_remedio->hide();
}

void MainWindow::on_pushButton_menu_admin_2_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(0);
    ui->pushButton_SolicitudesUsuarios->show();
    ui->pushButton_tip_2->show();
    ui->pushButton_agregar_remedio->show();
    QString concolor =":/ /imgs/fondo.png";
    QIcon color(concolor);
    ui->pushButton_Imgremedio->setIcon(color);
    ui->lineEdit_nomRemedio->clear();
    ui->plainTextEdit_procedimiento->clear();
    ui->plainTextEdit_ingredientes->clear();
    ui->plainTextEdit_procedimientoEli->clear();
    ui->label_CategoriaEli->clear();
    ui->label_nomRemedioEli->clear();
    ui->plainTextEdit_IngredientesEli->clear();
    ui->plainTextEdit_editProcedi->clear();
    ui->lineEdit_nombreEdit->clear();
    ui->plainTextEdit_editIngredi->clear();
    ui->label_imagenEditar->setPixmap(concolor);
    ui->label_imagEli->setPixmap(concolor);
}
//guardar la imagen del remedio casero
void MainWindow::on_pushButton_Imgremedio_clicked()
{
    ui->pushButton_Imgremedio->setText("");
    //Abrimos el dialogo para obtener la ruta
     imgRoute = QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto = file.readAll();
    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto);

    QIcon l(pix);
    ui->pushButton_Imgremedio->setIcon(l);

}
//metodo para guadar remedio casero
void MainWindow::on_pushButton_agregar_remedio_2_clicked()
{
    //estilo de line edit
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";

    //mensaje de aceptar
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de agregar el remdio casero?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

   agregarTipRemedio reme;
   QString nomRemedio,proce,ingre,categoria,buscCategori;
   QSqlQuery query;
   nomRemedio=ui->lineEdit_nomRemedio->text();
   proce=ui->plainTextEdit_procedimiento->toPlainText();
   ingre=ui->plainTextEdit_ingredientes->toPlainText();
   categoria=ui->comboBox_remedios->currentText();
   buscCategori=" select  idcategoria from tipoCategoriaRem where nombreCategoria='"+categoria+"'";
   query.exec(buscCategori);
   query.next();
   categoria=query.value(0).toString();
   if(nomRemedio=="" || proce=="" || ingre=="" || foto==""){
   QMessageBox messageBox(QMessageBox::Warning,
                                    tr("Warning"), tr("Por favor,ingrese los datos necesarios."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Actividad"));
            if (messageBox.exec() == QMessageBox::Yes){

             }
       }

        if(nomRemedio==""){
         ui->lineEdit_nomRemedio->setStyleSheet(estiloMalo);
        }else {
            ui->lineEdit_nomRemedio->setStyleSheet(estiloBueno);
        }
        if(proce==""){

            ui->plainTextEdit_procedimiento->setStyleSheet(estiloMalo);
        }else {
         ui->plainTextEdit_procedimiento->setStyleSheet(estiloBueno);
        }

        if(ingre==""){
        ui->plainTextEdit_ingredientes->setStyleSheet(estiloMalo);

        }else {
        ui->plainTextEdit_ingredientes->setStyleSheet(estiloBueno);

        }
        if(foto==""){
           ui->pushButton_Imgremedio->setStyleSheet(estiloMalo);

        }else {
        ui->pushButton_Imgremedio->setStyleSheet(estiloBueno);
        }

          if(nomRemedio!="" && proce!="" && ingre!="" && foto!="" ){
           if (message.exec() == QMessageBox::Yes){
               qDebug()<<"gregando remedio";
           reme.agregaRemedio(nomRemedio,ingre,proce,imgRoute,categoria);
           QString concolor =":/ /imgs/fondo.png";
           QIcon color(concolor);
           ui->pushButton_Imgremedio->setIcon(color);
           ui->lineEdit_nomRemedio->clear();
           ui->plainTextEdit_procedimiento->clear();
           ui->plainTextEdit_ingredientes->clear();
      }
    }
}



void MainWindow::on_pushButton_agergar_remedio_clicked()
{
    ui->stackedWidget_remedio->setCurrentIndex(1);
    ui->plainTextEdit_procedimientoEli->clear();
    ui->label_CategoriaEli->clear();
    ui->label_nomRemedioEli->clear();
    ui->plainTextEdit_IngredientesEli->clear();
    QString concolor =":/ /imgs/fondo.png";
    ui->label_imagEli->setPixmap(concolor);
    ui->plainTextEdit_editProcedi->clear();
    ui->lineEdit_nombreEdit->clear();
    ui->plainTextEdit_editIngredi->clear();
    ui->label_imagenEditar->setPixmap(concolor);
}

//metodo para el apartado de eliminar remedio
void MainWindow::on_pushButton_eliminar_remedio_clicked()
{
    mostrarRemedioEliminar();
    ui->stackedWidget_remedio->setCurrentIndex(2);
    QString concolor =":/ /imgs/fondo.png";
    QIcon color(concolor);
    ui->pushButton_Imgremedio->setIcon(color);
    ui->lineEdit_nomRemedio->clear();
    ui->plainTextEdit_procedimiento->clear();
    ui->plainTextEdit_ingredientes->clear();
    ui->plainTextEdit_procedimientoEli->clear();
    ui->label_CategoriaEli->clear();
    ui->label_nomRemedioEli->clear();
    ui->plainTextEdit_IngredientesEli->clear();
    ui->label_imagEli->setPixmap(concolor);
    ui->plainTextEdit_editProcedi->clear();
    ui->lineEdit_nombreEdit->clear();
    ui->plainTextEdit_editIngredi->clear();
    ui->label_imagenEditar->setPixmap(concolor);
}
// boton para ir al apartado de editar remedio
void MainWindow::on_pushButton_editar_remedio_clicked()
{
    ui->stackedWidget_remedio->setCurrentIndex(3);
    QString concolor =":/ /imgs/fondo.png";
    QIcon color(concolor);
    ui->pushButton_Imgremedio->setIcon(color);
    ui->lineEdit_nomRemedio->clear();
    ui->plainTextEdit_procedimiento->clear();
    ui->plainTextEdit_ingredientes->clear();
    ui->plainTextEdit_procedimientoEli->clear();
    ui->label_CategoriaEli->clear();
    ui->label_nomRemedioEli->clear();
    ui->plainTextEdit_IngredientesEli->clear();
    ui->label_imagEli->setPixmap(concolor);
    ui->plainTextEdit_editProcedi->clear();
    ui->lineEdit_nombreEdit->clear();
    ui->plainTextEdit_editIngredi->clear();
    ui->label_imagenEditar->setPixmap(concolor);
    mostarrRemedioEditar();
}

//con forme a lo que se encuentre en el comboBox hacemos la busqueda para mostrar en el menu dinamico
void MainWindow::on_comboBox_eliminarREmedios_currentTextChanged(const QString &arg1)
{
    ui->plainTextEdit_procedimientoEli->clear();
    ui->label_CategoriaEli->clear();
    ui->label_nomRemedioEli->clear();
    ui->plainTextEdit_IngredientesEli->clear();
    QString concolor =":/ /imgs/fondo.png";
    ui->label_imagEli->setPixmap(concolor);
    mostrarRemedioEliminar();
}

//metodo para poner la informacion de los remedios
void MainWindow::PonerRemedios(QString idremedios)
{
    QSqlQuery busqueda,query;
    QString nomRemedio,ingredientes,procedimineto,foto,categoria,nombreCategoria;
    busqueda.exec("select rem.nombreRemedio,rem.ingredientes,rem.procedimiento,rem.fotoRemedio,rem.idcategoria,rem.fotoRemedio,rem.idremedio from tipoCategoriaRem as tipo inner join remedios as rem on rem.idcategoria=tipo.idcategoria where rem.idremedio='"+idremedios+"'");
    busqueda.next();
    //nombreRemedio,ingredientes,procedimiento,fotoRemedio,idcategoria
    nomRemedio=busqueda.value(0).toString();
    ingredientes=busqueda.value(1).toString();
    procedimineto=busqueda.value(2).toString();
    categoria=busqueda.value(4).toString();
    idremedios=busqueda.value(6).toString();
    QPixmap pix;
    if(!pix.loadFromData(busqueda.value(5).toByteArray())){
        ui->label_imagEli->setText("<b>Error de imagen</b>");

    }

    int widWidth = this->ui->label_imagEli->width();
    int widHeight = this->ui->label_imagEli->height();
    ui->label_imagEli->setPixmap(pix.scaled(widWidth, widHeight, Qt::KeepAspectRatioByExpanding));
    query.exec("select tipo.nombreCategoria from tipoCategoriaRem as tipo inner join remedios as rem  on rem.idcategoria=tipo.idcategoria where rem.idcategoria='"+categoria+"'");
    query.next();
    nombreCategoria=query.value(0).toString();
    ui->plainTextEdit_procedimientoEli->setPlainText(procedimineto);
    ui->label_CategoriaEli->setText(nombreCategoria);
    ui->label_nomRemedioEli->setText(nomRemedio);
    ui->plainTextEdit_IngredientesEli->setPlainText(ingredientes);

    //editar
    ui->plainTextEdit_editProcedi->setPlainText(procedimineto);
    ui->comboBox_catEditar->setCurrentText(nombreCategoria);
    ui->lineEdit_nombreEdit->setText(nomRemedio);
    ui->plainTextEdit_editIngredi->setPlainText(ingredientes);
    int w = this->ui->label_imagEli->width();
    int wi = this->ui->label_imagEli->height();
    ui->label_imagenEditar->setPixmap(pix.scaled(w, wi, Qt::KeepAspectRatioByExpanding));

}
//menu dinamico para mostra los remedios existentes que pueden ser editados
void MainWindow::mostarrRemedioEditar(){
    clearLayout(ui->listatips_2);
    QString consultatips,nombre,remedioCatalogo;
    QSqlQuery querytips;
    int cont=0;
    remedioCatalogo=ui->comboBox_categoriaEdit->currentText();
    consultatips="select rem.idremedio,rem.nombreRemedio from tipoCategoriaRem as tipo inner join remedios as rem  on rem.idcategoria=tipo.idcategoria where tipo.nombreCategoria='"+remedioCatalogo+"'";
    querytips.exec(consultatips);

    while(querytips.next())
    {
        idtip=querytips.value(0).toString();
        nombre=querytips.value(1).toString();
        QPushButton *b=new QPushButton();
        b->setText(nombre);
        b->setFixedSize(QSize(200,40));
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(b,idtip);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(PonerRemedios(QString)));
        ui->listatips_2->addWidget(b,cont,0,1,1);
        cont++;
    }

}
//MENU DINAMICO PARA MOSTRAR LOS REMDIOS EXISTENTES QUE PUEDEN SER ELIMINADOS
void MainWindow::mostrarRemedioEliminar(){

    clearLayout(ui->listatips_4);
    QString consultatips,nombre,remedioCatalogo;
    QSqlQuery querytips;
    remedioCatalogo=ui->comboBox_eliminarREmedios->currentText();
    int cont=0;
    consultatips="select rem.idremedio,rem.nombreRemedio from tipoCategoriaRem as tipo inner join remedios as rem  on rem.idcategoria=tipo.idcategoria where tipo.nombreCategoria='"+remedioCatalogo+"'";
    querytips.exec(consultatips);

    while(querytips.next())
    {
        idremedios=querytips.value(0).toString();
        nombre=querytips.value(1).toString();
        QPushButton *b=new QPushButton();
        b->setText(nombre);
        b->setFixedSize(QSize(200,40));
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(b,idremedios);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(PonerRemedios(QString)));
        ui->listatips_4->addWidget(b,cont,0,1,1);
        cont++;
    }
}
//METODO QUE ELIMINA EL REMEDIO SELECCIONADO PARA SER ELIMINADO
void MainWindow::on_pushButton_eliminarRemedio_clicked()
{
    qDebug()<<"hola elimana:"<<idremedios;
    QSqlQuery eliminar;QMessageBox message(QMessageBox::Question,
    tr("Warning"), tr("¿Estas seguro de eliminar remedio casero?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if(ui->label_nomRemedioEli->text()==""){
        QMessageBox messageBox(QMessageBox::Warning,
                                         tr("Warning"), tr("Por favor,seleccione remedio a eliminar."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 if (messageBox.exec() == QMessageBox::Yes){

                  }
            }
    if(ui->label_nomRemedioEli->text()!=""){
    if (message.exec() == QMessageBox::Yes){
    eliminar.exec("delete from remedios where idremedio='"+idremedios+"'");
    ui->plainTextEdit_procedimientoEli->clear();
    ui->label_CategoriaEli->clear();
    ui->label_nomRemedioEli->clear();
    ui->plainTextEdit_IngredientesEli->clear();
    QString concolor =":/ /imgs/fondo.png";
    ui->label_imagEli->setPixmap(concolor);
    mostrarRemedioEliminar();
        }
    }


}
//METODO QUE DEPENDIEDON LO QUE SELECIIONE SE MUESTRA EN EL MENU DINAMICO PARA EDITAR
void MainWindow::on_comboBox_categoriaEdit_currentTextChanged(const QString &arg1)
{

    ui->plainTextEdit_editProcedi->clear();
    ui->label_CategoriaEli->clear();
    ui->lineEdit_nombreEdit->clear();
    ui->plainTextEdit_editIngredi->clear();
    QString concolor =":/ /imgs/fondo.png";
    ui->label_imagenEditar->setPixmap(concolor);
    mostarrRemedioEditar();

}

//METODO PARA EDITAR EL REMEDIO QUE SE HAYA SELECCIONADO
void MainWindow::on_pushButton_editarRemedio_clicked()
{
    agregarTipRemedio editar;
    QSqlQuery query;
    QString nomRemedio,ingredientes,procedimineto,foto,categoria,buscCategori;
    QString estiloBueno, estiloMalo;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    //mensaje de aceptar
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de editar el remdio casero?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    categoria=ui->comboBox_catEditar->currentText();
    buscCategori=" select  idcategoria from tipoCategoriaRem where nombreCategoria='"+categoria+"'";
    query.exec(buscCategori);
    query.next();
    categoria=query.value(0).toString();
    procedimineto=ui->plainTextEdit_editProcedi->toPlainText();
    nomRemedio=ui->lineEdit_nombreEdit->text();
    ingredientes=ui->plainTextEdit_editIngredi->toPlainText();
    //nombreRemedio,ingredientes,procedimiento,fotoRemedio,idcategoria
    if(nomRemedio=="" || procedimineto=="" || ingredientes==""){
    QMessageBox messageBox(QMessageBox::Warning,
                                     tr("Warning"), tr("Por favor,ingrese los datos necesarios."), QMessageBox::Yes);
             messageBox.setButtonText(QMessageBox::Yes, tr("Actividad"));
             if (messageBox.exec() == QMessageBox::Yes){

              }
        }
    if(nomRemedio==""){
     ui->lineEdit_nombreEdit->setStyleSheet(estiloMalo);
    }else {
        ui->lineEdit_nombreEdit->setStyleSheet(estiloBueno);
    }
    if(procedimineto==""){

        ui->plainTextEdit_editProcedi->setStyleSheet(estiloMalo);
    }else {
     ui->plainTextEdit_editProcedi->setStyleSheet(estiloBueno);
    }

    if(ingredientes==""){
    ui->plainTextEdit_editIngredi->setStyleSheet(estiloMalo);

    }else {
    ui->plainTextEdit_editIngredi->setStyleSheet(estiloBueno);

    }

    if(nomRemedio!="" && procedimineto!=""&& ingredientes!=""){
     if (message.exec() == QMessageBox::Yes){
      editar.editarRemedio(idremedios,nomRemedio,ingredientes,procedimineto,categoria);
      mostarrRemedioEditar();
      ui->plainTextEdit_editProcedi->clear();
      ui->lineEdit_nombreEdit->clear();
      ui->plainTextEdit_editIngredi->clear();
      QString concolor =":/ /imgs/fondo.png";
      ui->label_imagenEditar->setPixmap(concolor);
     }
   }
}


void MainWindow::on_pb_rechazarCitas_clicked()
{
    ui->stackedWidget_perfilDoctor->setCurrentIndex(4);
    CitasAceptadas();
    ui->lineEdit_matriculaMedico->clear();
    ui->label_mensajePaciente->hide();
    ui->label_espacioBlanco->hide();
}

void MainWindow::CitasAceptadas()
{
    clearLayout(ui->citasAceptadas);
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";

    QString citas,est,idD,idD2;
    idD="select iddoctor from doctor where idUser='"+id_usuario+"'; ";
    QSqlQuery busId;
    busId.exec(idD);
    busId.next();
    idD2=busId.value(0).toString();
    est="1";
    citas="select *from cita where doctor='"+idD2+"' and estado='"+est+"'; ";
    QSqlQuery citas1;
    citas1.exec(citas);

     int i=1;
     QString folio,matricu,fecha,hora,preparada;


    while (citas1.next()) {
        folio=citas1.value(0).toString();
        matricu=citas1.value(1).toString();
        fecha=citas1.value(2).toString();
        hora=citas1.value(3).toString();
        preparada=citas1.value(8).toString();

        if(preparada=="Pendiente")
        {



    if(ban==1)
    {
        rgb=r1;
        ban=2;
    }
    else
    {
        rgb=r2;
        ban=1;
    }


    QLabel *l=new QLabel;
    l->setText(folio);
    l->setFixedSize(QSize(100,25));
    l->setStyleSheet("background-color: rgb("+rgb+")");
    ui->citasAceptadas->addWidget(l,i,0,Qt::AlignTop);




    QLabel *m=new QLabel;
    m->setText(matricu);
    m->setFixedSize(QSize(100,25));
    m->setStyleSheet("background-color: rgb("+rgb+")");
    ui->citasAceptadas->addWidget(m,i,1,Qt::AlignTop);





    QLabel *r=new QLabel;
    r->setText(fecha);
    r->setStyleSheet("background-color: rgb("+rgb+")");
    r->setFixedSize(QSize(100,25));
    ui->citasAceptadas->addWidget(r,i,2,Qt::AlignTop);




    QLabel *h=new QLabel;
    h->setText(hora);
    h->setFixedSize(QSize(100,25));
    h->setStyleSheet("background-color: rgb("+rgb+")");
    ui->citasAceptadas->addWidget(h,i,3,Qt::AlignTop);

    QLabel *ss=new QLabel;
    ss->setText(" ");
    ss->setFixedSize(QSize(60,25));
    ui->citasAceptadas->addWidget(ss,i,4,Qt::AlignTop);



   QPushButton *b=new QPushButton();
   b->setText("Cancelar cita");
   b->setFixedSize(QSize(100,25));
   b->setStyleSheet("background-color: rgb(138,222,242);border: 1px solid rgb(60,200,234)");
   QSignalMapper *mapper2=new QSignalMapper(this);
   connect(b,SIGNAL(clicked(bool)),mapper2,SLOT(map()));
   mapper2->setMapping(b,folio);
   connect(mapper2,SIGNAL(mapped(QString)),this,SLOT(CancelarCita(QString)));
   ui->citasAceptadas->addWidget(b,i,5,Qt::AlignTop);

    i++;

        }
        else
        {

        }

    }

}

void MainWindow::CancelarCita(QString folio)
{
    QString upcita;
    upcita="update cita set preparada='Cancelada' where idCita='"+folio+"' ;";
    QSqlQuery citaUp;
    citaUp.exec(upcita);

    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("Deseas Cancelar la Cita"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        QString upcita;
        upcita="update cita set preparada='Cancelada' where idCita='"+folio+"' ;";
        QSqlQuery citaUp;
        citaUp.exec(upcita);


        //aqui mando la notificacion:
        QString fech,hor,tipo;
        QString cita,user1;
            cita="select *from  cita where idCita='"+folio+"';  ";
            QSqlQuery cita1;
            cita1.exec(cita);
            cita1.next();
            user1=cita1.value(1).toString();

        fech=cita1.value(2).toString();
        hor=cita1.value(3).toString();

        QString mensaj;
        mensaj="Se le informa que su cita para el dia: "+fech+" y con horario de: "+hor+" fue CANCELADA. Acuda o llame a nuestras oficinas para reagendar su cita";
        tipo="0";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"');";
        QSqlQuery mandarNoti;
        mandarNoti.exec(notificacion);

        clearLayout(ui->citasAceptadas);
        CitasAceptadas();
    }
}

void MainWindow::on_btnEditarDoctor_clicked()
{
    ui->lineCorreoDoctor->setReadOnly(false);
    ui->lineCorreoDoctor->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->lineTelDoctor->setReadOnly(false);
    ui->lineTelDoctor->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->btnEditarDoctor->setHidden(true);
    ui->btnGuardarEditarDoctor->setHidden(false);
    ui->btnCancelarEditarDoctor->setHidden(false);
    ui->imgCon1_2->setHidden(false);
    ui->imgCon2_2->setHidden(false);
    ui->lineContraseniaDoc->setHidden(false);
    ui->lineConfirmaContraseniaDoc->setHidden(false);
    ui->lineContraseniaDoc->setReadOnly(false);
    ui->lineConfirmaContraseniaDoc->setReadOnly(false);
    ui->btnMostrarContrasena_3->setHidden(false);
    ui->btnMostrarContrasena_4->setHidden(false);
    ui->lineContraseniaDoc->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->lineConfirmaContraseniaDoc->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->pb_bajaDoctor->show();
}

void MainWindow::on_btnCancelarEditarDoctor_clicked()
{
    ui->lineCorreoDoctor->setReadOnly(true);
    ui->lineCorreoDoctor->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->lineTelDoctor->setReadOnly(true);
    ui->lineTelDoctor->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none; background-color:transparent");
    ui->btnEditarDoctor->setHidden(false);
    ui->btnCancelarEditarDoctor->setHidden(true);
    ui->btnGuardarEditarDoctor->setHidden(true);
    ui->imgCon1_2->setHidden(true);
    ui->imgCon2_2->setHidden(true);
    ui->lineContraseniaDoc->setHidden(true);
    ui->lineConfirmaContraseniaDoc->setHidden(true);
    ui->btnMostrarContrasena_3->setHidden(true);
    ui->btnMostrarContrasena_4->setHidden(true);
    ui->lineContraseniaDoc->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->lineConfirmaContraseniaDoc->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->pb_bajaDoctor->hide();
}

void MainWindow::on_btnGuardarEditarDoctor_clicked()
{
    QString actualizacion;
    QSqlQuery perfilStaff;
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea guardar los cambios?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        qDebug() << "Dijo que aye";
        //datos de edición
        QString correoNuevo = ui->lineCorreoDoctor->text();
        QString telefonoNuevo = ui->lineTelDoctor->text();
        QString passwordNueva = ui->lineContraseniaDoc->text();
        QString passwordNuevaConf = ui->lineConfirmaContraseniaDoc->text();
        if(passwordNueva == "" || passwordNuevaConf == "" || correoNuevo == "" || telefonoNuevo == ""){
            QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Los campos no pueden estar vacíos."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));messageBox.exec();
        }
        else{
            if(passwordNueva != passwordNuevaConf){
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Verifique que las contraseñas coincidan."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else if(telefonoNuevo.size() < 10){
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Telefono debe ser 10 dígitos"), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else {
                qDebug() << "------------------------";
                qDebug() << "Id del usuario (doc)";
                qDebug(id_usuario.toLatin1());
                actualizacion="update usuario set clave='"+passwordNueva+"',email='"+correoNuevo+"',telefono="+telefonoNuevo+" WHERE matricula='"+id_usuario+"'";
                if( perfilStaff.exec(actualizacion) ){
                    QMessageBox messageBox(QMessageBox::Information,tr("Éxito"), tr("Los datos se han actualizado correctamente."), QMessageBox::Yes);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();
                } else qDebug()<<perfilStaff.lastError().text();
                ui->lineCorreoDoctor->setReadOnly(true);
                ui->lineCorreoDoctor->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->lineTelDoctor->setReadOnly(true);
                ui->lineTelDoctor->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none; background-color:transparent");
                ui->btnEditarDoctor->setHidden(false);
                ui->imgCon1_2->setHidden(true);
                ui->imgCon2_2->setHidden(true);
                ui->lineContraseniaDoc->setHidden(true);
                ui->lineConfirmaContraseniaDoc->setHidden(true);
                ui->btnMostrarContrasena_3->setHidden(true);
                ui->btnMostrarContrasena_4->setHidden(true);
                ui->btnCancelarEditarDoctor->setHidden(true);
                ui->btnGuardarEditarDoctor->setHidden(true);
                ui->lineContraseniaDoc->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->lineConfirmaContraseniaDoc->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->pb_bajaDoctor->hide();
                on_pushButton_miPerfil_clicked();
            }
        }
    }
    else{
        qDebug() << "Dijo que nel";
    }
}

void MainWindow::on_btnMostrarContrasena_3_clicked()
{
    if(toggleVision==0){
        QPixmap pixmap(":/imgs/view.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_3->setIcon(ButtonIcon);
        ui->lineContraseniaDoc->setEchoMode(QLineEdit::Normal);
        toggleVision = 1;
    }
    else if (toggleVision==1) {
        QPixmap pixmap(":/imgs/NoView.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_3->setIcon(ButtonIcon);
        ui->lineContraseniaDoc->setEchoMode(QLineEdit::Password);
        toggleVision = 0;
    }
}

void MainWindow::on_btnMostrarContrasena_4_clicked()
{
    if(toggleVision1==0){
        QPixmap pixmap(":/imgs/view.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_4->setIcon(ButtonIcon);
        ui->lineConfirmaContraseniaDoc->setEchoMode(QLineEdit::Normal);
        toggleVision1 = 1;
    }
    else if (toggleVision1==1) {
        QPixmap pixmap(":/imgs/NoView.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_4->setIcon(ButtonIcon);
        ui->lineConfirmaContraseniaDoc->setEchoMode(QLineEdit::Password);
        toggleVision1 = 0;
    }
}

void MainWindow::on_btnEditarPaciente_clicked()
{
    ui->lineCorreoPaciente->setReadOnly(false);
    ui->lineCorreoPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->lineTelPaciente->setReadOnly(false);
    ui->lineTelPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->btnEditarPaciente->setHidden(true);
    ui->btnCancelarEditarPaciente->setHidden(false);
    ui->btnGuardarEditarPaciente->setHidden(false);
    ui->imgCon1_3->setHidden(false);
    ui->imgCon2_3->setHidden(false);
    ui->lineContraseniaPaciente->setHidden(false);
    ui->lineConfirmaContraseniaPaciente->setHidden(false);
    ui->lineContraseniaPaciente->setReadOnly(false);
    ui->lineConfirmaContraseniaPaciente->setReadOnly(false);
    ui->btnMostrarContrasena_5->setHidden(false);
    ui->btnMostrarContrasena_6->setHidden(false);
    ui->lineContraseniaPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->lineConfirmaContraseniaPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border-top:none; border-bottom: 1px solid #5d80b6; background-color:transparent");
    ui->pb_bajaDoctor->show();
}

void MainWindow::on_btnCancelarEditarPaciente_clicked()
{
    ui->lineCorreoPaciente->setReadOnly(true);
    ui->lineCorreoPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->lineTelPaciente->setReadOnly(true);
    ui->lineTelPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none; background-color:transparent");
    ui->btnEditarPaciente->setHidden(false);
    ui->btnCancelarEditarPaciente->setHidden(true);
    ui->btnGuardarEditarPaciente->setHidden(true);
    ui->imgCon1_3->setHidden(true);
    ui->imgCon2_3->setHidden(true);
    ui->lineContraseniaPaciente->setHidden(true);
    ui->lineConfirmaContraseniaPaciente->setHidden(true);
    ui->btnMostrarContrasena_5->setHidden(true);
    ui->btnMostrarContrasena_6->setHidden(true);
    ui->lineContraseniaPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->lineConfirmaContraseniaPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
    ui->pb_bajaDoctor->hide();
}

void MainWindow::on_btnGuardarEditarPaciente_clicked()
{
    QString actualizacion;
    QSqlQuery perfilStaff;
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea guardar los cambios?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        qDebug() << "Dijo que aye";
        //datos de edición
        QString correoNuevo = ui->lineCorreoPaciente->text();
        QString telefonoNuevo = ui->lineTelPaciente->text();
        QString passwordNueva = ui->lineContraseniaPaciente->text();
        QString passwordNuevaConf = ui->lineConfirmaContraseniaPaciente->text();
        if(passwordNueva == "" || passwordNuevaConf == "" || correoNuevo == "" || telefonoNuevo == ""){
            QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Los campos no pueden estar vacíos."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));messageBox.exec();
        }
        else{
            if(passwordNueva != passwordNuevaConf){
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Verifique que las contraseñas coincidan."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else if(telefonoNuevo.size() < 10){
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Telefono debe ser 10 dígitos"), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else {
                qDebug() << "------------------------";
                qDebug() << "Id del usuario (paciente)";
                qDebug(id_usuario.toLatin1());
                actualizacion="update usuario set clave='"+passwordNueva+"',email='"+correoNuevo+"',telefono='"+telefonoNuevo+"' WHERE matricula='"+id_usuario+"'";
                if( perfilStaff.exec(actualizacion) ){
                    QMessageBox messageBox(QMessageBox::Information,tr("Éxito"), tr("Los datos se han actualizado correctamente."), QMessageBox::Yes);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();
                } else qDebug()<<perfilStaff.lastError().text();

                ui->lineCorreoPaciente->setReadOnly(true);
                ui->lineCorreoPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->lineTelPaciente->setReadOnly(true);
                ui->lineTelPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none; background-color:transparent");
                ui->btnEditarPaciente->setHidden(false);
                ui->imgCon1_3->setHidden(true);
                ui->imgCon2_3->setHidden(true);
                ui->lineContraseniaPaciente->setHidden(true);
                ui->lineConfirmaContraseniaPaciente->setHidden(true);
                ui->btnMostrarContrasena_5->setHidden(true);
                ui->btnMostrarContrasena_6->setHidden(true);
                ui->btnCancelarEditarPaciente->setHidden(true);
                ui->btnGuardarEditarPaciente->setHidden(true);
                ui->lineContraseniaPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->lineConfirmaContraseniaPaciente->setStyleSheet("font: 15pt MS Shell Dlg 2; border:none;background-color:transparent;");
                ui->pb_bajaDoctor->hide();
                on_pushButton_miPerfil_clicked();
            }
        }
    }
    else{
        qDebug() << "Dijo que nel";
    }
}

void MainWindow::on_btnMostrarContrasena_5_clicked()
{
    if(toggleVision==0){
        QPixmap pixmap(":/imgs/view.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_5->setIcon(ButtonIcon);
        ui->lineContraseniaPaciente->setEchoMode(QLineEdit::Normal);
        toggleVision = 1;
    }
    else if (toggleVision==1) {
        QPixmap pixmap(":/imgs/NoView.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_5->setIcon(ButtonIcon);
        ui->lineContraseniaPaciente->setEchoMode(QLineEdit::Password);
        toggleVision = 0;
    }
}

void MainWindow::on_btnMostrarContrasena_6_clicked()
{
    if(toggleVision1==0){
        QPixmap pixmap(":/imgs/view.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_6->setIcon(ButtonIcon);
        ui->lineConfirmaContraseniaPaciente->setEchoMode(QLineEdit::Normal);
        toggleVision1 = 1;
    }
    else if (toggleVision1==1) {
        QPixmap pixmap(":/imgs/NoView.png");
        QIcon ButtonIcon(pixmap);
        ui->btnMostrarContrasena_6->setIcon(ButtonIcon);
        ui->lineConfirmaContraseniaPaciente->setEchoMode(QLineEdit::Password);
        toggleVision1 = 0;
    }
}
//MENSAJE DE CANCELACION DE CITA POR PARTE DE UN PACINETE
void MainWindow::PonerCitas(QString folio){
    QString consulta;
    QSqlQuery query;
    qDebug()<<"folio:"<<folio;
    QMessageBox messageBox(QMessageBox::Warning,
                           tr(""), tr("Cita cancelada"), QMessageBox::Yes);
    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    QMessageBox message(QMessageBox::Question,
                        tr("Warning"), tr("¿Estas seguro de cancelar tu cita?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    if (message.exec() == QMessageBox::Yes){
        if (messageBox.exec() == QMessageBox::Yes){
            QString fech,hor,tipo;
            QString cita,user1;
            cita="select *from  cita where idCita='"+folio+"'";
            QSqlQuery cita1;
            cita1.exec(cita);
            cita1.next();
            user1=cita1.value(4).toString();
            fech=cita1.value(2).toString();
            hor=cita1.value(3).toString();

            QString mensaj;
            mensaj="Se le informa que su paciente de la : "+fech+" y con horario de: "+hor+" fue CANCELADA.";
            tipo="0";
            QString notificacion;
            notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"')";
            QSqlQuery mandarNoti;
            mandarNoti.exec(notificacion);
            consulta="delete from cita where idCita='"+folio+"'";
            query.exec(consulta);
            query.next();
            clearLayout(ui->citasLay_2);
            mostrarCitas();
        }
    }
}

void MainWindow::PagarCitas(QString folio){
    QString consulta;
    QSqlQuery query;
    qDebug()<<"folio:"<<folio;
    clearLayout(ui->pagoIntervenciones);
    QMessageBox messageBox(QMessageBox::Warning,
                           tr(""), tr("Cita cancelada"), QMessageBox::Yes);
    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    QMessageBox message(QMessageBox::Question,
                        tr("Warning"), tr("¿Estas seguro de pagar tu cita?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    QMessageBox metodo(QMessageBox::Question,
                        tr("Warning"), tr("¿Como quiere pagar?"), QMessageBox::Yes | QMessageBox::No);
    metodo.setButtonText(QMessageBox::Yes, tr("Ventanilla"));
    metodo.setButtonText(QMessageBox::No, tr("Tarjeta de Crédito"));
    if (message.exec() == QMessageBox::Yes){

        //Preguntar método de pago
        if (metodo.exec() == QMessageBox::Yes){
            //Notificar al cajero
            QMessageBox::information(this,"Ventanilla","Pase a pagar a ventanilla con su folio de cita.");
        }
        else{
                clearLayout(ui->pagoIntervenciones);
                pagar = new pagarCitasPaciente(folio,this);
                pagar->show();

                ocultar=new QTimer(this);
                connect(ocultar,SIGNAL(timeout()),this,SLOT(actTablaCitas()));
                ocultar->start(1000);
        }
    }
}
void MainWindow::mostrarCitas(){

    clearLayout(ui->citasLay_2);
    clearLayout(ui->encabezadoCitas_2);
    QString citas,est,preparada;
    QSqlQuery consulta,consulta2;
    est="1";
    preparada="Pendiente";
    citas="select cit.idCita,us.nombre,us.appaterno,us.apmaterno,cit.hora,cit.fecha from usuario as us inner join doctor as doc on us.matricula=doc.idUser inner join cita as cit on doc.iddoctor=cit.doctor where cit.matricula='"+id_usuario+"' and cit.estado='"+est+"' and cit.pagada=0 and cit.preparada='"+preparada+"'";
    if(!consulta.exec(citas)) consulta.lastError().text();

    int f=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";


    QLabel *foli=new QLabel;
    foli->setText("folio de Cita");
    foli->setFixedSize(QSize(100,25));
    foli->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas_2->addWidget(foli,0,0,Qt::AlignLeft);

    QLabel *Doc=new QLabel;
    Doc->setText("Doctor");
    Doc->setFixedSize(QSize(100,25));
    Doc->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas_2->addWidget(Doc,0,1,Qt::AlignLeft);

    QLabel *fech=new QLabel;
    fech->setText("Fecha");
    fech->setFixedSize(QSize(100,25));
    fech->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas_2->addWidget(fech,0,2,Qt::AlignLeft);

    QLabel *hor=new QLabel;
    hor->setText("Hora");
    hor->setFixedSize(QSize(100,25));
    hor->setStyleSheet("border: 1px solid rgb(9,9,9)");
    ui->encabezadoCitas_2->addWidget(hor,0,3,Qt::AlignLeft);


    QString folio,doctor,fecha,hora,nomDoct;
    int i=0;
    int l=0;

    while(consulta.next())
    {
        folio=consulta.value(0).toString();
        doctor=consulta.value(1).toString()+" "+consulta.value(2).toString()+" "+consulta.value(3).toString();
        hora=consulta.value(4).toString();
        fecha=consulta.value(5).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        QLabel *fol=new QLabel;
        fol->setText(folio);
        fol->setFixedSize(QSize(100,25));
        fol->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay_2->addWidget(fol,i,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setFixedSize(QSize(110,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay_2->addWidget(m,i,1,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->citasLay_2->addWidget(r,i,2,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay_2->addWidget(h,i,3,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(40,25));
        ui->citasLay_2->addWidget(ss,i,4,Qt::AlignTop);

        QPushButton *q=new QPushButton();
        q->setText("Cancelar");
        q->setFixedSize(QSize(100,25));
        q->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper1=new QSignalMapper(this);
        connect(q,SIGNAL(clicked(bool)),mapper1,SLOT(map()));
        mapper1->setMapping(q,folio);
        connect(mapper1,SIGNAL(mapped(QString)),this,SLOT(PonerCitas(QString)));
        ui->citasLay_2->addWidget(q,i,7,Qt::AlignTop);
        i++;
    }

    preparada="Completada";
    citas="select cit.idCita,us.nombre,us.appaterno,us.apmaterno,cit.hora,cit.fecha from usuario as us inner join doctor as doc on us.matricula=doc.idUser inner join cita as cit on doc.iddoctor=cit.doctor where cit.matricula='"+id_usuario+"' and cit.estado='"+est+"' and cit.pagada=0 and cit.preparada='"+preparada+"'";
    if(!consulta2.exec(citas)) consulta2.lastError().text();
    while(consulta2.next()){
        folio=consulta2.value(0).toString();
        doctor=consulta2.value(1).toString()+" "+consulta2.value(2).toString()+" "+consulta2.value(3).toString();
        hora=consulta2.value(4).toString();
        fecha=consulta2.value(5).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        QLabel *fol=new QLabel;
        fol->setText(folio);
        fol->setFixedSize(QSize(100,25));
        fol->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay_2->addWidget(fol,l,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setFixedSize(QSize(110,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay_2->addWidget(m,l,1,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->citasLay_2->addWidget(r,l,2,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->citasLay_2->addWidget(h,l,3,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(40,25));
        ui->citasLay_2->addWidget(ss,l,4,Qt::AlignTop);

        QPushButton *p= new QPushButton();
        p->setText("Pagar");
        p->setFixedSize(QSize(100,25));
        p->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper2=new QSignalMapper(this);
        connect(p,SIGNAL(clicked(bool)),mapper2,SLOT(map()));
        mapper2->setMapping(p,folio);
        connect(mapper2,SIGNAL(mapped(QString)),this,SLOT(PagarCitas(QString)));
        ui->citasLay_2->addWidget(p,l,9,Qt::AlignTop);
        clearLayout(ui->pagoIntervenciones);

        l++;
    }
}


void MainWindow::on_pushButton_Cancelar_Cita_clicked()
{
    ui->stackedWidget_perfilPaciente->setCurrentIndex(2);
    clearLayout(ui->citasLay_2);
    clearLayout(ui->encabezadoCitas_2);
    clearLayout(ui->pagoIntervenciones);
    mostrarCitas();
}
//BOTON DE NOTIFICACIONES QUE TIENE EL DOCTOR
void MainWindow::on_butonNotifi_4_clicked()
{
    ui->stackedWidget_perfilDoctor->setCurrentIndex(0);
    if(verNoti==1)
    {
        if(ui->notificacionL_4->text()!="")
        {
            ui->notificacionL_4->hide();

        }

        ui->nofi_2->show();
        verNoti=0;

    }
    else
    {
        ui->nofi_2->hide();

        QString idNoti,id,update1;
        idNoti="select idNoti from notificacion";
        QSqlQuery upNoti,upNoti1;
        upNoti.exec(idNoti);

        while(upNoti.next())
        {
            id=upNoti.value(0).toString();
            update1="update notificacion set vista=true where idNoti='"+id+"';";
            upNoti1.exec(update1);

        }

        verNoti=1;
    }

}
// ////////////////////////////// LLENAR EL HISTORIAL //////////////////////////////

//doctor quiere hacer una consulta
void MainWindow::on_pb_realizarConsulta_clicked()
{
    ui->stackedWidget_perfilDoctor->setCurrentIndex(5);
    //Ocultamos los menus
    ui->lb_noCoincideFecha->setHidden(true);
    ui->w_infoPacConsulta->setHidden(true);
    ui->sw_historialReceta->setHidden(true);
    ui->lineEdit_matriculaMedico->clear();
    ui->label_mensajePaciente->hide();
    ui->label_espacioBlanco->hide();
}

//Cuando busca un folio de cita
void MainWindow::on_pb_buscarCita_clicked()
{
    QSqlQuery cita,fecha;
    QPixmap pix;
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";

    if( !ui->le_folioCita->text().isEmpty() )
        cita.exec("SELECT * FROM cita WHERE idCita="+ui->le_folioCita->text());
    else{
        ui->le_folioCita->setStyleSheet(estiloMalo);
        ui->lb_noCoincideFecha->setText("Ingrese un folio.");
        ui->lb_noCoincideFecha->setHidden(false);
        return;
    }
    if( cita.next() ){
        ui->le_folioCita->setStyleSheet(estiloBueno);
        fecha.exec("SELECT CURDATE()");
        fecha.next();
        //Verificamos si es el dia de la consulta (La hora no se revisa porque puede pasar un paciente antes)
        if(fecha.value(0).toDate().toString("yyyy/MM/dd") == cita.value(2).toDate().toString("yyyy/MM/dd") && cita.value(8).toString() == "Pendiente"){
            //Si si es el dia
            //buscamos los datos del paciente de esa cita
            datosPac->exec("SELECT CONCAT(nombre,' ',appaterno,' ',apmaterno) as nomC, edad, sexo, religion,fotop FROM usuario WHERE matricula="+cita.value(1).toString());
            if( datosPac->next() ){
                ui->lb_noCoincideFecha->setHidden(true);

                //Llenamos el los datos del paciente
                ui->lb_nomPac->setText(datosPac->value(0).toString());
                ui->lb_edad->setText(datosPac->value(1).toString());
                ui->lb_sexoPac->setText(datosPac->value(2).toString());
                ui->lb_religion->setText(datosPac->value(3).toString());
                ui->te_sintomas->setText(cita.value(5).toString());
                pix.loadFromData(datosPac->value(4).toByteArray());
                ui->lb_imgPac->setPixmap(pix.scaled(ui->lb_imgPac->width(),ui->lb_imgPac->height(),Qt::KeepAspectRatio));
                //mostramos los menus
                ui->w_infoPacConsulta->setHidden(false);
                ui->sw_historialReceta->setCurrentIndex(0);
                ui->sw_historialReceta->setHidden(false);

                // ///////////////////// CARGAMOS LOS DATOS ACUTALES DEL HISTORIAL DEL PACIENTE /////////////////////

                QSqlQuery historialPac,idPac,defaultHistorial;
                idPac.exec("SELECT idpaciente FROM paciente WHERE idUser="+cita.value(1).toString());
                idPac.next();

                historialPac.exec("SELECT * FROM historialPaciente WHERE idPaciente="+idPac.value(0).toString());
                // Si no tiene historial, crear uno nuevo
                if(!historialPac.next()){
                    if(!defaultHistorial.exec("INSERT INTO historialPaciente(idPaciente) value ("+idPac.value(0).toString()+")"))
                        qDebug()<<defaultHistorial.lastError().text();
                        historialPac.exec("SELECT * FROM historialPaciente WHERE idPaciente="+idPac.value(0).toString());
                        historialPac.next();
                }
                // Cargar los datos a la ui
                    ui->le_estatura->setText(historialPac.value(1).toString());
                    ui->le_peso->setText(historialPac.value(2).toString());
                    ui->de_ultimaVacuna->setDate(historialPac.value(3).toDate());
                    ui->te_alergias->setText(historialPac.value(4).toString());
                    ui->te_accidentes->setText(historialPac.value(5).toString());
                    ui->te_enfermedades->setText(historialPac.value(6).toString());
                    ui->te_cirugias->setText(historialPac.value(7).toString());
                    ui->te_hospi->setText(historialPac.value(8).toString());
                    ui->te_trabajos->setText(historialPac.value(9).toString());
                    ui->te_habitos->setText(historialPac.value(10).toString());
                    ui->te_frecAlco->setText(historialPac.value(11).toString());
                    ui->te_frecFuma->setText(historialPac.value(12).toString());
                    ui->te_enfeFami->setText(historialPac.value(13).toString());

            }else qDebug()<<"Mala usuario"<<datosDoc->lastError().text();
        //Si no es el dia
        }else if(fecha.value(0).toDate().toString("yyyy/MM/dd") > cita.value(2).toDate().toString("yyyy/MM/dd")){
            ui->lb_noCoincideFecha->setHidden(false);
            ui->lb_noCoincideFecha->setText("Ya pasó la fecha de la cita");

            //Agregar un botón para borrarla ?????????????????????????
        }
        else if(cita.value(8).toString() != "Pendiente"){
            ui->lb_noCoincideFecha->setHidden(false);
            ui->lb_noCoincideFecha->setText("Cita concluida");
        }
        else{
            ui->lb_noCoincideFecha->setHidden(false);
            ui->lb_noCoincideFecha->setText("Aun no es la fecha de la cita");
        }
    }else {
        qDebug()<<"Mala cita"<<cita.lastError().text();
        ui->lb_noCoincideFecha->setText("No existe una cita con este folio.");
        ui->le_folioCita->setStyleSheet(estiloMalo);
        ui->lb_noCoincideFecha->setHidden(false);
    }
}

//Cuando termina de llenar el historial
void MainWindow::on_pb_llenarHistorial_clicked()
{
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";
    //Insertar los datos
    QSqlQuery llenarHist, cita,idPac;
    cita.exec("SELECT matricula FROM cita WHERE idCita="+ui->le_folioCita->text());
    cita.next();
    idPac.exec("SELECT idpaciente FROM paciente WHERE idUser="+cita.value(0).toString());
    idPac.next();
    //Ejecutmaos el update
    qDebug()<<idPac.value(0).toString();
    if( !llenarHist.exec("UPDATE historialPaciente SET "
                         " estatura="+ui->le_estatura->text()+
                         ", peso="+ui->le_peso->text()+
                         ", fechaUltimaVacuna='"+ui->de_ultimaVacuna->date().toString("yyyy-MM-dd")+
                         "', alergias='"+ui->te_alergias->toPlainText()+
                         "', accidente='"+ui->te_accidentes->toPlainText()+
                         "', enfermedad='"+ui->te_enfermedades->toPlainText()+
                         "', cirugias='"+ui->te_cirugias->toPlainText()+
                         "', hospitalizaciones='"+ui->te_hospi->toPlainText()+
                         "', trabajos='"+ui->te_trabajos->toPlainText()+
                         "', habitos='"+ui->te_habitos->toPlainText()+
                         "', frecuenciaAlcohol='"+ui->te_frecAlco->toPlainText()+
                         "', frecuenciaCigarro='"+ui->te_frecFuma->toPlainText()+
                         "', enfermedadesFamilia='"+ui->te_enfeFami->toPlainText()+
                         "' WHERE idPaciente="+idPac.value(0).toString() )){

        //Si no se puede hacer el update
        qDebug()<<llenarHist.lastError().text();
        QMessageBox::warning(this,"Error","No se pudieron guardar los datos, intente más tarde.");

    //si se ejecutó correctamente
    }else{
        //Nos movemos a la receta y diagnostico
        ui->sw_historialReceta->setCurrentIndex(1);
        //Desactivamos el boton para que no se pueda regresar al historial
        ui->pb_buscarCita->setEnabled(false);
    }
}
// ////////////////////////////// FIN : LLENAR EL HISTORIAL //////////////////////////////

// ////////////////////////////// LLENAR EL RECETA Y DIAGNOSTICO //////////////////////////////
void MainWindow::on_pb_receta_clicked()
{
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";

    //Validamos los datos
    if(!ui->te_diagnostico->toPlainText().isEmpty()){
        ui->te_diagnostico->setStyleSheet(estiloBueno);
        //si hay al menos una medicina ingresada
        if(medicinas.size() > 0){
            ui->pb_masMedicina->setStyleSheet(estiloBueno);
            ui->le_medicamento->setStyleSheet(estiloBueno);
            ui->le_porcion->setStyleSheet(estiloBueno);
            ui->le_frecuencia->setStyleSheet(estiloBueno);

            //Preguntar si esta seguro
            QMessageBox::StandardButton preg;
            if( medicinas.size() >1 )
                preg = QMessageBox::question(this,"Guardar datos","¿Esta seguro de guardar datos? Se guardarán "+QString::number(medicinas.size())+" medicamentos.");
            else preg = QMessageBox::question(this,"Guardar datos","¿Esta seguro de guardar datos? Se guardará "+QString::number(medicinas.size())+" medicamento.");
            if( preg == QMessageBox::No) return;

            //Hacemos los insert
            //receta
            QSqlQuery receta;
            QString value;
            int i;
            bool si=true;
            for(i=0; i<medicinas.size(); i++){
                value=ui->le_folioCita->text()+", '"+medicinas.at(i)+"','"
                        +porciones.at(i)+"', '"+frecMedicinas.at(i)+"'";
                if (!receta.exec("INSERT INTO receta() value("+value+")") ){
                    qDebug()<<receta.lastError().text();
                    si=false;
                }
            }
            //Diagnostico
            QSqlQuery diag;
            if( diag.exec("INSERT INTO diagnostico(idCita,diagnostico) value("+ui->le_folioCita->text()+",'"+ui->te_diagnostico->toPlainText()+"')") )
                qDebug()<<"diag insertado";

            //Marcamos como completada la cita
            QSqlQuery cita;
            if (cita.exec("UPDATE cita SET preparada='Completada' WHERE idCita="+ui->le_folioCita->text()) )
                qDebug()<<"Cita concluida";

            //Vaciamos las variables y movemos de página
            if(si){
                ui->le_folioCita->setText("");
                while(!medicinas.isEmpty()){
                    medicinas.removeLast();
                    porciones.removeLast();
                    frecMedicinas.removeLast();
                }
                ui->te_diagnostico->setText("");
                QMessageBox::information(this,"Correcto","Datos guardados correctamente.");
                on_pb_realizarConsulta_clicked();
                //Activamos el boton de buscar consultas
                ui->pb_buscarCita->setEnabled(true);
            }
            else{
                QMessageBox::warning(this,"Error","No se pudo ingresar su infomación, intente más tarde.");
            }

        }
        else{
            QMessageBox::warning(this,"Error","Por favor ingresa una medicina");
            ui->pb_masMedicina->setStyleSheet(estiloMalo);
            ui->le_medicamento->setStyleSheet(estiloMalo);
            ui->le_porcion->setStyleSheet(estiloMalo);
            ui->le_frecuencia->setStyleSheet(estiloMalo);
        }
    }
    else {
        ui->te_diagnostico->setStyleSheet(estiloMalo);
    }

}

//Agregar medicamentos
void MainWindow::on_pb_masMedicina_clicked()
{
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";
    QString medicina,porcion,frecuecia;
    //Validamos y asignamos los datos
    if(!ui->le_medicamento->text().isEmpty()){
        ui->le_medicamento->setStyleSheet(estiloBueno);
        if(!ui->le_porcion->text().isEmpty()){
            ui->le_porcion->setStyleSheet(estiloBueno);
            if(!ui->le_frecuencia->text().isEmpty()){
                ui->le_frecuencia->setStyleSheet(estiloBueno);
                //Si no estan vacíos los campos creamos objetos dinamicos para mostrarlos
                //Los guardamos en un layaout horizontal
                QHBoxLayout *hlay = new QHBoxLayout;
                hlay->setObjectName("medicinaNum"+QString::number(medicinas.size()));

                QLabel *l=new QLabel;
                l->setText(ui->le_medicamento->text()+": ");
                //l->setFixedSize(QSize(100,25));
                //l->setStyleSheet("border: 1px solid rgb(9,9,9)");
                hlay->addWidget(l,Qt::AlignLeft);

                l=new QLabel;
                l->setText(ui->le_porcion->text()+"grs,");
                //l->setFixedSize(QSize(100,25));
                //l->setStyleSheet("border: 1px solid rgb(9,9,9)");
                hlay->addWidget(l,Qt::AlignLeft);

                l=new QLabel;
                l->setText(ui->le_frecuencia->text());
                //l->setFixedSize(QSize(100,25));
                //l->setStyleSheet("border: 1px solid rgb(9,9,9)");
                hlay->addWidget(l,Qt::AlignLeft);

                QPushButton *pb = new QPushButton;
                pb->setText("Quitar");
                pb->setFixedSize(QSize(70,25));
                //b->setStyleSheet("background-color: rgb(138,222,242);border: 1px solid rgb(60,200,234)");
                QSignalMapper *mapper=new QSignalMapper(this);
                connect(pb,SIGNAL(clicked(bool)),mapper,SLOT(map()));
                mapper->setMapping(pb,contMedicinas);
                connect(mapper,SIGNAL(mapped(int)),this,SLOT(quitarMedicina(int)));
                hlay->addWidget(pb,Qt::AlignLeft);

                contMedicinas=medicinas.size();
                hlay->setAlignment(Qt::AlignLeft);
                hlay->setSpacing(0);
                ui->medicamentosLay->addLayout(hlay);

                //Guardamos los valores en los arreglos
                medicinas.append(ui->le_medicamento->text());
                porciones.append(ui->le_porcion->text());
                frecMedicinas.append(ui->le_frecuencia->text());

                //Vaciamos las variables del ui
                ui->le_medicamento->setText("");
                ui->le_porcion->setText("");
                ui->le_frecuencia->setText("");
                qDebug()<<"medicinas"<<medicinas.size();
            }
            else{
                ui->le_frecuencia->setStyleSheet(estiloMalo);
            }
        }
        else{
            ui->le_porcion->setStyleSheet(estiloMalo);
        }
    }
    else{
        ui->le_medicamento->setStyleSheet(estiloMalo);
    }
}

void MainWindow::quitarMedicina(int numMedicina){
    //El num de medicina nos dice la posicion en el arreglo
    medicinas.removeAt(numMedicina);
    porciones.removeAt(numMedicina);
    frecMedicinas.removeAt(numMedicina);
    QLayoutItem *child;
    child = ui->medicamentosLay->takeAt(numMedicina);
    clearLayout(child->layout());
    qDebug()<<"medicinas"<<medicinas.size();
}
// ////////////////////////////// FIN: LLENAR EL RECETA Y DIAGNOSTICO ////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////

// /////INICIO PARA PODER BUSCAR EL HISTORIAL DEL PACINETE (MEDICO: CONTIENE ALERGIAS Y DEMAS)//////
// ////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::on_pushButton_historial_clicked()
{
 ui->stackedWidget_perfilDoctor->setCurrentIndex(6);
 ui->lineEdit_matriculaMedico->clear();
 ui->sA_historialPac_2->hide();
 ui->label_mensajePaciente->hide();
 ui->label_espacioBlanco->hide();
 ui->verHistoClinico->hide();


}

void MainWindow::on_pushButton_buscarHistMedico_clicked()
{
    QString altura,peso,alergia,accidente,enfermedades,cirujias,hospital,trabajos,habitos,alcohol,cigarro,familia,consulta,idUsuario,idUsuario2;
    QSqlQuery query;
    idUsuario2="2019";
    idUsuario=ui->lineEdit_matriculaMedico->text();
    if(idUsuario==""){
        //qDebug()<<"entre 1 if";
            ui->label_espacioBlanco->show();
            ui->label_mensajePaciente->hide();
            ui->sA_historialPac_2->hide();

    }
        if(idUsuario!=""){
            //qDebug()<<"entre 2 if";
                consulta="select *from paciente where idUser='"+idUsuario+"'";
                query.exec(consulta);
                query.next();
                idUsuario2=query.value(1).toString();
                //qDebug()<<"USUARIO"<<idUsuario2;
                //qDebug()<<"USUARIO"<<idUsuario;
                ui->label_mensajePaciente->show();
                ui->label_espacioBlanco->hide();
                ui->sA_historialPac_2->hide();

   }
        if(idUsuario==idUsuario2){
           //  qDebug()<<"entre 3 if";
             ui->label_mensajePaciente->hide();
             ui->label_espacioBlanco->hide();
            consulta="select estatura,peso,fechaUltimaVacuna,alergias,accidente,enfermedad,cirugias,hospitalizaciones,trabajos,habitos,frecuenciaAlcohol,frecuenciaCigarro,enfermedadesFamilia "
                     " from historialPaciente inner join paciente on historialPaciente.idPaciente=paciente.idpaciente where paciente.idUser='"+idUsuario+"'";
            query.exec(consulta);
            query.next();
            altura=query.value(0).toString();
            peso=query.value(1).toString();
            alergia=query.value(3).toString();
            accidente=query.value(4).toString();
            enfermedades=query.value(5).toString();
            cirujias=query.value(6).toString();
            hospital=query.value(7).toString();
            trabajos=query.value(8).toString();
            habitos=query.value(9).toString();
            alcohol=query.value(10).toString();
            cigarro=query.value(11).toString();
            familia=query.value(12).toString();

            ui->lineEdit_estatura->setText(altura);
            ui->lineEdit_peso->setText(peso);
            ui->plainTextEdit_alergias->setPlainText(alergia);
            ui->plainTextEdit_accidentes->setPlainText(accidente);
            ui->plainTextEdit_enfermedades->setPlainText(enfermedades);
            ui->plainTextEdit_cirugias->setPlainText(cirujias);
            ui->plainTextEdit_hospitalizacion->setPlainText(hospital);
            ui->plainTextEdit_trabajos->setPlainText(trabajos);
            ui->plainTextEdit_habitos->setPlainText(habitos);
            ui->plainTextEdit_alcoholismo->setPlainText(alcohol);
            ui->plainTextEdit_cigarro->setPlainText(cigarro);
            ui->plainTextEdit_Familiares->setPlainText(familia);
            ui->verHistoClinico->show();
            ui->sA_historialPac_2->show();
        }
}

void MainWindow::on_verHistoClinico_clicked()
{
     ui->stackedWidget_perfilDoctor->setCurrentIndex(7);
    mostrarHistorialClinico();
}

void MainWindow::historialClinico(QString idCita){

    QString citas,fecha,cita,diagnostico,nombre;
    QSqlQuery consulta;
    citas=" select cit.idCita,cit.fecha,rec.medicamento,dig.diagnostico,us.nombre,us.appaterno,us.apmaterno "
          "from cita as cit inner join receta as rec on cit.idCita=rec.idCita inner join diagnostico as dig  on rec.idCita=dig.idCita "
          "inner join paciente as pac on pac.idUser=cit.matricula inner join usuario as us on us.matricula=pac.idUser where cit.idCita='"+idCita+"'";
    consulta.exec(citas);
    consulta.next();
    fecha=consulta.value(1).toString();
    cita=consulta.value(0).toString();
    diagnostico=consulta.value(3).toString();
    nombre=consulta.value(4).toString()+" "+consulta.value(5).toString()+" "+consulta.value(6).toString();
    static MostrarHistoclinico l;
    l.mostrarClinico(cita,diagnostico,nombre,fecha);
    l.show();
}

void MainWindow::mostrarHistorialClinico(){


    QString citas,est,id_usuario;
    QSqlQuery consulta;
    est="1";
    id_usuario=ui->lineEdit_matriculaMedico->text();
    citas="select cit.idCita,us.nombre,us.appaterno,us.apmaterno,cit.hora,cit.fecha from usuario as us inner join doctor as doc on us.matricula=doc.idUser inner join cita as cit on doc.iddoctor=cit.doctor where cit.matricula='"+id_usuario+"'";
    if(!consulta.exec(citas)) consulta.lastError().text();
    int f=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";

    QString folio,doctor,fecha,hora,nomDoct;
    int i=0;

    while(consulta.next())
    {
        folio=consulta.value(0).toString();
        doctor=consulta.value(1).toString()+" "+consulta.value(2).toString()+" "+consulta.value(3).toString();
        hora=consulta.value(4).toString();
        fecha=consulta.value(5).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }



        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setFixedSize(QSize(140,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->barraclinico->addWidget(m,i,1,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->barraclinico->addWidget(r,i,2,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->barraclinico->addWidget(h,i,3,Qt::AlignTop);


        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(120,25));
        ui->barraclinico->addWidget(ss,i,4,Qt::AlignTop);


        QPushButton *q=new QPushButton();
        q->setText("Ver diagnostico");
        q->setFixedSize(QSize(150,25));
        q->setStyleSheet("border:solid 1px #5d80b6;border-radius:5px;background-color: #5d80b6;color: white;font: 11pt 'MS Shell Dlg 2'");
        QSignalMapper *mapper1=new QSignalMapper(this);
        connect(q,SIGNAL(clicked(bool)),mapper1,SLOT(map()));
        mapper1->setMapping(q,folio);
        connect(mapper1,SIGNAL(mapped(QString)),this,SLOT(historialClinico(QString)));
        ui->barraclinico->addWidget(q,i,7,Qt::AlignTop);
        i++;

    }


}

// ////////////////////////////// FIN: LLENAR EL RECETA Y DIAGNOSTICO //////////////////////////////

void MainWindow::on_btnCitasCanceladas_clicked()
{
    clearLayout(ui->reagendas);
    qDebug() << "Yepo";
    QString citas;
    citas = "SELECT cita.idCita, cita.matricula, cita.fecha, cita.hora,CONCAT_WS(' ', usuario.nombre,usuario.appaterno,usuario.apmaterno) AS Doctor FROM cita INNER JOIN doctor on doctor.iddoctor = cita.doctor INNER JOIN usuario on doctor.idUser = usuario.matricula WHERE cita.estado = 1 AND cita.preparada = 'Cancelada';";
    QSqlQuery citasCanceladas;
    citasCanceladas.exec(citas);
    //qDebug() << citas;
    ui->stackedWidget_admin->setCurrentIndex(5);
    int fila = 1;

    while(citasCanceladas.next()){

        //datos del query
        QString folio = citasCanceladas.value(0).toString();
        QString matricula = citasCanceladas.value(1).toString();
        QString fecha = citasCanceladas.value(2).toString();
        QString hora = citasCanceladas.value(3).toString();
        QString doctor = citasCanceladas.value(4).toString();

        QPushButton* btnReagendar = new QPushButton();
        btnReagendar->setText("Reasignar Cita");
        btnReagendar->setStyleSheet("border:solid 1px #5d80b6;border-radius:5px;background-color: #5d80b6;color: white;font: 11pt 'MS Shell Dlg 2';");
        btnReagendar->setFixedSize(130, 30);

        QSignalMapper *mapper2=new QSignalMapper(this);
        connect(btnReagendar,SIGNAL(clicked(bool)),mapper2,SLOT(map()));
        mapper2->setMapping(btnReagendar,folio);
        connect(mapper2,SIGNAL(mapped(QString)),this,SLOT(cambiarVentana(QString)));


        QPushButton* btnEliminar = new QPushButton();
        btnEliminar->setText("Eliminar Cita");
        btnEliminar->setStyleSheet("border:solid 1px #ED6853;border-radius:5px;background-color: #ED6853;color: white;font: 11pt 'MS Shell Dlg 2';");
        btnEliminar->setFixedSize(130, 30);

        QSignalMapper *mapper1=new QSignalMapper(this);
        connect(btnEliminar,SIGNAL(clicked(bool)),mapper1,SLOT(map()));
        mapper1->setMapping(btnEliminar,folio);
        connect(mapper1,SIGNAL(mapped(QString)),this,SLOT(eliminarCita(QString)));


        QLabel *Paciente = new QLabel;
        Paciente->setAlignment(Qt::AlignCenter);
        Paciente->setText(matricula);
        Paciente->setFixedSize(90,25);

        QLabel *Fecha = new QLabel;
        Fecha->setAlignment(Qt::AlignCenter);
        Fecha->setText(fecha);
        Fecha->setFixedSize(110,25);

        QLabel *Hora = new QLabel;
        Hora->setAlignment(Qt::AlignCenter);
        Hora->setText(hora);
        Hora->setFixedSize(90,25);

        QLabel *Doctor = new QLabel;
        Doctor->setAlignment(Qt::AlignCenter);
        Doctor->setText(doctor);
        Doctor->setFixedSize(130,25);

        QLabel *vacio = new QLabel;
        vacio->setAlignment(Qt::AlignCenter);
        vacio->setText(" ");
        vacio->setFixedSize(60,25);



        ui->reagendas->addWidget(Paciente, fila, 1, Qt::AlignTop);
        ui->reagendas->addWidget(Fecha, fila, 2, Qt::AlignTop);
        ui->reagendas->addWidget(Hora, fila, 3, Qt::AlignTop);
        ui->reagendas->addWidget(Doctor, fila, 4, Qt::AlignTop);
        ui->reagendas->addWidget(vacio, fila, 5, Qt::AlignTop);
        ui->reagendas->addWidget(btnReagendar, fila, 6, Qt::AlignTop);
        ui->reagendas->addWidget(btnEliminar, fila, 7, Qt::AlignTop);

        fila++;
    }


}

void MainWindow::eliminarCita(QString folio)
{
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea eliminar la cita?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        QString borrarCitas;
        borrarCitas = "delete from cita where idCita ='"+folio+"'";
        QSqlQuery borrarCitasCanceladas;
        if (!borrarCitasCanceladas.exec(borrarCitas))
        qDebug()<<borrarCitasCanceladas.lastError().text();
        qDebug()<<folio;

        QMessageBox message2(QMessageBox::Question,
        tr("Information"), tr("La cita ha sido eliminada."), QMessageBox::Yes);
        message2.setButtonText(QMessageBox::Yes, tr("Aceptar"));

        on_btnCitasCanceladas_clicked();
    }
    else{
        message.close();
        qDebug()<<"else";
    }
}

void MainWindow::cambiarVentana(QString folio)
{
    idCita1=folio;
    ui->btnBuscarDoctor_2->hide();
    ui->stackedWidget_admin->setCurrentIndex(6);
    QString obtenerDatos;
    obtenerDatos = "select cita.matricula, cita.fecha, cita.hora, cita.sintomas from cita where idCita = '"+folio+"';";
    QSqlQuery obtenerCitas;
    obtenerCitas.exec(obtenerDatos);
    obtenerCitas.next();

    QString matricula = obtenerCitas.value(0).toString();
    QDate fecha = obtenerCitas.value(1).toDate();
    QString hora = obtenerCitas.value(2).toString();
    QString sintomas = obtenerCitas.value(3).toString();

    ui->fechaCita_2->setDate(fecha);
    ui->horaCita_2->addItem(hora);
    ui->sintomasCitas_2->setText(sintomas);

    ui->fechaCita_2->setEnabled(false);
    ui->horaCita_2->setEnabled(false);
    ui->sintomasCitas_2->setEnabled(false);
    docDisp(folio);

}

void MainWindow::on_btnCancelarAgenda_2_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(5);
    ui->le_nombreDoc_2->clear();
    on_btnCitasCanceladas_clicked();
}

void MainWindow::on_pushButton_menu_admin_3_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(0);
}

void MainWindow::docDisp(QString folio){
    qDebug()<<"entre a buscar doctores";
    QSqlQuery horarios, doc;
    QString hora,fecha;
    int diaNum, idDoc;
    diaNum = ui->fechaCita_2->date().dayOfWeek();
    hora = ui->horaCita_2->currentText();
    qDebug()<<hora;

    QString doc4,doc1,doc2;
    doc4="select doctor from cita where idCita='"+folio+"' ";
    qDebug()<<doc4;
    QSqlQuery doc3;
    doc3.exec(doc4);
    doc3.next();
    doc1=doc3.value(0).toString();

    QStringList dias;
    dias<< "Lunes" << "Martes" << "Miércoles" << "Jueves" << "Viernes" << "Sábado" << "Domingo";
    qDebug()<<dias.at(diaNum-1);
    horarios.exec("SELECT idDoc FROM horariodoc WHERE idDoc!='"+doc1+"' and  hora='"+hora+"' AND dia='"+dias.at(diaNum-1)+"'");
    if(!horarios.next()){
        ui->lb_noHayDocs_2->setHidden(false);
        ui->tv_listaDocCitas_2->setModel(nullptr);
        qDebug()<<"estoy vacio";
    }
    else{
        horarios.exec("SELECT idDoc FROM horariodoc WHERE idDoc!='"+doc1+"' and  hora='"+hora+"' AND dia='"+dias.at(diaNum-1)+"'");
        ui->lb_selDoc_2->setHidden(false);
        ui->lb_noHayDocs_2->setHidden(true);
        idDoc=horarios.value(0).toInt();
        doc2=horarios.value(0).toString();

        if(doc1==doc2)
        {
            qDebug()<<"son iguales";
            idDoc=0;
        }else{

        //Si movio la fecha pero aun no pone el nombre del doctor
        if(ui->le_nombreDoc_2->text().isEmpty()){
            QString nombre, apeM, apeP;

                model->setQuery("SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.idUser "
                              "FROM doctor as d , especialidad as e , usuario as u "
                              "WHERE d.iddoctor != "+doc1+" "
                              "AND d.idEspecialidad = e.idEsp "
                              "AND u.matricula = d.idUser ");
                qDebug()<<idDoc;
                ui->tv_listaDocCitas_2->setModel(model);
                ui->tv_listaDocCitas_2->setHidden(false);
                ui->tv_listaDocCitas_2->hideColumn(2);
                ui->tv_listaDocCitas_2->setColumnWidth(0,ui->tv_listaDocCitas_2->width()/2);
                ui->tv_listaDocCitas_2->setColumnWidth(1,ui->tv_listaDocCitas_2->width()/2);
            }
        }
    }
}

void MainWindow::on_tv_listaDocCitas_2_clicked(const QModelIndex &index)
{
        id_doctor = model->index(index.row(),2).data().toString();
}

void MainWindow::on_tv_listaDocCitas_2_doubleClicked(const QModelIndex &index)
{

        ui->le_nombreDoc_2->setText( model->index(index.row(),0).data().toString() );
}

void MainWindow::on_btnAgendarCita_2_clicked()
{
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea guardar los cambios?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        qDebug() << "aye";

        QString cita,doc1,doc2;
        QSqlQuery doc;
        doc1="select iddoctor from doctor where idUser='"+id_doctor+"'; ";
        doc.exec(doc1);
        doc.next();
        doc2=doc.value(0).toString();

        QString pend="Pendiente";
        cita="update cita set doctor='"+doc2+"', estado=0, preparada='"+pend+"' where idCita='"+idCita1+"'; ";
        qDebug()<<cita;
        QSqlQuery upd;
        upd.exec(cita);

        QMessageBox message(QMessageBox::Question,
        tr("Information"), tr("Los cambios se han guardado con éxito."), QMessageBox::Yes);
        message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        message.exec();

        ui->stackedWidget_admin->setCurrentIndex(0);
    }
    else{
        qDebug() << "nel pastel";
    }
}

void MainWindow::actTablaCitas()
{
    contador++;
    if(contador==180){
        ocultar->stop();

    }
    clearLayout(ui->citasLay_2);
    mostrarCitas();
    clearLayout(ui->pagoIntervenciones);
    on_pushButton_intervenciones_clicked();

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget_registros->setCurrentIndex(1);
}

void MainWindow::on_pb_ordenarI_clicked()
{
    OrdenarIntervencion* ventanaIntervencion = new OrdenarIntervencion(ui->le_folioCita->text(), this);
    ventanaIntervencion->show();
}

void MainWindow::on_btnEstudios_clicked()
{
    OrdenarEstudios* ventanaEstudios = new OrdenarEstudios(ui->le_folioCita->text(), this);
    ventanaEstudios->show();
}

void MainWindow::on_pb_inter_clicked()
{
    ui->tablaInterProgs->clear();
    ui->tablaInterProgs->setColumnWidth(1, 100);
    ui->tablaInterProgs->setColumnWidth(2, 60);
    ui->tablaInterProgs->setColumnWidth(3, 80);

    ui->stackedWidget_perfilDoctor->setCurrentIndex(8);
    QSqlQuery quirofanoCitas;
    QString idDoc = datosDoc->value(0).toString();
    quirofanoCitas.prepare("SELECT `fechaCita`, `idQuirofano`, `idDoctor`, CONCaT_WS(' ', usuario.nombre, usuario.appaterno, usuario.apmaterno) as Paciente, `horaInicio`, `descripcion` FROM `citasquirofano` inner JOIN paciente on paciente.idpaciente = citasquirofano.idPaciente INNER JOIN usuario on usuario.matricula = paciente.idUser WHERE idDoctor = '"+idDoc+"'");
    quirofanoCitas.exec();
    while(quirofanoCitas.next()){
        QString nomPaciente = quirofanoCitas.value(3).toString();
        QString fech = quirofanoCitas.value(0).toString();
        QString timeIni = quirofanoCitas.value(4).toString();
        QString idQuir = quirofanoCitas.value(1).toString();
        QString desc = quirofanoCitas.value(5).toString();

        QTableWidgetItem *paciente = new QTableWidgetItem(nomPaciente);
        paciente->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *fecha = new QTableWidgetItem(fech);
        fecha->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *horaIni = new QTableWidgetItem(timeIni);
        horaIni->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *quirofano = new QTableWidgetItem(idQuir);
        quirofano->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *descripcion = new QTableWidgetItem(desc);

        ui->tablaInterProgs->insertRow(ui->tablaInterProgs->rowCount());
        int fila = ui->tablaInterProgs->rowCount()-1;
        ui->tablaInterProgs->setItem(fila, 0, paciente);
        ui->tablaInterProgs->setItem(fila, 1, fecha);
        ui->tablaInterProgs->setItem(fila, 2, horaIni);
        ui->tablaInterProgs->setItem(fila, 3, quirofano);
        ui->tablaInterProgs->setItem(fila, 4, descripcion);
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(7);
    ui->Estancia->setCurrentIndex(0);
    verSoliEstancia();
}

void MainWindow::on_regresarAdmin_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(0);
    ui->Estancia->setCurrentIndex(0);
}


void MainWindow::PonerCuarto(QString num)
{
    ui->estCuarto->setText(num);
}

void MainWindow::CuartosDisponibles(QDate inicio,QDate fin)
{
    clearLayout(ui->cuartos);
        QString cuarto;
        QSqlQuery cuarto1;
        cuarto="select *from Cuarto";
        cuarto1.exec(cuarto);
        int r=0;
        int c=0;
        while(cuarto1.next())
        {
            QString reservado,idCuarto,numCuarto;
            idCuarto=cuarto1.value(0).toString();
            numCuarto=cuarto1.value(1).toString();

            QSqlQuery reservado1;
            reservado="select fecha_llega,fecha_salida from Estancia where idCuarto='"+idCuarto+"'; ";
            reservado1.exec(reservado);
            qDebug()<<reservado;
            bool libre=true;


            while(reservado1.next())
            {
                QDate in,fi;
                in=reservado1.value(0).toDate();
                fi=reservado1.value(1).toDate();

                if(inicio>in)
                {

                    if(inicio>fi)
                    {

                        libre=true;
                        break;
                    }
                    else
                    {

                        libre=false;

                        break;
                    }
                }
                else
                {

                 if(fin<in)
                 {

                    libre=true;
                    break;
                 }
                }

            }
            //aqui va
            if(libre==true)
            {
                QPushButton* cuartoo = new QPushButton();
                cuartoo->setText(numCuarto);
                cuartoo->setFixedSize(60, 40);

                QSignalMapper *mapper1=new QSignalMapper(this);
                connect(cuartoo,SIGNAL(clicked(bool)),mapper1,SLOT(map()));
                mapper1->setMapping(cuartoo,numCuarto);
                connect(mapper1,SIGNAL(mapped(QString)),this,SLOT(PonerCuarto(QString)));
                ui->cuartos->addWidget(cuartoo,r,c,1,1);
                c++;
                if(c==3)
                {
                    r++;
                    c=0;
                }
            }
        }
}

void MainWindow::on_pushButton_horarioDoc_2_clicked()
{
     ui->stackedWidget_PerfilStaff->setCurrentIndex(1);
}



void MainWindow::on_radioButton_clicked()
{
    clearLayout(ui->pagosConfirmados);
    QString citas,est,preparada;
    QSqlQuery consulta,consulta2;
    est="1";
    int f=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";
    QString folio,doctor,fecha,hora,nomDoct;
    int i=0;
    int l=0;
    preparada="Completada";
    citas="select cit.idCita,us.nombre,us.appaterno,us.apmaterno,cit.hora,cit.fecha from usuario as us inner join "
    "doctor as doc on us.matricula=doc.idUser inner join cita as cit on doc.iddoctor=cit.doctor where "
    "cit.estado='"+est+"' and cit.pagada=0 and cit.preparada='"+preparada+"' order by cit.fecha desc";
    if(!consulta2.exec(citas)) consulta2.lastError().text();
    while(consulta2.next()){
        folio=consulta2.value(0).toString();
        doctor=consulta2.value(1).toString()+" "+consulta2.value(2).toString()+" "+consulta2.value(3).toString();
        hora=consulta2.value(4).toString();
        fecha=consulta2.value(5).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        QLabel *fol=new QLabel;
        fol->setText(folio);
        fol->setFixedSize(QSize(100,25));
        fol->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(fol,l,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setFixedSize(QSize(110,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(m,l,1,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->pagosConfirmados->addWidget(r,l,2,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(h,l,3,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(40,25));
        ui->pagosConfirmados->addWidget(ss,l,4,Qt::AlignTop);

        QPushButton *p= new QPushButton();
        p->setText("Pagar");
        p->setFixedSize(QSize(100,25));
        p->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper3=new QSignalMapper(this);
        connect(p,SIGNAL(clicked(bool)),mapper3,SLOT(map()));
        mapper3->setMapping(p,folio);
        connect(mapper3,SIGNAL(mapped(QString)),this,SLOT(pagarCitasEfect(QString)));
        ui->pagosConfirmados->addWidget(p,l,6,Qt::AlignTop);

        l++;
    }
}

void MainWindow::pagarCitasEfect(QString folio)
{
    //parte para generar datos de pdf//
    qDebug()<<"folio"<<folio;

    QSqlQuery user,fecha,doc,usuarionoti;
    QString html,d,nombreUser,fechaPago,nombreDoc,fechaCita,usernoti;
    user.exec("select cit.idCita,CONCAT(' ', us.nombre,' ',us.appaterno,' ',us.apmaterno),cit.hora,cit.fecha from usuario as us inner join paciente as p on us.matricula=p.idUser inner join cita as cit on p.idUser=cit.matricula where cit.estado=1 and cit.pagada=0 and cit.preparada='Completada';");
    user.next();
    fecha.exec("select CURRENT_DATE()");
    fecha.next();
    doc.exec("select CONCAT(' ',us.nombre,' ',us.appaterno,' ',us.apmaterno)from usuario as us inner join doctor as doc on us.matricula=doc.idUser inner join cita as cit on doc.iddoctor=cit.doctor where cit.estado=1 and cit.pagada=0 and cit.preparada='Completada'");
    doc.next();

    nombreUser=user.value(1).toString();
    fechaPago=fecha.value(0).toString();
    nombreDoc=doc.value(0).toString();
    fechaCita=user.value(3).toString();
    usernoti=usuarionoti.value(0).toString();
    folio=user.value(0).toString();
    //termina parte de generar datos de pdf//

    //inicia parte para actualizar pago y enviar notificacion//
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea realizar el pago?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

    //querys para hacer actualizaciones de pago
    QSqlQuery update,insert,mandarNoti;
    QString cita,user1,descrip,total;
    descrip="Consulta";
    total="500";
    if(update.exec("UPDATE cita SET pagada=1 WHERE idCita='"+folio+"'"))
    {       
            update.next();
            qDebug()<<"pago actualizado";
             //inicia parte para generar pdf de pago"
             QMessageBox message(QMessageBox::Question,
             tr("Information"), tr("¿Desea generar comprobante de pago?"), QMessageBox::Yes | QMessageBox::No);
             message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
             message.setButtonText(QMessageBox::No, tr("Cancelar"));
             if (message.exec() == QMessageBox::Yes){

                 html=
                         "<H1 align=center> Comprobante de pago </H1>"
                         "<H6 align=center>*LOBOHOSPITAL* A.C DE C.V se deslinda de cualquier mal uso de este comprobante*</H6>"
                         "<H5 align=center>25 pte #1230, colonia Volcanes, Puebla,Pue</H5>"
                         "<br><br><br><br>"

                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> NOMBRE Y APELLIDO:"+nombreUser+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> MÉDICO RESPONSABLE:"+nombreDoc+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FECHA DE EMISIÓN DE PAGO:"+fechaPago+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FORMA DE PAGO:  EFECTIVO </div>"
                         "<br><br><br>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         "NO.FOLIO     FECHA DE CITA    DESCRIPCION      PRECIO UNITARIO        IMPORTE"
                         "</pre>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         ""+folio+"            "+fechaCita+"        "+descrip+"           "+total+".00                "+total+".00"
                          "<br><br><br><br><br><br><br><br><br><br><br><br><br>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         "                                                                SUBTOTAL:   "+total+".00 MXN"
                         "</pre>"
                         "<br><br>"
                         "<pre>"
                         "                                                                TOTAL:      "+total+".00 MXN"
                        ;
                 QTextDocument document;
                 document.setHtml(html);

                 QPrinter printer(QPrinter::HighResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setPaperSize(QPrinter::A4);
                  auto nombreArchivo=QFileDialog::getSaveFileName(this,"Guardar archivo",QDir::rootPath(),"Archivos (*.pdf);;");
                  if(nombreArchivo==""){

                                      }
                                      else{

                                          printer.setOutputFileName(nombreArchivo);
                                      }

                  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
                                      document.print(&printer);

             }
             else
             {

             }

             //termina parte para generar pdf//

        }else
        {
                qDebug()<<"Error al actualizar el pago"<<update.lastError().text();
        }
              }else
               {
            }
}

void MainWindow::on_radioButton_4_clicked()
{
    clearLayout(ui->pagosConfirmados);
    QString citas,est,preparada;
    QSqlQuery consulta,consulta2;
    est="1";
    int f=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";
    QString folio,doctor,fecha,hora,nomDoct,nombrePac;
    int i=0;
    int l=0;
    citas="select ur.idEmergencia,us.nombre,us.appaterno,us.apmaterno,ur.nombre_pacinete,ur.hora,ur.fecha from usuario as "
          "us inner join doctor as doc on us.matricula=doc.idUser inner join urgencias as ur on doc.iddoctor=ur.idDoctor";
    if(!consulta2.exec(citas)) consulta2.lastError().text();
    while(consulta2.next()){
        folio=consulta2.value(0).toString();
        doctor=consulta2.value(1).toString()+" "+consulta2.value(2).toString()+" "+consulta2.value(3).toString();
        nombrePac=consulta2.value(4).toString();
        hora=consulta2.value(5).toString();
        fecha=consulta2.value(6).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        QLabel *fol=new QLabel;
        fol->setText(folio);
        fol->setFixedSize(QSize(100,25));
        fol->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(fol,l,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setFixedSize(QSize(115,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(m,l,1,Qt::AlignTop);

        QLabel *np=new QLabel;
        np->setText(nombrePac);
        np->setFixedSize(QSize(140,25));
        np->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(np,l,2,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->pagosConfirmados->addWidget(r,l,3,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(h,l,4,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(40,25));
        ui->pagosConfirmados->addWidget(ss,l,5,Qt::AlignTop);

        QPushButton *p= new QPushButton();
        p->setText("Pagar");
        p->setFixedSize(QSize(100,25));
        p->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper3=new QSignalMapper(this);
        connect(p,SIGNAL(clicked(bool)),mapper3,SLOT(map()));
        mapper3->setMapping(p,folio);
        connect(mapper3,SIGNAL(mapped(QString)),this,SLOT(pagarUrgenciasV(QString)));
        ui->pagosConfirmados->addWidget(p,l,7,Qt::AlignTop);

        l++;
    }
}

void MainWindow::pagarUrgenciasV(QString folio)
{
    //parte para generar datos de pdf//
    QSqlQuery user,fecha,doc,usuarionoti,fol,datosacomp;
    QString html,d,nombrePac,fechaPago,nombreDoc,fechaCita,usernoti,id_doc,horaEmer,descripcion,total,nombreacomp,telacomp,
            parentezcoacomp,direccionacomp;
    user.exec("select ur.idEmergencia,ur.idDoctor,us.nombre,us.appaterno,us.apmaterno,ur.nombre_pacinete,ur.hora,ur.fecha,ur.Causas from usuario as "
              "us inner join doctor as doc on us.matricula=doc.idUser inner join urgencias as ur on doc.iddoctor=ur.idDoctor;");
    user.next();
    fecha.exec("select CURRENT_DATE()");
    fecha.next();
    doc.exec("select CONCAT(' ',us.nombre,' ',us.appaterno,' ',us.apmaterno)from usuario as us inner join doctor as doc "
             "on us.matricula=doc.idUser inner join urgencias as ur on doc.iddoctor=ur.idDoctor ;");
    doc.next();
    datosacomp.exec("select nombre,telefono,parentescos,direcion from acompanante where idEmergencia="+folio+"");
    datosacomp.next();


    nombrePac=user.value(5).toString();
    fechaPago=fecha.value(0).toString();
    nombreDoc=doc.value(0).toString();
    fechaCita=user.value(7).toString();
    folio=user.value(0).toString();
    id_doc=user.value(1).toString();
    horaEmer=user.value(6).toString();
    descripcion=user.value(8).toString();
    total="15320";
    nombreacomp=datosacomp.value(0).toString();
    telacomp=datosacomp.value(1).toString();
    parentezcoacomp=datosacomp.value(2).toString();
    direccionacomp=datosacomp.value(3).toString();
    //termina parte de generar datos de pdf//

    //inicia parte para actualizar pago y enviar notificacion//
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea realizar el pago de la urgencia?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

    //querys para hacer actualizaciones de pago
    QSqlQuery update,insert,mandarNoti;
    QString mensaj,tipo,cita,user1,notificacion;

    if(insert.exec("insert into pagoUrgencia(fecha,hora,total,nombrePac,idEmergencia) "
                   "value('"+fechaPago+"','"+horaEmer+"',"+total+",'"+nombrePac+"',"+folio+")"))
    {
            insert.next();
            qDebug()<<folio;
            qDebug()<<"pago efectuado";


             //inicia parte para generar pdf de pago"
             QMessageBox message(QMessageBox::Question,
             tr("Information"), tr("¿Desea generar comprobante de pago?"), QMessageBox::Yes | QMessageBox::No);
             message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
             message.setButtonText(QMessageBox::No, tr("Cancelar"));
             if (message.exec() == QMessageBox::Yes){

                 html=
                         "<H1 align=center> Comprobante de pago </H1>"
                         "<H6 align=center>*LOBOHOSPITAL* A.C DE C.V se deslinda de cualquier mal uso de este comprobante*</H6>"
                         "<H5 align=center>25 pte #1230, colonia Volcanes, Puebla,Pue</H5>"
                         "<br><br><br><br>"

                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> NOMBRE Y APELLIDO DE PACIENTE INGRESADO :  "+nombrePac+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> NOMBRE Y APELLIDO DE ACOMPAÑANTE DEL PACIENTE :  "+nombreacomp+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> NO. DE TELEFONO : "+telacomp+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> PARENTEZCO :  '"+parentezcoacomp+"' </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> DIRECCIÓN DOMICILIARIA : '"+direccionacomp+"' </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> MÉDICO RESPONSABLE : "+nombreDoc+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FECHA DE EMISIÓN DE PAGO : "+fechaPago+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FORMA DE PAGO :  EFECTIVO </div>"
                         "<br><br><br>"
                         "<hr style=color: #0056b2; />"
                        "<pre>"
                        "NO.FOLIO     FECHA DE HOSPITALIZACIÓN    DESCRIPCION      PRECIO UNITARIO        IMPORTE"
                        "</pre>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                        ""+folio+"                "+fechaCita+"             "+descripcion+"           "+total+"               "+total+" "
                         "</pre>"
                           " <br><br><br><br><br><br><br><br><br><br><br><br><br>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         "                                                                           SUBTOTAL: "+total+""
                         "</pre>"
                          "<pre>"
                         "                                                                           TOTAL:   "+total+"";
                 QTextDocument document;
                 document.setHtml(html);

                 QPrinter printer(QPrinter::HighResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setPaperSize(QPrinter::A4);
                  auto nombreArchivo=QFileDialog::getSaveFileName(this,"Guardar archivo",QDir::rootPath(),"Archivos (*.pdf);;");
                  if(nombreArchivo==""){

                                      }
                                      else{

                                          printer.setOutputFileName(nombreArchivo);
                                      }

                  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
                                      document.print(&printer);

             }
    }
    }
}


void MainWindow::AsginarCuartos(QString idSoli1)
{
    idSolicitudEstancia=idSoli1;
    QString folio,idDoc,idPa,fechaInter,idSeQ,SoliEst,dias,nombreD,nombreP,idD1,idP1;
    QDate fechai,fechaf;
    int fechaff;
    SoliEst="select * from SoliEstancia where idSoli='"+idSoli1+"';";
    QSqlQuery soli,idD2,idP2,nombreD1,nombreP1;
    soli.exec(SoliEst);
    soli.next();
    idDoc=soli.value(1).toString();
    idPa=soli.value(2).toString();
    idSoliQuirofano=soli.value(6).toString();

    idD1="select idUser from doctor where iddoctor='"+idDoc+"'; ";
    idP1="select idUSer from paciente where idpaciente='"+idPa+"'; ";
    idD2.exec(idD1);
    idD2.next();
    idP2.exec(idP1);
    idP2.next();

    nombreD="select nombre, appaterno,apmaterno from usuario where matricula='"+idD2.value(0).toString()+"'; ";
    nombreP="select nombre, appaterno,apmaterno from usuario where matricula='"+idP2.value(0).toString()+"'; ";
    nombreD1.exec(nombreD);
    nombreP1.exec(nombreP);
    nombreD1.next();
    nombreP1.next();

    QString nombreD2,nombreP2;
    nombreD2=nombreD1.value(0).toString()+" "+nombreD1.value(1).toString()+" "+nombreD1.value(2).toString();
    nombreP2=nombreP1.value(0).toString()+" "+nombreP1.value(1).toString()+" "+nombreP1.value(2).toString();

    ui->estDoctor->setText(nombreD2);
    ui->estPaciente->setText(nombreP2);

        ui->estCuarto->clear();

    QString fecha1,fecha2;

    fechai=soli.value(4).toDate();
    fecha1=soli.value(4).toString();
    dias=soli.value(5).toString();
    fechaff=soli.value(5).toInt();

    ui->estFechasoli->setText(fecha1);
    ui->estDias->setText(dias);
    fechaf=fechai.addDays(fechaff);
    int years,months,days;
    years=fechaf.year();
    months=fechaf.month();
    days=fechaf.day();
    fecha2=QString::number(years)+"-"+QString::number(months)+"-"+QString::number(days);
    ui->estFechaFin->setText(fecha2);
    ui->Estancia->setCurrentIndex(1);
    CuartosDisponibles(fechai,fechaf);



}


void MainWindow::verSoliEstancia()
{
    clearLayout(ui->estanciaPa);
    QString folio,idDoc,idPa,fechaInter,idSeQ,SoliEst;
    SoliEst="select se.idSoli,se.idDoctor,se.idPaciente,se.fechaIntervencion,idCitaQ from CitasQuirofano as cq inner join SoliEstancia as se on cq.idCita=se.idCitaQ where cq.estado='Pendiente'";
    QSqlQuery soli;
    soli.exec(SoliEst);
    int r=0;
    int c=0;

    QLabel *folio1 = new QLabel;
    folio1->setAlignment(Qt::AlignCenter);
    folio1->setText("Folio de Solicitud");
    folio1->setFixedSize(130,25);
   ui->estanciaPa->addWidget(folio1,r,0,1,1);


    QLabel *iddoc1 = new QLabel;
    iddoc1->setAlignment(Qt::AlignCenter);
    iddoc1->setText("ID Doctor");
    iddoc1->setFixedSize(80,25);
    ui->estanciaPa->addWidget(iddoc1,r,1,1,1);

    QLabel *idpa1 = new QLabel;
    idpa1->setAlignment(Qt::AlignCenter);
    idpa1->setText("ID Paciente");
    idpa1->setFixedSize(80,25);
   ui->estanciaPa->addWidget(idpa1,r,2,1,1);

    QLabel *fecha1 = new QLabel;
    fecha1->setAlignment(Qt::AlignCenter);
    fecha1->setText("Fecha Solicitada");
    fecha1->setFixedSize(130,25);
    ui->estanciaPa->addWidget(fecha1,r,3,1,1);

    QLabel *idQ = new QLabel;
    idQ->setAlignment(Qt::AlignCenter);
    idQ->setText("ID Cita Quirofano");
    idQ->setFixedSize(150,25);
    ui->estanciaPa->addWidget(idQ,r,4,1,1);

    while(soli.next())
    {
        r++;
        folio=soli.value(0).toString();
        idDoc=soli.value(1).toString();
        idPa=soli.value(2).toString();
        fechaInter=soli.value(3).toString();
        idSeQ=soli.value(4).toString();

        QLabel *folio1 = new QLabel;
        folio1->setAlignment(Qt::AlignCenter);
        folio1->setText(folio);
        folio1->setFixedSize(130,25);
       ui->estanciaPa->addWidget(folio1,r,0,1,1);


        QLabel *iddoc1 = new QLabel;
        iddoc1->setAlignment(Qt::AlignCenter);
        iddoc1->setText(idDoc);
        iddoc1->setFixedSize(80,25);
        ui->estanciaPa->addWidget(iddoc1,r,1,1,1);

        QLabel *idpa1 = new QLabel;
        idpa1->setAlignment(Qt::AlignCenter);
        idpa1->setText(idPa);
        idpa1->setFixedSize(80,25);
       ui->estanciaPa->addWidget(idpa1,r,2,1,1);

        QLabel *fecha1 = new QLabel;
        fecha1->setAlignment(Qt::AlignCenter);
        fecha1->setText(fechaInter);
        fecha1->setFixedSize(130,25);
        ui->estanciaPa->addWidget(fecha1,r,3,1,1);

        QLabel *idQ = new QLabel;
        idQ->setAlignment(Qt::AlignCenter);
        idQ->setText(idSeQ);
        idQ->setFixedSize(150,25);
        ui->estanciaPa->addWidget(idQ,r,4,1,1);

        QPushButton* Asignar = new QPushButton();
        Asignar->setText("Asignar Cuarto");
        Asignar->setStyleSheet("border:solid 1px #5d80b6;border-radius:5px;background-color: #5d80b6;color: white;font: 11pt 'MS Shell Dlg 2';");
        Asignar->setFixedSize(130, 30);
        ui->estanciaPa->addWidget(Asignar,r,5,1,1);

        QSignalMapper *mapper2=new QSignalMapper(this);
        connect(Asignar,SIGNAL(clicked(bool)),mapper2,SLOT(map()));
        mapper2->setMapping(Asignar,folio);
        connect(mapper2,SIGNAL(mapped(QString)),this,SLOT(AsginarCuartos(QString)));



    }

    QLabel *nombre1=new QLabel();

    ui->estanciaPa->addWidget(nombre1,r,c,1,1);

    QLabel *nombre2=new QLabel();
    r=r+1;
    ui->estanciaPa->addWidget(nombre2,r,c,1,1);

    QLabel *nombr3=new QLabel();
     r=r+1;
    ui->estanciaPa->addWidget(nombr3,r,c,1,1);

    QLabel *nombr4=new QLabel();
     r=r+1;
    ui->estanciaPa->addWidget(nombr4,r,c,1,1);

    QLabel *nombre5=new QLabel();
    r=r+1;
    ui->estanciaPa->addWidget(nombre5,r,c,1,1);


}

void MainWindow::on_agregarCuarto_clicked()
{
    QMessageBox informacion;
    QString cuarto,numero,idCuarto;
    numero=ui->estCuarto->text();
    QSqlQuery cuarto1;
    cuarto="select *from Cuarto where NumCuarto='"+numero+"'; ";
    cuarto1.exec(cuarto);
    cuarto1.next();
    idCuarto=cuarto1.value(0).toString();

    if(ui->estCuarto->text()=="")
    {
        informacion.setWindowTitle("Informacion");
        informacion.setText ("Seleccione un Cuarto");
        informacion.setStandardButtons( QMessageBox::Ok) ;
        informacion.setDefaultButton (QMessageBox ::Ok ) ;
        informacion.setButtonText( QMessageBox::Ok,"Aceptar");
        informacion.exec();
    }
    else
    {




    QString estanciaN;
    QSqlQuery estanciaN1;
    estanciaN="insert into Estancia(idCitaQ,NDias,fecha_llega,fecha_salida,idCuarto) values('"+idSoliQuirofano+"','"+ui->estDias->text()+"','"+ui->estFechasoli->text()+"','"+ui->estFechaFin->text()+"','"+idCuarto+"');";
    estanciaN1.exec(estanciaN);

    QString del;
    QSqlQuery del1;
    del="delete from SoliEstancia where idSoli='"+idSolicitudEstancia+"'; ";
    del1.exec(del);
    QSqlQuery query2;
    query2.exec("update CitasQuirofano as cq inner join Estancia as e on cq.idCita=e.idCitaQ set estado='Completa' where cq.idCita='"+idSoliQuirofano+"'");
    query2.next();
    verSoliEstancia();
    informacion.setWindowTitle("Informacion");
    informacion.setText ("EL proceso se ejecuto correctamente");
    informacion.setStandardButtons( QMessageBox::Ok) ;
    informacion.setDefaultButton (QMessageBox ::Ok ) ;
    informacion.setButtonText( QMessageBox::Ok,"Aceptar");
    informacion.exec();

    ui->Estancia->setCurrentIndex(0);

    }


}
void MainWindow::on_pushButton_guardaUrgencia_clicked()
{
    QString nombre,llegada,color,causas,signos,nombreAcom,telefono,direccion,parentescos,hora,idEspecialidad;
    //saber la fecha
    QDate fecha=QDate::currentDate();
    QString fecha_text=fecha.toString("yyyy-MM-dd");
    // ----------------------------------------------
    //saber la hora
    QTime time=QTime::currentTime();
    QString time_text=time.toString("hh:mm:s ap");

    if((time.second() % 3)==0){
    time_text[6]=' ';
    time_text[6]=' ';
    }
    qDebug()<<"fecha"<<fecha_text;
    qDebug()<<"hora"<<time_text;
    // ---------------------------------------------
    // mensaje
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Seguro de guardar los datos?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
   // -------------------------------------------------------------------------
   // mensaje de faltan campos
    QMessageBox messageBox(QMessageBox::Warning,
                                     tr("Warning"), tr("Por favor,ingrese los datos necesarios."), QMessageBox::Yes);
             messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
   // -------------------------------------------------------------------------
    //mensaje de informacion guardada
    QMessageBox mensaje(QMessageBox::Question,
     tr("Information"), tr("Datos guardadas"), QMessageBox::Yes);
    mensaje.setButtonText(QMessageBox::Yes, tr("Aceptar"));

  // -----------------------------------------------------------------------------------
   // expreciones regulares
     QRegExp re("^[a-zZ-A]*$"), re2("^[0-9]*$");
  // ------------------------------------------------------------------------------------
    nombre=ui->lineEdit_nombreEmergencia->text();
    llegada=ui->lineEdit_formaLLegada->text();
    causas=ui->plainTextEdit_causas->toPlainText();
    signos=ui->plainTextEdit_vitales->toPlainText();
    nombreAcom=ui->lineEdit_nombreAcom->text();
    telefono=ui->lineEdit_telefonoAcomp->text();
    direccion=ui->lineEdit_dirrecion->text();
    parentescos=ui->lineEdit_parentesco->text();

    QSqlQuery registroEmergencia,query;
    if(ui->lineEdit_nombreEmergencia->text()=="" || ui->lineEdit_formaLLegada->text()=="" || ui->plainTextEdit_causas->toPlainText()=="" || ui->plainTextEdit_vitales->toPlainText()=="" || ui->lineEdit_nombreAcom->text()=="" || ui->lineEdit_telefonoAcomp->text()=="" || ui->lineEdit_dirrecion->text()=="" || ui->lineEdit_parentesco->text()=="" ){

        if (messageBox.exec() == QMessageBox::Yes){

         }

     }else {

    if(ui->radioButton_azul->isChecked() ){
        if (message.exec() == QMessageBox::Yes ){
      qDebug()<<"entre azul";
      color="blue";
    registroEmergencia.exec("insert into urgencias(idDoctor,forma_llegada,Causas,colorEmergencia,signos_vitales,nombre_pacinete,hora,fecha)"
                            " values('"+id_doctor+"','"+llegada+"','"+causas+"','"+color+"','"+signos+"','"+nombre+"','"+time_text+"','"+fecha_text+"')");
    registroEmergencia.next();

     query.exec("SELECT MAX(idEmergencia)FROM urgencias");
     query.next();
     idEspecialidad=query.value(0).toString();
     qDebug()<<"id especialidad"<<idEspecialidad;

    registroEmergencia.exec("insert into acompanante(nombre,telefono,direcion,parentescos,idEmergencia)"
                            "values('"+nombreAcom+"','"+telefono+"','"+direccion+"','"+parentescos+"','"+idEspecialidad+"')");
    registroEmergencia.next();
     ui->lineEdit_nombreEmergencia->clear();ui->lineEdit_formaLLegada->clear();ui->plainTextEdit_causas->toPlainText();ui->plainTextEdit_vitales->clear();
     ui->lineEdit_nombreAcom->clear();ui->lineEdit_telefonoAcomp->clear();ui->lineEdit_dirrecion->clear();ui->lineEdit_parentesco->clear();
     ui->plainTextEdit_causas->clear();
     ui->radioButton_azul->setCheckable(false);
     ui->radioButton_azul->setCheckable(true);

     if (mensaje.exec() == QMessageBox::Yes){

      }
    return;
    }
        }if(ui->radioButton_verde->isChecked()){
        if (message.exec() == QMessageBox::Yes){
        qDebug()<<"entre verde";
         color="green";
        registroEmergencia.exec("insert into urgencias(idDoctor,forma_llegada,Causas,colorEmergencia,signos_vitales,nombre_pacinete,hora,fecha)"
                                "values('"+id_doctor+"','"+llegada+"','"+causas+"','"+color+"','"+signos+"','"+nombre+"','"+time_text+"','"+fecha_text+"')");
        registroEmergencia.next();

         query.exec("SELECT MAX(idEmergencia)FROM urgencias");
         query.next();
         idEspecialidad=query.value(0).toString();
         qDebug()<<"id especialidad"<<idEspecialidad;

        registroEmergencia.exec("insert into acompanante(nombre,telefono,direcion,parentescos,idEmergencia)"
                                "values('"+nombreAcom+"','"+telefono+"','"+direccion+"','"+parentescos+"','"+idEspecialidad+"')");
        registroEmergencia.next();
        ui->lineEdit_nombreEmergencia->clear();ui->lineEdit_formaLLegada->clear();ui->plainTextEdit_causas->toPlainText();ui->plainTextEdit_vitales->clear();
        ui->lineEdit_nombreAcom->clear();ui->lineEdit_telefonoAcomp->clear();ui->lineEdit_dirrecion->clear();ui->lineEdit_parentesco->clear();
        ui->radioButton_verde->setCheckable(false);
        ui->plainTextEdit_causas->clear();
        ui->radioButton_verde->setCheckable(true);
        if (mensaje.exec() == QMessageBox::Yes){

         }
    return;
    }
    }if(ui->radioButton_amarillo->isChecked()){
        if (message.exec() == QMessageBox::Yes){
        qDebug()<<"entre amarillo";
         color="yellow";
        registroEmergencia.exec("insert into urgencias(idDoctor,forma_llegada,Causas,colorEmergencia,signos_vitales,nombre_pacinete,hora,fecha)"
                                "values('"+id_doctor+"','"+llegada+"','"+causas+"','"+color+"','"+signos+"','"+nombre+"','"+time_text+"','"+fecha_text+"')");
        registroEmergencia.next();

         query.exec("SELECT MAX(idEmergencia)FROM urgencias");
         query.next();
         idEspecialidad=query.value(0).toString();
         qDebug()<<"id especialidad"<<idEspecialidad;

        registroEmergencia.exec("insert into acompanante(nombre,telefono,direcion,parentescos,idEmergencia)"
                                "values('"+nombreAcom+"','"+telefono+"','"+direccion+"','"+parentescos+"','"+idEspecialidad+"')");
        registroEmergencia.next();
        ui->lineEdit_nombreEmergencia->clear();ui->lineEdit_formaLLegada->clear();ui->plainTextEdit_causas->toPlainText();ui->plainTextEdit_vitales->clear();
        ui->lineEdit_nombreAcom->clear();ui->lineEdit_telefonoAcomp->clear();ui->lineEdit_dirrecion->clear();ui->lineEdit_parentesco->clear();
        ui->radioButton_amarillo->setCheckable(false);
           ui->plainTextEdit_causas->clear();
        ui->radioButton_amarillo->setCheckable(true);
        if (mensaje.exec() == QMessageBox::Yes){

         }
        return;
    }
        }if(ui->radioButton_naranja->isChecked()){
        if (message.exec() == QMessageBox::Yes){
        qDebug()<<"entre naranja";
         color="orange";
        registroEmergencia.exec("insert into urgencias(idDoctor,forma_llegada,Causas,colorEmergencia,signos_vitales,nombre_pacinete,hora,fecha)"
                                "values('"+id_doctor+"','"+llegada+"','"+causas+"','"+color+"','"+signos+"','"+nombre+"','"+time_text+"','"+fecha_text+"')");
        registroEmergencia.next();

         query.exec("SELECT MAX(idEmergencia)FROM urgencias");
         query.next();
         idEspecialidad=query.value(0).toString();
         qDebug()<<"id especialidad"<<idEspecialidad;

        registroEmergencia.exec("insert into acompanante(nombre,telefono,direcion,parentescos,idEmergencia)"
                                "values('"+nombreAcom+"','"+telefono+"','"+direccion+"','"+parentescos+"','"+idEspecialidad+"')");
        registroEmergencia.next();
        ui->lineEdit_nombreEmergencia->clear();ui->lineEdit_formaLLegada->clear();ui->plainTextEdit_causas->toPlainText();ui->plainTextEdit_vitales->clear();
        ui->lineEdit_nombreAcom->clear();ui->lineEdit_telefonoAcomp->clear();ui->lineEdit_dirrecion->clear();ui->lineEdit_parentesco->clear();

           ui->plainTextEdit_causas->clear();ui->radioButton_naranja->setCheckable(false);
        ui->radioButton_naranja->setCheckable(true);
        if (mensaje.exec() == QMessageBox::Yes){

         }
        return;
    }
     }if(ui->radioButton_rojo->isChecked()){
        if (message.exec() == QMessageBox::Yes){
        qDebug()<<"entre rojo";
         color="red";
        registroEmergencia.exec("insert into urgencias(idDoctor,forma_llegada,Causas,colorEmergencia,signos_vitales,nombre_pacinete,hora,fecha)"
                                "values('"+id_doctor+"','"+llegada+"','"+causas+"','"+color+"','"+signos+"','"+nombre+"','"+time_text+"','"+fecha_text+"')");
        registroEmergencia.next();

         query.exec("SELECT MAX(idEmergencia)FROM urgencias");
         query.next();
         idEspecialidad=query.value(0).toString();
         qDebug()<<"id especialidad"<<idEspecialidad;

        registroEmergencia.exec("insert into acompanante(nombre,telefono,direcion,parentescos,idEmergencia)"
                                "values('"+nombreAcom+"','"+telefono+"','"+direccion+"','"+parentescos+"','"+idEspecialidad+"')");
        registroEmergencia.next();
        ui->lineEdit_nombreEmergencia->clear();ui->lineEdit_formaLLegada->clear();ui->plainTextEdit_causas->toPlainText();ui->plainTextEdit_vitales->clear();
        ui->lineEdit_nombreAcom->clear();ui->lineEdit_telefonoAcomp->clear();ui->lineEdit_dirrecion->clear();ui->lineEdit_parentesco->clear();
          ui->plainTextEdit_causas->clear();
        ui->radioButton_rojo->setCheckable(false);
        ui->radioButton_rojo->setCheckable(true);
        if (mensaje.exec() == QMessageBox::Yes){

                 }
        return;
             }
        }

    }
}

void MainWindow::on_pb_urg_clicked()
{
    ui->stackedWidget_perfilDoctor->setCurrentIndex(9);
}
void MainWindow::on_pb_addUrgencia_clicked()
{
    agregarUrgencia *urg= new agregarUrgencia();
    urg->show();
}

void MainWindow::on_radioButton_2_clicked()
{
    clearLayout(ui->pagosConfirmados);
    QString citas,est,pacNom;
    QSqlQuery consulta,consulta2,pac;
    est="1";
    int f=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";
    QString folio,doctor,fecha,hora,nomDoct,descripc;
    int i=0;
    int l=0;
    citas="select inter.idCita,inter.idDoctor,inter.idPaciente,inter.horaInicio,inter.fechaCita,inter.descripcion,CONCAT(us.nombre,' ',us.appaterno,' ',us.apmaterno) from usuario as "
          "us inner join doctor as doc on us.matricula=doc.idUser inner join citasQuirofano as inter on doc.iddoctor=inter.idDoctor";

    if(!consulta2.exec(citas)) consulta2.lastError().text();
    while(consulta2.next()){

        folio=consulta2.value(0).toString();
        doctor=consulta2.value(6).toString();
        descripc=consulta2.value(5).toString();
        hora=consulta2.value(3).toString();
        fecha=consulta2.value(4).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        QLabel *fol=new QLabel;
        fol->setText(folio);
        fol->setFixedSize(QSize(100,25));
        fol->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(fol,l,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setFixedSize(QSize(115,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(m,l,1,Qt::AlignTop);

        QLabel *np=new QLabel;
        np->setText(descripc);
        np->setFixedSize(QSize(140,25));
        np->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(np,l,2,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(100,25));
        ui->pagosConfirmados->addWidget(r,l,3,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setFixedSize(QSize(100,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagosConfirmados->addWidget(h,l,4,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        ss->setFixedSize(QSize(40,25));
        ui->pagosConfirmados->addWidget(ss,l,5,Qt::AlignTop);

        QPushButton *p= new QPushButton();
        p->setText("Pagar");
        p->setFixedSize(QSize(100,25));
        p->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper3=new QSignalMapper(this);
        connect(p,SIGNAL(clicked(bool)),mapper3,SLOT(map()));
        mapper3->setMapping(p,folio);
        connect(mapper3,SIGNAL(mapped(QString)),this,SLOT(pagarIntervencion(QString)));
        ui->pagosConfirmados->addWidget(p,l,7,Qt::AlignTop);

        l++;
    }
}

void MainWindow::pagarIntervencion(QString folio)
{
    //parte para generar datos de pdf//
    QSqlQuery user,fecha,doc,usuarionoti,fol,datosacomp,paciente,pago;
    QString html,d,nombrePac,fechaPago,nombreDoc,fechaCita,usernoti,id_doc,horaEmer,descripcion,total,nombredc,iva,subtotal;

    user.exec("select inter.idCita,inter.idDoctor,inter.idPaciente,inter.horaInicio,inter.fechaCita,inter.descripcion,CONCAT(us.nombre,' ',us.appaterno,' ',us.apmaterno) from usuario as "
              "us inner join doctor as doc on us.matricula=doc.idUser inner join citasQuirofano as inter on doc.iddoctor=inter.idDoctor");
    user.next();
    paciente.exec("select inter.idCita,inter.idDoctor,inter.idPaciente,inter.horaInicio,inter.fechaCita,inter.descripcion,"
                  "CONCAT(us.nombre,' ',us.appaterno,' ',us.apmaterno) from usuario as us inner join paciente as p on "
                  "us.matricula=p.idUser inner join citasQuirofano as inter on p.idpaciente=inter.idPaciente");
    paciente.next();

    fecha.exec("select CURRENT_DATE()");
    fecha.next();

    doc.exec("select CONCAT(' ',us.nombre,' ',us.appaterno,' ',us.apmaterno)from usuario as us inner join doctor as doc "
             "on us.matricula=doc.idUser inner join urgencias as ur on doc.iddoctor=ur.idDoctor ;");
    doc.next();


    pago.exec("select inter.idCita,cs.idCitaQ,cs.Subtotal,cs.total,cs.importeIva from citasQuirofano as "
              "inter inner join CostoServicio as cs on inter.idCita=cs.idCitaQ where idCitaQ="+folio+"");
    pago.next();


    nombredc=user.value(6).toString();
    fechaPago=fecha.value(0).toString();
    nombrePac=paciente.value(6).toString();
    fechaCita=user.value(4).toString();
    folio=user.value(0).toString();
    horaEmer=user.value(3).toString();
    descripcion=user.value(5).toString();
    total=pago.value(3).toString();
    iva=pago.value(4).toString();
    subtotal=pago.value(2).toString();


    //termina parte de generar datos de pdf//

    //inicia parte para actualizar pago y enviar notificacion//
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea realizar el pago de la intervencion?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

    //querys para hacer actualizaciones de pago
    QSqlQuery update,insert,mandarNoti;
    QString mensaj,tipo,cita,user1,notificacion;

    if(insert.exec("update CostoServicio set estado=1 where idCitaQ='"+folio+"'"))
    {
            insert.next();
            qDebug()<<folio;
            qDebug()<<"pago efectuado";

             //inicia parte para generar pdf de pago"
             QMessageBox message(QMessageBox::Question,
             tr("Information"), tr("¿Desea generar comprobante de pago?"), QMessageBox::Yes | QMessageBox::No);
             message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
             message.setButtonText(QMessageBox::No, tr("Cancelar"));
             if (message.exec() == QMessageBox::Yes){

                 html=
                         "<H1 align=center> Comprobante de pago </H1>"
                         "<H6 align=center>*LOBOHOSPITAL* A.C DE C.V se deslinda de cualquier mal uso de este comprobante*</H6>"
                         "<H5 align=center>25 pte #1230, colonia Volcanes, Puebla,Pue</H5>"
                         "<br><br><br><br>"

                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> NOMBRE Y APELLIDO DE PACIENTE INGRESADO :  "+nombrePac+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> MÉDICO RESPONSABLE : "+nombredc+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FECHA DE INTERVENCIÓN : "+fechaCita+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> HORA ASIGNADA : "+horaEmer+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FECHA DE EMISIÓN DE PAGO : "+fechaPago+" </div>"
                         "<div align=left style='font-family: serif; font-weight: 600; font-size: 1.5em'> FORMA DE PAGO :  EFECTIVO </div>"
                         "<br><br><br>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         "NO.FOLIO     FECHA DE HOSPITALIZACIÓN    DESCRIPCION      PRECIO UNITARIO        IMPORTE"
                         "</pre>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         ""+folio+"              "+fechaCita+"                "+descripcion+"       "+total+"             "+total+""
                         "</pre>"
                         "<hr style=color: #0056b2; />"
                          "<br><br><br><br><br><br><br><br><br><br><br><br><br>"
                         "<hr style=color: #0056b2; />"
                         "<pre>"
                         "                                                                           SUBTOTAL:"+subtotal+" MXN"
                        "<br><br>"
                         "                                                                           IVA:     "+iva+" %"
                         "</pre>";

                 QTextDocument document;
                 document.setHtml(html);

                 QPrinter printer(QPrinter::HighResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setPaperSize(QPrinter::A4);
                  auto nombreArchivo=QFileDialog::getSaveFileName(this,"Guardar archivo",QDir::rootPath(),"Archivos (*.pdf);;");
                  if(nombreArchivo==""){

                                      }
                                      else{

                                          printer.setOutputFileName(nombreArchivo);
                                      }

                  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
                                      document.print(&printer);

             }
             else
             {

             }

             //termina parte para generar pdf//


        }else
        {
                qDebug()<<"Error al enviar el pago"<<update.lastError().text();

        }
              }else
               {
            }
}

void MainWindow::actTablaInter()
{
    /*contador++;
    if(contador==180){
        ocultar->stop();
    }*/

    clearLayout(ui->pagoIntervenciones);
    on_pushButton_intervenciones_clicked();

}

void MainWindow::cancelarIntervencion(QString folio){

    QSqlQuery query,query2,query3,query4;
    QString consulta,eliminacion,eliminarSoli,notifi;
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Estas seguro de Cancelar tu intervenion?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    QMessageBox messag(QMessageBox::Question,
    tr("Information"), tr("Intervencion cancelada"), QMessageBox::Yes);
    messag.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    if (message.exec() == QMessageBox::Yes ){
    eliminacion="update CitasQuirofano set estado='Cancelada' where idCita='"+folio+"'";
    query2.exec(eliminacion);
    query2.next();
    qDebug()<<"folio:"<<folio;

    eliminarSoli="delete from SoliEstancia where idCitaQ='"+folio+"'";
    query3.exec(eliminarSoli);
    query3.next();

     if (messag.exec() == QMessageBox::Yes ){
          clearLayout(ui->pagoIntervenciones);
          notifi=" select  cq.fechaCita,cq.horaInicio,d.iddoctor from CitasQuirofano as cq inner join  doctor as d on cq.idDoctor=d.iddoctor where cq.idCita='"+folio+"'";
          query4.exec(notifi);
          query4.next();
          QString fech,hor,user1;
          fech=query.value(0).toString();
          hor=query.value(1).toString();
          user1=query.value(3).toString();
          QString mensaj,tipo;
          mensaj="Se le informa que su paciente ha cancelado su intervencion del dia: "+fech+" y con horario de: "+hor+" fue CANCELADA.";
          tipo="0";
          QString notificacion;
          notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"')";
          query.exec(notificacion);
          query.next();
          on_pushButton_intervenciones_clicked();
         }
    }

}


void MainWindow::on_pushButton_intervenciones_clicked()
{
    ui->stackedWidget_perfilPaciente->setCurrentIndex(3);
    //clearLayout(ui->pagoIntervenciones);
    QString citas,est,pacNom;
    QSqlQuery consulta,consulta2,pac;
    est="1";
    //int f=0;
    int ban=1;
    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";
    QString folio,doctor,fecha,hora,nomDoct,descripc,estado;
    //int i=0;
    int l=0;
    citas="select inter.idCita,inter.idDoctor,inter.idPaciente,inter.horaInicio,inter.fechaCita,inter.descripcion,CONCAT(us.nombre,' ',us.appaterno,' ',us.apmaterno),inter.estado "
                        "from usuario as us "
                      "inner join doctor as doc "
                      "on us.matricula=doc.idUser "
                     "inner join citasQuirofano as inter "
                       "on doc.iddoctor=inter.idDoctor "
                      "inner join CostoServicio as costo "
                       "on inter.idCita = costo.idCitaQ "
                      "where costo.estado=0";


    if(!consulta2.exec(citas)) consulta2.lastError().text();
    while(consulta2.next()){
        estado=consulta2.value(7).toString();
        //qDebug()<<estado;
        if(estado!="Cancelada"){
        folio=consulta2.value(0).toString();
        doctor=consulta2.value(6).toString();
        descripc=consulta2.value(5).toString();
        hora=consulta2.value(3).toString();
        fecha=consulta2.value(4).toString();
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        QLabel *fol=new QLabel;
        fol->setText(folio);
        fol->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagoIntervenciones->addWidget(fol,l,0,Qt::AlignTop);


        QLabel *m=new QLabel;
        m->setText(doctor);
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagoIntervenciones->addWidget(m,l,1,Qt::AlignTop);

        QLabel *np=new QLabel;
        np->setText(descripc);
        np->setFixedSize(QSize(140,25));
        np->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagoIntervenciones->addWidget(np,l,2,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(fecha);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagoIntervenciones->addWidget(r,l,3,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(hora);
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->pagoIntervenciones->addWidget(h,l,4,Qt::AlignTop);

        QLabel *ss=new QLabel;
        ss->setText(" ");
        //ss->setFixedSize(QSize(40,25));
        ui->pagoIntervenciones->addWidget(ss,l,5,Qt::AlignTop);
        }
      if(estado=="Completa"){
        QPushButton *p= new QPushButton();
        p->setText("Pagar con Tarjeta");
        p->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper3=new QSignalMapper(this);
        connect(p,SIGNAL(clicked(bool)),mapper3,SLOT(map()));
        mapper3->setMapping(p,folio);
        connect(mapper3,SIGNAL(mapped(QString)),this,SLOT(pagarIntervencionTarjeta(QString)));
        ui->pagoIntervenciones->addWidget(p,l,7,Qt::AlignTop);
        }
        if(estado=="Pendiente"){
        QPushButton *k= new QPushButton();
        k->setText("Cancelar intervencion");
        connect(k,&QPushButton::clicked,[=](){emit cancelarIntervencion(folio);});
        ui->pagoIntervenciones->addWidget(k,l,8,Qt::AlignTop);
       }
        l++;

    }
}

void MainWindow::pagarIntervencionTarjeta(QString folio)
{
   // clearLayout(ui->pagoIntervenciones);
    QString consulta;
    QSqlQuery query;
    query.exec("select inter.idCita,us.matricula from usuario as  us inner join paciente as p "
           "on us.matricula=p.idUser inner join citasQuirofano as inter on inter.idCita='"+folio+"'");
    query.next();
    folio=query.value(0).toString();
    qDebug()<<"folio:"<<folio;
    QMessageBox messageBox(QMessageBox::Warning,
                           tr(""), tr("Cita cancelada"), QMessageBox::Yes);
    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    QMessageBox message(QMessageBox::Question,
                        tr("Warning"), tr("¿Estas seguro de pagar tu cita?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    QMessageBox metodo(QMessageBox::Question,
                        tr("Warning"), tr("¿Como quiere pagar?"), QMessageBox::Yes | QMessageBox::No);
    metodo.setButtonText(QMessageBox::Yes, tr("Ventanilla"));
    metodo.setButtonText(QMessageBox::No, tr("Tarjeta de Crédito"));
    if (message.exec() == QMessageBox::Yes){

        //Preguntar método de pago
        if (metodo.exec() == QMessageBox::Yes){
            //Notificar al cajero
            QMessageBox::information(this,"Ventanilla","Pase a pagar a ventanilla con su folio de intervencion.");
        }
        else{
             pagoIntervenciones *pagoIt = new pagoIntervenciones(folio,this);
             pagoIt->setModal(true);
             pagoIt->show();
             //ocultar=new QTimer(this);
             //connect(ocultar,SIGNAL(timeout()),this,SLOT(actTablaInter()));
             //ocultar->start(1000);
             while(pagoIt->exec()){
             }
             actTablaInter();
        }
    }
}



void MainWindow::on_pb_agregarUser_clicked()
{
    static AgregarUsuario usuario;
    usuario.show();

}



void MainWindow::on_pb_modificarUser_clicked()
{
    static eliminarUsuarios eliminar;
    eliminar.show();
}

void MainWindow::on_pushButton_forgotPass_clicked()
{
    CambiarContrasenia* popup = new CambiarContrasenia();
    popup->show();
}

void MainWindow::on_pushButton__dirMedico_clicked()
{
     ui->stackedWidget_principal->setCurrentIndex(8);

     QString especi;
     QSqlQuery especiali;
     especi="select *from especialidad;";
     especiali.exec(especi);
     ui->especialidades->addItem("Todas");
     while(especiali.next())
     {
         ui->especialidades->addItem(especiali.value(1).toString());
     }
     //autocomplete de apellidos
     QStringList wordList;

         QString busqueda,valor,valor2;
         QSqlQuery busq;
         busqueda="SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp,u.apmaterno,u.appaterno"
                  " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp;";
         busq.exec(busqueda);

         while(busq.next())
         {
             valor=busq.value(6).toString();
             valor2=busq.value(7).toString();
             wordList << valor;
         }

         QCompleter *completer = new QCompleter(wordList, this);
             completer->setCaseSensitivity(Qt::CaseInsensitive);

             ui->apellidos->setCompleter(completer);





     llenarTDoctores("","Todas");
}

void MainWindow::on_pb_remedios_clicked()
{
    QSqlQuery cat;
      ui->stackedWidget_principal->setCurrentIndex(10);
      llenarTablaR("Todas");
      cat.exec("select nombreCategoria from tipoCategoriaRem");
      ui->remedioscateg->addItem("Todas");
      while(cat.next())
      {
          ui->remedioscateg->addItem(cat.value(0).toString());
      }

}

void MainWindow::llenarTablaR(QString rem)
{
    clearLayout(ui->gridRemedios);
     QSqlQuery qr;
     QString nombre,ingr,proc,foto,idcateg;
        QByteArray foto1;

        int conti=0;
      if(rem=="Todas"){
     qr.exec("select nombreRemedio,ingredientes,procedimiento,fotoRemedio from remedios");
     while(qr.next())
     {
        nombre=qr.value(0).toString();
        ingr=qr.value(1).toString();
        proc=qr.value(2).toString();
        foto1=qr.value(3).toByteArray();
        remedios *r = new remedios(nombre,ingr,proc,foto1);
        r->llenarInformacion();

        ui->gridRemedios->addWidget(r,conti,0);
        conti++;
     }
      }
      else
      {

          qr.exec("select * from tipoCategoriaRem where nombreCategoria='"+rem+"'");
          qr.next();
          idcateg=qr.value(0).toString();
          qr.exec("select nombreRemedio,ingredientes,procedimiento,fotoRemedio from remedios where idcategoria='"+idcateg+"'");
          while(qr.next())
          {
             nombre=qr.value(0).toString();
             ingr=qr.value(1).toString();
             proc=qr.value(2).toString();
             foto1=qr.value(3).toByteArray();
             remedios *r = new remedios(nombre,ingr,proc,foto1);
             r->llenarInformacion();

             ui->gridRemedios->addWidget(r,conti,0);
             conti++;
          }
      }

}

void MainWindow::on_buscarremedio_clicked()
{
    QString nombrecat;
    nombrecat=ui->remedioscateg->currentText();
    llenarTablaR(nombrecat);
}

void MainWindow::llenarTDoctores(QString apellido,QString especialidad)
{
    //WHERE d.iddoctor = +QString::number((idDoc))+
    clearLayout(ui->gridDoctores);
    int cont=0;
    QSqlQuery doct,idcon;
    QString idconsul,nombre,numcons,especid,nombreespec,tel;


    if(apellido=="" && especialidad=="Todas" )
    {
        doct.exec("SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp"
                  " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp");

        int vacio=0;
        while(doct.next()){
        idconsul=doct.value(3).toString();
        nombre=doct.value(0).toString();
        tel=doct.value(4).toString();
        nombreespec=doct.value(1).toString();


            //num consultorio
        idcon.exec("select numConsultorio from consultorio where idconsultorio='"+ idconsul+"'");
        idcon.next();
        numcons=idcon.value(0).toString();

        DialogDoctor *d = new DialogDoctor(nombre,nombreespec,numcons,tel);
        d->llenarInfoDoc();
        ui->gridDoctores->addWidget(d,cont,0);
        cont++;
        vacio=1;
        }

    if(vacio==0)
    {
        QMessageBox messageBox(QMessageBox::Warning,
                               tr(""), tr("No se encontro ningun resultado"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }


    }

    if(apellido=="" && especialidad!="Todas" )
    {
        doct.exec("SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp"
                  " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp and e.nombre='"+especialidad+"' ");

        int vacio=0;
        while(doct.next()){
        idconsul=doct.value(3).toString();
        nombre=doct.value(0).toString();
        tel=doct.value(4).toString();
        nombreespec=doct.value(1).toString();


            //num consultorio
        idcon.exec("select numConsultorio from consultorio where idconsultorio='"+ idconsul+"'");
        idcon.next();
        numcons=idcon.value(0).toString();

        DialogDoctor *d = new DialogDoctor(nombre,nombreespec,numcons,tel);
        d->llenarInfoDoc();
        ui->gridDoctores->addWidget(d,cont,0);
        cont++;
        vacio=1;
        }

        if(vacio==0)
        {
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr(""), tr("No se encontro ningun resultado"), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }

    }
    else
    {
        if(apellido!="" && especialidad=="Todas")
        {
            QString busqueP;
            QSqlQuery busqueP2;
            int vacio=0;
            busqueP="SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp,u.apmaterno,u.appaterno"
                   " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp and u.appaterno='"+apellido+"' ";


            busqueP2.exec(busqueP);
            bool bandera=false;
                while(busqueP2.next())
                {
                    idconsul=busqueP2.value(3).toString();
                    nombre=busqueP2.value(0).toString();
                    tel=busqueP2.value(4).toString();
                    nombreespec=busqueP2.value(1).toString();


                        //num consultorio
                    idcon.exec("select numConsultorio from consultorio where idconsultorio='"+ idconsul+"'");
                    idcon.next();
                    numcons=idcon.value(0).toString();

                    DialogDoctor *d = new DialogDoctor(nombre,nombreespec,numcons,tel);
                    d->llenarInfoDoc();
                    ui->gridDoctores->addWidget(d,cont,0);
                    cont++;
                    vacio=1;
                    bandera=true;
                }

            if(bandera==false)
            {
                QString busqueP;
                QSqlQuery busqueP2;

                busqueP="SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp,u.apmaterno,u.appaterno"
                       " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp and u.apmaterno='"+apellido+"' ";

                vacio=0;

                busqueP2.exec(busqueP);
                while(busqueP2.next())
                {
                    idconsul=busqueP2.value(3).toString();
                    nombre=busqueP2.value(0).toString();
                    tel=busqueP2.value(4).toString();
                    nombreespec=busqueP2.value(1).toString();


                        //num consultorio
                    idcon.exec("select numConsultorio from consultorio where idconsultorio='"+ idconsul+"'");
                    idcon.next();
                    numcons=idcon.value(0).toString();

                    DialogDoctor *d = new DialogDoctor(nombre,nombreespec,numcons,tel);
                    d->llenarInfoDoc();
                    ui->gridDoctores->addWidget(d,cont,0);
                    cont++;

                    vacio=1;
                 //   bandera=true;
                }



            }
            if(vacio==0)
            {
                qDebug()<<"no encontrados 1";
                //mensaje de no encontrado
                QMessageBox messageBox(QMessageBox::Warning,
                                       tr(""), tr("No se encontro ningun resultado"), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }


        }
        else if(apellido!="" && especialidad!="Todas")
        {
//aqui va la busqueda de apellido por especialidad
            QString busqueP;
            QSqlQuery busqueP2;
            int vacio=0;
            busqueP="SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp,u.apmaterno,u.appaterno"
                   " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp and u.appaterno='"+apellido+"' and e.nombre='"+especialidad+"' ";


            busqueP2.exec(busqueP);
            bool bandera=false;
                while(busqueP2.next())
                {
                    idconsul=busqueP2.value(3).toString();
                    nombre=busqueP2.value(0).toString();
                    tel=busqueP2.value(4).toString();
                    nombreespec=busqueP2.value(1).toString();


                        //num consultorio
                    idcon.exec("select numConsultorio from consultorio where idconsultorio='"+ idconsul+"'");
                    idcon.next();
                    numcons=idcon.value(0).toString();

                    DialogDoctor *d = new DialogDoctor(nombre,nombreespec,numcons,tel);
                    d->llenarInfoDoc();
                    ui->gridDoctores->addWidget(d,cont,0);
                    cont++;
                    vacio=1;
                    bandera=true;
                }

            if(bandera==false)
            {
                QString busqueP;
                QSqlQuery busqueP2;

                busqueP="SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp,u.apmaterno,u.appaterno"
                       " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp and u.apmaterno='"+apellido+"' and e.nombre='"+especialidad+"' ";

                vacio=0;

                busqueP2.exec(busqueP);
                while(busqueP2.next())
                {
                    idconsul=busqueP2.value(3).toString();
                    nombre=busqueP2.value(0).toString();
                    tel=busqueP2.value(4).toString();
                    nombreespec=busqueP2.value(1).toString();


                        //num consultorio
                    idcon.exec("select numConsultorio from consultorio where idconsultorio='"+ idconsul+"'");
                    idcon.next();
                    numcons=idcon.value(0).toString();

                    DialogDoctor *d = new DialogDoctor(nombre,nombreespec,numcons,tel);
                    d->llenarInfoDoc();
                    ui->gridDoctores->addWidget(d,cont,0);
                    cont++;

                    vacio=1;
                 //   bandera=true;
                }



            }
            if(vacio==0)
            {
                qDebug()<<"no encontrados 2";
                //mensaje de no encontrado
                QMessageBox messageBox(QMessageBox::Warning,
                                       tr(""), tr("No se encontro ningun resultado"), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }




        }
    }

}

void MainWindow::on_buscarDoctores_clicked()
{
    llenarTDoctores(ui->apellidos->text(),ui->especialidades->currentText());
}

void MainWindow::on_pushButton_6_clicked()
{
    PermisoLaboral* permiso = new PermisoLaboral(this,id_staff);
    permiso->show();
}

void MainWindow::on_pb_permisosStaff_clicked()
{
    clearLayout(ui->layPermisos);
    QSqlQuery* permisos = new QSqlQuery;
    permisos->exec("SELECT idPermiso,fechaI,fechaF,estado FROM permisoLaboral WHERE idStaff="+id_staff);

    //Ciclo para poner botones y cosas
    QLabel* lb;
    QPushButton* pb;
    QHBoxLayout* hlay;
    QString *contenido;
    while(permisos->next()){
        hlay = new QHBoxLayout;

        //fechaI
        contenido = new QString(permisos->value("fechaI").toString());
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);
        //fechaF
        contenido = new QString(permisos->value("fechaF").toString());
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);
        //estado
        contenido = new QString(permisos->value("estado").toString());
        if(*contenido == "0"){
            *contenido = "En espera";
        }
        else{
            *contenido = "Aceptada";
        }
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);

        //Boton de eliminar sólo si aun no se acepta
        if(permisos->value("estado").toString() == "0"){
            pb= new QPushButton(" Cancelar ");
            qDebug()<<permisos->value("idPermiso").toString();
            QString* id =new QString(permisos->value("idPermiso").toString());
            connect(pb,&QPushButton::clicked,[=](){emit eliminarPermisoLaboral(*id);});
            hlay->addWidget(pb);
        }
        else {
            pb= new QPushButton(" Cancelar ");
            pb->setEnabled(0);
            pb->setStyleSheet("background:grey");
            hlay->addWidget(pb);
        }
        ui->layPermisos->addLayout(hlay);
    }
    //Cuando termine hay que agregar una barra espaciadora para empujar el contenido
    QSpacerItem *barraVertical= new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    ui->layPermisos->addSpacerItem(barraVertical);
    ui->stackedWidget_PerfilStaff->setCurrentIndex(2);
}


void MainWindow::eliminarPermisoLaboral(QString idPermiso){
    QMessageBox::StandardButton res = QMessageBox::question(this,"Cancelar solicitud","¿Está seguro de cancelar su solicitud?");
    if(res == QMessageBox::Yes){
        QSqlQuery* deletear = new QSqlQuery;
        if( deletear->exec("DELETE FROM permisoLaboral WHERE idPermiso="+idPermiso) ){
            QMessageBox::information(this, "Éxito","Borrado correctamente.");
            on_pb_permisosStaff_clicked();
        }
        else{
            qDebug()<<deletear->lastError().text();
            QMessageBox::critical(this, "Error","Error al borrar.");
        }
    }
}

void MainWindow::on_pb_adminPermisos_clicked()
{
    VerPermisosLaborales* permisos = new VerPermisosLaborales(this);
    permisos->show();
}


void MainWindow::on_pb_notiStaff_clicked()
{
    ui->stackedWidget_PerfilStaff->setCurrentIndex(0);
    if(verNoti==1)
    {
        if(ui->notiStaff->text()!="")
        {
            ui->notiStaff->hide();
        }
        ui->nofi_staff->show();
        verNoti=0;
    }
    else
    {
      ui->nofi_staff->hide();

      QString idNoti,id,update1;
      idNoti="select idNoti from notificacion where UserP="+id_usuario;
      QSqlQuery upNoti,upNoti1;
      upNoti.exec(idNoti);

      while(upNoti.next())
      {
          id=upNoti.value(0).toString();
          update1="update notificacion set vista=true where idNoti='"+id+"';";
          upNoti1.exec(update1);
            qDebug()<<id;
            qDebug()<<update1;
      }
      verNoti=1;
    }
}

void MainWindow::on_pb_bajaPaciente_clicked()
{
    QMessageBox::StandardButton res = QMessageBox::question(this,"Confirmar","¿Está seguro de eliminar su cuenta? \nEsto no se puede revertir.");
    if(res == QMessageBox::Yes){
        //Hacer proceso de daniel
        eliminarUsuarios* del = new eliminarUsuarios;
        del->eliminarPaciente(id_usuario);
        on_pushButton_salir_clicked();
        delete del;
    }
}

void MainWindow::on_pb_bajaDoctor_clicked()
{
    QMessageBox::StandardButton res = QMessageBox::question(this,"Confirmar","¿Está seguro de eliminar su cuenta? \nEsto no se puede revertir.");
    if(res == QMessageBox::Yes){
        //Hacer proceso de daniel
        eliminarUsuarios* del = new eliminarUsuarios;
        del->eliminarDoc(id_usuario);
        on_pushButton_salir_clicked();
        delete del;
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton res = QMessageBox::question(this,"Confirmar","¿Está segurode eliminar su cuenta? \nEsto no se puede revertir.");
    if(res == QMessageBox::Yes){
        //Hacer proceso de daniel
        eliminarUsuarios* del = new eliminarUsuarios;
        del->eliminarStaff(id_usuario);
        on_pushButton_salir_clicked();
        delete del;
    }
}

void MainWindow::on_pushButton_infoHospital_clicked()
{
}

void MainWindow::info_ser(QString tipo)
{
    ui->foto1S->clear();
    ui->foto2S->clear();

    QString bus,titulo,info1,lema,info2;
    QSqlQuery bus1;

    bus="select *from Servicios where nombreS='"+tipo+"'; ";

    bus1.exec(bus);
    bus1.next();

    titulo=bus1.value(1).toString();
    info1=bus1.value(2).toString();
    lema=bus1.value(3).toString();
    info2=bus1.value(4).toString();

    ui->tituloS->setText(titulo);
    ui->info1->setPlainText(info1);
    ui->lema->setPlainText(lema);
    ui->info2->setPlainText(info2);

    QString foto1,foto2;


    if(tipo=="URGENCIAS")
    {

           foto1=":/fotoservi/hospifotos/urgencias1.jpg";
           foto2=":/fotoservi/hospifotos/urgencias2.jpg";
            QPixmap f1(foto1);
            QPixmap f2(foto2);

            int a=ui->foto1S->height();
            int b=ui->foto1S->width();
            int a1=ui->foto2S->height();
            int b1=ui->foto2S->width();

           ui->foto1S->setPixmap(f1.scaled(b,a,Qt::AspectRatioMode::IgnoreAspectRatio));
            ui->foto2S->setPixmap(f2.scaled(b1,a1,Qt::AspectRatioMode::IgnoreAspectRatio));

    }
    else if(tipo=="QUIROFANOS")
    {

        foto1=":/fotoservi/hospifotos/quirofano1.jpeg";
        foto2=":/fotoservi/hospifotos/quirofano2.jpeg";
         QPixmap f1(foto1);
         QPixmap f2(foto2);

         int a=ui->foto1S->height();
         int b=ui->foto1S->width();
         int a1=ui->foto2S->height();
         int b1=ui->foto2S->width();

        ui->foto1S->setPixmap(f1.scaled(b,a,Qt::AspectRatioMode::IgnoreAspectRatio));
         ui->foto2S->setPixmap(f2.scaled(b1,a1,Qt::AspectRatioMode::IgnoreAspectRatio));
    }
    else if(tipo=="TERAPIA INTENSIVA")
    {
        foto1=":/fotoservi/hospifotos/terapia1.jpg";
        foto2=":/fotoservi/hospifotos/terapia2.jpg";
         QPixmap f1(foto1);
         QPixmap f2(foto2);

         int a=ui->foto1S->height();
         int b=ui->foto1S->width();
         int a1=ui->foto2S->height();
         int b1=ui->foto2S->width();

        ui->foto1S->setPixmap(f1.scaled(b,a,Qt::AspectRatioMode::IgnoreAspectRatio));
         ui->foto2S->setPixmap(f2.scaled(b1,a1,Qt::AspectRatioMode::IgnoreAspectRatio));
    }
    else if(tipo=="HOSPITALIZACION")
    {
        foto1=":/fotoservi/hospifotos/hospitalizacion1.jpg";
        foto2=":/fotoservi/hospifotos/hospitalizacion2.jpg";
         QPixmap f1(foto1);
         QPixmap f2(foto2);

         int a=ui->foto1S->height();
         int b=ui->foto1S->width();
         int a1=ui->foto2S->height();
         int b1=ui->foto2S->width();

        ui->foto1S->setPixmap(f1.scaled(b,a,Qt::AspectRatioMode::IgnoreAspectRatio));
         ui->foto2S->setPixmap(f2.scaled(b1,a1,Qt::AspectRatioMode::IgnoreAspectRatio));
    }
    else if(tipo=="HEMODINAMIA")
    {
        foto1=":/fotoservi/hospifotos/hemodinamia1.png";
        foto2=":/fotoservi/hospifotos/hemodinamia2.jpg";
         QPixmap f1(foto1);
         QPixmap f2(foto2);

         int a=ui->foto1S->height();
         int b=ui->foto1S->width();
         int a1=ui->foto2S->height();
         int b1=ui->foto2S->width();

        ui->foto1S->setPixmap(f1.scaled(b,a,Qt::AspectRatioMode::IgnoreAspectRatio));
         ui->foto2S->setPixmap(f2.scaled(b1,a1,Qt::AspectRatioMode::IgnoreAspectRatio));
    }





}



void MainWindow::on_cb_servicios_currentTextChanged(const QString &arg1)
{
    if(arg1=="SERVICIOS")
    {

    }
    else
    {
       ui->stackedWidget_principal->setCurrentIndex(6);
       info_ser(arg1);
       ui->cb_servicios->setCurrentIndex(0);
    }
}
