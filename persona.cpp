#include "persona.h"

//Constructor por Defecto
Persona::Persona()
{
    Persona::Apellido = "";
    Persona::Nombre = "";
    Persona::Sexo = "";
    Persona::Fecha_Nacimiento.setDate(2000,01,01);
    Persona::Tipo_Documento = "DNI";
    Persona::Documento = 0;
    Persona::Domicilio = "";
    Persona::Numero = 0;
    Persona::Piso = "-";
    Persona::Dpto = "-";
    Persona::Telefono = 4000000;
    Persona::Nacionalidad = "Argentina";
    Persona::Provincia = "Santa Fe";
    Persona::Localidad = "Santa Fe";
    Persona::Lugar_Nac = "Santa Fe";
    Persona::Correo_elec = "";
    Persona::Cod_Persona = "no";
}

//Constructor
Persona::Persona(QString Ape,QString Nom,QString S,QDate F_N,QString T_D,int Doc,QString Dom,int Num,QString P,QString D,int Tel,QString Nac,QString Prov,QString Loc,QString Lug_N,QString C_elec,QString Cod_Persona)
{
    Persona::Apellido = Ape;
    Persona::Nombre = Nom;
    Persona::Sexo = S;
    Persona::Fecha_Nacimiento = F_N;
    Persona::Tipo_Documento = T_D;
    Persona::Documento = Doc;
    Persona::Domicilio = Dom;
    Persona::Numero = Num;
    Persona::Piso = P;
    Persona::Dpto = D;
    Persona::Telefono = Tel;
    Persona::Nacionalidad = Nac;
    Persona::Provincia = Prov;
    Persona::Localidad = Loc;
    Persona::Lugar_Nac = Lug_N;
    Persona::Correo_elec = C_elec;
    Persona::Cod_Persona = Cod_Persona;
}

//Constructor de copia
Persona::Persona(Persona &P)
{
    Persona::Apellido = P.Apellido;
    Persona::Nombre = P.Nombre;
    Persona::Sexo = P.Sexo;
    Persona::Fecha_Nacimiento = P.Fecha_Nacimiento;
    Persona::Tipo_Documento = P.Tipo_Documento;
    Persona::Documento = P.Documento;
    Persona::Domicilio = P.Domicilio;
    Persona::Numero = P.Numero;
    Persona::Piso = P.Piso;
    Persona::Dpto = P.Dpto;
    Persona::Telefono = P.Telefono;
    Persona::Nacionalidad = P.Nacionalidad;
    Persona::Provincia = P.Provincia;
    Persona::Localidad = P.Localidad;
    Persona::Lugar_Nac = P.Lugar_Nac;
    Persona::Correo_elec = P.Correo_elec;
    Persona::Cod_Persona = P.Cod_Persona;

}

//Sobrecargo el = para poder copiar de una variable a la otra
Persona& Persona::operator =(const Persona &P)
{
    Persona::Apellido = P.Apellido;
    Persona::Nombre = P.Nombre;
    Persona::Sexo = P.Sexo;
    Persona::Fecha_Nacimiento = P.Fecha_Nacimiento;
    Persona::Tipo_Documento = P.Tipo_Documento;
    Persona::Documento = P.Documento;
    Persona::Domicilio = P.Domicilio;
    Persona::Numero = P.Numero;
    Persona::Piso = P.Piso;
    Persona::Dpto = P.Dpto;
    Persona::Telefono = P.Telefono;
    Persona::Nacionalidad = P.Nacionalidad;
    Persona::Provincia = P.Provincia;
    Persona::Localidad = P.Localidad;
    Persona::Lugar_Nac = P.Lugar_Nac;
    Persona::Correo_elec = P.Correo_elec;
    Persona::Cod_Persona = P.Cod_Persona;
    return *this;
}

