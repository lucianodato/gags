#include "cuenta_miembro.h"
#include "conectarconbd.h"

Cuenta_Miembro::Cuenta_Miembro()
{
    Cuenta_Miembro::Cod_Miembro = "no";
    Cuenta_Miembro::Cod_CON = "no";
    Cuenta_Miembro::motivo = "";
    Cuenta_Miembro::monto = 0.0;
    Cuenta_Miembro::categoria = "";
    Cuenta_Miembro::fecha_ingreso = QDate::currentDate();
    Cuenta_Miembro::cantidad = 0;
    Cuenta_Miembro::debe_miembro = 0;
    Cuenta_Miembro::CON_Evento = " ";
}

void Cuenta_Miembro::Guardar(QString Cod_Miembro,QString Cod_CON, QString motivo, double monto, QString categoria,QDate fecha_ingreso,int cantidad,double debe_miembro,QString CON_Evento)
{
    Cuenta_Miembro::Cod_Miembro = Cod_Miembro;
    Cuenta_Miembro::Cod_CON = Cod_CON;
    Cuenta_Miembro::motivo = motivo;
    Cuenta_Miembro::monto = monto;
    Cuenta_Miembro::categoria = categoria;
    Cuenta_Miembro::fecha_ingreso = fecha_ingreso;
    Cuenta_Miembro::cantidad = cantidad;
    Cuenta_Miembro::debe_miembro = debe_miembro;
    Cuenta_Miembro::CON_Evento = CON_Evento;   
}

bool Cuenta_Miembro::actualizar_BD(int indicador)
{
    //PARA ACTUALIZAR EN LA CONTABLE_GRUPO   //llamar al metodo guardar para pasarle las variables correspondientes

    if(indicador == 1)
    {
        CG.guardar(Cod_CON,motivo,QString::null,categoria,fecha_ingreso,Cod_Miembro,QString::null,QString::null,QString::null,QString::null,monto,0,0,QString::null,0,0,0);
        CG.actualizar_BD_Cuenta_Miembro();
        return true;
    }

    else
    {
        //double gan_mie,gan_grup;
        //buscar_datos_evento(gan_mie,gan_grup);
        CG.guardar(Cod_CON,motivo,QString::null,categoria,fecha_ingreso,Cod_Miembro,QString::null,QString::null,QString::null,QString::null,monto,0,cantidad,CON_Evento,debe_miembro,0,0);
        CG.actualizar_BD_CM_Evento();
        return true;
    }

    return false;
}

bool Cuenta_Miembro::almacenar_BD()
{
    //PARA ALMACENAR EN LA CONTABLE_GRUPO

    QString ti_mot = "Ingreso";
    QString mot,nom,ape,rama;
    busca_datos(nom,ape,rama);
    mot = motivo;
    double Egreso = 0;
    int cantidad = 0;

    CG.guardar(Cod_CON,mot,ti_mot,categoria,fecha_ingreso,Cod_Miembro,QString::null,rama,ape,nom,monto,Egreso,cantidad,"",0,0,0);

    if(CG.almacenar_BD())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cuenta_Miembro::borrar_BD(QString Cod_Miembro)
{
    QSqlQuery  *borrar = new QSqlQuery();

    borrar->prepare("DELETE From Cuenta_Miembro WHERE Cod_Miembro = :cod_mie"); // Hay que ver bien aca cual es la onda, por que no se si borra el total de las selecciones con Cod_Miembro
    borrar->bindValue(":cod_mie",Cod_Miembro);

    if(borrar->exec())
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

void Cuenta_Miembro::buscar_BD(QString &Filtro,QString Cod_Miembro)
{
    QSqlQuery  *buscar = new QSqlQuery();

    buscar->prepare("SELECT  * From Contable_Grupo WHERE Cod_Miembro = :cod_mie");
    buscar->bindValue("cod_mie",Cod_Miembro);
    buscar->exec();

    if(buscar->next())
    {
        Filtro = "Cod_Miembro='" + Cod_Miembro + "'"; //seteo el primer filtro.
    }
    else
    {
        Filtro="NO";
    }

    buscar->~QSqlQuery();
}

void Cuenta_Miembro::busca_datos(QString &Nombre,QString &Apellido,QString &Rama)
{
    QSqlQuery *buscar = new QSqlQuery();

    buscar->prepare("SELECT Nombre,Apellido,Rama FROM Miembro WHERE Cod_Miembro = :cod_mie");
    buscar->bindValue(":cod_mie",Cuenta_Miembro::Cod_Miembro);
    buscar->exec();
    buscar->next();

    Nombre = buscar->value(0).toString();
    Apellido = buscar->value(1).toString();
    Rama = buscar->value(2).toString();

    buscar->~QSqlQuery();
}

void Cuenta_Miembro::buscar_BD_2(double & gan_grupo, QString CON_evento)
{
    QSqlQuery  *buscar = new QSqlQuery();

    buscar->prepare("SELECT  ingreso From Contable_Grupo WHERE Cod_CON = :cod_con");
    buscar->bindValue("cod_con",CON_evento);
    buscar->exec();
    buscar->next();

    gan_grupo = buscar->value(0).toDouble();

    buscar->~QSqlQuery();
}

