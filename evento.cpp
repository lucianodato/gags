#include "evento.h"

Evento::Evento()
{
    Evento::Cod_evento = "no";
    Evento::monto = 0;
    Evento::ganancia_grupo = 0;
    Evento::ganancia_miembro = 0;
    Evento::descripcion = "";
    Evento::dia.currentDate();
}

void Evento::guardar(QString Cod_evento,double monto, double gan_mie,double gan_grup,QString descripcion,QDate dia)
{
    Evento::monto = monto;
    Evento::Cod_evento = Cod_evento;
    Evento::ganancia_grupo = gan_grup;
    Evento::ganancia_miembro = gan_mie;
    Evento::dia = dia;
    Evento::descripcion = descripcion;
}

bool Evento::almacenar_BD()
{
    QSqlQuery  *almacenar = new QSqlQuery();

    almacenar->prepare("INSERT INTO Evento (cod_evento,descripcion,monto,gan_mie,gan_grup,dia) VALUES(:cod_event,:des,:mon,:gm,:gg,:dia)");
    almacenar->bindValue(":cod_event",Evento::Cod_evento);
    almacenar->bindValue(":des",Evento::descripcion);
    almacenar->bindValue(":mon",Evento::monto);
    almacenar->bindValue(":gm",Evento::ganancia_miembro);
    almacenar->bindValue(":gg",Evento::ganancia_grupo);
    almacenar->bindValue(":dia",Evento::dia);

    if(almacenar->exec())
    {
        QString motivo;
        motivo = Evento::descripcion + " - " + Evento::dia.toString("dd-MM");
        inicializar_BD(motivo);
        almacenar->~QSqlQuery();
        return true;
    }
    else
    {
        almacenar->~QSqlQuery();
        return false;
    }
}

bool Evento::inicializar_BD(QString motivo)
{
    QSqlQuery *buscar = new QSqlQuery();

    // primero buscamos todos los miembros que se encuentran en la BD (Activos);
    buscar->prepare("SELECT Cod_Miembro,Rama,Nombre,Apellido,Activo FROM Miembro");

    if(buscar->exec())
    {
        QString Cod_CON,CON_Evento;

        while(buscar->next())
        {
            if(buscar->value(4).toString() == "Activo")
            {
                ConectarconBD::cod_setup("Contable_Grupo",Cod_CON);
                ConectarconBD::cod_setup("Contable_Grupo",CON_Evento);

                Cont_Grup.guardar(Cod_CON,motivo,"Ingreso",Cod_evento,dia,buscar->value(0).toString(),Cod_evento,buscar->value(1).toString(),buscar->value(3).toString(),buscar->value(2).toString(),0,0,0,CON_Evento,0,ganancia_miembro,ganancia_grupo);
                Cont_Grup.almacenar_BD();

                Cont_Grup.guardar(CON_Evento,motivo,"Ingreso",Cod_evento,dia," ",Cod_evento,"Grupo"," "," ",0,0,0,QString::null,0,0,0);
                Cont_Grup.almacenar_BD();
            }
        }
        buscar->~QSqlQuery();
        return true;
    }
    buscar->~QSqlQuery();
    return false;

}

bool Evento::borrar_evento(QString Cod_Evento)
{
    //Paso el cod_evento al metodo correspondiente de Contable_Grupo.
    if(Cont_Grup.borrar_Evento_BD(Cod_Evento))
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool Evento::actualizar_BD()
{
    QSqlQuery *actualizar = new QSqlQuery();

    actualizar->prepare("update Evento set descripcion = :des,monto = :mon,gan_mie = :gm,gan_grup = :gg,dia = :dia where cod_evento = :cod_event");

    actualizar->bindValue(":cod_event",Evento::Cod_evento);
    actualizar->bindValue(":des",Evento::descripcion);
    actualizar->bindValue(":mon",Evento::monto);
    actualizar->bindValue(":gm",Evento::ganancia_miembro);
    actualizar->bindValue(":gg",Evento::ganancia_grupo);
    actualizar->bindValue(":dia",Evento::dia);

    if(actualizar->exec())
    {
        QString motivo = descripcion + " - " + dia.toString("dd-MM");

        QSqlDatabase::database().transaction();

        Cont_Grup.guardar("",motivo,"",Evento::Cod_evento,Evento::dia,"",Evento::Cod_evento,"","","",0,0,0,"",0,ganancia_miembro,ganancia_grupo);
        Cont_Grup.actualizar_BD_Evento();

        QSqlDatabase::database().commit();

        QSqlDatabase::database().transaction();

        Cont_Grup.guardar("",motivo,"",Evento::Cod_evento,Evento::dia,"",Evento::Cod_evento,"","","",0,0,0,"",0,ganancia_miembro,ganancia_grupo);
        Cont_Grup.actualizar_monto_evento(Evento::Cod_evento);

        QSqlDatabase::database().commit();

        actualizar->~QSqlQuery();
        return true;
    }
    else
    {
        actualizar->~QSqlQuery();
        return false;
    }


}

void Evento::ganancia_evento(QString Cod_Evento, double &ganancia_miembro, double &ganancia_grupo)
{
    QSqlQuery *busca = new QSqlQuery();
    busca->prepare("Select gan_mie,gan_grup FROM Evento where cod_evento = :cod_eve");
    busca->bindValue(":cod_evento", Cod_Evento);
    busca->exec();

    ganancia_miembro = busca->value(0).toDouble();
    ganancia_grupo = busca->value(1).toDouble();

    busca->~QSqlQuery();
}
