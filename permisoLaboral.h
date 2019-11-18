#ifndef PERMISOLABORAL_H
#define PERMISOLABORAL_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class PermisoLaboral;
}

class PermisoLaboral : public QDialog
{
    Q_OBJECT

public:
    explicit PermisoLaboral(QWidget *parent = nullptr, QString staff = "");
    ~PermisoLaboral();

private slots:
    void on_rb_1dia_clicked();

    void on_rb_masDias_clicked();

    void on_te_motivos_textChanged();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
    void closeEvent(QCloseEvent *event);

private:
    Ui::PermisoLaboral *ui;
    QString idStaff;
};

#endif // PERMISOLABORAL_H
