#ifndef TIPDELDIA_H
#define TIPDELDIA_H
#include <QDialog>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtSql/QSqlError>
#include <QDesktopServices>
#include <time.h>
#include <QUrl>

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

    void on_pushButton_link_clicked();

private:
    Ui::tipdeldia *ui;
    QSqlDatabase database;
    QString link;
    int rando;

};

#endif // TIPDELDIA_H
