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

    QSqlQuery p2;
    p2.exec("select matricula from cita where idCita='"+folioR+"'");
    p2.next();


    d.exec("select * from tarjeta where idUser='"+p2.value(0).toString()+"'");
    while(d.next()){
        tarjetas.append(d.record());
        ui->cb_tarjetas->addItem(d.value(3).toString());
    }

}
pagarCitasPaciente::~pagarCitasPaciente()
{
    delete ui;

}

bool pagarCitasPaciente::pagarCitas(QString idUser, QString titular, QString digitos, QString fechaV, QString codigsec)
{
    QSqlQuery insertar,update;
    d.exec("select matricula from cita where idCita='"+ui->folio1->text()+"'");
    d.next();
    idUser=d.value(0).toString();

    //Sino es una tarjeta nueva
    if(ui->cb_tarjetas->currentText() == "Nueva"){
        if(insertar.exec("insert into tarjeta(idUser,titular,digitos,fechaV,codigsec) values("+idUser+",'"+ titular +"','"+digitos+"','"+fechaV+"','"+codigsec+"')") )
        {
            insertar.next();
            if(update.exec("UPDATE cita SET pagada=1 WHERE idCita='"+ui->folio1->text()+"'") ){
                return true;
            }
            else qDebug()<<"Error al actualizar el pago"<<insertar.lastError().text();
        } else qDebug()<<"Error al realizar pago"<<insertar.lastError().text();
    }
    else{
        if(update.exec("UPDATE cita SET pagada=1 WHERE idCita='"+ui->folio1->text()+"'") ){
            return true;
        }
    }
    return false;
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
        if(!ui->line_titulart->text().isEmpty() && ui->line_titulart->text().contains(letras)){
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
            this->close();
        }else{
            QMessageBox::information(this,"Error", "Hubo un problema al procesar tu pago.");
        }

    }
    return flag;
}

void pagarCitasPaciente::on_cb_tarjetas_currentTextChanged(const QString &arg1)
{
    if(arg1!= "Nueva"){
        for(int i=0; i<tarjetas.size(); i++){
            if( tarjetas.at(i).value(3).toString() == arg1 ){
                ui->line_titulart->setText(tarjetas.at(i).value(2).toString());
                ui->line_noTarjeta->setText(tarjetas.at(i).value(3).toString());
                ui->lineEdit_vencim->setDate(tarjetas.at(i).value(4).toDate());
                ui->lineEdit_cvv->setText(tarjetas.at(i).value(5).toString());
                ui->line_titulart->setEnabled(false);
                ui->line_noTarjeta->setEnabled(false);
                ui->lineEdit_vencim->setEnabled(false);
                ui->lineEdit_cvv->setEnabled(false);
                break;
            }
        }
    }
    else {
        ui->line_titulart->setText("");
        ui->line_noTarjeta->setText("");
        ui->lineEdit_cvv->setText("");
        ui->line_titulart->setEnabled(true);
        ui->line_noTarjeta->setEnabled(true);
        ui->lineEdit_vencim->setEnabled(true);
        ui->lineEdit_cvv->setEnabled(true);
    }
}