//Para leer los datos --OK
void Persona::Leer(QString &Ape,QString &Nom,QString &S,QDate &F_N,QString &T_D,int &Doc,QString &Dom,int &Num,QString &P,QString &D,int &Tel,QString &Nac,QString &Prov,QString &Loc,QString &Lug_N,QString &C_elec,QString &Cod_Persona)
{
    Ape = Persona::Apellido ;
    Nom = Persona::Nombre;
    S = Persona::Sexo;
    F_N = Persona::Fecha_Nacimiento;
    T_D = Persona::Tipo_Documento;
    Doc = Persona::Documento;
    Dom = Persona::Domicilio;
    Num = Persona::Numero;
    P = Persona::Piso;
    D = Persona::Dpto;
    Tel = Persona::Telefono;
    Nac = Persona::Nacionalidad;
    Prov = Persona::Provincia;
    Loc = Persona::Localidad;
    Lug_N = Persona::Lugar_Nac;
    C_elec = Persona::Correo_elec;
    Cod_Persona = Persona::Cod_Persona;
}

//Para guardar los datos en las variables --OK
void Persona::Guardar(QString Ape,QString Nom,QString S,QDate F_N,QString T_D,int Doc,QString Dom,int Num,QString P,QString D,int Tel,QString Nac,QString Prov,QString Loc,QString Lug_N,QString C_elec,QString Cod_Persona)
{
    Persona::Apellido = Ape;
    Persona::Nombre = Nom;
    Persona::Sexo = S;
    Persona::Fecha_Nacimiento = F_N;
    Persona::Tipo_Documento = T_D;
    Persona::Documento = Doc;
    Persona::Domicilio = Dom;
    Persona::Numero = Num;
    Persona::Piso = P;
    Persona::Dpto = D;
    Persona::Telefono = Tel;
    Persona::Nacionalidad = Nac;
    Persona::Provincia = Prov;
    Persona::Localidad = Loc;
    Persona::Lugar_Nac = Lug_N;
    Persona::Correo_elec = C_elec;
    Persona::Cod_Persona = Cod_Persona;
}

bool Persona::almacenar_BD(QString Cod_Miembro)
{
    //Inserta segun el sexo en la tabla Padre o Madre
    QSqlQuery *inserta = new QSqlQuery();
    if (Persona::Sexo == "F")
    {
        inserta->prepare("INSERT INTO Madre (Nombre,Apellido,Fecha_Nac,Sexo,Tipo_Doc,Num_Doc,Cod_Miembro,Cod_Madre,Domicilio,Numero,Piso,Depto,Telefono,Nacionalidad,Provincia,Localidad,Lugar_Nac,Correo_elec) VALUES (:Nom,:Ape,:fn,:s,:td,:nd,:Cod_Mie,:Cod_Ma,:dom,:num,:p,:dpto,:tel,:nac,:prov,:loc,:ln,:ce)");
        inserta->bindValue(":Nom",Persona::Nombre);
        inserta->bindValue(":Ape",Persona::Apellido);
        inserta->bindValue(":fn",Persona::Fecha_Nacimiento);
        inserta->bindValue(":s",Persona::Sexo);
        inserta->bindValue(":td",Persona::Tipo_Documento);
        inserta->bindValue(":nd",Persona::Documento);
        inserta->bindValue(":Cod_Mie",Cod_Miembro);
        inserta->bindValue(":Cod_Ma",Persona::Cod_Persona);
        inserta->bindValue(":dom",Persona::Domicilio);
        inserta->bindValue(":num",Persona::Numero);
        inserta->bindValue(":p",Persona::Piso);
        inserta->bindValue(":dpto",Persona::Dpto);
        inserta->bindValue(":tel",Persona::Telefono);
        inserta->bindValue(":nac",Persona::Nacionalidad);
        inserta->bindValue(":prov",Persona::Provincia);
        inserta->bindValue(":loc",Persona::Localidad);
        inserta->bindValue(":ln",Persona::Lugar_Nac);
        inserta->bindValue(":ce",Persona::Correo_elec);

        if (inserta->exec())
        {
            inserta->~QSqlQuery();
            return true;
        }
        else
        {
            inserta->~QSqlQuery();
            return false;
        }

    }
    else
    {
        if(Persona::Sexo == "M")
        {
            inserta->prepare("INSERT INTO Padre (Nombre,Apellido,Fecha_Nac,Sexo,Tipo_Doc,Num_Doc,Cod_miembro,Cod_Padre,Domicilio,Numero,Piso,Depto,Telefono,Nacionalidad,Provincia,Localidad,Lugar_Nac,Correo_elec) VALUES (:Nom,:Ape,:fn,:s,:td,:nd,:Cod_Mie,:Cod_Pa,:dom,:num,:p,:dpto,:tel,:nac,:prov,:loc,:ln,:ce)");
            inserta->bindValue(":Nom",Persona::Nombre);
            inserta->bindValue(":Ape",Persona::Apellido);
            inserta->bindValue(":fn",Persona::Fecha_Nacimiento);
            inserta->bindValue(":s",Persona::Sexo);
            inserta->bindValue(":td",Persona::Tipo_Documento);
            inserta->bindValue(":nd",Persona::Documento);
            inserta->bindValue(":Cod_Mie",Cod_Miembro);
            inserta->bindValue(":Cod_Pa",Persona::Cod_Persona);
            inserta->bindValue(":dom",Persona::Domicilio);
            inserta->bindValue(":num",Persona::Numero);
            inserta->bindValue(":p",Persona::Piso);
            inserta->bindValue(":dpto",Persona::Dpto);
            inserta->bindValue(":tel",Persona::Telefono);
            inserta->bindValue(":nac",Persona::Nacionalidad);
            inserta->bindValue(":prov",Persona::Provincia);
            inserta->bindValue(":loc",Persona::Localidad);
            inserta->bindValue(":ln",Persona::Lugar_Nac);
            inserta->bindValue(":ce",Persona::Correo_elec);

            if (inserta->exec())
            {
                inserta->~QSqlQuery();
                return true;
            }
            else
            {
                inserta->~QSqlQuery();
                return false;
            }
        }

    }
    inserta->~QSqlQuery();
    return false;

}

