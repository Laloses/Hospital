#ifndef AGREGARACTIVIDADDOCTOR_H
#define AGREGARACTIVIDADDOCTOR_H
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>

namespace Ui {
class agregarActividadDoctor;
}

class agregarActividadDoctor : public QDialog
{
    Q_OBJECT

public:
    explicit agregarActividadDoctor(QWidget *parent ,QString id);
    ~agregarActividadDoctor();
    QString idDoc,especialidad,turno;
    QSqlQueryModel *qModel;

private slots:

    void on_aceptar_clicked();

    void on_close_clicked();

private:
    Ui::agregarActividadDoctor *ui;
    QSqlQuery q;
};

#endif // AGREGARACTIVIDADDOCTOR_H
