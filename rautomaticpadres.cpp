#include "rautomaticpadres.h"
#include "ui_rautomaticpadres.h"

rautomaticpadres::rautomaticpadres(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rautomaticpadres)
{
    ui->setupUi(this);

    sugerencias = new QSqlTableModel();
    //Cargo los datos de la tabla PadMa en el modelo
    sugerencias->setTable("PadMa");
}

rautomaticpadres::~rautomaticpadres()
{
    sugerencias->~QSqlTableModel();
    delete ui;

}

void rautomaticpadres::estructura_dialog()
{
    sugerencias->select();

    sugerencias->setHeaderData(0, Qt::Horizontal, tr("Nombre Padre"));
    sugerencias->setHeaderData(1, Qt::Horizontal, tr("Apellido Padre"));
    sugerencias->setHeaderData(3, Qt::Horizontal, tr("Nombre Madre"));
    sugerencias->setHeaderData(4, Qt::Horizontal, tr("Apellido Madre"));
    sugerencias->setHeaderData(6, Qt::Horizontal, tr("Nombre Miembro"));
    sugerencias->setHeaderData(7, Qt::Horizontal, tr("Apellido Miembro"));

    //Aca asocio el modelo con el dialogo emergente
    ui->tableView->setModel(sugerencias);
    //Ajustar al tamaño optimo
    ui->tableView->resizeColumnsToContents();

    //Oculto las columnas innecesarias
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(8);
}

void rautomaticpadres::on_tableView_doubleClicked(QModelIndex index)
{

    //Aca en el caso que haya opciones y las elija tendria que devolver el indice como para que rellene automaticamente
    CodP = "no";
    CodM = "no";

    CodP = index.sibling(index.row(),2).data().toString();
    CodM = index.sibling(index.row(),5).data().toString();
}

void rautomaticpadres::on_pushButton_pressed()
{
    this->close();
}
