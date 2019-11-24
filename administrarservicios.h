#ifndef ADMINISTRARSERVICIOS_H
#define ADMINISTRARSERVICIOS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

namespace Ui {
class AdministrarServicios;
}

class AdministrarServicios : public QDialog
{
    Q_OBJECT

public:
    explicit AdministrarServicios(QWidget *parent = nullptr);
    ~AdministrarServicios();
    void verServicios();


private slots:
    void on_regresarLista_clicked();
    void infoServicio(QString);
    void elminiarServico(QString);
    void editarServicio(QString);

    void on_regresarE_clicked();

    void on_q1_clicked();

    void on_q2_clicked();

    void on_a1_clicked();

    void on_a2_clicked();

    void on_cancelare_clicked();

    void on_guardare_clicked();

    void on_regresarNuevo_clicked();

    void on_nuevoServicio_clicked();

    void on_cancelare_2_clicked();

    void on_guardare_2_clicked();

    void on_q1_2_clicked();

    void on_q2_2_clicked();

    void on_a1_2_clicked();

    void on_a2_2_clicked();

private:
    Ui::AdministrarServicios *ui;
    QByteArray foto,foto2;
    QString imgRoute,imgRoute2;
    QString tituloActual;
};

#endif // ADMINISTRARSERVICIOS_H
