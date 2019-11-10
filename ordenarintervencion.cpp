#include "ordenarintervencion.h"
#include "ui_ordenarintervencion.h"

#include <QString>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QSignalMapper>
#include <QDebug>
#include <QDate>

OrdenarIntervencion::OrdenarIntervencion(QString idCita, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OrdenarIntervencion)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    idCita1 = idCita;

    //Conexion a la base de datos
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("lobohospital");
    database.setUserName("root");
    database.setPassword("");
    if(!database.open()){
        qDebug()<<database.lastError().text();
    }
    else {
        qDebug()<<"Base de datos conectada";
    }

    datosPaciente = new QSqlQuery;
    datosPaciente->prepare("SELECT CONCAT_WS( ' ', usuario.nombre, usuario.appaterno, usuario.apmaterno ) AS Paciente, paciente.idpaciente FROM cita INNER JOIN paciente ON paciente.idpaciente = cita.doctor INNER JOIN usuario ON paciente.idUser = usuario.matricula WHERE cita.idCita ='"+idCita+"'");
    datosPaciente->exec();
    datosPaciente->next();
    QString paciente = datosPaciente->value(0).toString();
    datosDoctor = new QSqlQuery;
    datosDoctor->prepare("SELECT CONCAT_WS( ' ', usuario.nombre, usuario.appaterno, usuario.apmaterno ) AS Doctor, doctor.iddoctor FROM cita INNER JOIN doctor ON doctor.iddoctor = cita.doctor INNER JOIN usuario ON doctor.idUser = usuario.matricula WHERE cita.idCita ='"+idCita+"'");
    datosDoctor->exec();
    datosDoctor->next();
    QString doctor = datosDoctor->value(0).toString();

    ui->intervencionPaciente->setText(paciente);
    ui->intervencionNombreDoc->setText(doctor);

    //timer precios
    QStringList wordList;

        QString busqueda,valor;
        QSqlQuery busq;
        busqueda="select *from Producto";
        busq.exec(busqueda);

        while(busq.next())
        {
            valor=busq.value(1).toString();
            wordList << valor;
        }

        QCompleter *completer = new QCompleter(wordList, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);

            ui->lineServ->setCompleter(completer);

            cuenta=new QTimer(this);
               connect(cuenta, SIGNAL(timeout()), this, SLOT(precio()));
                cuenta->start(100);
    //---------------------------------------------------------
    //int anchoTabla = ui->tablaArticulos->width();
    ui->tablaArticulos->verticalHeader()->hide();
    ui->tablaArticulos->setColumnWidth(0,270);
    ui->tablaArticulos->setColumnWidth(1,92);
    ui->tablaArticulos->setColumnWidth(2,92);
    ui->tablaArticulos->setColumnWidth(3,59);
    ui->tablaArticulos->setColumnWidth(4,32);

    QSqlQuery listaQuirofanos;
    listaQuirofanos.prepare("select NumQ from quirofano");
    listaQuirofanos.exec();
    while(listaQuirofanos.next()){
        ui->comboQuiro->addItem(listaQuirofanos.value(0).toString());
    }

    ui->label_5->hide();
    iva = 0;
    ui->intervencionFecha->setDate(QDate::currentDate());
}

OrdenarIntervencion::~OrdenarIntervencion()
{
    delete ui;
}

void OrdenarIntervencion::on_btnContinuarInter_clicked()
{
    if(ui->label_5->isVisible()){
        qDebug() << "no puede continuar";
        QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Seleccione un quirófano o fecha disponible para continuar."), QMessageBox::Yes);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        QString desc = ui->intervencionDesc->toPlainText();
        if(desc == ""){
            QMessageBox messageBox(QMessageBox::Warning,tr("Advertencia"), tr("Ingrese la descripción de la intervención para continuar."), QMessageBox::Yes);
                     messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                     messageBox.exec();
        }
        else{
            ui->tabWidget->setCurrentIndex(1);
        }
    }
}

