#ifndef ELIMINARUSUARIOS_H
#define ELIMINARUSUARIOS_H

#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
namespace Ui {
class eliminarUsuarios;
}

class eliminarUsuarios : public QDialog
{
    Q_OBJECT

public:
    explicit eliminarUsuarios(QWidget *parent = nullptr);
    ~eliminarUsuarios();

private slots:
    void on_radioButton_doc_clicked();

private:
    Ui::eliminarUsuarios *ui;
};

#endif // ELIMINARUSUARIOS_H
