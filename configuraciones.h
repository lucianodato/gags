#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <QDialog>
#include <QDebug>
#include <QListView>
#include <QTableView>
#include <QSqlTableModel>
#include <costos.h>
#include <QMessageBox>
#include <evento.h>
#include <QtCore>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrintDialog>
#include <QPrinter>

namespace Ui {
    class configuraciones;
}

class configuraciones : public QDialog
{
    Q_OBJECT

public:

    explicit configuraciones(QWidget *parent = 0);
    ~configuraciones();
    void estructura_costos();
    void estructura_modcamposBD();
    void estructura_evento();
    void limpiarCOS();
    void activarCOS();
    void desactivarCOS();
    void limpiarEVE();
    void activarEVE();
    void desactivarEVE();
    void activarAGR();
    void desactivarAGR();
    void activarQ();
    void desactivarQ();
    bool verifica_sihay();
    void activar_finciclo();
    void desactivar_finciclo();
    void nuevo_ciclo();
    void desactivar_interfaz();
    void activar_interfaz();
    void desactivar_impresion();
    void activar_impresion();
    void pes_uno();

private:
    Ui::configuraciones *ui;

    QSqlTableModel *costos,*lista_pat,*lista_car,*lista_ram,*lista_catmat,*lista_catpag,*evento;
    Costos cos;
    Evento eve;
    int op;
    QMessageBox *Msg;
    QString Cod_Evento_Actual,Cod_Costos;
    int Edicion_Evento;
    int Edicion_Costos;

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_doubleSpinBox_valueChanged(double);
    void on_pushButton_pressed();
    void on_pushButton_15_pressed();
    void on_pushButton_16_pressed();
    void on_pushButton_2_pressed();
    void on_pushButton_5_pressed();
    void on_pushButton_6_pressed();
    void on_pushButton_7_pressed();
    void on_pushButton_8_pressed();
    void on_pushButton_3_pressed();
    void on_pushButton_4_pressed();
    void on_pushButton_9_pressed();
    void on_pushButton_10_pressed();
    void on_pushButton_11_pressed();
    void on_pushButton_12_pressed();
    void on_pushButton_13_pressed();
    void on_pushButton_17_pressed();
    void on_pushButton_14_pressed();
    void on_pushButton_18_pressed();
    void on_pushButton_21_pressed();
    void on_pushButton_22_pressed();
    void on_tableView_3_pressed(const QModelIndex &index);
    void on_tableView_7_pressed(const QModelIndex &index);
    void on_tableView_31_pressed(const QModelIndex &index);
    void on_tableView_8_pressed(const QModelIndex &index);
    void on_tableView_9_pressed(const QModelIndex &index);
    void on_Ciclocontable_currentChanged(QWidget *arg1);
    void on_tableView_2_doubleClicked(const QModelIndex &index);
    void on_tableView_2_pressed(const QModelIndex &index);
    void on_pushButton_19_pressed();
    void on_pushButton_20_pressed();
    void on_pushButton_23_pressed();
    void on_pushButton_24_pressed();
    void on_tableView_clicked(const QModelIndex &);
    void on_tableView_doubleClicked(const QModelIndex &index);

public slots:
    void asignar_fechas();



};

#endif // CONFIGURACIONES_H
