#ifndef TIPDELDIA_H
#define TIPDELDIA_H

#include <QDialog>

namespace Ui {
class tipdeldia;
}

class tipdeldia : public QDialog
{
    Q_OBJECT

public:
    explicit tipdeldia(QWidget *parent = nullptr);
    ~tipdeldia();

private:
    Ui::tipdeldia *ui;
};

#endif // TIPDELDIA_H
