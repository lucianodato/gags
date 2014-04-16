#ifndef MATERIAL_H
#define MATERIAL_H
#include <QString>
#include <QDate>


class Material
{
public:

    Material();
    Material (Material &);
    Material(QString,QString,QString,QDate,QString,QString);
    bool almacenar_BD();
    bool borrar_BD(int);
    void Guardar(QString,QString,QString,QDate,QString,QString);
    bool actualizar_BD_Materiales(QString);
    void buscar_tabla(QString &);

private:
    QString Descripcion,observaciones,categoria,estado;
    QDate Fecha_ingreso;
    QString Cod_MAT;

};

#endif // MATERIAL_H
