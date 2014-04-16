#include "contable_grupo.h"
#include "conectarconbd.h"

contable_grupo::contable_grupo()
{
    contable_grupo::Cod_CON = "no";
    contable_grupo::motivo = "";
    contable_grupo::tipo_motivo = "Ingreso";
    contable_grupo::fecha_ingreso.currentDate();
    contable_grupo::categoria = "General";
    contable_grupo::cod_evento = " ";
    contable_grupo::Cod_Miembro ="";
    contable_grupo::Rama = "Grupo";
    contable_grupo::Apellido = "";
    contable_grupo::Nombre = "";
    contable_grupo::cantidad = 0;
    contable_grupo::Ingreso = 0;
    contable_grupo::Egreso = 0;
    contable_grupo::CON_Evento ="";
    contable_grupo::debe_miembro = 0;
    contable_grupo::gan_mie = 0;
    contable_grupo::gan_grup = 0;

}

void contable_grupo::Leer(QString &Cod_CON, QString &motivo, QString &tipo_motivo,QString &categoria,QDate &fecha_ingreso,QString &Cod_Miembro,QString &Cod_Evento,QString &Rama,QString &Apellido, QString &Nombre, double &ingreso,double &egreso,int &cantidad)
{
    Cod_CON = contable_grupo::Cod_CON;
    motivo = contable_grupo::motivo;
    tipo_motivo = contable_grupo::tipo_motivo;
    fecha_ingreso = contable_grupo::fecha_ingreso;
    categoria = contable_grupo::categoria;
    Cod_Miembro = contable_grupo::Cod_Miembro;
    Cod_Evento = contable_grupo::cod_evento;
    Nombre = contable_grupo::Nombre;
    Apellido = contable_grupo::Apellido;
    ingreso = contable_grupo::Ingreso;
    egreso = contable_grupo::Egreso;
    cantidad = contable_grupo::cantidad;
    Rama = contable_grupo::Rama;

}

void contable_grupo::guardar(QString Cod_CON,QString motivo,QString tipo_motivo,QString categoria,QDate fecha_ingreso,QString Cod_Miembro,QString Cod_Evento,QString Rama,QString Apellido, QString Nombre, double ingreso,double egreso,int cantidad,QString CON_Evento,double debe_miembro,double gan_mie,double gan_grup)
{
    contable_grupo::Cod_CON = Cod_CON;
    contable_grupo::motivo = motivo;
    contable_grupo::tipo_motivo = tipo_motivo;
    contable_grupo::fecha_ingreso = fecha_ingreso;
    contable_grupo::categoria = categoria;
    contable_grupo::cod_evento = Cod_Evento;
    contable_grupo::Cod_Miembro = Cod_Miembro;
    contable_grupo::Rama = Rama;
    contable_grupo::Apellido = Apellido ;
    contable_grupo::Nombre = Nombre;
    contable_grupo::cantidad = cantidad;
    contable_grupo::Ingreso = ingreso;
    contable_grupo::Egreso = egreso;
    contable_grupo::CON_Evento = CON_Evento;
    contable_grupo::debe_miembro = debe_miembro;
    contable_grupo::gan_mie = gan_mie;
    contable_grupo::gan_grup = gan_grup;

}

bool contable_grupo::almacenar_BD()
{
    QSqlQuery  * almacenar = new QSqlQuery();

    almacenar->prepare("INSERT INTO Contable_Grupo (Cod_CON,motivo,tipo_motivo,fecha_ingreso,categoria,Cod_Miembro,cod_evento,Rama,Nombre,Apellido,cantidad,Ingreso,Egreso,CON_Evento,debe_miembro,gan_mie,gan_grup) VALUES(:cod_con,:mot,:tip_mot,:fi,:categ,:Cod_Mie,:cod_eve,:Ram,:Nomb,:Ape,:cant,:Ing,:Egr,:con_eve,:debe_miembro,:gan_mie,:gan_grup)");
    almacenar->bindValue(":cod_con",contable_grupo::Cod_CON);
    almacenar->bindValue(":mot",contable_grupo::motivo);
    almacenar->bindValue(":tip_mot",contable_grupo::tipo_motivo);
    almacenar->bindValue(":fi",contable_grupo::fecha_ingreso);
    almacenar->bindValue(":categ",contable_grupo::categoria);
    almacenar->bindValue(":Cod_Mie",contable_grupo::Cod_Miembro);
    almacenar->bindValue(":Cod_eve",contable_grupo::cod_evento);
    almacenar->bindValue(":Ram",contable_grupo::Rama);
    almacenar->bindValue(":Nomb",contable_grupo::Nombre);
    almacenar->bindValue(":Ape",contable_grupo::Apellido);
    almacenar->bindValue(":cant",contable_grupo::cantidad);
    almacenar->bindValue(":Ing",contable_grupo::Ingreso);
    almacenar->bindValue(":Egr",contable_grupo::Egreso);
    almacenar->bindValue(":con_eve",contable_grupo::CON_Evento);
    almacenar->bindValue(":debe_miembro",contable_grupo::debe_miembro);
    almacenar->bindValue(":gan_mie",contable_grupo::gan_mie);
    almacenar->bindValue(":gan_grup",contable_grupo::gan_grup);

    if(almacenar->exec())
    {
        almacenar->~QSqlQuery();
        return true;
    }
    else
    {
        almacenar->~QSqlQuery();
        return false;
    }
}

