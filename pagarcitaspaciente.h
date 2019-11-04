#ifndef PAGARCITASPACIENTE_H
#define PAGARCITASPACIENTE_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>

namespace Ui {
class pagarCitasPaciente;
}

class pagarCitasPaciente : public QDialog
{
    Q_OBJECT
    QString pago;

public:
    explicit pagarCitasPaciente(QString,QWidget *parent = nullptr);
    ~pagarCitasPaciente();
    bool pagarCitas(QString,QString,QString,QString,QString);

private slots:
    bool on_pushButton_pagarC_clicked();

    void on_cb_tarjetas_currentTextChanged(const QString &arg1);

private:
    Ui::pagarCitasPaciente *ui;
    QString folioR;
    QString nombreP;
    QSqlQuery q,d;
    QList<QSqlRecord> tarjetas;
};

#endif // PAGARCITASPACIENTE_H
