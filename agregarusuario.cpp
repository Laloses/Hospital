#include "agregarusuario.h"
#include "ui_agregarusuario.h"

AgregarUsuario::AgregarUsuario(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AgregarUsuario)
{
    ui->setupUi(this);
    ui->stackedWidget_registros->setCurrentIndex(0);
}

AgregarUsuario::~AgregarUsuario()
{
    delete ui;
}

bool AgregarUsuario::verificarPasswordRegistro(){
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

QString AgregarUsuario::calcularEdad(QString fechaN){
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

bool AgregarUsuario::verificarDatosRegistro(){
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
void AgregarUsuario::on_pushButton_registrarse_clicked()
{
    if(verificarDatosRegistro() && verificarPasswordRegistro()){
             //Pasamos a la pagina de preguntas
             ui->stackedWidget_registros->setCurrentIndex(1);
             //Cargamos las preguntas
             QSqlQueryModel *queryPreguntas;
             queryPreguntas = new QSqlQueryModel;
             queryPreguntas->setQuery("SELECT pregunta FROM pregunta");
             ui->comboBox_pregunta->setModel(queryPreguntas);
    }
}

void AgregarUsuario::on_pushButton_imgPerfil_clicked()
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

void AgregarUsuario::on_pushButton_respuesta_clicked()
{
    registroStaff regStaff;
    registropaciente regPaciente;
    registroDoctor regDoctor;
    QString correcto;
    QString idPuesto=QString::number( ui->comboBox_puesto->currentIndex()+1);
    QString idPregunta= QString::number(ui->comboBox_pregunta->currentIndex()+1);
    QString edad =calcularEdad(ui->dateEdit_fNacimiento->text());
    QString idEspecialidad = QString::number(ui->comboBox_especiDoc->currentIndex()+1);
    QSqlQuery query;

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
                              ui->lineEdit_respuesta->text(),
                              ui->le_sexo->text());

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
            ui->le_sexo->clear();
            ui->radioButton_doc->setCheckable(false);
            ui->radioButton_doc->setCheckable(true);
            ui->radioButton_staff->setCheckable(false);
            ui->radioButton_staff->setCheckable(true);
            ui->radioButton_paciente->setCheckable(false);
            ui->radioButton_paciente->setCheckable(true);
            ui->stackedWidget_registros->setCurrentIndex(0);
    }
    else {
        QMessageBox::critical(this,"No se Registro", "Hay un error en el servidor, intente más tarde.");
    }

}

void AgregarUsuario::on_pushButton_2_clicked()
{
    ui->stackedWidget_registros->setCurrentIndex(0);
}

void AgregarUsuario::on_radioButton_doc_toggled(bool checked)
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

void AgregarUsuario::on_radioButton_paciente_toggled(bool checked)
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

void AgregarUsuario::on_radioButton_staff_toggled(bool checked)
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

void AgregarUsuario::limpiar(){
    ui->radioButton_doc->setCheckable(false);
    ui->radioButton_doc->setCheckable(true);
    ui->radioButton_staff->setCheckable(false);
    ui->radioButton_staff->setCheckable(true);
    ui->radioButton_paciente->setCheckable(false);
    ui->radioButton_paciente->setCheckable(true);

}
