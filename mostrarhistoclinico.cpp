#include "mostrarhistoclinico.h"
#include "ui_mostrarhistoclinico.h"

MostrarHistoclinico::MostrarHistoclinico(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MostrarHistoclinico)
{
    ui->setupUi(this);
}

MostrarHistoclinico::~MostrarHistoclinico()
{
    delete ui;
}


void MostrarHistoclinico::mostrarClinico(QString idcita,QString diagnostico,QString nombre,QString fecha){

    ui->label_nombre->setText(nombre);
    ui->plainTextEdit_diagnostico->setPlainText(diagnostico);
    ui->label_fecha->setText(fecha);

    QString medicamento,porcion,frecuencia,consulta;
    QSqlQuery query;

    consulta="select *from receta where idCita='"+idcita+"'";
    query.exec(consulta);


    QString r1,g1,b1;
    r1="172,189,211";
    QString r2,g2,b2;
    r2="221,221,221";
    QString rgb="";
    int i=0;
    int ban=1;
    while(query.next())
    {
        if(ban==1)
        {
            rgb=r1;
            ban=2;
        }
        else
        {
            rgb=r2;
            ban=1;
        }

        medicamento=query.value(1).toString();
        porcion=query.value(2).toString();
        frecuencia=query.value(3).toString();


        QLabel *m=new QLabel;
        m->setText(medicamento);
        m->setFixedSize(QSize(110,25));
        m->setStyleSheet("background-color: rgb("+rgb+")");
        ui->barraclinico->addWidget(m,i,1,Qt::AlignTop);


        QLabel *r=new QLabel;
        r->setText(porcion);
        r->setStyleSheet("background-color: rgb("+rgb+")");
        r->setFixedSize(QSize(50,25));
        ui->barraclinico->addWidget(r,i,2,Qt::AlignTop);


        QLabel *h=new QLabel;
        h->setText(frecuencia);
        h->setFixedSize(QSize(50,25));
        h->setStyleSheet("background-color: rgb("+rgb+")");
        ui->barraclinico->addWidget(h,i,3,Qt::AlignTop);
        i++;

    }

}

void MostrarHistoclinico::on_pushButton_regresar_clicked()
{
    close();
}
