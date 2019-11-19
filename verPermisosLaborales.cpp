#include "verPermisosLaborales.h"
#include "ui_verPermisosLaborales.h"

VerPermisosLaborales::~VerPermisosLaborales()
{
    delete ui;
}

VerPermisosLaborales::VerPermisosLaborales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerPermisosLaborales)
{
    ui->setupUi(this);
    cargarPermisos();

}

void VerPermisosLaborales::clearLayout(QLayout* layout){
    if (layout) {
        QLayoutItem *item;
            while((item = layout->takeAt(0))){
                if (item->layout()) {
                    clearLayout(item->layout());
                    delete item->layout();
                }
                if(item->widget())
                    item->widget();
                delete item->widget();
            }
    }
}

void VerPermisosLaborales::cargarPermisos(){
    clearLayout(ui->layPermisos);
    QSqlQuery *permisos = new QSqlQuery;
    QSqlQuery *usuario = new QSqlQuery;
    permisos->exec("SELECT idPermiso,idUser,p.idStaff,fechaI,fechaF,p.estado "
                   "FROM permisoLaboral as p INNER JOIN staff "
                   "ON staff.idstaff=p.idStaff "
                   "WHERE p.estado=0");

    //Ciclo para poner botones y cosas
    QLabel* lb;
    QPushButton* pb;
    QHBoxLayout* hlay;
    QString *contenido, *id, *user;
    QPixmap* pix;
    while(permisos->next()){
        if( !usuario->exec("SELECT CONCAT(nombre,' ',appaterno,' ',apmaterno) AS nombre, fotop, matricula "
                      "FROM usuario "
                      "WHERE matricula="+permisos->value("idUser").toString()) ){
            qDebug()<<usuario->lastError().text();
        }
        usuario->next();
        user = new QString(usuario->value("matricula").toString());
        hlay = new QHBoxLayout;

        //nombre
        contenido = new QString(usuario->value("nombre").toString());
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);
        //Foto
        pix = new QPixmap;
        pix->loadFromData(usuario->value("fotop").toByteArray());
        lb = new QLabel;
        lb->setPixmap(pix->scaled(50,50,Qt::KeepAspectRatioByExpanding));
        hlay->addWidget(lb);
        //fechaI
        contenido = new QString(permisos->value("fechaI").toString());
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);
        //fechaF
        contenido = new QString(permisos->value("fechaF").toString());
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);
        //estado
        contenido = new QString("En espera");
        lb = new QLabel(*contenido);
        hlay->addWidget(lb);

        qDebug()<<permisos->value("idPermiso").toString();
        //Botones
        pb= new QPushButton;
        pb->setText(" Rechazar ");
        id = new QString(permisos->value("idPermiso").toString());
        connect(pb,&QPushButton::clicked,[=](){emit rechazarPermisoLaboral(*id,*user);});
        hlay->addWidget(pb);

        pb= new QPushButton;
        pb->setText(" Aceptar ");
        connect(pb,&QPushButton::clicked,[=](){emit aceptarPermisoLaboral(*id,*user);});
        hlay->addWidget(pb);

        ui->layPermisos->addLayout(hlay);
    }
    //Cuando termine hay que agregar una barra espaciadora para empujar el contenido
    QSpacerItem *barraVertical= new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    ui->layPermisos->addSpacerItem(barraVertical);
}

void VerPermisosLaborales::aceptarPermisoLaboral(QString idPermiso, QString matricula){
    QMessageBox::StandardButton res = QMessageBox::question(this,"Aceptar solicitud","¿Está seguro de aceptar la solicitud?");
    if(res == QMessageBox::Yes){
        QSqlQuery* deletear = new QSqlQuery;
        qDebug()<<"ide: "+idPermiso;

        //Mandar notificacion
        QString fechaI,fechaF,tipo;
        QSqlQuery *permiso = new QSqlQuery;
        permiso->exec("SELECT fechaI, fechaF FROM permisoLaboral WHERE idPermiso="+idPermiso);
        permiso->next();
        fechaI=permiso->value("fechaI").toString();
        fechaF=permiso->value("fechaF").toString();

        QString mensaj;
        mensaj="Se Le informa que su permiso para el: "+fechaI+" hasta : "+fechaF+" fue aceptado.";
        tipo="1";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+matricula+"');";
        QSqlQuery mandarNoti;
        mandarNoti.exec(notificacion);

        if( deletear->exec("UPDATE permisoLaboral SET estado=1 WHERE idPermiso="+idPermiso) ){

            QMessageBox::information(this, "Éxito","Aceptado correctamente.");
            cargarPermisos();
        }
        else{
            qDebug()<<deletear->lastError().text();
            QMessageBox::critical(this, "Error","Error al aceptar.");
        }
    }
}

void VerPermisosLaborales::rechazarPermisoLaboral(QString idPermiso, QString matricula){
    QMessageBox::StandardButton res = QMessageBox::question(this,"Cancelar solicitud","¿Está seguro de cancelar la solicitud?");
    if(res == QMessageBox::Yes){
        QSqlQuery* deletear = new QSqlQuery;
        qDebug()<<"ide: "+idPermiso;

        //Mandar notificacion
        QString fechaI,fechaF,tipo;
        QSqlQuery *permiso = new QSqlQuery;
        permiso->exec("SELECT fechaI, fechaF FROM permisoLaboral WHERE idPermiso="+idPermiso);
        permiso->next();
        fechaI=permiso->value("fechaI").toString();
        fechaF=permiso->value("fechaF").toString();

        QString mensaj;
        mensaj="Se Le informa que su permiso para el: "+fechaI+" hasta : "+fechaF+" fue rechazado.";
        tipo="0";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+matricula+"');";
        QSqlQuery mandarNoti;
        mandarNoti.exec(notificacion);

        if( deletear->exec("DELETE FROM permisoLaboral WHERE idPermiso="+idPermiso) ){
            QMessageBox::information(this, "Éxito","Rechazado correctamente.");
            cargarPermisos();
        }
        else{
            qDebug()<<deletear->lastError().text();
            QMessageBox::critical(this, "Error","Error al rechazar.");
        }
    }
}
