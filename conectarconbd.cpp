#include "conectarconbd.h"

//Constructor
ConectarconBD::ConectarconBD()
{
    //Configuracion del motor de base de datos y nombre del archivo
    db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("DB.sqlite");
    crearestructura();
}

//Inicializacion de la base de datos y creacion de tablas
bool ConectarconBD::crearestructura()
{

    //Abre la base de datos y muestra un aviso en caso de error
    if (!db->open()) {
      QMessageBox::critical(0, qApp->tr("Error al abrir la BD"),qApp->tr("Ha sido imposible abrir la Base de Datos"),QMessageBox::Ok);
      return false;
    }
    else
    {
    //Esta seccion seria para crear las tablas de la base de Datos

    QSqlQuery *query = new QSqlQuery();
      query->exec("create table if not exists Miembro (Nombre varchar(20) not null,Apellido varchar(20) not null,Fecha_Nac date not null,Sexo varchar(1) not null,Tipo_Doc varchar(4) not null,Num_Doc int not null,Domicilio varchar(50) not null,Numero integer null,Piso varchar(2) null,Depto varchar(10) null,Telefono integer null,Nacionalidad varchar(20) not null,Provincia varchar(25) not null,Localidad varchar(25) not null,Lugar_Nac varchar(50) not null,Correo_elec varchar(25) null,Fecha_Ing date not null,Sacramentos varchar(30) null,Estudia varchar(30) null,Trabaja varchar(30) null,Rama varchar(15) not null,Patrulla varchar(15) null,Cargo varchar(15) null,Observaciones varchar(300) null,Progresiones varchar(50) null,Foto blob null,Activo varchar(15) not null,Cod_Miembro varchar(10) not null,Saldo double null,constraint pk_miembro primary key (Cod_Miembro))");
      query->exec("create table if not exists Padre (Nombre varchar(20) not null,Apellido varchar(20) not null,Fecha_Nac date not null,Sexo varchar(1) not null,Tipo_Doc varchar(4) not null,Num_Doc int not null,Cod_Miembro varchar(10) not null,Cod_Padre varchar(10) not null,Domicilio varchar(50) not null,Numero integer null,Piso varchar(2) null,Depto varchar(10) null,Telefono integer null,Nacionalidad varchar(20) not null,Provincia varchar(25) not null,Localidad varchar(25) not null,Lugar_Nac varchar(50) not null,Correo_elec varchar(25) null,constraint pk_padre primary key (Cod_Padre,Cod_Miembro), constraint fk_padre_miembro foreign key (Cod_Miembro) references Miembro(Cod_Miembro))");
      query->exec("create table if not exists Madre (Nombre varchar(20) not null,Apellido varchar(20) not null,Fecha_Nac date not null,Sexo varchar(1) not null,Tipo_Doc varchar(4) not null,Num_Doc int not null,Cod_Miembro varchar(10) not null,Cod_Madre varchar(10) not null,Domicilio varchar(50) not null,Numero integer null,Piso varchar(2) null,Depto varchar(10) null,Telefono integer null,Nacionalidad varchar(20) not null,Provincia varchar(25) not null,Localidad varchar(25) not null,Lugar_Nac varchar(50) not null,Correo_elec varchar(25) null,constraint pk_madre primary key (Cod_Madre,Cod_Miembro), constraint fk_madre_miembro foreign key (Cod_Miembro) references Miembro(Cod_Miembro))");
      query->exec("create table if not exists Material (Cod_MAT varchar(10) not null,fecha_ingreso date not null,descripcion varchar(30) not null,categoria varchar(15) null,estado varchar(10) not null,observaciones varchar(20) null,constraint pk_material primary key (Cod_MAT))");
      query->exec("create table if not exists Ficha_Medica (Cod_Miembro varchar(10) not null,Cod_FM varchar(10) not null,gruporh varchar(10) not null,enfermedades varchar(100) null,ambientes varchar(100) null,alimentos varchar(100) null,alergico varchar(100) null,quirurguicas varchar(100) null, observaciones varchar(100) null,padece varchar(100) null,medicamentos varchar(100) null,medico varchar(100) null,direccion_med varchar(40) null,nombre_urg varchar(100) null,telefono_urgencias integer null,telefono_medico integer null,constraint pk_fichamed primary key (Cod_FM,Cod_Miembro), constraint fk_ficha_miembro foreign key (Cod_Miembro) references Miembro(Cod_Miembro))");
      query->exec("create table if not exists Contable_Grupo (Cod_CON varchar(10) not null,motivo varchar(100) null,tipo_motivo varchar(20) not null,Ingreso double null,Egreso double null,fecha_ingreso date not null,categoria varchar(40) not null,Cod_Miembro varchar(10) null,cod_evento varchar(10) null,Rama varchar(15) null,Nombre varchar(20) null,Apellido varchar(20) null,cantidad int,CON_Evento varchar(10) null,debe_miembro double null,gan_mie double null,gan_grup double null,constraint pk_contablegrupo primary key (Cod_CON))");
      query->exec("create table if not exists Setup (nom_tabla varchar(15) not null, ultimo_cod int not null,constraint pk_setup primary key (nom_tabla))");
      query->exec("create table if not exists PadMa (NombreP varchar(20) null,ApellidoP varchar(20) null,Cod_Padre varchar(10) null,NombreM varchar(20) null,ApellidoM varchar(20) null,Cod_Madre varchar(10) null,NombreMie varchar(20) null,ApellidoMie varchar(20) null,Cod_Miembro varchar(10) null)");

      //Tablas de la Configuracion
      query->exec("create table if not exists Costos (cod_costo varchar(10) not null, descripcion varchar(60),monto double not null,constraint pk_setup primary key (cod_costo))");
      query->exec("create table if not exists Rama (Rama varchar(15) not null)");
      query->exec("create table if not exists Patrulla (Patrulla varchar(15) not null)");
      query->exec("create table if not exists Cargo (Cargo varchar(15) not null)");
      query->exec("create table if not exists Cat_mat (Catmat varchar(15) not null)");
      query->exec("create table if not exists Cat_pago (Catpago varchar(15) not null)");
      query->exec("create table if not exists Evento (cod_evento varchar(10) not null,descripcion varchar(60) not null,monto double not null,gan_mie double not null,gan_grup double not null,dia date not null, constraint pk_setup primary key (cod_evento))");
      query->exec("create table if not exists fecha_ciclo_contable (cod_fech varchar(10) not null,fecha_ini date not null,fecha_fin date null)");
      query->exec("create table if not exists Historico_Contable_Grupo (Cod_CON varchar(10) not null,motivo varchar(100) null,tipo_motivo varchar(20) not null,Ingreso double null,Egreso double null,fecha_ingreso date not null,categoria varchar(40) not null,Cod_Miembro varchar(10) null,cod_evento varchar(10) null,Rama varchar(15) null,Nombre varchar(20) null,Apellido varchar(20) null,cantidad int,CON_Evento varchar(10) null,debe_miembro double null,gan_mie double null,gan_grup double null)");


      config_setup_table();
      query->~QSqlQuery();
      return true;
      }
}

