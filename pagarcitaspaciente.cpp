#include "pagarcitaspaciente.h"
#include "ui_pagarcitaspaciente.h"
#include "mainwindow.h"


#include <QDebug>
#include <QSqlQuery>

pagarCitasPaciente::pagarCitasPaciente(QString p,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pagarCitasPaciente)
{
    folioR=p;
    ui->setupUi(this);
    ui->folio1->setText(folioR);

    //q.exec("select idUser,nombre from paciente as p inner join usuario as u "
      //     "on u.matricula=p.idUser where idUser='"+id_usuario+"'");


}
pagarCitasPaciente::~pagarCitasPaciente()
{
    delete ui;
}

bool pagarCitasPaciente::pagarCitas(QString idUser, QString titular, QString digitos, QString fechaV, QString codigsec)
{
    QSqlQuery insertar,d,update;
    d.exec("select matricula from cita where idCita='"+ui->folio1->text()+"'");
    d.next();
    idUser=d.value(0).toString();
    if(insertar.exec("insert into tarjeta(idUser,titular,digitos,fechaV,codigsec) values("+idUser+",'"+ titular +"','"+digitos+"',"+fechaV+",'"+codigsec+"')") )
    {
        insertar.next();
        if(update.exec("UPDATE cita SET pagada=1 WHERE idCita='"+ui->folio1->text()+"'") ){

        }
        else qDebug()<<"Error al actualizar el pago"<<insertar.lastError().text();
    } else qDebug()<<"Error al realizar pago"<<insertar.lastError().text();

}



bool pagarCitasPaciente::on_pushButton_pagarC_clicked()
{
    bool flag=false;
    QRegExp letras("^[a-zZ-A ]*$"), numeros("^[0-9]*$");
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";

    if(!ui->line_noTarjeta->text().isEmpty()){
        ui->line_noTarjeta->setStyleSheet(estiloBueno);
        if(!ui->line_titulart->text().isEmpty()){
            ui->line_titulart->setStyleSheet(estiloBueno);
        if(!ui->lineEdit_vencim->text().isEmpty()){
            ui->lineEdit_vencim->setStyleSheet(estiloBueno);
            if(!ui->lineEdit_cvv->text().isEmpty()){
                ui->lineEdit_cvv->setStyleSheet(estiloBueno);
                   flag= true;
            }else
            {
                 ui->line_titulart->setStyleSheet(estiloMalo);
            }
        }else
            {
                  ui->lineEdit_cvv->setStyleSheet(estiloMalo);
            }
    }else
        {
             ui->lineEdit_vencim->setStyleSheet(estiloMalo);
        }

    }else
    {
          ui->line_noTarjeta->setStyleSheet(estiloMalo);
    }
    QString idUser,titular,digitos,fechaV,codigsec;
    QSqlQuery d;
    d.exec("select matricula from cita where idCita='"+ui->folio1->text()+"'");
    d.next();
    idUser=d.value(0).toString();
    titular=ui->line_titulart->text();
    digitos=ui->line_noTarjeta->text();
    fechaV=ui->lineEdit_vencim->text();
    codigsec=ui->lineEdit_cvv->text();

    if(!flag){
        QMessageBox::warning(this,"Faltan campos","Por favor verifique los campos.");
    }else
    {
        if ( pagarCitas(idUser,titular,digitos,fechaV,codigsec) ){
            QMessageBox::information(this,"Correcto", "Pago realizado");
        }else{
            QMessageBox::information(this,"Error", "Hubo un problema al procesar tu pago.");
        }

    }
    return flag;

    this->close();






}
