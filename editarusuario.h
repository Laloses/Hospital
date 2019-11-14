#ifndef EDITARUSUARIO_H
#define EDITARUSUARIO_H

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
#include <QtWidgets>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class EditarUsuario;
}

class EditarUsuario : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditarUsuario(QWidget *parent = nullptr);
    ~EditarUsuario();

private:
    Ui::EditarUsuario *ui;

    QSqlDatabase database;
    QSqlQueryModel *model;

    QSqlQuery *datosUsr;
};

#endif // EDITARUSUARIO_H
