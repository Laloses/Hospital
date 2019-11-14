#include "eliminarusuarios.h"
#include "ui_eliminarusuarios.h"

eliminarUsuarios::eliminarUsuarios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eliminarUsuarios)
{
    ui->setupUi(this);
}

eliminarUsuarios::~eliminarUsuarios()
{
    delete ui;
}

void clearLayou(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayou(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}


void eliminarUsuarios::on_radioButton_doc_clicked()
{
    qDebug()<<"entre:";
    clearLayou(ui->gridLayout_eliminar);
    int cont=0;
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryDoc,queryStaff,userEst;
    consultaDoc="select *from Doctores";
    queryDoc.exec(consultaDoc);
            while(queryDoc.next())
            {
                if(queryDoc.value(5).toString()=="1")
                {

                }
                else {
                nombre=queryDoc.value(0).toString();
                espec=queryDoc.value(4).toString();
                espera="en espera";

                matricula=queryDoc.value(3).toString();

                QPushButton *b=new QPushButton();
                b->setText("Ver Solicitud");
                QLabel *l=new QLabel;
                l->setText(nombre);
                b->setFixedSize(QSize(120,40));
                QSignalMapper *mapper=new QSignalMapper(this);
                connect(b,SIGNAL(clicked(bool)),mapper,SLOT(map()));
                mapper->setMapping(b,matricula);
                connect(mapper,SIGNAL(mapped(QString)),this,SLOT(PonerInfo(QString)));
                QLabel *espacio=new QLabel();
                QLabel *esp=new QLabel();
                esp->setText(espec);
                QLabel *estado=new QLabel();
                estado->setText(espera);
                ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
                ui->gridLayout_eliminar->addWidget(l,cont,1,1,1);
                ui->gridLayout_eliminar->addWidget(esp,cont,2,1,1);
                ui->gridLayout_eliminar->addWidget(estado,cont,3,1,1);

                cont++;
                }
            }

}
