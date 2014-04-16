#ifndef RAUTOMATICPADRES_H
#define RAUTOMATICPADRES_H

#include <QDialog>
#include "conectarconbd.h"

namespace Ui {
    class rautomaticpadres;
}

class rautomaticpadres : public QDialog
{
    Q_OBJECT

public:
    explicit rautomaticpadres(QWidget *parent = 0);
    ~rautomaticpadres();
    void estructura_dialog();
    QString CodP,CodM;

private:
    Ui::rautomaticpadres *ui;
    QSqlTableModel *sugerencias;

private slots:
    void on_tableView_doubleClicked(QModelIndex index);
    void on_pushButton_pressed();

};

#endif // RAUTOMATICPADRES_H
