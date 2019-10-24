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
    q.next();
    especialidad=q.value(0).toString();
    turno=q.value(1).toString();
    //Cruzamos las actividades por default y su especialidad
    qModel = new QSqlQueryModel;
    qModel->setQuery("SELECT nombreAct FROM actividad UNION "
                     "SELECT nombre FROM especialidad WHERE idEsp="+especialidad);
    ui->comboBox_2->setModel(qModel);

    //Cargamos las horas dependiendo de su turno
    if(turno=="Matutino"){
        ui->timeEdit->setTimeRange(QTime(05,00),QTime(13,00));
        ui->timeEdit->setTime(QTime(05,00));
    }
    if(turno=="Vespertino"){
        ui->timeEdit->setTimeRange(QTime(13,00),QTime(21,00));
        ui->timeEdit->setTime(QTime(13,00));
    }
    if(turno=="Nocturno"){
        ui->timeEdit->setTimeRange(QTime(21,00),QTime(05,00));
        ui->timeEdit->setTime(QTime(21,00));
    }
    //Ocultamos los errores
    ui->error->setHidden(true);
    ui->error2->setHidden(true);
}

agregarActividadDoctor::~agregarActividadDoctor()
{
    delete ui;
}

void agregarActividadDoctor::on_buttonBox_accepted()
{
    QString diaS;
    int horaS;
    bool noHay=true;
    //Buscamos disponibilidad
    q.exec("SELECT dia,hora,idActividad FROM horarioDoc WHERE idDoc="+idDoc);
    while (q.next()) {
        diaS=q.value(0).toString();
        horaS=q.value(1).toInt();
        if(diaS==ui->comboBox->currentText() && ui->timeEdit->time().hour()==horaS){
            noHay=false;
            break;
        }
    }
    //Hacemos el insert
    if(noHay){
        QString valores="";
        valores+="'"+ui->comboBox->currentText()+"',"+QString::number(ui->timeEdit->time().hour())+",'"+turno+"','"+ui->comboBox_2->currentText()+"',"+idDoc;
        q.exec("INSERT INTO horarioDoc VALUE("+valores+")");
        QMessageBox::information(this,"Correcto","Actividad agregada correctamente.");
        close();
    }
    //Ya exsite una actividad ahi
    else{
        ui->error->setHidden(false);
    }
}

void agregarActividadDoctor::on_buttonBox_rejected()
{
    close();
}

void agregarActividadDoctor::on_timeEdit_timeChanged(const QTime &time)
{
    if(time.minute()!=0){
        ui->error2->setHidden(false);
    }
    else{
        ui->error2->setHidden(true);
    }
}
