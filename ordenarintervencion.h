#ifndef ORDENARINTERVENCION_H
#define ORDENARINTERVENCION_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QTimer>
#include <QStringList>
#include <QCompleter>

namespace Ui {
class OrdenarIntervencion;
}

class OrdenarIntervencion : public QMainWindow
{
    Q_OBJECT

public:
    explicit OrdenarIntervencion( QString, QWidget *parent = nullptr);
    ~OrdenarIntervencion();

private slots:
    void on_btnContinuarInter_clicked();

    void on_btnBackInter_clicked();

    void on_btnCancelarInter_clicked();

    void precio();

    void on_lineServ_editingFinished();

    void on_lineServ_textChanged(const QString &arg1);

private:
    Ui::OrdenarIntervencion *ui;
    QString idCita1;

    QSqlDatabase database;
    QSqlQueryModel *model;

    QSqlQuery *datosPaciente;
    QSqlQuery *datosDoctor;

    QTimer *cuenta;
};

#endif // ORDENARINTERVENCION_H