bool contable_grupo::actualizar_BD_Contable_Grupo()
{
    QSqlQuery *actualizar = new QSqlQuery();

    actualizar->prepare("UPDATE Contable_Grupo SET motivo = :mot,tipo_motivo = :tip_mot,fecha_ingreso = :fi,Ingreso = :Ing,Egreso = :Egr WHERE Cod_CON = :cod_con");
    actualizar->bindValue(":cod_con",contable_grupo::Cod_CON);
    actualizar->bindValue(":mot",contable_grupo::motivo);
    actualizar->bindValue(":tip_mot",contable_grupo::tipo_motivo);
    actualizar->bindValue(":fi",contable_grupo::fecha_ingreso);
    actualizar->bindValue(":Ing",contable_grupo::Ingreso);
    actualizar->bindValue(":Egr",contable_grupo::Egreso);

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

bool contable_grupo::borrar_BD()
{
    QSqlQuery *borrar = new QSqlQuery();

    borrar->prepare("DELETE FROM Contable_grupo WHERE Cod_CON = :cod_con");
    borrar->bindValue(":cod_con",Cod_CON);

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

bool contable_grupo::buscar_BD(QString Cod_CON)
{
    QSqlQuery *buscar = new QSqlQuery();

    buscar->prepare("SELECT motivo,tipo_motivo,fecha_ingreso,categoria,Cod_Miembro,cod_evento,Rama,Nombre,Apellido,cantidad,Ingreso,Egreso WHERE Cod_CON = :cod_con");
    buscar->bindValue(":cod_con",Cod_CON);

    if(buscar->exec())
    {
        buscar->next();
        contable_grupo::motivo = buscar->value(0).toString();
        contable_grupo::tipo_motivo = buscar->value(1).toString();
        contable_grupo::fecha_ingreso = buscar->value(2).toDate();
        contable_grupo::categoria = buscar->value(3).toString();
        contable_grupo::Cod_Miembro = buscar->value(4).toString();
        contable_grupo::cod_evento = buscar->value(5).toString();
        contable_grupo::Rama = buscar->value(6).toString() ;
        contable_grupo::Apellido = buscar->value(7).toString();
        contable_grupo::Nombre = buscar->value(8).toString();
        contable_grupo::cantidad = buscar->value(9).toInt();
        contable_grupo::Ingreso = buscar->value(10).toDouble();
        contable_grupo::Egreso = buscar->value(11).toDouble();

       buscar->~QSqlQuery();
       return true;
    }
    else
    {
        buscar->~QSqlQuery();
        return false;
    }
}

void contable_grupo::actualizar_resumen(QString &haber, QString &deber)
{
    QSqlQuery *actualiza = new QSqlQuery();
    actualiza->prepare("SELECT sum(Ingreso),sum(Egreso) FROM Contable_Grupo");
    actualiza->exec();

    if(actualiza->next())
    {
        int auxhaber = actualiza->value(0).toInt();
        int auxdeber = actualiza->value(1).toInt();
        haber = QString::number(auxhaber);
        deber = QString::number(auxdeber);

        actualiza->~QSqlQuery();

    }
}

bool contable_grupo::actualizar_BD_Cuenta_Miembro()
{
    QSqlQuery *actualizar = new QSqlQuery();

    actualizar->prepare("UPDATE Contable_Grupo SET motivo = :mot,categoria = :categ,cod_evento = :Cod_eve,Ingreso = :Ing WHERE Cod_CON = :cod_con");
    actualizar->bindValue(":cod_con",Cod_CON);
    actualizar->bindValue(":mot",contable_grupo::motivo);
    actualizar->bindValue(":categ",contable_grupo::categoria);
    actualizar->bindValue(":Cod_eve",contable_grupo::cod_evento); //si cambio la categoria puede llegar a cambiar el cod_evento si tengo mas de 2
    actualizar->bindValue(":Ing",contable_grupo::Ingreso);

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

bool contable_grupo::actualizar_BD_Evento()
{
    QSqlQuery *actualizar = new QSqlQuery();

    actualizar->prepare("UPDATE Contable_Grupo set motivo = :mot,fecha_ingreso = :f_ing WHERE Cod_Evento = :cod_eve");
    actualizar->bindValue(":mot",contable_grupo::motivo);
    actualizar->bindValue(":f_ing",contable_grupo::fecha_ingreso);
    actualizar->bindValue(":cod_eve",contable_grupo::cod_evento);

    actualizar->exec(); //Actualizo primro motivo,fecha_ingreso

    actualizar->clear();

   //actualizo la gan_mie y gan_grup solo para los miembros

    actualizar->prepare("UPDATE Contable_Grupo set gan_mie = :gan_mie,gan_grup = :gan_grup WHERE Cod_Evento = :cod_eve and CON_Evento is not NULL");
    actualizar->bindValue(":cod_eve",contable_grupo::cod_evento);
    actualizar->bindValue(":gan_mie",contable_grupo::gan_mie);
    actualizar->bindValue(":gan_grup",contable_grupo::gan_grup);

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

bool contable_grupo::borrar_Evento_BD(QString Cod_Evento)
{
    QSqlQuery *borrar = new QSqlQuery();
    borrar->prepare("DELETE FROM Contable_Grupo where Cod_Evento = :cod_eve");
    borrar->bindValue(":cod_eve",Cod_Evento);

    if(borrar->exec()){
        borrar->~QSqlQuery();
        return true;
    }
    else
    {
        borrar->~QSqlQuery();
        return false;
    }

}

bool contable_grupo::actualizar_BD_CM_Evento()
{
    QSqlQuery *actualizar = new QSqlQuery();

    //Primero calculo los montos a ingresar tanto en el Miembro como en el Grupo.

    double gan_mi,gan_gr;
    buscar_datos_evento(gan_mi,gan_gr);

    //(gan_grup / (gan_mie + gan_grup)) * ingreso_monto //_Ingreso Monto_Grupo
    //(gan_mie / (gan_mie + gan_grup)) * ingreso_monto //_Ingreso Monto_Miembro

    double up,down,ready;
    down= gan_mi + gan_gr;
    up = gan_mi;
    ready = up / down;
    double ingreso_miembro = ready * Ingreso;

    double up2 = gan_gr;
    double down2 = gan_mi +gan_gr;
    double ready2 = up2 /down2;
    double ingreso_grupo = ready2 * Ingreso;

    //Debe_monto total = (cant * (gan_mie + gan_grup)) - ingreso_monto //debe_miembro

    double down3 = gan_mi + gan_gr;
    double  debe_total= cantidad * down3 - Ingreso;

    //Segundo actualizamos los valores del Miembro

    actualizar->prepare("UPDATE Contable_Grupo set cantidad = :cant,ingreso = :ing_mie,debe_miembro = :debe WHERE Cod_CON = :cod_con");

    actualizar->bindValue(":cod_con",Cod_CON);
    actualizar->bindValue(":cant",cantidad);
    actualizar->bindValue(":ing_mie",ingreso_miembro);
    actualizar->bindValue(":debe",debe_total);

    if(actualizar->exec())
    {
        //Finalmente actualizamos los valores del Grupo
        QSqlQuery *actualizar2 = new QSqlQuery();

        actualizar2->prepare("UPDATE Contable_Grupo set ingreso = :ing_grup WHERE Cod_CON = :cod_con");
        actualizar2->bindValue(":cod_con",CON_Evento);
        actualizar2->bindValue(":ing_grup",ingreso_grupo);

        if(actualizar2->exec())
        {
            actualizar->~QSqlQuery();
            actualizar2->~QSqlQuery();
            return true;
        }
        else
        {
            actualizar->~QSqlQuery();
            actualizar2->~QSqlQuery();
            return false;
        }
    }
    else
    {
        actualizar->~QSqlQuery();
        return false;
    }

}

void contable_grupo::buscar_datos_evento(double &gan_miembro, double &gan_grupo)
{
    QSqlQuery *buscar = new QSqlQuery();
    buscar->prepare("select gan_mie,gan_grup from Contable_Grupo where Cod_CON = :cod_con");
    buscar->bindValue(":cod_con",Cod_CON);

    buscar->exec();
    buscar->next();

    gan_miembro = buscar->value(0).toDouble();
    gan_grupo = buscar->value(1).toDouble();

    buscar->~QSqlQuery();
}

bool contable_grupo::actualizar_monto_evento(QString cod_event)
{
    QSqlQuery *buscar = new QSqlQuery();

    buscar->prepare("SELECT debe_miembro,Ingreso,CON_Evento,cantidad,gan_mie,gan_grup,Cod_CON FROM Contable_Grupo WHERE Cod_evento = :cod_eve");
    buscar->bindValue(":cod_eve",cod_event);

    if(buscar->exec())
    {
        QSqlQuery *actualizar_miembro_grupo = new QSqlQuery();

        double up,down,ready;
        double up2,down2,ready2;
        double down3;

        double ingreso_miembro,ingreso_grupo,debe_total;

        QString cod_mie,cod_grup;
        double parcial_mie,parcial_grup;
        double ingreso_real;
        double cantidad;

        while(buscar->next())
        {
            //(gan_grup / (gan_mie + gan_grup)) * ingreso_monto //_Ingreso Monto_Grupo
            //(gan_mie / (gan_mie + gan_grup)) * ingreso_monto //_Ingreso Monto_Miembro

            down= buscar->value(4).toDouble() + buscar->value(5).toDouble();
            up = buscar->value(4).toDouble();
            ready = up / down;

            down2 = buscar->value(4).toDouble() + buscar->value(5).toDouble();
            up2 = buscar->value(5).toDouble();
            ready2 = up2 /down2;

            //Debe_monto total = (cant * (gan_mie + gan_grup)) - ingreso_monto //debe_miembro

            down3 = buscar->value(4).toDouble() + buscar->value(5).toDouble();

            cod_mie = buscar->value(6).toString(); //Cod_CON miembro
            cod_grup = buscar->value(2).toString(); //Cod_CON del grupo
            cantidad = buscar->value(3).toInt();
            parcial_mie = buscar->value(1).toDouble(); //ingreso en el miembro

            buscar->next(); //paso a la proxima linea nesesito el ingreso hecho en el grupo.
            parcial_grup = buscar->value(1).toDouble();

            // asigno los valores
            ingreso_real = parcial_mie + parcial_grup;
            ingreso_miembro = ready * ingreso_real;
            ingreso_grupo = ready2 * ingreso_real;
            debe_total= cantidad * down3 - ingreso_real;

            //actualizo el ingreso y el debe_miembro en el Miembro
            actualizar_miembro_grupo->prepare("UPDATE Contable_Grupo set debe_miembro = :debe,Ingreso = :ing WHERE cod_CON = :cod_con");
            actualizar_miembro_grupo->bindValue(":cod_con",cod_mie);
            actualizar_miembro_grupo->bindValue(":ing",ingreso_miembro);
            actualizar_miembro_grupo->bindValue(":debe",debe_total);

            actualizar_miembro_grupo->exec(); //ejecuto la consulta- Actualizo monto ingresado para miembro.

            actualizar_miembro_grupo->clear(); //limpio para proxima actualizacion.

            //actualizo el ingreso en el Grupo
            actualizar_miembro_grupo->prepare("UPDATE Contable_Grupo set Ingreso = :ing WHERE cod_CON = :cod_con");
            actualizar_miembro_grupo->bindValue(":cod_con",cod_grup);
            actualizar_miembro_grupo->bindValue(":ing",ingreso_grupo);

            actualizar_miembro_grupo->exec(); //ejecuto la consulta- Actualizo monto ingresado para grupo.
            actualizar_miembro_grupo->clear(); //limpio para ciclo nuevo.


        }

        actualizar_miembro_grupo->~QSqlQuery();
        buscar->clear();
        buscar->~QSqlQuery();
        return true;
    }
    else
    {
        buscar->~QSqlQuery();
        return false;
    }

}