bool Persona::actualizar_BD(QString Cod_Miembro)
{
    QSqlQuery *actualiza = new QSqlQuery();
    if (Persona::Sexo == "F")
    {
        actualiza->prepare("UPDATE Madre SET Nombre = :Nom,Apellido = :Ape,Fecha_Nac = :fn,Sexo = :s,Tipo_Doc = :td,Num_Doc = :nd,Domicilio = :dom,Numero = :num,Piso = :p,Depto = :dpto,Telefono = :tel,Nacionalidad = :nac,Provincia = :prov,Localidad = :loc,Lugar_Nac = :ln,Correo_elec = :ce WHERE Cod_Miembro = :Cod_Mie and Cod_Madre = :Cod_Ma"); // and Cod_Madre = :Cod_Ma
        actualiza->bindValue(":Nom",Persona::Nombre);
        actualiza->bindValue(":Ape",Persona::Apellido);
        actualiza->bindValue(":fn",Persona::Fecha_Nacimiento);
        actualiza->bindValue(":s",Persona::Sexo);
        actualiza->bindValue(":td",Persona::Tipo_Documento);
        actualiza->bindValue(":nd",Persona::Documento);
        actualiza->bindValue(":Cod_Mie",Cod_Miembro);
        actualiza->bindValue(":Cod_Ma",Persona::Cod_Persona);
        actualiza->bindValue(":dom",Persona::Domicilio);
        actualiza->bindValue(":num",Persona::Numero);
        actualiza->bindValue(":p",Persona::Piso);
        actualiza->bindValue(":dpto",Persona::Dpto);
        actualiza->bindValue(":tel",Persona::Telefono);
        actualiza->bindValue(":nac",Persona::Nacionalidad);
        actualiza->bindValue(":prov",Persona::Provincia);
        actualiza->bindValue(":loc",Persona::Localidad);
        actualiza->bindValue(":ln",Persona::Lugar_Nac);
        actualiza->bindValue(":ce",Persona::Correo_elec);
        //actualiza->bindValue(":tipdoc",Tip_Doc);
       //actualiza->bindValue(":numdoc",Num);


        if (actualiza->exec())
        {
            actualiza->~QSqlQuery();
            return true;
        }
        else
        {
            actualiza->~QSqlQuery();
            return false;
        }

    }
    else
    {
        if(Persona::Sexo == "M")
        {
            actualiza->prepare("UPDATE Padre SET Nombre = :Nom,Apellido = :Ape,Fecha_Nac = :fn,Sexo = :s,Tipo_Doc = :td,Num_Doc = :nd,Domicilio = :dom,Numero = :num,Piso = :p,Depto = :dpto,Telefono = :tel,Nacionalidad = :nac,Provincia = :prov,Localidad = :loc,Lugar_Nac = :ln,Correo_elec = :ce WHERE Cod_Miembro = :Cod_Mie and Cod_Padre = :Cod_Pa");
            actualiza->bindValue(":Nom",Persona::Nombre);
            actualiza->bindValue(":Ape",Persona::Apellido);
            actualiza->bindValue(":fn",Persona::Fecha_Nacimiento);
            actualiza->bindValue(":s",Persona::Sexo);
            actualiza->bindValue(":td",Persona::Tipo_Documento);
            actualiza->bindValue(":nd",Persona::Documento);
            actualiza->bindValue(":Cod_Mie",Cod_Miembro);
            actualiza->bindValue(":Cod_Pa",Persona::Cod_Persona);
            actualiza->bindValue(":dom",Persona::Domicilio);
            actualiza->bindValue(":num",Persona::Numero);
            actualiza->bindValue(":p",Persona::Piso);
            actualiza->bindValue(":dpto",Persona::Dpto);
            actualiza->bindValue(":tel",Persona::Telefono);
            actualiza->bindValue(":nac",Persona::Nacionalidad);
            actualiza->bindValue(":prov",Persona::Provincia);
            actualiza->bindValue(":loc",Persona::Localidad);
            actualiza->bindValue(":ln",Persona::Lugar_Nac);
            actualiza->bindValue(":ce",Persona::Correo_elec);


            if (actualiza->exec())
            {
                actualiza->~QSqlQuery();
                return true;
            }
            else
            {
                actualiza->~QSqlQuery();
                return false;
            }
        }

    }

    actualiza->~QSqlQuery();
    return false;
}

