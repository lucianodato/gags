#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <miembro.h>
#include <QFile>
#include <material.h>
#include <ficha_medica.h>
#include <cuenta_miembro.h>
#include <contable_grupo.h>
#include <QCompleter>
#include <QListView>
#include <QTableView>
#include <QTreeView>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QBrush>
#include <QColor>
#include <QPixmapCache>
#include <configuraciones.h>
#include <QtSql>
#include <qdialog.h>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDate>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTextDocument>
#include <QTextFormat>
#include "configuraciones.h"
#include "rautomaticpadres.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void escribe_entiemporeal(QString &,QString &);
    void escribe_entiemporealP(QString&,QString&);
    void escribe_entiemporealM(QString&,QString&);
    void estructura_material();
    void estructura_contable_grupo();
    void limpiarD();
    void activaD();
    void desactivaD();
    void limpiarP();
    void activaP();
    void desactivaP();
    void limpiarM();
    void activaM();
    void desactivaM();
    void activaME();
    void desactivaME();
    void limpiarME();
    void limpiarFM();
    void activaFM();
    void desactivaFM();
    void limpiarCM();
    void activaCM();
    void desactivaCM();
    void limpiarMAT();
    void activarMAT();
    void desactivarMAT();
    void limpiarCGR();
    void activarCGR();
    void desactivarCGR();
    void activar_reporte();
    void desactivar_reporte();
    void limpiar_reporte();
    void config_busqueda_miembro();
    void camposcombobox();
    void rellenopamasa(QString,QString);
    void calculos_miembro(QString);
    void imprimir_cuenta_miembro(QString&);
    void desactivar_impresion();
    void activar_impresion();
    void desactivarCM_Asignacion();

private:
    ConectarconBD *BD;
    Ui::MainWindow *ui;
    configuraciones *conf;
    Miembro Aux;
    Cuenta_Miembro CuM;
    contable_grupo contable;
    QCompleter *mCompleter;
    QSqlTableModel *cme,*cum,*mat,*CGR,*AuxM,*AuxP,*MWlista_pat,*MWlista_car,*MWlista_ram,*MWlista_catmat,*MWlista_catpag;
    QSqlQueryModel *model;
    QTreeView *view;
    QString Cod_Miembro;
    QString Cod_Padre;
    QString Cod_Madre;
    QString Cod_FM;
    QString Cod_CU;
    QString Cod_Contable;
    QString Cod_Evento,CON_Evento,Cod_MAT;
    int Fila;
    Persona AuxPer;
    rautomaticpadres *RAP;
    int edicion_contable;
    int edicion_CUM;
    int edicion_Material;
    double debe_miembro;
    double ganancia_grupo,ganancia_miembro;

private slots:
    void on_contable_grupo_doubleClicked(QModelIndex index);
    void on_pushButton_9E_pressed();
    void on_pushButton_8E_pressed();
    void on_pushButton_5_pressed();
    void on_lineEdit_textEdited(QString);
    void on_pushButton_6_pressed();
    void on_lineEdit_53_textEdited(QString);
    void on_lineEdit_52_textEdited(QString);
    void on_lineEdit_40_textEdited(QString);
    void on_lineEdit_39_textEdited(QString);
    void on_checkBox_12_pressed();
    void on_checkBox_11_pressed();
    void on_pushButton_7NM_pressed();
    void on_pushButton_6NM_pressed();
    void on_pushButton_4_pressed();
    void on_lineEdit_34_textEdited(QString);
    void on_lineEdit_35_textEdited(QString);
    void on_lineEdit_returnPressed();
    void ShowContextMenu(const QPoint&);
    void on_checkBox_6_stateChanged(int );
    void on_checkBox_5_stateChanged(int );
    void on_pushButton_2_pressed();
    void on_pushButton_7_pressed();
    void on_pushButton_8_pressed();
    void on_pushButton_9_pressed();
    void on_pushButton_3_pressed();
    void on_lineEdit_4_returnPressed();
    void on_pushButton_15_pressed();
    void on_pushButton_16_pressed();
    void on_pushButton_17_pressed();
    void on_pushButton_18_pressed();
    void actualizar_resumen_CG();
    void estructura_Cuenta_Miembro();
    void activarCM_Asignacion();
    void on_pushButton_pressed();
    void on_lineEdit_4_textEdited(QString);
    void on_pushButton_10_pressed();
    void on_pushButton_11_pressed();
    void on_pushButton_12_pressed();
    void on_pushButton_13_pressed();
    void on_pushButton_14_pressed();
    void on_lineEdit_28_returnPressed();
    void on_lineEdit_28_textEdited(QString);
    void filtrar_cuentamiembro(QString &,QString);
    void on_dateEdit_7_dateChanged(QDate date);
    void on_dateEdit_6_dateChanged(QDate date);
    void on_checkBox_14_pressed();
    void abrir_opciones();
    void on_cuenta_miembro_doubleClicked(QModelIndex index);
    void on_tabla_material_doubleClicked(QModelIndex index);
    void asignar_fechas();
    void desactivar_interfaz();
    void activar_interfaz();
    void acercade();
    void acercadeQT();
    void help();
    void on_cuenta_miembro_pressed(const QModelIndex &index);
    void on_radioButton_pressed();
    void on_radioButton_2_pressed();
    void on_EventosMie_doubleClicked(const QModelIndex &index);
    void on_EventosMie_clicked(const QModelIndex &index);
    void on_cuenta_miembro_clicked(const QModelIndex &index);
    void on_spinBox_2_valueChanged(int arg1);
    void on_contable_grupo_clicked(const QModelIndex &index);
    void on_pushButton_19_pressed();
    void on_checkBox_15_pressed();
    void on_tabla_material_clicked(const QModelIndex &index);

public slots:
    void actualizar_modelos();



};

#endif // MAINWINDOW_H
