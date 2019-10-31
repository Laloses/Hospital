#include "agregaractividaddoctor.h"
#include "ui_agregaractividaddoctor.h"

agregarActividadDoctor::agregarActividadDoctor(QWidget *parent, QString id) :
    QDialog(parent),
    ui(new Ui::agregarActividadDoctor)
{
    ui->setupUi(this);
    this->setWindowTitle("Agregar Actividad");
    //Llenamos las actividades
    idDoc=id;
    QString especialidad;
    //Buscamos su especialidad
    q.exec("SELECT idEspecialidad,horario FROM doctor WHERE iddoctor="+idDoc);
    if( !q.next() ) qDebug()<<idDoc<<" "<<q.lastError().text();
    especialidad=q.value(0).toString();
    turno=q.value(1).toString();
    //Cruzamos las actividades por default y su especialidad
    qModel = new QSqlQueryModel;
    qModel->setQuery("SELECT nombreAct FROM actividad WHERE idEsp="+especialidad+" OR idEsp=0");
    ui->comboBox_2->setModel(qModel);

        QStringList hrs;
    //Cargamos las horas dependiendo de su turno
    if(turno=="Matutino"){
        hrs<< "05:00" << "06:00" << "07:00" << "08:00" << "09:00" << "10:00" << "11:00" << "12:00" << "13:00";
        ui->comboBox_3->addItems(hrs);
    }
    if(turno=="Vespertino"){
        hrs<< "13:00" << "14:00" << "15:00" << "16:00" << "17:00" << "18:00" << "19:00" << "20:00" << "21:00";
        ui->comboBox_3->addItems(hrs);
    }
    if(turno=="Nocturno"){
        hrs<< "21:00" << "22:00" << "23:00" << "00:00" << "01:00" << "02:00" << "03:00" << "04:00" << "05:00";
        ui->comboBox_3->addItems(hrs);
    }
    //Ocultamos los errores
    ui->error->setHidden(true);
}

agregarActividadDoctor::~agregarActividadDoctor()
{
    delete ui;
}

void agregarActividadDoctor::on_aceptar_clicked()
{
    QString diaS, horaS;
    bool noHay=true;
    //Buscamos disponibilidad
    q.exec("SELECT dia,hora FROM horariodoc WHERE idDoc="+idDoc);
    while (q.next()) {
        diaS=q.value(0).toString();
        horaS=q.value(1).toString();
        if(diaS==ui->comboBox->currentText() && ui->comboBox_3->currentText()==horaS){
            noHay=false;
            break;
        }
    }
    //Hacemos el insert
    if(noHay){
        QString valores="";
        valores+="'"+ui->comboBox->currentText()+"','"+ui->comboBox_3->currentText()+"','"+turno+"','"+ui->comboBox_2->currentText()+"',"+idDoc;
        if( q.exec("INSERT INTO horariodoc() VALUE("+valores+")") ){
                QMessageBox::information(this,"Correcto","Actividad agregada correctamente.");
                close();
        }else {
            qDebug()<<q.lastError().text();
        }
    }
    //Ya exsite una actividad ahi
    else{
        ui->error->setHidden(false);
    }
}

void agregarActividadDoctor::on_close_clicked()
{
    close();
}
