#include "ficha_medica.h"
#include "conectarconbd.h"

ficha_medica::ficha_medica()
{
    ficha_medica::Cod_Miembro = "no";
    ficha_medica::Cod_FM = "no";
    ficha_medica::gruporh = "";
    ficha_medica::enfermedades = "";
    ficha_medica::ambientes = "";
    ficha_medica::alimentos = "";
    ficha_medica::alergico = "";
    ficha_medica::quirurguicas = "";
    ficha_medica::observaciones = "";
    ficha_medica::padece = "";
    ficha_medica::medicamentos = "";
    ficha_medica::medico = "";
    ficha_medica::direccion_med = "";
    ficha_medica::nombre_urg = "";
    ficha_medica::telefono_medico = 0;
    ficha_medica::telefono_urgencia = 0;
}

ficha_medica::ficha_medica(ficha_medica &Aux)
{
    ficha_medica::Cod_Miembro = Aux.Cod_Miembro;
    ficha_medica::Cod_FM = Aux.Cod_FM;
    ficha_medica::gruporh = Aux.gruporh;
    ficha_medica::enfermedades = Aux.enfermedades;
    ficha_medica::ambientes = Aux.ambientes;
    ficha_medica::alimentos = Aux.alimentos;
    ficha_medica::alergico = Aux.alergico;
    ficha_medica::quirurguicas = Aux.quirurguicas;
    ficha_medica::observaciones = Aux.observaciones;
    ficha_medica::padece = Aux.padece;
    ficha_medica::medicamentos = Aux.medicamentos;
    ficha_medica::medico = Aux.medico;
    ficha_medica::direccion_med = Aux.direccion_med;
    ficha_medica::nombre_urg = Aux.nombre_urg;
    ficha_medica::telefono_medico = Aux.telefono_medico ;
    ficha_medica::telefono_urgencia = Aux.telefono_urgencia;
}

ficha_medica::ficha_medica(QString Cod_Miembro,QString Cod_FM,QString gruporh, QString enfermedades, QString ambientes, QString alimentos, QString alergico, QString quirurguicas, QString observaciones, QString padece, QString medicamentos, QString medico, QString direccion_med, QString nombre_urg, int telefono_urgencia, int telefono_medico)
{
    ficha_medica::Cod_Miembro = Cod_Miembro;
    ficha_medica::Cod_FM = Cod_FM;
    ficha_medica::gruporh = gruporh;
    ficha_medica::enfermedades = enfermedades;
    ficha_medica::ambientes = ambientes;
    ficha_medica::alimentos = alimentos;
    ficha_medica::alergico = alergico;
    ficha_medica::quirurguicas = quirurguicas;
    ficha_medica::observaciones = observaciones;
    ficha_medica::padece = padece;
    ficha_medica::medicamentos = medicamentos;
    ficha_medica::medico = medico;
    ficha_medica::direccion_med = direccion_med;
    ficha_medica::nombre_urg = nombre_urg;
    ficha_medica::telefono_medico = telefono_medico;
    ficha_medica::telefono_urgencia = telefono_urgencia;
}

void ficha_medica::Leer(QString &Cod_FM,QString &gruporh, QString &enfermedades, QString &ambientes, QString &alimentos, QString &alergico, QString &quirurguicas, QString &observaciones, QString &padece, QString &medicamentos, QString &medico, QString &direccion_med, QString &nombre_urg, int & telefono_urgencia, int &telefono_medico)
{
    Cod_FM = ficha_medica::Cod_FM;
    gruporh = ficha_medica::gruporh;
    enfermedades = ficha_medica::enfermedades;
    ambientes = ficha_medica::ambientes;
    alimentos = ficha_medica::alimentos;
    alergico = ficha_medica::alergico;
    quirurguicas = ficha_medica::quirurguicas;
    observaciones = ficha_medica::observaciones;
    padece = ficha_medica::padece;
    medicamentos = ficha_medica::medicamentos;
    medico = ficha_medica::medico;
    direccion_med = ficha_medica::direccion_med;
    nombre_urg = ficha_medica::nombre_urg;
    telefono_medico = ficha_medica::telefono_medico;
    telefono_urgencia = ficha_medica::telefono_urgencia;
}

