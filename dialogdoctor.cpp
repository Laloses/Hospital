#include "dialogdoctor.h"
#include "ui_dialogdoctor.h"
#include "dialoginfodoc.h"
#include <QSqlQuery>

DialogDoctor::DialogDoctor(QString nom,QString espec,QString nocon,QString phone,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDoctor)
{
    ui->setupUi(this);
    nombre=nom;
    espc=espec;
    consult=nocon;
    tel=phone;

}

DialogDoctor::~DialogDoctor()
{
    delete ui;
}

void DialogDoctor::llenarInfoDoc()
{
    ui->nombre_doc->setText(nombre);
    ui->espec->setText(espc);
    ui->num_consul->setText(consult);
    ui->tel_doc->setText(tel);
}

void DialogDoctor::on_masinfo_clicked()
{
    QSqlQuery masinfo,masinfo2;
    QString info,idconsul,correo,cedula,horario,horario2,info2;
    QByteArray fotoF;

    info="select * from consultorio where numConsultorio='"+ui->num_consul->text()+"' ;";
    masinfo.exec(info);
    masinfo.next();
    idconsul=masinfo.value(0).toString();

    info2="SELECT CONCAT(u.nombre,' ', u.apmaterno, ' ',u.appaterno) as Nombre, e.nombre as Especialidad , d.iddoctor,d.idconsultorio,u.telefono,e.idEsp,"
           " u.email,d.horario, u.fotop, d.cedulapro"
            " FROM doctor as d , especialidad as e , usuario as u where u.matricula = d.idUser AND d.idEspecialidad = e.idEsp and d.idConsultorio='"+idconsul+"' " ;
    masinfo2.exec(info2);
    masinfo2.next();

   correo=masinfo2.value(6).toString();
   cedula=masinfo2.value(9).toString();
   horario=masinfo2.value(7).toString();
   fotoF=masinfo2.value(8).toByteArray();

   DialogInfoDoc *info1=new DialogInfoDoc(ui->nombre_doc->text(),ui->espec->text(),ui->num_consul->text(),ui->tel_doc->text(),correo,horario,cedula,fotoF);
   info1->llenarDatos();
   info1->show();

}
