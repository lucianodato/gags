#include "material.h"
#include "conectarconbd.h"

//Constructor por defecto
Material::Material()
{
    Material::Cod_MAT ="no";
    Material::Descripcion ="";
    Material::categoria = "Carpa";
    Material::Fecha_ingreso.currentDate();
    Material::estado = "bien";
    Material::observaciones = "";
}

//Constructor de Copia
Material::Material(Material &Aux)
{
    Material::Cod_MAT = Aux.Cod_MAT;
    Material::Descripcion = Aux.Descripcion;
    Material::categoria = Aux.categoria;
    Material::Fecha_ingreso = Aux.Fecha_ingreso;
    Material::estado = Aux.estado;
    Material::observaciones = Aux.observaciones;
}

//Constructor
Material::Material(QString Cod_mat,QString Descrip,QString Cat, QDate Fecha, QString Est, QString Obs)
{
    Material::Cod_MAT = Cod_mat;
    Material::Descripcion = Descrip;
    Material::categoria = Cat;
    Material::Fecha_ingreso = Fecha;
    Material::estado = Est;
    Material::observaciones = Obs;
}

void Material::buscar_tabla(QString &Nombre)
{
    Nombre = "Material";
}

bool Material::almacenar_BD()
{
    QSqlQuery *insertar = new QSqlQuery();

    insertar->prepare("INSERT INTO Material (Cod_MAT,descripcion,categoria,fecha_ingreso,estado,observaciones) values(:cod_mat,:Descrip,:Cat,:Fecha,:Est,:Obs)");
    insertar->bindValue(":cod_mat",Material::Cod_MAT);
    insertar->bindValue(":Descrip",Material::Descripcion);
    insertar->bindValue(":Cat",Material::categoria);
    insertar->bindValue(":Fecha",Material::Fecha_ingreso);
    insertar->bindValue(":Est",Material::estado);
    insertar->bindValue(":Obs",Material::observaciones);

    if(insertar->exec())
    {
        insertar->~QSqlQuery();
        return true;
       }
    else
    {
        insertar->~QSqlQuery();
        return false;
    }

}

bool Material::borrar_BD(int cod_material)
{
   QSqlQuery *borrar = new QSqlQuery();

   borrar->prepare("DELETE from Material where cod_material = :Cod_mat");
   borrar->bindValue(":Cod_mat",cod_material);

   borrar->exec();

   if(borrar->isValid() and !borrar->isNull(0))
   {
       borrar->~QSqlQuery();
       return true;
      }
   else
   {
       borrar->~QSqlQuery();
       return false;
   }
}

void Material::Guardar(QString Cod_mat,QString Descrip,QString Cat, QDate Fecha, QString Est, QString Obs)
{
    Material::Cod_MAT = Cod_mat;
    Material::Descripcion = Descrip;
    Material::categoria = Cat;
    Material::Fecha_ingreso = Fecha;
    Material::estado = Est;
    Material::observaciones = Obs;
}

bool Material::actualizar_BD_Materiales(QString Cod_mat)
{
    QSqlQuery  *actualizar= new QSqlQuery();

    actualizar->prepare("UPDATE Material set fecha_ingreso = :fecha_ing,descripcion = :descrip,categoria = :cat,estado = :est,observaciones = :obs WHERE Cod_MAT = :cod_mat");
    actualizar->bindValue(":cod_mat",Cod_mat);
    actualizar->bindValue(":fecha_ing",Material::Fecha_ingreso);
    actualizar->bindValue(":descrip",Material::Descripcion);
    actualizar->bindValue(":cat",Material::categoria);
    actualizar->bindValue(":est",Material::estado);
    actualizar->bindValue(":obs",Material::observaciones);

    if(actualizar->exec())
    {
        actualizar->~QSqlQuery();
        return true;
    }
    else
    {
        actualizar->~QSqlQuery();
        return false;
    }

}














