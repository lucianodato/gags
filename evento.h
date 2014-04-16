#ifndef EVENTO_H
#define EVENTO_H

#include <QDate>
#include <contable_grupo.h>
#include <conectarconbd.h>

class Evento
{
public:
    Evento();
    bool almacenar_BD();
    void guardar(QString,double,double,double,QString,QDate);
    bool inicializar_BD(QString);
    bool borrar_evento(QString);
    void ganancia_evento(QString,double&,double&);
    bool actualizar_BD();

private:

    QString Cod_evento;
    double monto,ganancia_grupo, ganancia_miembro;
    QDate dia;
    QString descripcion;
    contable_grupo Cont_Grup;


};

#endif // EVENTO_H
