#ifndef MIEMBRO_H
#define MIEMBRO_H

#include "persona.h"
#include "cuenta_miembro.h"
#include "ficha_medica.h"
#include <QPixmap>
#include <QPainter>
#include <QTextDocument>

class Miembro : public Persona
{

public:
    Miembro();
    void Leer(QString &,QPixmap &,QString &, QString &, QString &, QDate &, QString &, int &, QString &, int &, QString &, QString &, int &, QString &, QString &, QString &, QString &, QString &,Persona &,Persona &,QDate &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,QString &,ficha_medica &);
    void Guardar(QString,QPixmap,QString, QString, QString, QDate, QString, int, QString, int, QString, QString, int, QString, QString, QString, QString, QString,Persona,Persona,QDate,QString,QString,QString,QString,QString,QString,QString,QString,ficha_medica,QString);
    bool almacenar_BD();
    void buscar_BD(QString Cod_Miembro);
    bool actualizar_BD(QString Cod_Miembro,bool);
    bool borrar_BD(QString Cod_Miembro);
    bool se_encuentra(QString Cod_Miembro);
    QString rec_nom(QString Cod_Miembro);
    QString rec_ape(QString Cod_Miembro);
    void generarreport(QString&);
    bool tienefoto();
    void borra_foto();
    void grP(QString&);
    void grM(QString&);
    void grFM(QString&);

protected:
    Persona Padre, Madre;
    QDate Fecha_Ingreso;
    QString Sacramentos,Estudia,Trabaja,Rama,Cargo,Patrulla,Observaciones,Progresiones,Cod_Miembro;
    ficha_medica FM;
    QPixmap Foto;
    QString Ma;

};

#endif // MIEMBRO_H
