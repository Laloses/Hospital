#ifndef PAGOINTERVENCIONES_H
#define PAGOINTERVENCIONES_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>

namespace Ui {
class pagoIntervenciones;
}

class pagoIntervenciones : public QDialog
{
    Q_OBJECT
    QString pago;

public:
    explicit pagoIntervenciones(QString p,QWidget *parent = nullptr);
    ~pagoIntervenciones();
    bool pagarIntervenciones(QString,QString,QString,QString,QString);


private slots:
    bool on_pushButton_PI_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);


private:
    Ui::pagoIntervenciones *ui;
    QString folioR;
    QString nombreP;
    QSqlQuery q,d;
    QList<QSqlRecord> tarjetas;
};

#endif // PAGOINTERVENCIONES_H
