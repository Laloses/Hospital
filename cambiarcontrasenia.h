#ifndef CAMBIARCONTRASENIA_H
#define CAMBIARCONTRASENIA_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class CambiarContrasenia;
}

class CambiarContrasenia : public QMainWindow
{
    Q_OBJECT

public:
    explicit CambiarContrasenia(QWidget *parent = nullptr);
    ~CambiarContrasenia();

private slots:
    void on_btnCancelar_clicked();

    void on_btnContinuar_clicked();

    void on_btnEnviarRepuesta_clicked();

    void on_btnRegresar_clicked();

    void on_btnRestablecerPass_clicked();

private:
    Ui::CambiarContrasenia *ui;
    QString id;
    QSqlQuery obtenerPregunta;
    QString pregunta;
    QString respuesta;

};

#endif // CAMBIARCONTRASENIA_H