//Destructor
ConectarconBD::~ConectarconBD()
{
    db->~QSqlDatabase();
    QSqlDatabase::removeDatabase(db->connectionName());
}

//Funcion auxiliar para el sistema de claves primarias
bool ConectarconBD::check_setup()
{
    QSqlQuery *chequear = new QSqlQuery();
    chequear->prepare("select nom_tabla from Setup where nom_tabla = :tabla");
    chequear->bindValue(":tabla","Miembro");
    chequear->exec();

    QString variable = chequear->value(0).toString();

    //chequear->next();
    if(!variable.isNull())
    {
        chequear->~QSqlQuery();
        return true;
    }
    else
    {
        chequear->~QSqlQuery();
        return false;
    }
}

//Funcion de creacion de tabla para el sistema de claves primarias
void ConectarconBD::config_setup_table()
{
    QSqlQuery *insertar = new QSqlQuery();

    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Miembro',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Madre',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Padre',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Ficha_Medica',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Material',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Contable_Grupo',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Costos',1)");
    insertar->exec("insert into Setup (nom_tabla,ultimo_cod) values ('Evento',1)");

    insertar->~QSqlQuery();

}

//Funcion auxiliar para el sistema de claves primarias
void ConectarconBD::cod_setup(QString nombre_tabla, QString &cod_tabla)
{

    //Busco en la tabla setup el valor del ultimo codigo correspondiente a la tabla pasada como parametro

    QSqlQuery *buscar = new QSqlQuery();
    buscar->prepare("select ultimo_cod from setup where nom_tabla = :nom_tabla");
    buscar->bindValue(":nom_tabla",nombre_tabla);

    buscar->exec();
    buscar->next();

    int cod = buscar->value(0).toInt();
    QString ultimo_cod;
    ultimo_cod.append(QString("%1").arg(cod));


    // En funcion del nombre de la tabla y el ultimo_cod genero el codigo correspondiente al PK de la tabla.

    if (nombre_tabla == "Miembro"){
        cod_tabla = "MIE" + ultimo_cod;
    }
    if (nombre_tabla == "Madre"){
        cod_tabla = "MAD" + ultimo_cod;
    }
    if (nombre_tabla == "Padre"){
        cod_tabla = "PAD" + ultimo_cod;
    }
    if (nombre_tabla == "Material"){
        cod_tabla = "MAT" + ultimo_cod;
    }
    if (nombre_tabla == "Contable_Grupo"){
        cod_tabla = "CON" + ultimo_cod;
    }
    if (nombre_tabla == "Ficha_Medica"){
        cod_tabla = "FCM" + ultimo_cod;
    }
    if (nombre_tabla == "Costos"){
        cod_tabla = "COSTOS" + ultimo_cod;
    }
    if (nombre_tabla == "Evento"){
        cod_tabla = "EVENTO" + ultimo_cod;
    }

    //actualizo el ultimo_cod correspondiente al nom_tabla
    actualizar_setup(nombre_tabla,cod);

    buscar->~QSqlQuery();
}

//Funcion auxiliar para el sistema de claves primarias
void ConectarconBD::actualizar_setup(QString nombre_tabla,int ultimo_cod)
{
    int cod = ultimo_cod + 1;

    QSqlQuery *actualizar = new QSqlQuery();
    actualizar->prepare("update setup set ultimo_cod = :ultimo where nom_tabla = :tabla");
    actualizar->bindValue(":tabla",nombre_tabla);
    actualizar->bindValue(":ultimo",cod);
    actualizar->exec();

    actualizar->~QSqlQuery();

}


