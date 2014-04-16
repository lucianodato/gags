#ifndef PERSONA_H
#define PERSONA_H

#include <QVariant>
#include <QString>
#include <QDate>
#include <QDebug>
#include <QBuffer>
#include <QPainter>
#include <conectarconbd.h>

class Persona
{
public:
    Persona();
    Persona(Persona &);
    Persona(QString,QString,QString,QDate,QString,int,QString,int,QString,QString,int,QString,QString,QString,QString,QString,QString);
    void Leer(QString &,QString &,QString &,QDate &,QString &,int &,QString &,int &,QString &,QString &,int &,QString &,QString &,QString &,QString &,QString &,QString &);
    void Guardar(QString,QString,QString,QDate,QString,int,QString,int,QString,QString,int,QString,QString,QString,QString,QString,QString);
    bool almacenar_BD(QString);
    bool actualizar_BD(QString);
    bool borrar_BD(QString);
    void buscar_BD(QString,QString);
    bool estaenBD(QString,QString);
    Persona &operator = (const Persona &);
    bool Llena_siexisten(QString);
    void generarreportP(QString&);

protected:
    QString Apellido;
    QString Nombre;
    QString Sexo;
    QDate Fecha_Nacimiento;
    QString Tipo_Documento;
    int Documento;
    QString Domicilio;
    int Numero;
    QString Piso;
    QString Dpto;
    int Telefono;
    QString Nacionalidad;
    QString Provincia;
    QString Localidad;
    QString Lugar_Nac;
    QString Correo_elec;
    QString Cod_Persona;

};

#endif // PERSONA_H
