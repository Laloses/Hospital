#include "permisoLaboral.h"
#include "ui_permisoLaboral.h"


PermisoLaboral::~PermisoLaboral()
{
    delete ui;
}

void PermisoLaboral::on_buttonBox_rejected()
{
    this->close();
}

PermisoLaboral::PermisoLaboral(QWidget *parent, QString Staff) :
    QDialog(parent),
    ui(new Ui::PermisoLaboral)
{
    QSqlDatabase database = QSqlDatabase::database();
    ui->setupUi(this);
    idStaff=Staff;
    //Bloqueamos las fechas para hacer la solicitud, minimo 3 dias antes
    QSqlQuery* fechas = new QSqlQuery;
    fechas->exec("SELECT CURDATE()");
    fechas->next();
    /*
    ui->de_fechaUnica->setDate(fechas->value(0).toDate());
    ui->de_fechaInicio->setDate(fechas->value(0).toDate());
    ui->de_fechaFin->setDate(fechas->value(0).toDate());
    */

    ui->rb_1dia->click();
    ui->de_fechaUnica->setMinimumDate(fechas->value(0).toDate().addDays(3));
    ui->de_fechaInicio->setMinimumDate(fechas->value(0).toDate().addDays(3));
    ui->de_fechaFin->setMinimumDate(fechas->value(0).toDate().addDays(3));
}

void PermisoLaboral::on_rb_1dia_clicked()
{
    ui->sw_fechas->setCurrentIndex(0);
}

void PermisoLaboral::on_rb_masDias_clicked()
{
    ui->sw_fechas->setCurrentIndex(1);
}

void PermisoLaboral::on_te_motivos_textChanged()
{
    if (ui->te_motivos->toPlainText().length() > 500) {
        ui->te_motivos->textCursor().deletePreviousChar();
    }
}

void PermisoLaboral::on_buttonBox_accepted()
{
    QString estiloBueno,estiloMalo;
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    estiloMalo="border:2px red; border-style:solid";
    //Verificamos que no esten vacíos los motivos
    if(!ui->te_motivos->toPlainText().isEmpty()){
        ui->te_motivos->setStyleSheet(estiloBueno);

        //REVISAMOS QUE NO EXISTA UN PERMISO PARA ESOS DIAS, Y NO REPITA
        QSqlQuery* existencia = new QSqlQuery;
        QString * consulta;

        //PARA UN DIA
        if(ui->rb_1dia->isChecked()){
            consulta = new QString("SELECT idPermiso FROM permisoLaboral "
                                   "WHERE idStaff="+idStaff+" "
                                   "AND fechaI='"+ui->de_fechaUnica->date().toString("yyyy-MM-dd")+"'");
                    existencia->exec(*consulta);
                    qDebug()<<*consulta;
                    if( existencia->next() ){
                        QMessageBox::critical(this,"Error","Ya tienes una solicitud pendiente de ese dia.");
                        return;
                    }
        }
        else{
            //Si es el mismo dia
            consulta = new QString("SELECT fechaI,fechaF FROM permisoLaboral "
                                   "WHERE idStaff="+idStaff+" "
                                   "AND fechaI='"+ui->de_fechaInicio->date().toString("yyyy-MM-dd")+"'");
                    existencia->exec(*consulta);
                    qDebug()<<*consulta;
                    if( existencia->next() ){
                        QMessageBox::critical(this,"Error","Ya tienes una solicitud pendiente de ese dia.");
                        return;
                    }
            //Si es un intervalo dentro de otra reservacion
            consulta = new QString("SELECT fechaI,fechaF FROM permisoLaboral "
                                   "WHERE idStaff="+idStaff+" "
                                   "AND fechaI<'"+ui->de_fechaInicio->date().toString("yyyy-MM-dd")+"' "
                                   "AND fechaF>'"+ui->de_fechaFin->date().toString("yyyy-MM-dd")+"' ");
                     existencia->exec(*consulta);
                     qDebug()<<*consulta;
                     if( existencia->next() ){
                         QMessageBox::critical(this,"Error","Ya tienes una solicitud pendiente de ese dia.");
                         return;
                     }
        }

        //INSERTAMOS UNA NUEVA SOLICITUD
        QMessageBox::StandardButton res = QMessageBox::question(this,"Confirmar","¿Está seguro de pedir permiso?");
        if(res == QMessageBox::Yes){
            //PARA UN DIA
            QSqlQuery* insert = new QSqlQuery;
            if(ui->rb_1dia->isChecked()){
                //INSERT
                consulta= new QString("INSERT INTO permisoLaboral(idStaff,motivos,fechaI,fechaF) "
                                      "value("+idStaff+",'"+ui->te_motivos->toPlainText()+"','"+ui->de_fechaUnica->date().toString("yyyy-MM-dd")+"','"+ui->de_fechaUnica->date().toString("yyyy-MM-dd")+"')");
                if ( insert->exec(*consulta) ){
                    QMessageBox::information(this,"Éxito","Solicitud enviada correctamente, espera a que la acepten.");
                    this->close();
                }
                else {
                    qDebug()<<insert->lastError().text();
                    QMessageBox::critical(this,"Error","Solicitud no enviada.");
                }
            }
            //PARA MAS DIAS
            else{
                //INSERT
                consulta= new QString("INSERT INTO permisoLaboral(idStaff,motivos,fechaI,fechaF) "
                                      "value("+idStaff+",'"+ui->te_motivos->toPlainText()+"','"+ui->de_fechaInicio->date().toString("yyyy-MM-dd")+"','"+ui->de_fechaFin->date().toString("yyyy-MM-dd")+"')");
                if ( insert->exec(*consulta) ){
                    QMessageBox::information(this,"Éxito","Solicitud enviada correctamente, espera a que la acepten.");
                    this->close();
                }
                else {
                    qDebug()<<insert->lastError().text();
                    QMessageBox::critical(this,"Error","Solicitud no enviada.");
                }
            }
        }
    }
    //No tiene nada escrito
    else {
        ui->te_motivos->setStyleSheet(estiloMalo);
        this->show();
    }

}

void PermisoLaboral::closeEvent(QCloseEvent  *event) {
    return event->ignore(); //to avoid closing.
}