void ficha_medica::Guardar(QString Cod_FM,QString gruporh, QString enfermedades, QString ambientes, QString alimentos, QString alergico, QString quirurguicas, QString observaciones, QString padece, QString medicamentos, QString medico, QString direccion_med, QString nombre_urg, int telefono_urgencia, int telefono_medico)
{
    ficha_medica::Cod_FM = Cod_FM;
    ficha_medica::gruporh = gruporh;
    ficha_medica::enfermedades = enfermedades;
    ficha_medica::ambientes = ambientes;
    ficha_medica::alimentos = alimentos;
    ficha_medica::alergico = alergico;
    ficha_medica::quirurguicas = quirurguicas;
    ficha_medica::observaciones = observaciones;
    ficha_medica::padece = padece;
    ficha_medica::medicamentos = medicamentos;
    ficha_medica::medico = medico;
    ficha_medica::direccion_med = direccion_med;
    ficha_medica::nombre_urg = nombre_urg;
    ficha_medica::telefono_medico = telefono_medico;
    ficha_medica::telefono_urgencia = telefono_urgencia;
}

bool ficha_medica::actualizar_BD(QString Cod_Miembro)
{
    QSqlQuery *actualizar = new QSqlQuery();
    actualizar->prepare("UPDATE Ficha_Medica SET gruporh = :grh, enfermedades = :enf, ambientes = :amb, alimentos = :alim,alergico = :alerg, quirurguicas = :quir, observaciones = :obs, padece = :pad,medicamentos = :med, medico = :medic, direccion_med = :dir, nombre_urg = :nomb, telefono_urgencias = :telurg, telefono_medico = :telmed WHERE Cod_Miembro = :cod_mie"); //Cod_FM = :cod_fm

    actualizar->bindValue(":grh",ficha_medica::gruporh);
    actualizar->bindValue(":enf",ficha_medica::enfermedades);
    actualizar->bindValue(":amb",ficha_medica::ambientes);
    actualizar->bindValue(":alim",ficha_medica::alimentos);
    actualizar->bindValue(":alerg",ficha_medica::alergico);
    actualizar->bindValue(":quir",ficha_medica::quirurguicas);
    actualizar->bindValue(":obs",ficha_medica::observaciones);
    actualizar->bindValue(":pad",ficha_medica::padece);
    actualizar->bindValue(":med",ficha_medica::medicamentos);
    actualizar->bindValue(":medic",ficha_medica::medico);
    actualizar->bindValue(":dir",ficha_medica::direccion_med);
    actualizar->bindValue(":nomb",ficha_medica::nombre_urg);
    actualizar->bindValue(":cod_mie",Cod_Miembro);
    actualizar->bindValue(":telmed",ficha_medica::telefono_medico);
    actualizar->bindValue(":telurg",ficha_medica::telefono_urgencia);

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

bool ficha_medica::almacenar_BD(QString Cod_Miembro)
{
    QSqlQuery *almacenar = new QSqlQuery();

    almacenar->prepare("INSERT INTO Ficha_Medica (Cod_Miembro,Cod_FM,gruporh,enfermedades,ambientes,alimentos,alergico,quirurguicas,observaciones,padece,medicamentos,medico,direccion_med,nombre_urg,telefono_urgencias,telefono_medico) VALUES(:cod_mie,:cod_fm,:grh,:enf,:amb,:alim,:alerg,:quir,:obs,:pad,:med,:medic,:dir,:nomb,:telurg,:telmed)");
    almacenar->bindValue(":grh",ficha_medica::gruporh);
    almacenar->bindValue(":enf",ficha_medica::enfermedades);
    almacenar->bindValue(":amb",ficha_medica::ambientes);
    almacenar->bindValue(":alim",ficha_medica::alimentos);
    almacenar->bindValue(":alerg",ficha_medica::alergico);
    almacenar->bindValue(":quir",ficha_medica::quirurguicas);
    almacenar->bindValue(":obs",ficha_medica::observaciones);
    almacenar->bindValue(":pad",ficha_medica::padece);
    almacenar->bindValue(":med",ficha_medica::medicamentos);
    almacenar->bindValue(":medic",ficha_medica::medico);
    almacenar->bindValue(":dir",ficha_medica::direccion_med);
    almacenar->bindValue(":nomb",ficha_medica::nombre_urg);
    almacenar->bindValue(":cod_mie",Cod_Miembro);
    almacenar->bindValue(":cod_fm",ficha_medica::Cod_FM);
    almacenar->bindValue(":telmed",ficha_medica::telefono_medico);
    almacenar->bindValue(":telurg",ficha_medica::telefono_urgencia);

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

bool ficha_medica::borrar_BD(QString Cod_Miembro)
{
    QSqlQuery *borrando = new QSqlQuery("conectMiembros");

    borrando->prepare("DELETE from Ficha_Medica WHERE Cod_Miembro = :cod_mie");
    borrando->bindValue(":cod_mie",Cod_Miembro);

    if(borrando->exec())
    {
        borrando->~QSqlQuery();
        return true;
    }
    else
    {
        borrando->~QSqlQuery();
        return false;
    }
}

bool ficha_medica::buscar_BD(QString Cod_Miembro)
{
    QSqlQuery *buscar = new QSqlQuery("ConectMiembros");

    buscar->prepare("SELECT Cod_FM,gruporh,enfermedades,ambientes,alimentos,alergico,quirurguicas,observaciones,padece,medicamentos,medico,direccion_med,nombre_urg,telefono_urgencias,telefono_medico FROM ficha_Medica WHERE Cod_Miembro = :cod_mie");
    buscar->bindValue(":cod_mie",Cod_Miembro);

    if(buscar->exec())
    {
        buscar->next();

        ficha_medica::Cod_FM = buscar->value(0).toString();
        ficha_medica::gruporh = buscar->value(1).toString();
        ficha_medica::enfermedades = buscar->value(2).toString();
        ficha_medica::ambientes = buscar->value(3).toString();
        ficha_medica::alimentos =buscar->value(4).toString();
        ficha_medica::alergico = buscar->value(5).toString();
        ficha_medica::quirurguicas = buscar->value(6).toString();
        ficha_medica::observaciones = buscar->value(7).toString();
        ficha_medica::padece = buscar->value(8).toString();
        ficha_medica::medicamentos = buscar->value(9).toString();
        ficha_medica::medico = buscar->value(10).toString();
        ficha_medica::direccion_med = buscar->value(11).toString();
        ficha_medica::nombre_urg = buscar->value(12).toString();
        ficha_medica::telefono_urgencia = buscar->value(13).toInt();
        ficha_medica::telefono_medico = buscar->value(14).toInt();

        buscar->~QSqlQuery();
        return true;
    }
    else
    {
        buscar->~QSqlQuery();
        return false;
    }

}

void ficha_medica::generarreportFM(QString & html)
{
    //Sin Tablas
    html += //Grupo Sanguineo
            "<font size=2><b>" "Grupo Sanguineo: ""</b>" + ficha_medica::gruporh +"</font><br><br>" +

            //Enfermedades
            "<font size=2><b>" "Enfermedades: " "</b>" + ficha_medica::enfermedades +"</font><br><br>" +

            //Intolerancia a Ambientes
            "<font size=2><b>" "Intolerancia a Ambientes: " "</b>" + ficha_medica::ambientes +"</font><br><br>" +

            //Intolerancia a Alimentos
            "<font size=2><b>" "Intolerancia a Alimentos: " "</b>" + ficha_medica::alimentos +"</font><br><br>" +

            //Alergico a
            "<font size=2><b>" "Alergico a: " "</b>" + ficha_medica::alergico +"</font><br><br>" +

            //Intervenciones Quirurgicas
            "<font size=2><b>" "Intervenciones Quirurgicas: " "</b>" + ficha_medica::quirurguicas +"</font><br><br>" +

            //Padece de
            "<font size=2><b>" "Padece de: " "</b>" + ficha_medica::padece +"</font><br><br>" +

            //Medicamentos
            "<font size=2><b>" "Debe tomar los siguientes Medicamentos: " "</b>" + ficha_medica::medicamentos +"</font><br><br>" +

            //Observaciones
            "<font size=2><b>" "Observaciones: " "</b>" + ficha_medica::observaciones +"</font><br><br>" +

            //Medico de Cabecera
            "<font size=2><b>" "Medico de Cabecera: " "</b>" + ficha_medica::medico +"</font><br><br>" +

            //Direccion Medico de Cabecera
            "<font size=2><b>" "Direccion del Medico: " "</b>" + ficha_medica::direccion_med +"</font><br><br>" +

            //Telefono Medico de Cabecera
            "<font size=2><b>" "Telefono del Medico: " "</b>" + QString::number(ficha_medica::telefono_medico) + "</font><br><br>" +

            //En caso de urgencia
            "<font size=2><b>" "En caso de urgencia llamar a: " "</b>" + ficha_medica::nombre_urg + " - Telefono:" + QString::number(ficha_medica::telefono_urgencia) + "</font><br>";

}
