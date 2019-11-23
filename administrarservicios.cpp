#include "administrarservicios.h"
#include "ui_administrarservicios.h"
#include <QLabel>
#include <QMessageBox>
#include <QSignalMapper>
#include <QFileDialog>
#include <QDebug>

//mensaje de si o no
/*
QMessageBox message(QMessageBox::Question,
 tr("Information"), tr("Deseas aceptar la solicitud?"), QMessageBox::Yes | QMessageBox::No);
message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
message.setButtonText(QMessageBox::No, tr("Cancelar"));
if (message.exec() == QMessageBox::Yes){
*/

//mensaje de aceptar
/*
QMessageBox messageBox(QMessageBox::Information,tr("Éxito"), tr("Los datos se han actualizado correctamente."), QMessageBox::Yes);
messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
messageBox.exec();
*/

AdministrarServicios::AdministrarServicios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdministrarServicios)
{
    ui->setupUi(this);
    ui->ServiciosPrincipal->setCurrentIndex(0);
    verServicios();
}


void clearLayout2(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout2(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

AdministrarServicios::~AdministrarServicios()
{
    delete ui;
}

void AdministrarServicios::verServicios()
{
    clearLayout2(ui->listaServicios);

    QString lista1,nombre;
    QSqlQuery lista2;
    lista1="select *from Servicios";
    lista2.exec(lista1);
    int r=0;

    while(lista2.next())
    {
        nombre=lista2.value(1).toString();

        QLabel *nombre1=new QLabel();
        nombre1->setText(nombre+" --------------------");
        nombre1->setFixedSize(140,30);

        ui->listaServicios->addWidget(nombre1,r,0,1,1);

        QPushButton *ver=new QPushButton();
        ver->setFixedSize(90,30);
        ver->setText("ver");
        ver->setStyleSheet("background-color: rgb(138,222,242);border: 1px solid rgb(60,200,234)");
        QSignalMapper *mapper1=new QSignalMapper(this);
        connect(ver,SIGNAL(clicked(bool)),mapper1,SLOT(map()));
        mapper1->setMapping(ver,nombre);
        connect(mapper1,SIGNAL(mapped(QString)),this,SLOT(infoServicio(QString)));


        ui->listaServicios->addWidget(ver,r,1,1,1);


        QPushButton *editar=new QPushButton();
        editar->setFixedSize(90,30);
        editar->setText("editar");
        editar->setStyleSheet("background-color: rgb(60,160,234)");
        QSignalMapper *mapper2=new QSignalMapper(this);
        connect(editar,SIGNAL(clicked(bool)),mapper2,SLOT(map()));
        mapper2->setMapping(editar,nombre);
        connect(mapper2,SIGNAL(mapped(QString)),this,SLOT(editarServicio(QString)));

         ui->listaServicios->addWidget(editar,r,2,1,1);

        QPushButton *eliminar=new QPushButton();
        eliminar->setFixedSize(90,30);
        eliminar->setText("eliminar");
        eliminar->setStyleSheet("background-color: rgb(138,198,242)");
        QSignalMapper *mapper3=new QSignalMapper(this);
        connect(eliminar,SIGNAL(clicked(bool)),mapper3,SLOT(map()));
        mapper3->setMapping(eliminar,nombre);
        connect(mapper3,SIGNAL(mapped(QString)),this,SLOT(elminiarServico(QString)));

         ui->listaServicios->addWidget(eliminar,r,3,1,1);

        r++;


    }




}

void AdministrarServicios::on_regresarLista_clicked()
{
    ui->info1->clear();
    ui->info2->clear();
    ui->lema->clear();
    ui->foto1->clear();
    ui->foto2->clear();
    ui->tituloS->clear();

    ui->ServiciosPrincipal->setCurrentIndex(0);
}

void AdministrarServicios::infoServicio(QString nombre)
{
    QString bus,titu,info1,lema1,info2;
    QByteArray img1,img2;
    QSqlQuery bus1;
    bus="select *from Servicios where nombreS='"+nombre+"'; ";
    bus1.exec(bus);
    bus1.next();

    titu=bus1.value(1).toString();
    info1=bus1.value(2).toString();
    lema1=bus1.value(3).toString();
    info2=bus1.value(4).toString();
    img1=bus1.value(5).toByteArray();
    img2=bus1.value(6).toByteArray();

    QPixmap imag;
    QPixmap imag2;
    QString f1,f2;
    f1=imag.loadFromData(img1);
    f2=imag2.loadFromData(img2);
    int w=ui->foto1->width();
    int h=ui->foto1->height();
    ui->foto1->setPixmap(imag.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
    ui->tituloS->setText(titu);
    ui->lema->setPlainText(lema1);
    ui->info1->setPlainText(info1);
    int w2=ui->foto2->width();
    int h2=ui->foto2->height();
    ui->foto2->setPixmap(imag2.scaled(w2,h2,Qt::AspectRatioMode::IgnoreAspectRatio));
    ui->info2->setPlainText(info2);

    ui->ServiciosPrincipal->setCurrentIndex(1);
}

void AdministrarServicios::elminiarServico(QString nombre)
{
    QString eli,idSer,eli2;
    QSqlQuery eli1,eli3;
    eli="select *from Servicios where nombreS='"+nombre+"'; ";
    eli1.exec(eli);
    eli1.next();
    idSer=eli1.value(0).toString();

    eli2="delete from Servicios where idServicio='"+idSer+"'; ";
    QMessageBox message(QMessageBox::Question,
     tr("Informacion"), tr("Deseas Eliminar este Servicio"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){


    if(eli3.exec(eli2))
    {

        QMessageBox messageBox(QMessageBox::Information,tr("Éxito"), tr("Se elimino con exito el Servicio"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
        verServicios();
    }
    else
    {

        QMessageBox messageBox(QMessageBox::Information,tr("Fracaso"), tr("Ocurrio un problema en el proceso de eliminicacion, vuelve a intentarlo"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    }




}

void AdministrarServicios::editarServicio(QString nombre)
{

    imgRoute="";
    foto=nullptr;
    QString bus,titu,info1,lema1,info2;
    QByteArray img1,img2;
    QSqlQuery bus1;
    bus="select *from Servicios where nombreS='"+nombre+"'; ";
    bus1.exec(bus);
    bus1.next();

    titu=bus1.value(1).toString();
    tituloActual=titu;
    info1=bus1.value(2).toString();
    lema1=bus1.value(3).toString();
    info2=bus1.value(4).toString();
    img1=bus1.value(5).toByteArray();
    img2=bus1.value(6).toByteArray();

    QPixmap imag;
    QPixmap imag2;
    QString f1,f2;
    f1=imag.loadFromData(img1);
    f2=imag2.loadFromData(img2);

    int w=ui->nfoto1->width();
    int h=ui->nfoto1->height();
    ui->nfoto1->setPixmap(imag.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
    ui->tituloE->setText(titu);
    ui->lemae->setPlainText(lema1);
    ui->infoe->setPlainText(info1);
    int w2=ui->nfoto2->width();
    int h2=ui->nfoto2->height();
    ui->nfoto2->setPixmap(imag2.scaled(w2,h2,Qt::AspectRatioMode::IgnoreAspectRatio));
    ui->ifno2e->setPlainText(info2);



    ui->ServiciosPrincipal->setCurrentIndex(2);

}

void AdministrarServicios::on_regresarE_clicked()
{
    ui->ServiciosPrincipal->setCurrentIndex(0);
}

void AdministrarServicios::on_q1_clicked()
{
    imgRoute="";
    foto=nullptr;
    ui->nfoto1->clear();
}

void AdministrarServicios::on_q2_clicked()
{
    imgRoute2="";
    foto2=nullptr;
    ui->nfoto2->clear();
}

void AdministrarServicios::on_a1_clicked()
{
    //Abrimos el dialogo para obtener la ruta
    imgRoute = QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto = file.readAll();

    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto);
    int w=ui->nfoto1->width();
    int h=ui->nfoto1->height();
    ui->nfoto1->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
}

void AdministrarServicios::on_a2_clicked()
{
    //Abrimos el dialogo para obtener la ruta
    imgRoute2= QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute2);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto2= file.readAll();

    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto2);
    int w=ui->nfoto2->width();
    int h=ui->nfoto2->height();
    ui->nfoto2->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
}

void AdministrarServicios::on_cancelare_clicked()
{
    QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("Deseas restaurar los cambios"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
    editarServicio(tituloActual);
    }
}

void AdministrarServicios::on_guardare_clicked()
{
    QString titu,info1,lema1,info2,imagen1,imagen2;

    titu=ui->tituloE->text();
    info1=ui->infoe->toPlainText();
    lema1=ui->lemae->toPlainText();
    info2=ui->ifno2e->toPlainText();
    imagen1=imgRoute;
    imagen2=imgRoute2;

    QString inser;
    inser="update Servicios set nombreS='"+titu+"',info1='"+info1+"',lema1='"+lema1+"',info2='"+info2+"',imag1=LOAD_FILE('"+imagen1+"'),imag2=LOAD_FILE('"+imagen2+"') where nombreS='"+tituloActual+"';";
    QSqlQuery inser2;
    qDebug()<<inser;
    QMessageBox message(QMessageBox::Question,
     tr("Informacion"), tr("Deseas guardar los cambios"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

    if(imgRoute=="" && imgRoute2=="")
    {
        inser="update Servicios set nombreS='"+titu+"',info1='"+info1+"',lema1='"+lema1+"',info2='"+info2+"' where nombreS='"+tituloActual+"';";
    }
    else if(imgRoute!="" && imgRoute2=="")
    {
        inser="update Servicios set nombreS='"+titu+"',info1='"+info1+"',lema1='"+lema1+"',info2='"+info2+"',imag1=LOAD_FILE('"+imagen1+"') where nombreS='"+tituloActual+"';";
    }
    else if(imgRoute=="" && imgRoute2!="")
    {
        inser="update Servicios set nombreS='"+titu+"',info1='"+info1+"',lema1='"+lema1+"',info2='"+info2+"',imag2=LOAD_FILE('"+imagen2+"') where nombreS='"+tituloActual+"';";
    }
    else if(imgRoute!="" && imgRoute2!="")
    {
        inser="update Servicios set nombreS='"+titu+"',info1='"+info1+"',lema1='"+lema1+"',info2='"+info2+"',imag1=LOAD_FILE('"+imagen1+"'),imag2=LOAD_FILE('"+imagen2+"') where nombreS='"+tituloActual+"';";
    }


    if(inser2.exec(inser))
    {
        QMessageBox messageBox(QMessageBox::Information,tr("Éxito"), tr("Los datos se han actualizado correctamente."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
        ui->ServiciosPrincipal->setCurrentIndex(0);
    }else
    {
        QMessageBox messageBox(QMessageBox::Information,tr("Fracaso"), tr("Ocurrio un problema con la actualizacion"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }

    }


    else
    {

    }

}

void AdministrarServicios::on_regresarNuevo_clicked()
{
    ui->ServiciosPrincipal->setCurrentIndex(0);
    ui->nfoto1_2->clear();
    ui->nfoto2_2->clear();
    ui->tituloE_2->clear();
    ui->lemae_2->clear();
    ui->infoe_2->clear();
    ui->ifno2e_2->clear();
}

void AdministrarServicios::on_nuevoServicio_clicked()
{
    ui->nfoto1_2->clear();
    ui->nfoto2_2->clear();
    ui->tituloE_2->clear();
    ui->lemae_2->clear();
    ui->infoe_2->clear();
    ui->ifno2e_2->clear();
    ui->ServiciosPrincipal->setCurrentIndex(3);
}

void AdministrarServicios::on_cancelare_2_clicked()
{
    QMessageBox message(QMessageBox::Question,
     tr("Informacion"), tr("Deseas borrar todos tus cambios"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

    ui->nfoto1_2->clear();
    ui->nfoto2_2->clear();
    ui->tituloE_2->clear();
    ui->lemae_2->clear();
    ui->infoe_2->clear();
    ui->ifno2e_2->clear();
    }
    else
    {

    }
}

void AdministrarServicios::on_guardare_2_clicked()
{
    QMessageBox message(QMessageBox::Question,
     tr("Informacion"), tr("Deseas guardar los cambios"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){

    if(imgRoute=="" && imgRoute2=="")
    {
        QMessageBox messageBox(QMessageBox::Information,tr("Datos incompletos"), tr("Necesitas insertar las dos fotos"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else if(imgRoute!="" && imgRoute2=="")
    {
        QMessageBox messageBox(QMessageBox::Information,tr("Datos incompletos"), tr("Necesitas insertar las dos fotos"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else if(imgRoute=="" && imgRoute2!="")
    {
        QMessageBox messageBox(QMessageBox::Information,tr("Datos incompletos"), tr("Necesitas insertar las dos fotos"), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else if(imgRoute!="" && imgRoute2!="")
    {
        if(ui->tituloE_2->text()=="")
        {
            QMessageBox messageBox(QMessageBox::Information,tr("Datos incompletos"), tr("Necesitas insertar un titutlo"), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
        else
        {
        QString titu,info1,lema1,info2,imagen1,imagen2;

        titu=ui->tituloE_2->text();
        info1=ui->infoe_2->toPlainText();
        lema1=ui->lemae_2->toPlainText();
        info2=ui->ifno2e_2->toPlainText();
        imagen1=imgRoute;
        imagen2=imgRoute2;

        QString inser;
        inser="insert into Servicios(nombreS,info1,lema1,info2,imag1,imag2)values('"+titu+"','"+info1+"','"+lema1+"',info2='"+info2+"',LOAD_FILE('"+imagen1+"'),LOAD_FILE('"+imagen2+"'));";
        QSqlQuery inser2;
        qDebug()<<inser;

        if(inser2.exec(inser))
        {
            QMessageBox messageBox(QMessageBox::Information,tr("Exito"), tr("El servicio se agrego con exito"), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
            verServicios();
            ui->ServiciosPrincipal->setCurrentIndex(0);
        }

        else {
            QMessageBox messageBox(QMessageBox::Information,tr("Fracaso"), tr("No se logor agregrar el nuevo servicio"), QMessageBox::Yes);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
        }

    }
    }
}

void AdministrarServicios::on_q1_2_clicked()
{
    ui->nfoto1_2->clear();
}

void AdministrarServicios::on_q2_2_clicked()
{
    ui->nfoto2_2->clear();
}

void AdministrarServicios::on_a1_2_clicked()
{
    //Abrimos el dialogo para obtener la ruta
    imgRoute = QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto = file.readAll();

    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto);
    int w=ui->nfoto1->width();
    int h=ui->nfoto1->height();
    ui->nfoto1_2->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
}

void AdministrarServicios::on_a2_2_clicked()
{
    //Abrimos el dialogo para obtener la ruta
    imgRoute2= QFileDialog::getOpenFileName(this, "Seleccionar Imagen ", "c://","Image Files (*.png *.jpg )");
    QFile file(imgRoute2);
    file.open(QIODevice::ReadOnly);

    //Guardamos los datos de la foto en la variable
    foto2= file.readAll();

    //Cargamos la foto al label
    QPixmap pix;
    pix.loadFromData(foto2);
    int w=ui->nfoto2->width();
    int h=ui->nfoto2->height();
    ui->nfoto2_2->setPixmap(pix.scaled(w,h,Qt::AspectRatioMode::IgnoreAspectRatio));
}