bool Persona::borrar_BD(QString Cod_Miembro)
{
    QSqlQuery *elimina = new QSqlQuery();

    if (Persona::Sexo == "F")
    {
        elimina->prepare("DELETE from Madre WHERE Cod_Miembro = :Cod_Mie");
        elimina->bindValue(":Cod_Mie",Cod_Miembro);
        elimina->exec();

        if (!elimina->isValid())
        {
            elimina->~QSqlQuery();
            return false;
        }
        elimina->~QSqlQuery();
        return true;
    }
    else
    {
        if (Persona::Sexo == "M")
        {
            elimina->prepare("DELETE from Padre WHERE Cod_Miembro = :Cod_Mie");
            elimina->bindValue(":Cod_Mie",Cod_Miembro);
            elimina->exec();

            if (!elimina->isValid())
            {
                elimina->~QSqlQuery();
                return false;
            }
            elimina->~QSqlQuery();
            return true;
        }

    }

    elimina->~QSqlQuery();
    return false;

}

void Persona::buscar_BD(QString Cod_Miembro,QString sexo)
{
    QSqlQuery *consulta = new QSqlQuery();
    if(sexo == "M")
    {
        consulta->prepare("select Nombre,Apellido,Fecha_Nac,Sexo,Tipo_Doc,Num_Doc,Cod_Padre,Domicilio,Numero,Piso,Depto,Telefono,Nacionalidad,Provincia,Localidad,Lugar_Nac,Correo_elec from Padre where Cod_Miembro = :Cod_Mie");
        consulta->bindValue(":Cod_Mie",Cod_Miembro);

        if (consulta->exec())
        {
            consulta->next();
            if(consulta->isNull(6) == false)
            {
                Persona::Nombre = consulta->value(0).toString();
                Persona::Apellido = consulta->value(1).toString();
                Persona::Fecha_Nacimiento = consulta->value(2).toDate();
                Persona::Sexo = consulta->value(3).toString();
                Persona::Tipo_Documento = consulta->value(4).toString();
                Persona::Documento = consulta->value(5).toInt();
                Persona::Cod_Persona = consulta->value(6).toString();
                Persona::Domicilio = consulta->value(7).toString();
                Persona::Numero = consulta->value(8).toInt();
                Persona::Piso = consulta->value(9).toString();
                Persona::Dpto = consulta->value(10).toString();
                Persona::Telefono = consulta->value(11).toInt();
                Persona::Nacionalidad = consulta->value(12).toString();
                Persona::Provincia = consulta->value(13).toString();
                Persona::Localidad = consulta->value(14).toString();
                Persona::Lugar_Nac = consulta->value(15).toString();
                Persona::Correo_elec = consulta->value(16).toString();
            }
        }
    }
    else
    {
        if (sexo == "F")
        {
            consulta->prepare("SELECT Nombre,Apellido,Fecha_Nac,Sexo,Tipo_Doc,Num_Doc,Cod_Madre,Domicilio,Numero,Piso,Depto,Telefono,Nacionalidad,Provincia,Localidad,Lugar_Nac,Correo_elec FROM Madre WHERE Cod_Miembro = :Cod_Mie");
            consulta->bindValue(":Cod_Mie",Cod_Miembro);

            if (consulta->exec())
            {
                consulta->next();
                if(consulta->isNull(6) == false)
                {
                    Persona::Nombre = consulta->value(0).toString();
                    Persona::Apellido = consulta->value(1).toString();
                    Persona::Fecha_Nacimiento = consulta->value(2).toDate();
                    Persona::Sexo = consulta->value(3).toString();
                    Persona::Tipo_Documento = consulta->value(4).toString();
                    Persona::Documento = consulta->value(5).toInt();
                    Persona::Cod_Persona = consulta->value(6).toString();
                    Persona::Domicilio = consulta->value(7).toString();
                    Persona::Numero = consulta->value(8).toInt();
                    Persona::Piso = consulta->value(9).toString();
                    Persona::Dpto = consulta->value(10).toString();
                    Persona::Telefono = consulta->value(11).toInt();
                    Persona::Nacionalidad = consulta->value(12).toString();
                    Persona::Provincia = consulta->value(13).toString();
                    Persona::Localidad = consulta->value(14).toString();
                    Persona::Lugar_Nac = consulta->value(15).toString();
                    Persona::Correo_elec = consulta->value(16).toString();

                }
            }
        }

    }

    consulta->~QSqlQuery();
}

