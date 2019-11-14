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


void eliminarUsuarios::eliminarDoc(QString matricula){

    qDebug()<<"eliminar doctor";


}

void eliminarUsuarios::eliminarStaff(QString matricula){

    qDebug()<<"eliminar staff";

}
void eliminarUsuarios::eliminarPaciente(QString matricula){

    qDebug()<<"eliminar Paciente";


}

void eliminarUsuarios::ModificarUsuario(QString){

    qDebug()<<"modificar";

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
                nombre=queryDoc.value(0).toString()+" "+queryDoc.value(1).toString()+" "+queryDoc.value(2).toString();
                espec=queryDoc.value(4).toString();
                espera="en espera";

                matricula=queryDoc.value(3).toString();

                QPushButton *b=new QPushButton();
                QPushButton *p=new QPushButton();
                b->setText("Eliminar Doctor");
                p->setText("Modificar Doctor");
                QLabel *l=new QLabel;
                l->setText(nombre);
                connect(b,&QPushButton::clicked,[=](){emit eliminarDoc(matricula);});
                connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula);});
                QLabel *espacio=new QLabel();
                QLabel *esp=new QLabel();
                esp->setText(espec);
                QLabel *estado=new QLabel();
                estado->setText(espera);
                ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
                 ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
                ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
                ui->gridLayout_eliminar->addWidget(esp,cont,3,1,1);
                ui->gridLayout_eliminar->addWidget(estado,cont,4,1,1);
                cont++;
                }
            }

}


void eliminarUsuarios::on_radioButton_staff_clicked()
{
    qDebug()<<"entre:";
    clearLayou(ui->gridLayout_eliminar);
    int cont=0;
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryStaff,userEst;
    consultaDoc="select *from  Staffs";
    queryStaff.exec(consultaDoc);
    while(queryStaff.next())
    {
        if(queryStaff.value(5).toString()=="1")
        {

        }
        else {
            nombre=queryStaff.value(0).toString()+" "+queryStaff.value(1).toString()+" "+queryStaff.value(2).toString();
            espec=queryStaff.value(4).toString();
            espera="en espera";

            matricula=queryStaff.value(3).toString();

            QPushButton *b=new QPushButton();
            QPushButton *p=new QPushButton();
            b->setText(" Eliminar Staff ");
            p->setText("Modificar Staff");
            QLabel *l=new QLabel;
            l->setText(nombre);
            connect(b,&QPushButton::clicked,[=](){emit eliminarStaff(matricula);});
            connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula);});
            QLabel *espacio=new QLabel();
            QLabel *esp=new QLabel();
            esp->setText(espec);
            QLabel *estado=new QLabel();
            estado->setText(espera);
            ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
            ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
            ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
            ui->gridLayout_eliminar->addWidget(esp,cont,3,1,1);
            cont++;
        }
    }

}

void eliminarUsuarios::on_radioButton_pac_clicked()
{
    qDebug()<<"entre: paciente";
    clearLayou(ui->gridLayout_eliminar);
    int cont=0;
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryPac;
    consultaDoc="select *from pacientes";
    queryPac.exec(consultaDoc);
    while(queryPac.next())
    {
            nombre=queryPac.value(0).toString()+" "+queryPac.value(1).toString()+" "+queryPac.value(2).toString();
            matricula=queryPac.value(3).toString();

            QPushButton *b=new QPushButton();
            QPushButton *p=new QPushButton();
            b->setText("Eliminar Paciente");
            p->setText("Modificar Paciente");
            QLabel *l=new QLabel;
            l->setText(nombre);

            connect(b,&QPushButton::clicked,[=](){emit eliminarPaciente(matricula);});
            connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula);});
            ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
            ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
            ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
            cont++;
        }
}

