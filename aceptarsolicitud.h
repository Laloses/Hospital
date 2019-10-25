#ifndef ACEPTARSOLICITUD_H
#define ACEPTARSOLICITUD_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

namespace Ui {
class aceptarSolicitud;
}

class aceptarSolicitud : public QDialog
{
    Q_OBJECT

public:
    explicit aceptarSolicitud(QWidget *parent = nullptr);
    ~aceptarSolicitud();
    aceptarSolicitud(QString,int,QString,QString,QString);
    void mostrarZonas();

private slots:
    void on_pushButton_guardar_clicked();

private:
    Ui::aceptarSolicitud *ui;
      QSqlDatabase database;
};

#endif // ACEPTARSOLICITUD_H