bool Persona::estaenBD(QString Cod_Miembro, QString sexo)
{
    QSqlQuery *consulta = new QSqlQuery();
    if (sexo == "F")
    {
        consulta->prepare("SELECT Cod_Miembro From Madre WHERE Cod_Miembro = :Cod_Mie"); //Cod_Madre = :Cod_Ma
        consulta->bindValue(":Cod_Mie",Cod_Miembro);

        if(consulta->exec())
        {
            consulta->next();
            if(consulta->isNull(0) == false)
            {
                consulta->~QSqlQuery();
                return true;
            }
            else
            {
                consulta->~QSqlQuery();
                return false;
            }

        }
        else
        {
            consulta->~QSqlQuery();
            return false;
        }
    }
    else
    {
        if(sexo == "M")
        {
            consulta->prepare("SELECT Cod_Miembro From Padre WHERE Cod_Miembro = :Cod_Mie"); // AND Cod_Padre = :Cod_Pa
            consulta->bindValue(":Cod_Mie",Cod_Miembro);

            if(consulta->exec())
            {
                consulta->next();
                if(consulta->isNull(0) == false)
                {
                    consulta->~QSqlQuery();
                    return true;
                }
                else
                {
                    consulta->~QSqlQuery();
                    return false;
                }

            }
            else
            {
                consulta->~QSqlQuery();
                return false;
            }
        }
    }
    consulta->~QSqlQuery();
    return false;
}

