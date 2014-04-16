#ifndef COSTOS_H
#define COSTOS_H

#include <QString>
#include <QDebug>

class Costos
{
public:
    Costos();
    Costos(Costos &);
    Costos(QString,double,QString);
    bool cargar_bd();
    bool actualizar_bd();
    void guardar(QString,double,QString);
    void calcular_monto();
    double mostrar_monto();

private:
    QString Descripcion;
    double monto;
    QString Cod_costos;
};

#endif // COSTOS_H
