#include "tipdeldia.h"
#include "ui_tipdeldia.h"

tipdeldia::tipdeldia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tipdeldia)
{
    ui->setupUi(this);
    setWindowTitle("Tip del día");
    rando=randon();
}

tipdeldia::~tipdeldia()
{
    delete ui;
}

//metodo para generar un numero aleatorio dependiendo
//del tamaño de la tabla tip
int tipdeldia::randon(){
      QString numero;
      int rand=0;
      QSqlQuery query;
       srand(time(NULL));
      query.prepare(QString("select count(*) as numero from tip"));
      query.exec();
      query.next();
      rand=query.value(0).toInt();
      query.lastError().text();
      numero= QString::number( 1 + qrand() % rand);
      return  numero.toInt();
}

//metodo para mostra los datos del tip
void tipdeldia::mostrarTip(){
    QString mostra,nombretip,descripcion,foto,ran,imagen;
    ran=QString::number(rando);
    QSqlQuery query;
    QPixmap pix;
    mostra.append("select *from tip where idtip='"+ran+"'");
    query.prepare(mostra);
    query.exec();
    query.next();
    nombretip=query.value(1).toString();
    descripcion=query.value(2).toString();
    link=query.value(3).toString();
    imagen=pix.loadFromData(query.value(4).toByteArray());
    int w=ui->label_imagen->width();
    int h=ui->label_imagen->height();
     ui->label_imagen->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
    query.lastError().text();
    ui->label_nombretip->setText(nombretip);
    ui->label_descripcion->setText(descripcion);


}
//metodo que copia la direcion de una url que se encuentra en la base de datos
void tipdeldia::on_pushButton_link_clicked()
{

    QDesktopServices::openUrl(QUrl(link));

}
