#include "miembro.h"

//Constructor por defecto --OK
Miembro::Miembro()
{
    Miembro::Apellido = "";
    Miembro::Nombre = "";
    Miembro::Sexo = "M";
    Miembro::Fecha_Nacimiento.setDate(2000,01,01);
    Miembro::Tipo_Documento = "DNI";
    Miembro::Documento = 0;
    Miembro::Domicilio = "";
    Miembro::Numero = 0;
    Miembro::Piso = "-";
    Miembro::Dpto = "-";
    Miembro::Telefono = 4000000;
    Miembro::Nacionalidad = "Argentina";
    Miembro::Provincia = "Santa Fe";
    Miembro::Localidad = "Santa Fe";
    Miembro::Lugar_Nac = "Santa Fe";
    Miembro::Correo_elec = "";
    Persona Pa;
    Persona Ma;
    Miembro::Padre = Pa;
    Miembro::Madre = Ma;
    Miembro::Fecha_Ingreso.setDate(2000,01,01);
    Miembro::Sacramentos = "Bautismo,Comunion,Confirmacion";
    Miembro::Estudia = "";
    Miembro::Trabaja = "";
    Miembro::Rama = "";
    Miembro::Cargo = "";
    Miembro::Patrulla = "";
    Miembro::Observaciones = "";
    Miembro::Progresiones = "";
    ficha_medica Fim;
    Miembro::FM = Fim;
    Miembro::Ma = "Activo";
    Miembro::Foto = QPixmap();
    Miembro::Cod_Miembro = "no";
}

//Para leer los datos --OK
void Miembro::Leer(QString &act,QPixmap &Fot,QString &Ape,QString &Nom,QString &S,QDate &F_N,QString &T_D,int &Doc,QString &Dom,int &Num,QString &Pi,QString &D,int &Tel,QString &Nac,QString &Prov,QString &Loc,QString &Lug_N,QString &C_elec, Persona &Per,Persona &M,QDate &F_I,QString &Sac,QString &Est,QString &Trab,QString &R,QString &P,QString &Car,QString &Obs,QString &Prog,ficha_medica &fn) //,QString Cod_Miembro
{
    Ape = Miembro::Apellido ;
    Nom = Miembro::Nombre;
    S = Miembro::Sexo;
    F_N = Miembro::Fecha_Nacimiento;
    T_D = Miembro::Tipo_Documento;
    Doc = Miembro::Documento;
    Dom = Miembro::Domicilio;
    Num = Miembro::Numero;
    Pi = Miembro::Piso;
    D = Miembro::Dpto;
    Tel = Miembro::Telefono;
    Nac = Miembro::Nacionalidad;
    Prov = Miembro::Provincia;
    Loc = Miembro::Localidad;
    Lug_N = Miembro::Lugar_Nac;
    C_elec = Miembro::Correo_elec;
    Per = Miembro::Padre;
    M = Miembro::Madre;
    F_I = Miembro::Fecha_Ingreso;
    Sac = Miembro::Sacramentos;
    Est = Miembro::Estudia;
    Trab = Miembro::Trabaja;
    R = Miembro::Rama;
    Car = Miembro::Cargo;
    P = Miembro::Patrulla;
    Obs = Miembro::Observaciones;
    Prog = Miembro::Progresiones;
    fn = Miembro::FM;
    act = Miembro::Ma;
    Fot = Miembro::Foto;
    Cod_Miembro = Miembro::Cod_Miembro;
}

