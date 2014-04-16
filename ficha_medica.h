#ifndef FICHA_MEDICA_H
#define FICHA_MEDICA_H
#include <QString>
#include <QDate>
#include <QDebug>

#include <QPainter>



class ficha_medica
{

public:
    ficha_medica();
    ficha_medica(ficha_medica &Aux);
    ficha_medica(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int);
    void Leer(QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,int &,int &);
    void Guardar(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int);
    bool almacenar_BD(QString Cod_Miembro);
    bool actualizar_BD(QString Cod_Miembro);
    bool borrar_BD(QString Cod_Miembro);
    bool buscar_BD(QString Cod_Miembro);
    void generarreportFM(QString &);

protected:

    QString Cod_Miembro,Cod_FM;
    QString gruporh,enfermedades,ambientes,alimentos,alergico,quirurguicas,observaciones,padece,medicamentos;
    QString medico,direccion_med,nombre_urg;
    int telefono_urgencia,telefono_medico;

};

#endif // FICHA_MEDICA_H
