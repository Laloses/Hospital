#include "cambiarcontrasenia.h"
#include "ui_cambiarcontrasenia.h"

CambiarContrasenia::CambiarContrasenia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CambiarContrasenia)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

CambiarContrasenia::~CambiarContrasenia()
{
    delete ui;
}

void CambiarContrasenia::on_btnCancelar_clicked()
{
    this->close();
}

void CambiarContrasenia::on_btnContinuar_clicked()
{
    if(ui->lineIDUsr->text() == ""){
        //il-elle est bimbo
        QMessageBox messageBox(QMessageBox::Critical,
                               tr(""), tr("Por favor ingrese su ID de usuario."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else {
        id=ui->lineIDUsr->text();
        obtenerPregunta.prepare("SELECT pregunta.pregunta, `respuesta` FROM `usuario` INNER JOIN pregunta ON pregunta.idpregunta = usuario.idpregunta WHERE `matricula` = '"+id+"'");
        obtenerPregunta.exec();
        if(obtenerPregunta.next()){
            pregunta = obtenerPregunta.value(0).toString();
            respuesta = obtenerPregunta.value(1).toString();
            ui->lblPreguntaSeguridad->setText(pregunta);
            ui->stackedWidget->setCurrentIndex(1);
            ui->lineIDUsr->clear();
        }
        else{
            //n'existe pas
            QMessageBox messageBox(QMessageBox::Critical,
                                   tr(""), tr("El ID proporcionado no existe."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
    }
}

void CambiarContrasenia::on_btnEnviarRepuesta_clicked()
{
    if(ui->lineRespuesta->text() == respuesta){
        //c'est bien
        ui->stackedWidget->setCurrentIndex(2);
    }
    else {
        //ca n'est pas bien
        QMessageBox messageBox(QMessageBox::Critical,
                               tr(""), tr("Respuesta incorrecta."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
        ui->lineRespuesta->clear();
    }
}

void CambiarContrasenia::on_btnRegresar_clicked()
{
    QMessageBox message(QMessageBox::Question,
                        tr("Warning"), tr("¿Desea cancelar la operación?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if(message.exec() == QMessageBox::Yes)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void CambiarContrasenia::on_btnRestablecerPass_clicked()
{
    QString newPass = ui->lineNewPass->text();
    QString confPass = ui->lineConfPass->text();
    if(newPass == "" || confPass == ""){
        QMessageBox messageBox(QMessageBox::Critical,
                               tr(""), tr("Ingrese una contraseña."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        if(newPass == confPass){
            QSqlQuery actualizaPass;
            actualizaPass.prepare("update usuario set clave='"+newPass+"' WHERE matricula='"+id+"'");
            actualizaPass.exec();
            QMessageBox messageBox(QMessageBox::Information,
                                   tr(""), tr("La contraseña ha sido restablecida."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
            if(messageBox.exec() == QMessageBox::Yes)
            {
                this->close();
                ui->lineNewPass->clear();
                ui->lineConfPass->clear();
            }
        }
        else {
            QMessageBox messageBox(QMessageBox::Critical,
                                   tr(""), tr("Las contraseñas deben coincidir."), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
            ui->lineNewPass->clear();
            ui->lineConfPass->clear();
        }
    }
}
