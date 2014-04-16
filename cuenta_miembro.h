#ifndef CUENTA_MIEMBRO_H
#define CUENTA_MIEMBRO_H
#include <QString>
#include <QDate>
#include <QDebug>
#include <contable_grupo.h>
#include <conectarconbd.h>
#include <QDate>


class Cuenta_Miembro
{
protected:
    QString Cod_Miembro,Cod_CON,motivo,categoria;
    double monto;
    contable_grupo CG;
    QDate fecha_ingreso;
    double debe_miembro;
    int cantidad;
    QString CON_Evento;

public:

    Cuenta_Miembro();
    void Guardar(QString,QString,QString,double,QString,QDate,int,double,QString);
    bool almacenar_BD();
    bool actualizar_BD(int);
    bool borrar_BD(QString Cod_Miembro);
    void buscar_BD(QString &,QString);
    void buscar_BD_2(double &,QString);
    void busca_datos(QString &,QString &,QString &);

};

#endif // CUENTA_H