//Para guardar los datos --OK
void Miembro::Guardar(QString act,QPixmap Fot,QString Ape,QString Nom,QString S,QDate F_N,QString T_D,int Doc,QString Dom,int Num,QString Pi,QString D,int Tel,QString Nac,QString Prov,QString Loc,QString Lug_N,QString C_elec,Persona Per,Persona M,QDate F_I,QString Sac,QString Est,QString Trab,QString R,QString P,QString Car,QString Obs,QString Prog,ficha_medica fn,QString Cod_Miembro)
{
    Miembro::Apellido = Ape;
    Miembro::Nombre = Nom;
    Miembro::Sexo = S;
    Miembro::Fecha_Nacimiento = F_N;
    Miembro::Tipo_Documento = T_D;
    Miembro::Documento = Doc;
    Miembro::Domicilio = Dom;
    Miembro::Numero = Num;
    Miembro::Piso = Pi;
    Miembro::Dpto = D;
    Miembro::Telefono = Tel;
    Miembro::Nacionalidad = Nac;
    Miembro::Provincia = Prov;
    Miembro::Localidad = Loc;
    Miembro::Lugar_Nac = Lug_N;
    Miembro::Correo_elec = C_elec;
    Miembro::Padre = Per;
    Miembro::Madre = M;
    Miembro::Fecha_Ingreso = F_I;
    Miembro::Sacramentos = Sac;
    Miembro::Estudia = Est;
    Miembro::Trabaja = Trab;
    Miembro::Rama = R;
    Miembro::Cargo = Car;
    Miembro::Patrulla = P;
    Miembro::Observaciones = Obs;
    Miembro::Progresiones = Prog;
    Miembro::FM = fn;
    Miembro::Ma = act;
    Miembro::Foto = Fot;
    Miembro::Cod_Miembro = Cod_Miembro;
}