//Busca y crea la tabla relacional para no crear padres repetidos
bool Persona::Llena_siexisten(QString Apellidox)
{
    QSqlTableModel *AuxP = new QSqlTableModel();
    AuxP->setTable("Padre");
    QString filtroP = "Apellido = '" + Apellidox + "'";
    AuxP->setFilter(filtroP);
    AuxP->select();

    QSqlTableModel *AuxM = new QSqlTableModel();
    AuxM->setTable("Madre");
    QString filtroM = "Apellido = '" + Apellidox + "'";
    AuxM->setFilter(filtroM);
    AuxM->select();

    QSqlQuery *consultaAux = new QSqlQuery();

    //Si lo encuentra el porque figuara la madre o el padre
    //Caso que existe el apellido en los padres o en las madres para los padres en pareja

    if(AuxM->rowCount() > 0 or AuxP->rowCount() > 0)
    {
        //Caso que existen ambos
        consultaAux->prepare("INSERT INTO PadMa SELECT P.Nombre,P.Apellido,P.Cod_Padre,M.Nombre,M.Apellido,M.Cod_Madre,Mi.Nombre,Mi.Apellido,Mi.Cod_Miembro FROM Padre P INNER JOIN Miembro Mi ON P.Cod_Miembro = Mi.Cod_Miembro INNER JOIN Madre M ON Mi.Cod_Miembro = M.Cod_Miembro WHERE P.Apellido = :apellido");
        consultaAux->bindValue(":apellido",Apellidox);
        consultaAux->exec();

        //Caso que existe en las madres
        if(AuxM->rowCount() > 0 and AuxP->rowCount() == 0)
        {
            consultaAux->prepare("INSERT INTO PadMa SELECT '','','',M.Nombre,M.Apellido,M.Cod_Madre,Mi.Nombre,Mi.Apellido,Mi.Cod_Miembro from Madre M INNER JOIN Miembro Mi ON M.Cod_Miembro = Mi.Cod_Miembro WHERE M.Apellido = :apellido");
            consultaAux->bindValue(":apellido",Apellidox);
            consultaAux->exec();

            consultaAux->~QSqlQuery();
            AuxP->~QSqlTableModel();
            AuxM->~QSqlTableModel();

            return true;
        }

        //Caso que existe en los padres
        if(AuxP->rowCount() > 0 and AuxM->rowCount() == 0)
        {
            consultaAux->prepare("INSERT INTO PadMa SELECT P.Nombre,P.Apellido,P.Cod_Padre,'','','',Mi.Nombre,Mi.Apellido,Mi.Cod_Miembro from Padre P INNER JOIN Miembro Mi ON P.Cod_Miembro = Mi.Cod_Miembro WHERE P.Apellido = :apellido");
            consultaAux->bindValue(":apellido",Apellidox);
            consultaAux->exec();

            consultaAux->~QSqlQuery();
            AuxP->~QSqlTableModel();
            AuxM->~QSqlTableModel();

            return true;
        }
    }

    consultaAux->~QSqlQuery();
    AuxP->~QSqlTableModel();
    AuxM->~QSqlTableModel();
    return false;

}

//Generar el reporte para la impresion
void Persona::generarreportP(QString & html)
{

    html += //Tabla 1 para acomodar los datos de forma ordenada
            "<table border=0 align=center width=100% cellpadding=1 cellspacing=10>"

            //Nombre y Apellido
            "<tr><td align=left><font size=2><b>" "Nombre y Apellido:" "</b></font>"
            "<td align=left><font size=2>" + Persona::Apellido + ", " + Persona::Nombre + "</font>"

            //Documento
            "<tr><td align=left><font size=2><b>" "Documento:" "</b></font>"
            "<td align=left><font size=2>" + Persona::Tipo_Documento + " " + QString::number(Persona::Documento) + "</font>"

            //Fecha de Nacimiento
            "<td align=left><font size=2><b>" "Fecha de Nacimiento:" "</b></font>"
            "<td align=left><font size=2>" + QString::number(Persona::Fecha_Nacimiento.day())+ "/" + QString::number(Persona::Fecha_Nacimiento.month()) + "/" + QString::number(Persona::Fecha_Nacimiento.year()) + "</font>"

            //Direccion
            "<tr><td align=left><font size=2><b>" "Direccion:" "</b></font>"
            "<td align=left><font size=2>" + Persona::Domicilio + " " + QString::number(Persona::Numero) + "</font>"

            //Telefono
            "<td align=left><font size=2><b>" "Telefono:" "</b></font>"
            "<td align=left><font size=2>" + QString::number(Persona::Telefono) + "</font>"

            //Nacionalidad
            "<tr><td align=left><font size=2><b>" "Nacionalidad:" "</b></font>"
            "<td align=left><font size=2>" + Persona::Nacionalidad + "</font>"

            //Lugar de Nacimiento
            "<td align=left><font size=2><b>" "Lugar de Nacimiento:" "</b></font>"
            "<td align=left><font size=2>" + Persona::Lugar_Nac + "</font>"

            //Fin Tabla 1
            "</table>";


}
