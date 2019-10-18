#include "tipdeldia.h"
#include "ui_tipdeldia.h"

tipdeldia::tipdeldia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tipdeldia)
{
    ui->setupUi(this);
}

tipdeldia::~tipdeldia()
{
    delete ui;
}


int tipdeldia::randon(){
      QString numero;
      int rando=0;
      QSqlQuery query;
      query.prepare(QString("select count(*) as numero from tip"));
      query.exec();
      query.next();
      rando=query.value(0).toInt();
      query.lastError().text();
      numero= QString::number( 1+qrand() % rando);
      qDebug()<<"Randon: "<<numero;
      return  numero.toInt();
}
//metodo para mostra los datos del tip
void tipdeldia::mostrarTip(){

    QString mostra,nombretip,descripcion,link,foto,ran;
    //srand(time(nullptr));
    int rando=randon();
    ran=QString::number(rando);
    QSqlQuery query;
    mostra.append("select *from tip where idtip='"+ran+"'");
    query.prepare(mostra);
    query.exec();
    query.next();
    nombretip=query.value(1).toString();
    descripcion=query.value(2).toString();
    link=query.value(3).toString();
    qDebug()<<"Nombre"<<nombretip;
    qDebug()<<"Descirpcion"<<descripcion;
    qDebug()<<"Link"<<link;
    query.lastError().text();
    ui->label_nombretip->setText(nombretip);
    ui->textBrowser_descripcion->setText(descripcion);
    ui->label_link->setText(link);

}

void tipdeldia::on_pushButton_cerrar_clicked()
{
    close();
}
