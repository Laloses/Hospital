#include "editarusuario.h"
#include "ui_editarusuario.h"

EditarUsuario::EditarUsuario(QString matricula, QString typeU,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditarUsuario)
{
    ui->setupUi(this);
    ui->swEditar->setCurrentIndex(0);
    ui->btnSiguiente->hide();
    ui->btnGuardarCambios->hide();

    mat = matricula;
    tipoUsur = typeU;
/*
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
*/
    QSqlQuery preguntas;
    preguntas.prepare("select * from pregunta");
    preguntas.exec();
    int i = 0;
    while(preguntas.next()){
        ui->cbPreguntas->insertItem(i,preguntas.value(1).toString());
        i++;
    }

    ui->cbTurno->insertItem(0,"Matutino");
    ui->cbTurno->insertItem(1,"Vespertino");
    ui->cbTurno->insertItem(2,"Nocturno");

    ui->cbTurnoStaff->insertItem(0,"Matutino");
    ui->cbTurnoStaff->insertItem(1,"Vespertino");
    ui->cbTurnoStaff->insertItem(2,"Nocturno");

    datosUsr = new QSqlQuery;

    if(tipoUsur == "doctor")
    {
        ui->btnSiguiente->show();
        qDebug() << "Buscamos doctor";
        datosUsr->prepare("SELECT nombre, appaterno, apmaterno, fechaN, edad, email, telefono, fotop, idpregunta, respuesta, religion, sexo FROM usuario WHERE matricula = "+mat+"");
        datosUsr->exec();
        datosUsr->next();
        nombreUsr = datosUsr->value(0).toString();
        appatUst = datosUsr->value(1).toString();
        apmatUsr = datosUsr->value(2).toString();
        email = datosUsr->value(5).toString();
        telefono = datosUsr->value(6).toString();
        img.loadFromData(datosUsr->value(7).toByteArray());
        pregunta = datosUsr->value(8).toString();
        respuesta = datosUsr->value(9).toString();
        religion = datosUsr->value(10).toString();
        sexo = datosUsr->value(11).toString();
        QSqlQuery doctorExtr;
        doctorExtr.prepare("SELECT idEspecialidad, horario, idpuesto, cedulapro, universidad, idconsultorio FROM doctor WHERE doctor.idUser = '"+mat+"'");
        doctorExtr.exec();
        doctorExtr.next();

        QString esp = doctorExtr.value(0).toString();
        QString turno = doctorExtr.value(1).toString();
        QString puesto = doctorExtr.value(2).toString();
        QString cedula = doctorExtr.value(3).toString();
        QString uni = doctorExtr.value(4).toString();
        QString consultorio = doctorExtr.value(5).toString();

        QSqlQuery especialidades;
        especialidades.prepare("select * from especialidad");
        especialidades.exec();
        int i = 0;
        while(especialidades.next()){
            ui->cbEspecialidad->insertItem(i,especialidades.value(1).toString());
            //if(ui->cbEspecialidad->currentText() == )
            i++;
        }
        ui->cbEspecialidad->setCurrentIndex(esp.toInt()-1);

        QSqlQuery puestos;
        puestos.prepare("select * from puesto");
        puestos.exec();
        int j = 0;
        while(puestos.next()){
            ui->cbPuesto->insertItem(j,puestos.value(1).toString());
            j++;
        }
        ui->cbPuesto->setCurrentIndex(puesto.toInt()-1);

        QSqlQuery consultorios;
        consultorios.prepare("select * from consultorio");
        consultorios.exec();
        int k = 0;
        while(consultorios.next()){
            ui->cbConsultorio->insertItem(k,consultorios.value(2).toString());
            k++;
        }
        ui->cbConsultorio->setCurrentIndex(consultorio.toInt()-1);

        ui->lnUniversidad->setText(uni);
        ui->lnCedProf->setText(cedula);

        //combobox turno
        for (int q = 0; q <= 2; q++) {
            ui->cbTurno->setCurrentIndex(q);
            if(ui->cbTurno->currentText() == turno){
                qDebug() << turno << " = " << ui->cbTurno->currentText();
                break;
            }
        }
    }
    else if (tipoUsur == "staff") {
        ui->btnSiguiente->show();
        datosUsr->prepare("SELECT nombre, appaterno, apmaterno, fechaN, edad, email, telefono, fotop, idpregunta, respuesta, religion, sexo FROM usuario WHERE matricula = '"+mat+"'");
        datosUsr->exec();
        datosUsr->next();
        qDebug() << "staff";
        nombreUsr = datosUsr->value(0).toString();
        appatUst = datosUsr->value(1).toString();
        apmatUsr = datosUsr->value(2).toString();
        email = datosUsr->value(5).toString();
        telefono = datosUsr->value(6).toString();
        img.loadFromData(datosUsr->value(7).toByteArray());
        pregunta = datosUsr->value(8).toString();
        respuesta = datosUsr->value(9).toString();
        religion = datosUsr->value(10).toString();
        sexo = datosUsr->value(11).toString();

        QSqlQuery staffExtr;
        staffExtr.prepare("SELECT idpuesto, idArea, turno FROM staff WHERE idUser = '"+mat+"'");
        staffExtr.exec();
        staffExtr.next();

        QString puesto = staffExtr.value(0).toString();
        QString area = staffExtr.value(1).toString();
        QString turno = staffExtr.value(2).toString();

        QSqlQuery puestos;
        puestos.prepare("select * from puesto");
        puestos.exec();
        int i = 0;
        while(puestos.next()){
            ui->cbPuestoStaff->insertItem(i,puestos.value(1).toString());
            i++;
        }
        ui->cbPuestoStaff->setCurrentIndex(puesto.toInt()-1);

        QSqlQuery areas;
        areas.prepare("select * from areah");
        areas.exec();
        int j = 0;
        while(areas.next()){
            ui->cbAreaStaff->insertItem(j,areas.value(1).toString());
            j++;
        }
        ui->cbAreaStaff->setCurrentIndex(area.toInt()-1);

        //combobox turno
        for (int q = 0; q <= 2; q++) {
            ui->cbTurnoStaff->setCurrentIndex(q);
            if(ui->cbTurnoStaff->currentText() == turno){
                qDebug() << turno << " = " << ui->cbTurnoStaff->currentText();
                break;
            }
        }
    }
    else if (tipoUsur == "paciente") {
        datosUsr->prepare("SELECT usuario.nombre, usuario.appaterno, usuario.apmaterno, usuario.fechaN, usuario.edad, usuario.email, usuario.telefono, usuario.fotop, usuario.idpregunta, usuario.respuesta, usuario.religion, usuario.sexo FROM paciente INNER JOIN usuario on paciente.idUser = usuario.matricula WHERE paciente.idUser = '"+mat+"'");
        datosUsr->exec();
        datosUsr->next();
        qDebug() << "paciente";
        nombreUsr = datosUsr->value(0).toString();
        appatUst = datosUsr->value(1).toString();
        apmatUsr = datosUsr->value(2).toString();
        email = datosUsr->value(5).toString();
        telefono = datosUsr->value(6).toString();
        img.loadFromData(datosUsr->value(7).toByteArray());
        pregunta = datosUsr->value(8).toString();
        respuesta = datosUsr->value(9).toString();
        religion = datosUsr->value(10).toString();
        sexo = datosUsr->value(11).toString();
        ui->btnGuardarCambios->show();
    }
    ui->lblFotoPerfil->setPixmap(img);
    ui->lnNombre->setText(nombreUsr);
    ui->lnPaterno->setText(appatUst);
    ui->lnMaterno->setText(apmatUsr);
    ui->lnCorreo->setText(email);
    ui->lnTel->setText(telefono);
    ui->cbPreguntas->setCurrentIndex(pregunta.toInt()-1);
    ui->lnRespuesta->setText(respuesta);
    ui->lnReligion->setText(religion);
    ui->lnSexo->setText(sexo);
}

