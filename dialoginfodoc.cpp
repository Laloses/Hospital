#include "dialoginfodoc.h"
#include "ui_dialoginfodoc.h"

DialogInfoDoc::DialogInfoDoc(QString a,QString b,QString c,QString d,QString f,QString g ,QString h,QByteArray i, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfoDoc)
{
    ui->setupUi(this);
    nombre=a;
    especialidad=b;
    consul=c;
    celu=d;
    email=f;
    horario=g;
    cedu=h;
    fotodoc=i;
}

DialogInfoDoc::~DialogInfoDoc()
{
    delete ui;
}

void DialogInfoDoc::llenarDatos(){
    ui->nombre_doctor->setText(nombre);
    ui->doc_espec->setText(especialidad);
            ui->num_consult->setText(consul);
            ui->num_Tel->setText(celu);
            ui->correo->setText(email);
            QString turno;
            turno ="lunes a sabado turno: "+horario+"";
            ui->turnos->setText(turno);
            ui->cedula->setText(cedu);

            QPixmap fot;
            QString fot2;
            fot2=fot.loadFromData(fotodoc);
            int a=ui->foto_doc->height();
            int b=ui->foto_doc->width();

            ui->foto_doc->setPixmap(fot.scaled(b,a,Qt::AspectRatioMode::IgnoreAspectRatio));



}
