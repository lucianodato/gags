#ifndef CONTABLE_GRUPO_H
#define CONTABLE_GRUPO_H

#include <QDate>
#include <QString>



class contable_grupo
{
public:

    contable_grupo();

    void Leer(QString &,QString &,QString &,QString &,QDate &,QString &,QString &,QString &,QString &,QString &,double &,double &,int &);
    void guardar(QString,QString,QString,QString,QDate,QString,QString,QString,QString,QString,double,double,int,QString,double,double,double);
    bool almacenar_BD();
    bool actualizar_BD_Contable_Grupo();
    bool actualizar_BD_Cuenta_Miembro();
    bool actualizar_BD_Evento();
    bool actualizar_BD_CM_Evento();
    bool borrar_BD();
    bool buscar_BD(QString Cod_CON);
    bool borrar_Evento_BD(QString);
    void actualizar_resumen(QString &,QString &);
    void buscar_datos_evento(double &,double &);
    bool actualizar_monto_evento(QString);

protected:

    QString Cod_CON,motivo,tipo_motivo,categoria,Cod_Miembro,Rama,Nombre,Apellido,cod_evento,CON_Evento;
    QDate fecha_ingreso;
    double Ingreso,Egreso,debe_miembro,gan_mie,gan_grup;
    int cantidad;

};

#endif // CONTABLE_GRUPO_H