EditarUsuario::~EditarUsuario()
{
    delete ui;
}

void EditarUsuario::on_btnCancelar_clicked()
{
    this->close();
}

void EditarUsuario::on_btnSiguiente_clicked()
{
    if(tipoUsur == "doctor")
    {
        ui->swEditar->setCurrentIndex(1);
    }
    else{
        ui->swEditar->setCurrentIndex(2);
    }
}

void EditarUsuario::on_btnAtrasDoc_clicked()
{
    ui->swEditar->setCurrentIndex(0);
}

void EditarUsuario::on_btnAtrasStaff_clicked()
{
    ui->swEditar->setCurrentIndex(0);
}

void EditarUsuario::on_btnFotoPerfil_clicked()
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
    ui->lblFotoPerfil->setPixmap(pix);
}

void EditarUsuario::on_btnGuardarCambios_clicked()
{
    //Paciente
    nombreUsr = ui->lnNombre->text();
    appatUst = ui->lnPaterno->text();
    apmatUsr = ui->lnMaterno->text();
    email = ui->lnCorreo->text();
    telefono = ui->lnTel->text();
    int numPregunta = ui->cbPreguntas->currentIndex()+1;
    pregunta = QString::number(numPregunta);
    respuesta = ui->lnRespuesta->text();
    religion = ui->lnReligion->text();
    sexo = ui->lnSexo->text();

    const QPixmap *fotoPerfil = new QPixmap();
    fotoPerfil = ui->lblFotoPerfil->pixmap();
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    fotoPerfil->save(&buffer, "PNG");

    if(nombreUsr == "" || appatUst == "" || apmatUsr == "" || email == "" || telefono == "" || pregunta == "" || respuesta == "" || religion == "" || sexo == ""){
        QMessageBox messageBox(QMessageBox::Critical,
                               tr(""), tr("Todos los campos deben ser rellenados."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
            QMessageBox message(QMessageBox::Question,
                                tr("Warning"), tr("Se actualizarán los datos. ¿Desea continuar?"), QMessageBox::Yes | QMessageBox::No);
            message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            message.setButtonText(QMessageBox::No, tr("Cancelar"));
            if(message.exec() == QMessageBox::No)
            {}
            else{
                qDebug() << "UPDATE `usuario` SET `nombre`= nombreUsr,`appaterno`= appatUst,`apmaterno`= apmatUsr, email= email,`telefono`= telefono,`fotop`= bytes,`idpregunta`= pregunta,`respuesta`= respuesta,`religion`= religion,`sexo`= sexo WHERE matricula = mat";
                QSqlQuery updatePaciente;
                updatePaciente.prepare("UPDATE `usuario` SET `nombre`= '"+nombreUsr+"',`appaterno`= '"+appatUst+"',`apmaterno`= '"+apmatUsr+"',`email`= '"+email+"',`telefono`= '"+telefono+"',`fotop`= ?,`idpregunta`= '"+pregunta+"',`respuesta`= '"+respuesta+"',`religion`= '"+religion+"',`sexo`= '"+sexo+"' WHERE matricula = '"+mat+"'");
                updatePaciente.addBindValue(bytes);
                if(!updatePaciente.exec()){
                    updatePaciente.lastError();
                }
                QMessageBox messageBox(QMessageBox::Information,
                                       tr(""), tr("Los datos han sido actualizados."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                if(messageBox.exec() == QMessageBox::Yes)
                {
                    this->close();
                }
            }
    }
}

void EditarUsuario::on_btnGuardarCambiosDoctor_clicked()
{
    //Doctor
    nombreUsr = ui->lnNombre->text();
    appatUst = ui->lnPaterno->text();
    apmatUsr = ui->lnMaterno->text();
    email = ui->lnCorreo->text();
    telefono = ui->lnTel->text();
    int numPregunta = ui->cbPreguntas->currentIndex()+1;
    pregunta = QString::number(numPregunta);
    respuesta = ui->lnRespuesta->text();
    religion = ui->lnReligion->text();
    sexo = ui->lnSexo->text();

    const QPixmap *fotoPerfil = new QPixmap();
    fotoPerfil = ui->lblFotoPerfil->pixmap();
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    fotoPerfil->save(&buffer, "PNG");

    //datos extra doctor
    QString ced = ui->lnCedProf->text();
    QString uni = ui->lnUniversidad->text();
    int numEsp = ui->cbEspecialidad->currentIndex()+1;
    QString especialidad = QString::number(numEsp);
    QString turno= ui->cbTurno->currentText();
    int numPuesto = ui->cbPuesto->currentIndex()+1;
    QString puesto= QString::number(numPuesto);
    int numConsult = ui->cbConsultorio->currentIndex()+1;
    QString consultorio= QString::number(numConsult);

    if(ced == "" || uni == "" || nombreUsr == "" || appatUst == "" || apmatUsr == "" || email == "" || telefono == "" || pregunta == "" || respuesta == "" || religion == "" || sexo == ""){
        QMessageBox messageBox(QMessageBox::Critical,
                               tr(""), tr("Todos los campos deben ser rellenados."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
            QMessageBox message(QMessageBox::Question,
                                tr("Warning"), tr("Se actualizarán los datos. ¿Desea continuar?"), QMessageBox::Yes | QMessageBox::No);
            message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            message.setButtonText(QMessageBox::No, tr("Cancelar"));
            if(message.exec() == QMessageBox::No)
            {}
            else{
                qDebug() << "UPDATE `usuario` SET `nombre`= nombreUsr,`appaterno`= appatUst,`apmaterno`= apmatUsr, email= email,`telefono`= telefono,`fotop`= bytes,`idpregunta`= pregunta,`respuesta`= respuesta,`religion`= religion,`sexo`= sexo WHERE matricula = mat";
                QSqlQuery updatePaciente;
                updatePaciente.prepare("UPDATE `usuario` SET `nombre`= '"+nombreUsr+"',`appaterno`= '"+appatUst+"',`apmaterno`= '"+apmatUsr+"',`email`= '"+email+"',`telefono`= '"+telefono+"',`fotop`= ?,`idpregunta`= '"+pregunta+"',`respuesta`= '"+respuesta+"',`religion`= '"+religion+"',`sexo`= '"+sexo+"' WHERE matricula = '"+mat+"'");
                updatePaciente.addBindValue(bytes);
                if(!updatePaciente.exec()){
                    updatePaciente.lastError();
                }
                QSqlQuery extraDoc;
                extraDoc.prepare("UPDATE `doctor` SET `idEspecialidad`='"+especialidad+"',`horario`='"+turno+"',`idpuesto`='"+puesto+"',`cedulapro`='"+ced+"',`universidad`='"+uni+"',`idconsultorio`='"+consultorio+"' WHERE `idUser` = '"+mat+"'");
                extraDoc.exec();
                QMessageBox messageBox(QMessageBox::Information,
                                       tr(""), tr("Los datos han sido actualizados."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                if(messageBox.exec() == QMessageBox::Yes)
                {
                    this->close();
                }
            }
    }
}

void EditarUsuario::on_btnGuardarCambiosStaff_clicked()
{
    //Staff
    nombreUsr = ui->lnNombre->text();
    appatUst = ui->lnPaterno->text();
    apmatUsr = ui->lnMaterno->text();
    email = ui->lnCorreo->text();
    telefono = ui->lnTel->text();
    int numPregunta = ui->cbPreguntas->currentIndex()+1;
    pregunta = QString::number(numPregunta);
    respuesta = ui->lnRespuesta->text();
    religion = ui->lnReligion->text();
    sexo = ui->lnSexo->text();

    const QPixmap *fotoPerfil = new QPixmap();
    fotoPerfil = ui->lblFotoPerfil->pixmap();
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    fotoPerfil->save(&buffer, "PNG");

    //datos extra staff
    int numArea = ui->cbAreaStaff->currentIndex()+1;
    QString area = QString::number(numArea);
    QString turno = ui->cbTurnoStaff->currentText();
    int numPuesto = ui->cbPuestoStaff->currentIndex()+1;
    QString puesto = QString::number(numPuesto);

    if(nombreUsr == "" || appatUst == "" || apmatUsr == "" || email == "" || telefono == "" || respuesta == "" || religion == "" || sexo == ""){
        QMessageBox messageBox(QMessageBox::Critical,
                               tr(""), tr("Todos los campos deben ser rellenados."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
            QMessageBox message(QMessageBox::Question,
                                tr("Warning"), tr("Se actualizarán los datos. ¿Desea continuar?"), QMessageBox::Yes | QMessageBox::No);
            message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            message.setButtonText(QMessageBox::No, tr("Cancelar"));
            if(message.exec() == QMessageBox::No)
            {}
            else{
                qDebug() << "UPDATE `usuario` SET `nombre`= nombreUsr,`appaterno`= appatUst,`apmaterno`= apmatUsr, email= email,`telefono`= telefono,`fotop`= bytes,`idpregunta`= pregunta,`respuesta`= respuesta,`religion`= religion,`sexo`= sexo WHERE matricula = mat";
                QSqlQuery updatePaciente;
                updatePaciente.prepare("UPDATE `usuario` SET `nombre`= '"+nombreUsr+"',`appaterno`= '"+appatUst+"',`apmaterno`= '"+apmatUsr+"',`email`= '"+email+"',`telefono`= '"+telefono+"',`fotop`= ?,`idpregunta`= '"+pregunta+"',`respuesta`= '"+respuesta+"',`religion`= '"+religion+"',`sexo`= '"+sexo+"' WHERE matricula = '"+mat+"'");
                updatePaciente.addBindValue(bytes);
                if(!updatePaciente.exec()){
                    updatePaciente.lastError();
                }
                QSqlQuery extraStaff;
                extraStaff.prepare("UPDATE `staff` SET `idpuesto`='"+puesto+"',`idArea`='"+area+"',`turno`='"+turno+"' WHERE `idUser` = '"+mat+"'");
                extraStaff.exec();
                QMessageBox messageBox(QMessageBox::Information,
                                       tr(""), tr("Los datos han sido actualizados."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                if(messageBox.exec() == QMessageBox::Yes)
                {
                    this->close();
                }
            }
    }

}
