#ifndef INFOHOSPITAL_H
#define INFOHOSPITAL_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class InfoHospital;
}

class InfoHospital : public QWidget
{
    Q_OBJECT

public:
    explicit InfoHospital(QWidget *parent = nullptr);
    ~InfoHospital();

private slots:
    void on_pb_acerca_clicked();

    void on_pb_MyV_clicked();

    void on_pb_valores_clicked();

    void on_pb_calidad_clicked();

private:
    Ui::InfoHospital *ui;
    QString bienvenida, mision,valores, calidad;
};

#endif // INFOHOSPITAL_H