//Almacenar en la BD -- OK
bool Miembro::almacenar_BD()
{
    QSqlQuery *insertar = new QSqlQuery();

    insertar->prepare("INSERT INTO Miembro (Nombre,Apellido,Fecha_Nac,Sexo,Tipo_Doc,Num_Doc,Domicilio,Numero,Piso,Depto,Telefono,Nacionalidad,Provincia,Localidad,Lugar_Nac,Correo_elec,Fecha_Ing,Sacramentos,Estudia,Trabaja,Rama,Patrulla,Cargo,Observaciones,Progresiones,Foto,Activo,Cod_Miembro) VALUES(:Nom,:Ape,:fn,:s,:td,:nd,:dom,:num,:p,:dpto,:tel,:nac,:prov,:loc,:ln,:ce,:fi,:sac,:est,:trab,:r,:pat,:cargo,:obs,:prog,:fot,:activo,:Cod_Mie)");
    insertar->bindValue(":Nom",Miembro::Nombre);
    insertar->bindValue(":Ape",Miembro::Apellido);
    insertar->bindValue(":fn",Miembro::Fecha_Nacimiento);
    insertar->bindValue(":s",Miembro::Sexo);
    insertar->bindValue(":td",Miembro::Tipo_Documento);
    insertar->bindValue(":nd",Miembro::Documento);
    insertar->bindValue(":dom",Miembro::Domicilio);
    insertar->bindValue(":num",Miembro::Numero);
    insertar->bindValue(":dpto",Miembro::Dpto);
    insertar->bindValue(":p",Miembro::Piso);
    insertar->bindValue(":tel",Miembro::Telefono);
    insertar->bindValue(":nac",Miembro::Nacionalidad);
    insertar->bindValue(":prov",Miembro::Provincia);
    insertar->bindValue(":loc",Miembro::Localidad);
    insertar->bindValue(":ln",Miembro::Lugar_Nac);
    insertar->bindValue(":ce",Miembro::Correo_elec);
    insertar->bindValue(":fi",Miembro::Fecha_Ingreso);
    insertar->bindValue(":sac",Miembro::Sacramentos);
    insertar->bindValue(":est",Miembro::Estudia);
    insertar->bindValue(":trab",Miembro::Trabaja);
    insertar->bindValue(":r",Miembro::Rama);
    insertar->bindValue(":pat",Miembro::Patrulla);
    insertar->bindValue(":cargo",Miembro::Cargo);
    insertar->bindValue(":obs",Miembro::Observaciones);
    insertar->bindValue(":prog",Miembro::Progresiones);
    insertar->bindValue(":Cod_Mie",Miembro::Cod_Miembro);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    Miembro::Foto.save(&buffer,"PNG");
    insertar->bindValue(":fot",buffer.data());

    insertar->bindValue(":activo",Miembro::Ma);

    if (insertar->exec())
    {
        if(Miembro::FM.almacenar_BD(Cod_Miembro))
        {
            if (Miembro::Padre.almacenar_BD(Miembro::Cod_Miembro) and Miembro::Madre.almacenar_BD(Miembro::Cod_Miembro))
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
        else
        {
            insertar->~QSqlQuery();
            return false;
        }

    }
    else
    {
        insertar->~QSqlQuery();
        return false;
    }
}

//Actualizar en la BD
bool Miembro::actualizar_BD(QString Cod_Miembro,bool ambos)
{
    QSqlQuery *actualiza = new QSqlQuery();

    actualiza->prepare("UPDATE Miembro SET Nombre = :Nom,Apellido = :Ape,Fecha_Nac = :fn,Sexo = :s,Tipo_Doc = :td,Num_Doc = :nd,Domicilio = :dom,Numero = :num,Piso = :p,Depto = :dpto,Telefono = :tel,Nacionalidad = :nac,Provincia = :prov,Localidad = :loc,Lugar_Nac = :ln,Correo_elec = :ce,Fecha_Ing = :fi,Sacramentos = :sac,Estudia = :est,Trabaja = :trab,Rama = :r,Patrulla = :pat,Cargo = :car,Observaciones = :obs,Progresiones = :prog,Foto = :fot,Activo = :activo WHERE Cod_Miembro = :Cod_Mie");
    actualiza->bindValue(":Nom",Miembro::Nombre);
    actualiza->bindValue(":Ape",Miembro::Apellido);
    actualiza->bindValue(":fn",Miembro::Fecha_Nacimiento);
    actualiza->bindValue(":s",Miembro::Sexo);
    actualiza->bindValue(":td",Miembro::Tipo_Documento);
    actualiza->bindValue(":nd",Miembro::Documento);
    actualiza->bindValue(":dom",Miembro::Domicilio);
    actualiza->bindValue(":num",Miembro::Numero);
    actualiza->bindValue(":p",Miembro::Piso);
    actualiza->bindValue(":dpto",Miembro::Dpto);
    actualiza->bindValue(":tel",Miembro::Telefono);
    actualiza->bindValue(":nac",Miembro::Nacionalidad);
    actualiza->bindValue(":prov",Miembro::Provincia);
    actualiza->bindValue(":loc",Miembro::Localidad);
    actualiza->bindValue(":ln",Miembro::Lugar_Nac);
    actualiza->bindValue(":ce",Miembro::Correo_elec);
    actualiza->bindValue(":fi",Miembro::Fecha_Ingreso);
    actualiza->bindValue(":sac",Miembro::Sacramentos);
    actualiza->bindValue(":est",Miembro::Estudia);
    actualiza->bindValue(":trab",Miembro::Trabaja);
    actualiza->bindValue(":r",Miembro::Rama);
    actualiza->bindValue(":pat",Miembro::Patrulla);
    actualiza->bindValue(":car",Miembro::Cargo);
    actualiza->bindValue(":obs",Miembro::Observaciones);
    actualiza->bindValue(":prog",Miembro::Progresiones);
    actualiza->bindValue(":Cod_Mie",Cod_Miembro);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    Miembro::Foto.save(&buffer,"PNG");
    actualiza->bindValue(":fot",buffer.data());

    actualiza->bindValue(":activo",Miembro::Ma);

    //Primer caso es el que se quiera modificar ambos padres
    if (actualiza->exec())
    {
        if(Miembro::FM.actualizar_BD(Miembro::Cod_Miembro))
        {
            if (ambos)
            {
                if (Miembro::Padre.actualizar_BD(Cod_Miembro) and Miembro::Madre.actualizar_BD(Cod_Miembro))
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
                if (Miembro::Padre.actualizar_BD(Cod_Miembro) or Miembro::Madre.actualizar_BD(Cod_Miembro))
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

    }
    else       
    {
        actualiza->~QSqlQuery();
        return false;
    }
    actualiza->~QSqlQuery();
    return false;

}

//Borrar en la BD
bool Miembro::borrar_BD(QString Cod_Miembro)
{
    QSqlQuery *elimina = new QSqlQuery();
    elimina->prepare("DELETE from Miembro WHERE Cod_Miembro = :Cod_Mie");
    elimina->bindValue(":Cod_Mie",Cod_Miembro);

    elimina->exec();

    if (elimina->isValid() and !elimina->isNull(0))
    {
        if (Miembro::Padre.borrar_BD(Cod_Miembro) and Miembro::Madre.borrar_BD(Cod_Miembro))
        {
            elimina->~QSqlQuery();
            return true;
        }
        else
        {
            elimina->~QSqlQuery();
            return false;
        }
    }
    else
    {
        elimina->~QSqlQuery();
        return false;
    }
}

//Buscar en la BD --OK
void Miembro::buscar_BD(QString Cod_Miembro)
{
    QSqlQuery *consulta = new QSqlQuery();

    consulta->prepare("SELECT Nombre,Apellido,Fecha_Nac,Sexo,Tipo_Doc,Num_Doc,Domicilio,Numero,Piso,Depto,Telefono,Nacionalidad,Provincia,Localidad,Lugar_Nac,Correo_elec,Fecha_Ing,Sacramentos,Estudia,Trabaja,Rama,Patrulla,Cargo,Observaciones,Progresiones,Foto,Activo,Cod_Miembro FROM Miembro WHERE Cod_Miembro = :Cod_Mie");
    consulta->bindValue(":Cod_Mie",Cod_Miembro);

    if (consulta->exec())
    {
        consulta->next();
        Persona PadreM,MadreM;
        ficha_medica fm;

        PadreM.buscar_BD(Cod_Miembro,"M");
        MadreM.buscar_BD(Cod_Miembro,"F");
        fm.buscar_BD(Cod_Miembro);

        Miembro::Nombre = consulta->value(0).toString();
        Miembro::Apellido = consulta->value(1).toString();
        Miembro::Fecha_Nacimiento = consulta->value(2).toDate();
        Miembro::Sexo = consulta->value(3).toString();//transformar de alguna forma
        Miembro::Tipo_Documento = consulta->value(4).toString();
        Miembro::Documento = consulta->value(5).toInt();
        Miembro::Domicilio = consulta->value(6).toString();
        Miembro::Numero = consulta->value(7).toInt();
        Miembro::Piso = consulta->value(8).toString();
        Miembro::Dpto = consulta->value(9).toString();
        Miembro::Telefono = consulta->value(10).toInt();
        Miembro::Nacionalidad = consulta->value(11).toString();
        Miembro::Provincia = consulta->value(12).toString();
        Miembro::Localidad = consulta->value(13).toString();
        Miembro::Lugar_Nac = consulta->value(14).toString();
        Miembro::Correo_elec = consulta->value(15).toString();
        Miembro::Padre = PadreM;
        Miembro::Madre = MadreM;
        Miembro::FM = fm;
        Miembro::Fecha_Ingreso = consulta->value(16).toDate();
        Miembro::Sacramentos = consulta->value(17).toString();
        Miembro::Estudia = consulta->value(18).toString();
        Miembro::Trabaja = consulta->value(19).toString();
        Miembro::Rama = consulta->value(20).toString();
        Miembro::Cargo = consulta->value(22).toString();
        Miembro::Patrulla = consulta->value(21).toString();
        Miembro::Observaciones = consulta->value(23).toString();
        Miembro::Progresiones = consulta->value(24).toString();

        QByteArray bytes = consulta->value(25).toByteArray();
        if(bytes.isNull())
        {
            Miembro::Foto = QPixmap();
        }
        else
        {
            Miembro::Foto.loadFromData(bytes);
        }


        Miembro::Ma = consulta->value(26).toString();

    }
    consulta->~QSqlQuery();

}

//devuelve si se encuentra o no en la BD
bool Miembro::se_encuentra(QString Cod_Miembro)
{
    QSqlQuery *busca = new QSqlQuery();
    busca->prepare("select * from Miembro where Cod_Miembro = :Cod_Mie");
    busca->bindValue(":Cod_mie",Cod_Miembro);
    busca->exec();
    busca->next();

    if (busca->isValid() and !busca->isNull(0))
    {
        busca->~QSqlQuery();
        return true;
    }
    else
    {
        busca->~QSqlQuery();
        return false;
    }
}

//recupera el apellido de la persona a partir del primary key
QString Miembro::rec_ape(QString Cod_Miembro)
{
    QString Apellido;
    QSqlQuery *ape = new QSqlQuery();
    ape->prepare("select Apellido from Miembro where Cod_Miembro = :Cod_Mie");
    ape->bindValue(":Cod_Mie",Cod_Miembro);
    ape->exec();
    ape->next();

    Apellido = ape->value(0).toString();

    ape->~QSqlQuery();
    return Apellido;

}

//recupera el nombre de la persona a partir del primary key
QString Miembro::rec_nom(QString Cod_Miembro)
{
    QString Nombre;
    QSqlQuery *nom = new QSqlQuery();
    nom->prepare("select Nombre from Miembro where Cod_Miembro = :Cod_Mie");
    nom->bindValue("Cod_Mie",Cod_Miembro);
    nom->exec();
    nom->next();

    Nombre = nom->value(0).toString();
    nom->~QSqlQuery();
    return Nombre;

}

//Datasheet del miembro en html
void Miembro::generarreport(QString & html)
{
    //Foto
    if(!Foto.isNull())
    {
        Miembro::Foto.save("Foto.JPG");
    }
    else
    {
        if(Sexo == "M")
        {
            QPixmap(":/icons/male").save("Foto.JPG");
        }
        else
        {
            QPixmap(":/icons/female").save("Foto.JPG");
        }
    }

    html += //Tabla 1 para acomodar los datos de forma ordenada
            "<table border=0 align=center width=100% cellpadding=1 cellspacing=10>"

            //Nombre y Apellido
            "<tr><td align=left><font size=2><b>" "Miembro:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Apellido + ", " + Miembro::Nombre + "</font>"

            //Rama
            "<td align=left><font size=2><b>" "Rama:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Rama + "</font>"

            //Imagen
            "<td align=right rowspan=4> <img src= Foto.JPG border=10 width=80 height=80 align=center>"

            //Documento
            "<tr><td align=left><font size=2><b>" "Documento:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Tipo_Documento + " " + QString::number(Miembro::Documento) + "</font>"

            //Fecha de Nacimiento
            "<td align=left><font size=2><b>" "Fecha de Nacimiento:" "</b></font>"
            "<td align=left><font size=2>" + QString::number(Miembro::Fecha_Nacimiento.day())+ "/" + QString::number(Miembro::Fecha_Nacimiento.month()) + "/" + QString::number(Miembro::Fecha_Nacimiento.year()) + "</font>"

            //Direccion
            "<tr><td align=left><font size=2><b>" "Direccion:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Domicilio + " " + QString::number(Miembro::Numero) + "</font>"

            //Telefono
            "<td align=left><font size=2><b>" "Telefono:" "</b></font>"
            "<td align=left><font size=2>" + QString::number(Miembro::Telefono) + "</font>"

            //Estudia
            "<tr><td align=left><font size=2><b>" "Estudia:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Estudia+ "</font>"

            //Trabaja
            "<td align=left><font size=2><b>" "Trabaja:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Trabaja + "</font>"

            //Nacionalidad
            "<tr><td align=left><font size=2><b>" "Nacionalidad:" "</b></font>"
            "<td align=left><font size=2>" + Miembro::Nacionalidad + "</font>"

            //Lugar de Nacimiento
            "<td align=left><font size=2><b>" "Lugar de Nacimiento:" "</b></font>"
            "<td align=left colspan=2><font size=2>" + Miembro::Lugar_Nac + "</font>"

            //Scramentos
            "<tr><td align=left><font size=2><b>" "Sacramentos:" "</b></font>"
            "<td align=left colspan=3><font size=2>" + Miembro::Sacramentos + "</font>"

            //Fin Tabla 1
            "</table>";

}

bool Miembro::tienefoto()
{
    if(Foto.isNull())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Miembro::borra_foto()
{
    Foto = NULL;
}

void Miembro::grP(QString & html)
{
    Padre.generarreportP(html);
}

void Miembro::grM(QString & html)
{
    Madre.generarreportP(html);
}

void Miembro::grFM(QString & html)
{
    FM.generarreportFM(html);
}


