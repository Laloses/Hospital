#include "ordenarestudios.h"
#include "ui_ordenarestudios.h"

OrdenarEstudios::OrdenarEstudios(QString idCita, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OrdenarEstudios)
{
    ui->setupUi(this);

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
    paciente = datosPaciente->value(0).toString();
    datosDoctor = new QSqlQuery;
    datosDoctor->prepare("SELECT CONCAT_WS( ' ', usuario.nombre, usuario.appaterno, usuario.apmaterno ) AS Doctor, doctor.iddoctor FROM cita INNER JOIN doctor ON doctor.iddoctor = cita.doctor INNER JOIN usuario ON doctor.idUser = usuario.matricula WHERE cita.idCita ='"+idCita+"'");
    datosDoctor->exec();
    datosDoctor->next();
    doctor = datosDoctor->value(0).toString();

    ui->nombrePa->setText(paciente);

    QString tipoEst;
        QSqlQuery tipo;
        tipoEst="select DISTINCT tipoEst from EstudiosMedicos;";
        tipo.exec(tipoEst);
        while(tipo.next())
        {
            ui->tipoEst->addItem(tipo.value(0).toString());
        }


        QString eli,elif;
        QSqlQuery eli1,eli2;
        eli="select idO from OrdenEstudios;";
        eli1.exec(eli);
        while(eli1.next())
        {

            elif="delete from OrdenEstudios where idO='"+eli1.value(0).toString()+"' ";
            eli2.exec(elif);
        }
}

OrdenarEstudios::~OrdenarEstudios()
{
    delete ui;
}

void limpiaLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            limpiaLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void OrdenarEstudios::on_agregaEst_clicked()
{
    QString inser;
        QSqlQuery inser1;
        QMessageBox message(QMessageBox::Question,
            tr("Information"), tr("¿Deseas agregar el estudio"), QMessageBox::Yes | QMessageBox::No);
            message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            message.setButtonText(QMessageBox::No, tr("Cancelar"));
            if (message.exec() == QMessageBox::Yes){

            inser="insert into OrdenEstudios(tipo,nombre)values('"+ui->tipoEst->currentText()+"','"+ui->nombreEst->currentText()+"')";
            inser1.exec(inser);
            }


            VerEsrtudios();
}

void OrdenarEstudios::on_btnPrint_clicked()
{
    QPrinter printer(QPrinter::ScreenResolution);
              printer.setOrientation(QPrinter::Landscape);
              printer.setPageSize(QPrinter::A5);
              printer.setOutputFormat(QPrinter::PdfFormat);
              auto nombreArchivo=QFileDialog::getSaveFileName(this,"Guardar archivo",QDir::rootPath(),"Archivos (*.pdf);;");
              printer.setOutputFileName(nombreArchivo);
              printer.setPageMargins(20,20,20,20,QPrinter::Millimeter);


              QString estu,nombrePac,docN,cedula,descripcion;
                descripcion=ui->recomendacion->toPlainText();
              nombrePac=ui->nombrePa->text();
              QSqlQuery Estudios;
              estu="select tipo as Tipo,nombre as Estudio from OrdenEstudios;";
              Estudios.exec(estu);

              imprimirOrden(&printer,Estudios,docN,cedula,nombrePac,QDate::currentDate().toString(),descripcion);
              this->close();
}


void OrdenarEstudios::VerEsrtudios(){
    limpiaLayout(ui->Estudios);
        QString est;
        QSqlQuery est1;
        est="select tipo,nombre from OrdenEstudios";
        est1.exec(est);
        int r=0;
        int c=0;
        while(est1.next())
        {


            QLabel *tipo=new QLabel();
            tipo->setText(est1.value(0).toString()+"  ---------- ");
            ui->Estudios->addWidget(tipo,r,c,1,1);


            QLabel *nombre=new QLabel();
            nombre->setText(est1.value(1).toString());
            ui->Estudios->addWidget(nombre,r,c+1,1,1);
            r++;
            c++;
            if(c==1)
            {
                c=0;
            }


        }

        QLabel *nombre1=new QLabel();

        ui->Estudios->addWidget(nombre1,r,c,1,1);

        QLabel *nombre2=new QLabel();
        r=r+1;
        ui->Estudios->addWidget(nombre2,r,c,1,1);

        QLabel *nombr3=new QLabel();
         r=r+1;
        ui->Estudios->addWidget(nombr3,r,c,1,1);

        QLabel *nombr4=new QLabel();
         r=r+1;
        ui->Estudios->addWidget(nombr4,r,c,1,1);

        QLabel *nombre5=new QLabel();
        r=r+1;
        ui->Estudios->addWidget(nombre5,r,c,1,1);
}

void OrdenarEstudios::imprimirOrden(QPrinter *printer, QSqlQuery &Query, QString DocN, QString Cedula, QString Paciente, QString Fecha,QString indicaciones)
{
    QString strStream;
             QTextStream out(&strStream);

             const int rowCount = Query.size();
             const int columnCount = Query.record().count();

             out <<  "<html>\n"
                 "<head>\n"
                 "<link rel='stylesheet' type='text/css' href='format.css'>"
                 "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                 <<  QString("<title>%1</title>\n").arg("TITLE OF TABLE")
                 <<  "</head>\n"
                  "<body>"
                     "<h2>Orden de Estudios de Laboratorio</h2>"
                 "<p> <b>Doctor: </b><samp>"+doctor+"</samp>   </p>"
                     "<p> <b>Cedula Profesional: </b> <samp>"+Cedula+"</samp>    </p> "
                     "<p> <b>Paciente: </b> <samp>"+paciente+"</samp>    </p>"
                     "<p> <b>Fecha: </b><samp>"+Fecha+"</samp>   </p> "
                 "</body>"
                     "<BR>"
                 "<body bgcolor=#ffffff link=#5000A0>\n"
                 "<table text-align:center border=1 cellspacing=0 cellpadding=2>\n";

             // headers
             out << "<thead><tr bgcolor=#f0f0f0>";
             for (int column = 0; column < columnCount; column++)
               out << QString("<th>%1</th>").arg(Query.record().fieldName(column));
             out << "</tr></thead>\n";

             while (Query.next()) {
               out << "<tr>";
               for (int column = 0; column < columnCount; column++) {
                 QString data = Query.value(column).toString();
                 out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
               }
               out << "</tr>\n";
             }

             out <<  "</table>\n";
             out << "<p> <h3>Indicaciones: </h3></p>";
             out <<"<p>"+indicaciones+" </p>";
             out << "<BR>";
             out << "<BR>";
             out <<"<center><p>__________________________________ </p></center>"
                   "<center><h4>Firma del doctor </h4></center>"
                 "</body>\n"
                 "</html>\n";

             QSizeF paperSize;
             paperSize.setWidth(printer->width());
             paperSize.setHeight(printer->height());
             QTextDocument document;
             document.setHtml(strStream);
             document.setPageSize(paperSize);
             document.print(printer);
}

void OrdenarEstudios::on_tipoEst_currentTextChanged(const QString &arg1)
{
    ui->nombreEst->clear();
        QString nom;
        QSqlQuery nom1;
        nom="select nombreEst from EstudiosMedicos where tipoEst='"+arg1+"'; ";
        nom1.exec(nom);
        while(nom1.next())
        {
            ui->nombreEst->addItem(nom1.value(0).toString());
        }
}

void OrdenarEstudios::on_btnCancelar_clicked()
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
