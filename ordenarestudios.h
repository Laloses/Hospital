#ifndef ORDENARESTUDIOS_H
#define ORDENARESTUDIOS_H

#include <QMainWindow>
#include<QString>
#include <QSqlQuery>
#include<QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <QCompleter>
#include <QFileSystemModel>
#include<QPrinter>
#include <QTextDocument>
#include <QDate>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtDebug>
#include <QSqlRecord>

namespace Ui {
class OrdenarEstudios;
}

class OrdenarEstudios : public QMainWindow
{
    Q_OBJECT

public:
    explicit OrdenarEstudios(QString, QWidget *parent = nullptr);
    ~OrdenarEstudios();
    void imprimirOrden(QPrinter* printer,QSqlQuery& Query,QString,QString,QString,QString,QString);

private slots:
    void on_agregaEst_clicked();
    void VerEsrtudios();

    void on_btnPrint_clicked();

    void on_tipoEst_currentTextChanged(const QString &arg1);

    void on_btnCancelar_clicked();

private:
    Ui::OrdenarEstudios *ui;

    QString idCita1;

    QSqlDatabase database;
    QSqlQueryModel *model;

    QSqlQuery *datosPaciente;
    QSqlQuery *datosDoctor;

    QString paciente;
    QString doctor;

};

#endif // ORDENARESTUDIOS_H
