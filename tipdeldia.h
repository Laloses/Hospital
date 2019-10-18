#ifndef TIPDELDIA_H
#define TIPDELDIA_H
#include <QDialog>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtSql/QSqlError>

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

private:
    Ui::tipdeldia *ui;
    QSqlDatabase database;

};

#endif // TIPDELDIA_H
