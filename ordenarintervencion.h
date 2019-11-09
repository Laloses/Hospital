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
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>

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

    void on_btnAgregarArticulo_clicked();

    void eliminarFila(int fila);

    void on_comboQuiro_currentIndexChanged(int index);

    void on_intervencionFecha_dateChanged(const QDate &date);

    void on_tablaArticulos_itemClicked(QTableWidgetItem *item);

    void on_tablaArticulos_cellClicked(int row, int column);

    void on_tablaArticulos_cellPressed(int row, int column);

    void insertarArt();

    void on_btnAgendarQuiro_clicked();

private:
    Ui::OrdenarIntervencion *ui;
    QString idCita1;

    QSqlDatabase database;
    QSqlQueryModel *model;

    QSqlQuery *datosPaciente;
    QSqlQuery *datosDoctor;

    QTimer *cuenta;

    double CostoTotal;
    double iva;
    double subtotal;
};

#endif // ORDENARINTERVENCION_H
