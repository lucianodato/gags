#include "costos.h"
#include "conectarconbd.h"

Costos::Costos()
{
    Costos::Descripcion = "";
    Costos::monto = 0.0;
    Costos::Cod_costos ="no";
}

Costos::Costos(Costos &Aux)
{
    Costos::Descripcion = Aux.Descripcion;
    Costos::monto = Aux.monto;
    Costos::Cod_costos = Aux.Cod_costos;
}

Costos::Costos(QString descripcion,double monto,QString cod_costos)
{
    Costos::Descripcion = descripcion;
    Costos::monto = monto;
    Costos::Cod_costos = cod_costos;
}

void Costos::guardar(QString descripcion,double monto,QString cod_costos)
{
    Costos::Descripcion = descripcion;
    Costos::monto = monto;
    Costos::Cod_costos = cod_costos;
}

bool Costos::cargar_bd()
{
    QSqlQuery  *almacenar = new QSqlQuery();

    almacenar->prepare("INSERT INTO Costos (cod_costo,descripcion,monto) VALUES(:cod_cost,:des,:mon)");
    almacenar->bindValue(":cod_cost",Costos::Cod_costos);
    almacenar->bindValue(":des",Costos::Descripcion);
    almacenar->bindValue(":mon",Costos::monto);

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

bool Costos::actualizar_bd()
{
    QSqlQuery  *actualizar = new QSqlQuery();

    actualizar->prepare("update Costos set descripcion = :des,monto = :mon where cod_costo = :cod_cost");
    actualizar->bindValue(":cod_cost",Costos::Cod_costos);
    actualizar->bindValue(":des",Costos::Descripcion);
    actualizar->bindValue(":mon",Costos::monto);

    //cod_costo varchar(10) not null, descripcion varchar(60),monto double

    if(actualizar->exec())
    {
        qDebug()<<"actualizar_bd()::costos";
        actualizar->~QSqlQuery();
        return true;
    }
    else
    {
        actualizar->~QSqlQuery();
        return false;
    }
}

void Costos::calcular_monto()
{
    QSqlQuery  *calcular = new QSqlQuery();

    calcular->prepare("SELECT sum(monto) FROM Costos");
    if(calcular->exec())
    {
        calcular->next();
        Costos::monto = calcular->value(0).toDouble();
    }

    calcular->~QSqlQuery();
}

double Costos::mostrar_monto()
{
    return Costos::monto;
}


