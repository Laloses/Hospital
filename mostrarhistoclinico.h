#ifndef MOSTRARHISTOCLINICO_H
#define MOSTRARHISTOCLINICO_H

#include <QWidget>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtSql>

namespace Ui {
class MostrarHistoclinico;
}

class MostrarHistoclinico : public QWidget
{
    Q_OBJECT

public:
    explicit MostrarHistoclinico(QWidget *parent = nullptr);
    ~MostrarHistoclinico();
    void mostrarClinico(QString,QString,QString,QString);

private slots:
    void on_pushButton_regresar_clicked();

private:
    Ui::MostrarHistoclinico *ui;
};

#endif // MOSTRARHISTOCLINICO_H
