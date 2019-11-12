#ifndef AGREGARURGENCIA_H
#define AGREGARURGENCIA_H

#include <QWidget>
#include <QtSql>
#include <QtWidgets>
#include "registropaciente.h"

namespace Ui {
class agregarUrgencia;
}

class agregarUrgencia : public QWidget
{
    Q_OBJECT

public:
    explicit agregarUrgencia(QWidget *parent = nullptr);
    ~agregarUrgencia();

private slots:
    void on_pushButton_clicked();

    void on_regresarDetalles_clicked();

    void on_pb_siguiente_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();


private:
    Ui::agregarUrgencia *ui;
    QSqlQuery *urgencias;
    void buscarUrgencias();
    void crearBotones(QList<QSqlRecord> datos);
    void verDetalles(QSqlRecord datos);
    void verCompa(QString id);
    void nuevoExp();
    void agregarExistente(QString idE);
    void llenarHistorial(QString id,QString idE);
    QString idPac;
    void on_pb_llenarHistorial_clicked(QString idPac, QString idE);
    void on_pb_agregar_clicked(QString id);
};

#endif // AGREGARURGENCIA_H
