#ifndef AGREGARURGENCIA_H
#define AGREGARURGENCIA_H

#include <QWidget>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
class agregarUrgencia;
}

class agregarUrgencia : public QWidget
{
    Q_OBJECT

public:
    explicit agregarUrgencia(QWidget *parent = nullptr);
    ~agregarUrgencia();

private:
    Ui::agregarUrgencia *ui;
    QSqlQuery *urgencias;
    void buscarUrgencias();
    void crearBotones(QList<QSqlRecord> datos);
    void verDetalles(QString);
};

#endif // AGREGARURGENCIA_H
