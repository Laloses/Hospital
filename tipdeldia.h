#ifndef TIPDELDIA_H
#define TIPDELDIA_H
#include <QDialog>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtSql/QSqlError>
#include <time.h>
namespace Ui {
class tipdeldia;
}

class tipdeldia : public QDialog
{
    Q_OBJECT

public:
    explicit tipdeldia(QWidget *parent = nullptr);
    ~tipdeldia();
    int randon();
    void mostrarTip();

private slots:
    void on_pushButton_cerrar_clicked();

private:
    Ui::tipdeldia *ui;
    QSqlDatabase database;

};

#endif // TIPDELDIA_H
