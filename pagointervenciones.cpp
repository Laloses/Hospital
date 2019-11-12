#include "pagointervenciones.h"
#include "ui_pagointervenciones.h"
#include "mainwindow.h"

pagoIntervenciones::pagoIntervenciones(QString p,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pagoIntervenciones)
{
    ui->setupUi(this);
    qDebug()<<"entre contructor";
    folioR=p;
    ui->label_inter->setText(folioR);

    QSqlQuery p2;
    p2.exec("select us.matricula from usuario as  us inner join paciente as p "
            "on us.matricula=p.idUser inner join citasQuirofano as inter on inter.idCita='"+folioR+"'");
    p2.next();


    d.exec("select * from tarjeta where idUser='"+p2.value(0).toString()+"'");
    while(d.next()){
        tarjetas.append(d.record());
        ui->comboBox->addItem(d.value(3).toString());
    }

}

pagoIntervenciones::~pagoIntervenciones()
{
    delete ui;
}

bool pagoIntervenciones::pagarIntervenciones(QString idUser, QString titular, QString digitos, QString fechaV, QString codigsec)
{
    QSqlQuery insertar,update;
    d.exec("select us.matricula from usuario as  us inner join paciente as p "
           "on us.matricula=p.idUser inner join citasQuirofano as inter on inter.idCita='"+ui->label_inter->text()+"'");
    d.next();
    idUser=d.value(0).toString();

    //Sino es una tarjeta nueva
    if(ui->comboBox->currentText() == "Nueva"){
        if(insertar.exec("insert into tarjeta(idUser,titular,digitos,fechaV,codigsec) values("+idUser+",'"+ titular +"','"+digitos+"','"+fechaV+"','"+codigsec+"')") )
        {
            insertar.next();
            if(update.exec("UPDATE cita SET pagada=1 WHERE idCita='"+ui->label_inter->text()+"'") ){
                return true;
            }
            else qDebug()<<"Error al actualizar el pago"<<insertar.lastError().text();
        } else qDebug()<<"Error al realizar pago"<<insertar.lastError().text();
    }
    else{
        if(update.exec("UPDATE cita SET pagada=1 WHERE idCita='"+ui->label_inter->text()+"'") ){
            return true;
        }
    }
    return false;
}

bool pagoIntervenciones::on_pushButton_PI_clicked()
{
    qDebug()<<"entre boton";
    bool flag=false;
    QRegExp letras("^[a-zZ-A ]*$"), numeros("^[0-9]*$");
    QString estiloMalo, estiloBueno;
    estiloMalo="border:2px red; border-style:solid";
    estiloBueno="border:1px black; border-style:solid";

    if(!ui->lineEdit_noTarjeta->text().isEmpty()){
        ui->lineEdit_noTarjeta->setStyleSheet(estiloBueno);
        if(!ui->lineEdit_titular->text().isEmpty() && ui->lineEdit_titular->text().contains(letras)){
            ui->lineEdit_titular->setStyleSheet(estiloBueno);
        if(!ui->dateEditVencimiento->text().isEmpty()){
            ui->dateEditVencimiento->setStyleSheet(estiloBueno);
            if(!ui->lineEdit_cv->text().isEmpty()){
                ui->lineEdit_cv->setStyleSheet(estiloBueno);
                   flag= true;
            }else
            {
                 ui->lineEdit_titular->setStyleSheet(estiloMalo);
            }
        }else
            {
                  ui->lineEdit_cv->setStyleSheet(estiloMalo);
            }
    }else
        {
             ui->dateEditVencimiento->setStyleSheet(estiloMalo);
        }

    }else
    {
          ui->lineEdit_noTarjeta->setStyleSheet(estiloMalo);
    }

    QString idUser,titular,digitos,fechaV,codigsec;
    QSqlQuery d;
    d.exec("select us.matricula from usuario as  us inner join paciente as p "
           "on us.matricula=p.idUser inner join citasQuirofano as inter on inter.idCita='"+ui->label_inter->text()+"'");
    d.next();
    idUser=d.value(0).toString();
    titular=ui->lineEdit_titular->text();
    digitos=ui->lineEdit_noTarjeta->text();
    fechaV=ui->dateEditVencimiento->text();
    codigsec=ui->lineEdit_cv->text();

    if(!flag){
        QMessageBox::warning(this,"Faltan campos","Por favor verifique los campos.");
    }else
    {
        if ( pagarIntervenciones(idUser,titular,digitos,fechaV,codigsec)==true){
            QMessageBox::information(this,"Correcto", "Pago realizado");
            this->close();
        }else{
            QMessageBox::information(this,"Error", "Hubo un problema al procesar tu pago.");
        }

    }
    return flag;
}

void pagoIntervenciones::on_comboBox_currentTextChanged(const QString &arg1)
{
    qDebug()<<"entre";
    if(arg1!= "Nueva"){
        for(int i=0; i<tarjetas.size(); i++){
            if( tarjetas.at(i).value(3).toString() == arg1 ){
                ui->lineEdit_titular->setText(tarjetas.at(i).value(2).toString());
                ui->lineEdit_noTarjeta->setText(tarjetas.at(i).value(3).toString());
                ui->dateEditVencimiento->setDate(tarjetas.at(i).value(4).toDate());
                ui->lineEdit_cv->setText(tarjetas.at(i).value(5).toString());
                ui->lineEdit_titular->setEnabled(false);
                ui->lineEdit_noTarjeta->setEnabled(false);
                ui->dateEditVencimiento->setEnabled(false);
                ui->lineEdit_cv->setEnabled(false);
                break;
            }
        }
    }
    else {
        ui->lineEdit_titular->setText("");
        ui->lineEdit_noTarjeta->setText("");
        ui->lineEdit_cv->setText("");
        ui->lineEdit_titular->setEnabled(true);
        ui->lineEdit_noTarjeta->setEnabled(true);
        ui->dateEditVencimiento->setEnabled(true);
        ui->lineEdit_cv->setEnabled(true);
    }
}