void OrdenarIntervencion::on_btnBackInter_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void OrdenarIntervencion::on_btnCancelarInter_clicked()
{
    QMessageBox message(QMessageBox::Question,
    tr("Confirmación"), tr("¿Desea cancelar la operación?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        this->close();
    }
    else{}
}

void OrdenarIntervencion::precio()
{
    QString Busq,costo,productoo;
    productoo=ui->lineServ->text();
    QSqlQuery busca1;
    Busq="Select precio from Producto where nombre='"+productoo+"'";
    busca1.exec(Busq);
    busca1.next();
    costo=busca1.value(0).toString();
     ui->lineCosto->setText(costo);

}

void OrdenarIntervencion::on_lineServ_editingFinished()
{
    cuenta->stop();
}

void OrdenarIntervencion::on_lineServ_textChanged(const QString &arg1)
{
    cuenta->start(100);
}

void OrdenarIntervencion::on_btnAgregarArticulo_clicked()
{
    QString nombreItem = ui->lineServ->text();
    QString costo = ui->lineCosto->text();
    QString cantidad = ui->lineCantidad->text();
    if(nombreItem == "" || costo == ""){
        QMessageBox messageBox(QMessageBox::Warning,tr("Advertencia"), tr("Ingrese un artículo o servicio para continuar."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 messageBox.exec();
    }
    else if (cantidad == "") {
        QMessageBox messageBox(QMessageBox::Warning,tr("Advertencia"), tr("Ingrese la cantidad deseada."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 messageBox.exec();
    }
    else{
        int cuentaFilas = ui->tablaArticulos->rowCount();
        int band = 0;
        //verificamos que no sea el mismo elemento
        if(cuentaFilas < 1){
            band=0;
            insertarArt();
        }
        else{
            for(int i = 0; i <= ui->tablaArticulos->rowCount()-1;i++)
            {
                //obtenemos el precio del artículo
                QString elemento = ui->tablaArticulos->item(i, 0)->text();
                if(nombreItem == elemento)
                {
                    band=1;
                    qDebug() << "nigga plz";
                }
            }
            if(band == 0){
                insertarArt();
            }
            else{
                QMessageBox messageBox(QMessageBox::Critical,tr("Error"), tr("Este elemento ya fue ingresado."), QMessageBox::Yes);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
                ui->lineServ->clear();
                ui->lineCosto->clear();
                ui->lineCantidad->setText("1");
                cuenta->stop();
            }
        }
    }
}

void OrdenarIntervencion::eliminarFila(int fila)
{
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea eliminar este artículo?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
            ui->tablaArticulos->removeRow(fila);
            ui->tablaArticulos->setCurrentCell(0, 0);
        double subtotal = 0;
        for(int i = 0; i <= ui->tablaArticulos->rowCount()-1;i++)
        {
            //obtenemos el precio del artículo
            QString price = ui->tablaArticulos->item(i, 2)->text();
            QStringList listPrice = price.split(QRegExp("\\s+"));
            subtotal = subtotal + listPrice.at(1).toDouble();
            qDebug() << "Subto: " << subtotal;
        }
        ui->lblSubtotal->setText(QString::number(subtotal));
        double iva = 0;
        iva = (subtotal*16)/100;
        double CostoTotal = 0;
        CostoTotal = iva+subtotal;
        ui->lblTotal->setText(QString::number(CostoTotal));
    }
    else{
        message.close();
    }
}


void OrdenarIntervencion::on_comboQuiro_currentIndexChanged(int index)
{
    QString numQuiro = ui->comboQuiro->itemText(index);
    QString fechaInter = ui->intervencionFecha->text();
    QSqlQuery buscaCitas;
    buscaCitas.prepare("select fechaCita from citasquirofano where idQuirofano = '"+numQuiro+"' and fechaCita = '"+fechaInter+"'");
    buscaCitas.exec();
    if(buscaCitas.next()){
        ui->label_5->show();
    }
    else{
        ui->label_5->hide();
    }

}

void OrdenarIntervencion::on_intervencionFecha_dateChanged(const QDate &date)
{
    on_comboQuiro_currentIndexChanged(ui->comboQuiro->currentIndex());
}

void OrdenarIntervencion::insertarArt()
{
    QString nombreItem = ui->lineServ->text();
    QString costo = ui->lineCosto->text();
    QString cantidad = ui->lineCantidad->text();

    double costoInt = costo.toDouble();
    double cantidadDoub = cantidad.toDouble();
    double total = costoInt * cantidadDoub;

    QString Total = "$ " + QString::number(total);
    QTableWidgetItem *nombre = new QTableWidgetItem(nombreItem);

    QTableWidgetItem *numero = new QTableWidgetItem(cantidad);
    numero->setTextAlignment(Qt::AlignHCenter);

    QTableWidgetItem *precioTotal = new QTableWidgetItem(Total);
    precioTotal->setTextAlignment(Qt::AlignHCenter);

    QWidget* pWidget = new QWidget();

    QLabel* btnEliminar = new QLabel();
    QIcon icon(":/imgs/x-button.png");
    QPixmap pixmap = icon.pixmap(QSize(64, 64));
    btnEliminar->setPixmap(pixmap);
    btnEliminar->show();
    btnEliminar->setMinimumWidth(30);
    btnEliminar->setMinimumHeight(30);
    btnEliminar->setMaximumWidth(30);
    btnEliminar->setMaximumHeight(30);
    btnEliminar->setScaledContents(true);
    btnEliminar->setStyleSheet("background-color: transparent; border:none;");

    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(btnEliminar);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pLayout);

    QWidget* vacio = new QWidget();
    QLabel* vacia = new QLabel();
    QHBoxLayout* vacioLayout = new QHBoxLayout(vacio);
    vacioLayout->addWidget(vacia);
    vacioLayout->setAlignment(Qt::AlignCenter);
    vacioLayout->setContentsMargins(0, 0, 0, 0);
    vacio->setLayout(vacioLayout);

    ui->tablaArticulos->insertRow(ui->tablaArticulos->rowCount());
    int fila = ui->tablaArticulos->rowCount()-1;
    ui->tablaArticulos->setItem(fila, 0, nombre);
    ui->tablaArticulos->setItem(fila, 1, numero);
    ui->tablaArticulos->setItem(fila, 2, precioTotal);
    ui->tablaArticulos->setCellWidget(fila, 3, vacio);
    ui->tablaArticulos->setCellWidget(fila, 4, btnEliminar);

    //connect(btnEliminar, &QPushButton::clicked, this, &OrdenarIntervencion::eliminarFila);

    ui->lineServ->clear();
    ui->lineCosto->clear();
    ui->lineCantidad->setText("1");
    cuenta->stop();

    subtotal = 0;
    for(int i = 0; i <= ui->tablaArticulos->rowCount()-1;i++)
    {
        //obtenemos el precio del artículo
        QString price = ui->tablaArticulos->item(i, 2)->text();
        QStringList listPrice = price.split(QRegExp("\\s+"));
        subtotal = subtotal + listPrice.at(1).toDouble();
        qDebug() << "Subto: " << subtotal;
    }
    ui->lblSubtotal->setText(QString::number(subtotal));
    iva = (subtotal*16)/100;
    CostoTotal = 0;
    CostoTotal = iva+subtotal;
    ui->lblTotal->setText(QString::number(CostoTotal));
}

void OrdenarIntervencion::on_tablaArticulos_cellPressed(int row, int column)
{
    qDebug() << "yolo bruh";
    if(column == 4){
        eliminarFila(row);
    }
    else {
        qDebug() << "fuck you bruh: " << column;
    }
}

void OrdenarIntervencion::on_tablaArticulos_itemClicked(QTableWidgetItem *item)
{

}

void OrdenarIntervencion::on_tablaArticulos_cellClicked(int row, int column)
{

}

void OrdenarIntervencion::on_btnAgendarQuiro_clicked()
{
    // '"++"'

    QString fechaCita = ui->intervencionFecha->text();
    QString idQuiro = ui->comboQuiro->currentText();
    QString idDoc = datosDoctor->value(1).toString();;
    QString idPac = datosPaciente->value(1).toString();
    QString horaIni = ui->intervencionHora->text();
    QString desc = ui->intervencionDesc->toPlainText();
    QString diasSolic = ui->diasRecup->text();

    QSqlQuery agendarCita;
    agendarCita.prepare("INSERT INTO citasquirofano(fechaCita, idQuirofano, idDoctor, idPaciente, horaInicio, descripcion) VALUES ('"+fechaCita+"','"+idQuiro+"','"+idDoc+"','"+idPac+"','"+horaIni+"','"+desc+"')");
    agendarCita.exec();
    qDebug() << "INSERT INTO citasquirofano(fechaCita, idQuirofano, idDoctor, idPaciente, horaInicio, descripcion) VALUES ('"+fechaCita+"','"+idQuiro+"','"+idDoc+"','"+idPac+"','"+horaIni+"','"+desc+"')";

    QSqlQuery solicitudEstancia;
    solicitudEstancia.prepare("INSERT INTO soliestancia(idDoctor, idPaciente, descripcion, fechaIntervencion, DiasSolicitados, idCitaQ) VALUES ('"+idDoc+"','"+idPac+"','"+desc+"','"+fechaCita+"','"+diasSolic+"','"+agendarCita.lastInsertId().toString()+"')");
    solicitudEstancia.exec();

    QSqlQuery artiServ;
    for(int i = 0; i <= ui->tablaArticulos->rowCount()-1;i++)
    {
        //obtenemos el precio del artículo
        QString nombre = ui->tablaArticulos->item(i, 0)->text();
        QString howmany = ui->tablaArticulos->item(i, 1)->text();
        QString howmuch = ui->tablaArticulos->item(i, 2)->text();
        QStringList listPrice = howmuch.split(QRegExp("\\s+"));
        artiServ.prepare("INSERT INTO artiserv(idCitaQ, DescSerArt, Cantidad, Costo) VALUES ('"+agendarCita.lastInsertId().toString()+"','"+nombre+"','"+howmany+"','"+listPrice.at(1)+"')");
        artiServ.exec();
    }

    QSqlQuery costoServ;
    costoServ.prepare("INSERT INTO `costoservicio`(`idCitaQ`, `Subtotal`, `importeIva`, `Total`) VALUES ('"+agendarCita.lastInsertId().toString()+"','"+QString::number(subtotal)+"','"+QString::number(iva)+"','"+QString::number(CostoTotal)+"')");
    costoServ.exec();
    this->close();
}
