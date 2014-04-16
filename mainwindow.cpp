#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "conectarconbd.h"

//Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    //Interfaz Al INICIO la
    ui->setupUi(this);

    //Ventana Maximizada al Inicio
    MainWindow::showMaximized();

    //Foto al Inicio
    ui->Foto->setPixmap(QPixmap(":/icons/male"));

    ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
    ui->tabWidget_2->setCurrentIndex(0); //Pestaña Ficha de Datos

    //Base de Datos
    //Si no puede realizar la conexion a la BD finaliza el programa
    BD = new ConectarconBD();

    //Ventana Configuraciones
    conf = new configuraciones(this);
    connect(conf,SIGNAL(finished(int)),this,SLOT(actualizar_modelos()));

    //Ventana relleno automatico
    RAP = new rautomaticpadres(this);

    //Ventana MainWindow
    //Para conectar el context menu cuando se tocla click derecho
    connect(ui->Foto, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenu(const QPoint&)));

    //Validado de los Campos en los lineEdit
    ui->lineEdit_2->setValidator(new QIntValidator(ui->lineEdit_2));//valido que no me escriba texto en el documento
    ui->lineEdit_8->setValidator(new QIntValidator(ui->lineEdit_8));//valido que no me escriba texto en Telefono
    ui->lineEdit_36->setValidator(new QIntValidator(ui->lineEdit_36));//valido que no me escriba texto en el numero de la casa
    ui->lineEdit_3->setValidator(new QIntValidator(ui->lineEdit_3));//valido que no me escriba texto en el documento
    ui->lineEdit_37->setValidator(new QIntValidator(ui->lineEdit_37));//valido que no me escriba texto en el numero de la casa
    ui->lineEdit_19->setValidator(new QIntValidator(ui->lineEdit_19));//valido que no me escriba texto en el telefono
    ui->lineEdit_47->setValidator(new QIntValidator(ui->lineEdit_47));//valido que no me escriba texto en el documento
    ui->lineEdit_41->setValidator(new QIntValidator(ui->lineEdit_41));//valido que no me escriba texto en el numero de la casa
    ui->lineEdit_48->setValidator(new QIntValidator(ui->lineEdit_48));//valido que no me escriba texto en el telefono
    ui->lineEdit_59->setValidator(new QIntValidator(ui->lineEdit_59));//valido que no me escriba texto en el telefono urgencias. Ficha Medica
    ui->lineEdit_57->setValidator(new QIntValidator(ui->lineEdit_57));//valido que no me escriba texto en el telefono medico. Ficha Medica

    ui->pushButton_5->setVisible(false);
    ui->pushButton_6NM->setVisible(false);
    ui->pushButton_7NM->setVisible(false);
    ui->pushButton_8E->setVisible(false);
    ui->pushButton_9E->setVisible(false);

    //Llamo a las funciones que definen estructuras de configuracion de las distintas secciones
    config_busqueda_miembro();
    estructura_Cuenta_Miembro();
    estructura_material();    
    estructura_contable_grupo();
    camposcombobox();
    asignar_fechas(); //Luego de la asignacion de todas las estructuras se procede a hacer esto.

    //Tuve que agregar esto aquí por que no me deja desactivar la tabla EventosMie de otra manera. No me explico por que no se desactiva la tabla EventosMie
    desactivarCM_Asignacion();

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    //Control de categorias
    //Cartel que detecta si faltan al menos algunas categorias en las configuraciones y recomienda completarlas
    if(!conf->verifica_sihay())
    {
        QMessageBox::information(this,"Precaución","Se Recomienda Que llene los campos disponible en las Configuraciones para un correcto funcionamiento del sistema.",QMessageBox::Ok);
         //Abro las configuraciones Para que complete con los campos
         conf->show();
    }

}

//Destructor
MainWindow::~MainWindow()
{
    mat->~QSqlTableModel();
    model->~QSqlQueryModel();
    CGR->~QSqlTableModel();
    cum->~QSqlTableModel();
    cme->~QSqlTableModel();
    MWlista_pat->~QSqlTableModel();
    MWlista_car->~QSqlTableModel();
    MWlista_ram->~QSqlTableModel();
    MWlista_catpag->~QSqlTableModel();
    MWlista_catmat->~QSqlTableModel();
    conf->~configuraciones();
    RAP->~rautomaticpadres();
    BD->~ConectarconBD();

    delete ui;
}

//----------------------------------Funciones Auxiliares Generales----------------------------

//Dialogo de ACERCA DE
void MainWindow::acercade()
{
    QString Acerca;
    Acerca+="Programa de Gestion de Grupos Scouts diseñado para las nesesidades del Grupo Scout San Pablo N°213 de Santa Fe Argentina.""\n\n";
    Acerca+="Autores: Dato, Luciano Esteban - Manzo, Renato José.";
    QMessageBox::about(this,"Acerca de G.A.G.S",Acerca);
}

void MainWindow::acercadeQT()
{
    QMessageBox::aboutQt(this);
}

//Ayuda MANUAL
void MainWindow::help()
{

    //Pasos a seguir:
    //(1) crear o utilizar un directorio temporal.
    QString dir_temp = QDesktopServices::storageLocation(QDesktopServices::TempLocation);

    //(2) Copiar el archivo embebido (qt resource) al directorio.
    dir_temp = dir_temp + "/Manual_Usuario.pdf";

    QFile::copy(":/Manuales/Manual_Usuario",dir_temp);

    //(3) asignarle en el copiado el nombre correspondiente.
    //(4) abrir el archivo con esta funcion. QDesktopServices::openUrl

    dir_temp = "file:///" + dir_temp;
    qDebug()<<dir_temp;
    if(!QDesktopServices::openUrl(QUrl(dir_temp)))
    {
        QMessageBox::information(this,"Cuidado","No se puede abrir el manual de usuario.""\nPor Favor instale un lector de archivos pdf.",QMessageBox::Ok);
    }

    //(5) destruir o vaciar la variable.
    //dir_temp.~QString();

}

//Configuracion general - Asignacion de fechas desde y hasta
void MainWindow::asignar_fechas()
{
    QSqlQuery *fech = new  QSqlQuery();
    fech->prepare("Select fecha_ini,fecha_fin from fecha_ciclo_contable where cod_fech = 'Actual'");
    //QSqlDatabase::database().transaction();
    fech->exec();
    fech->next();

    if(!fech->isValid())
    {
        //QSqlDatabase::database().commit(); //finalizo la transaccion.

        //Primera creacion de la base de datos
        ui->dateEdit_7->setDate(QDate::currentDate()); //le asigno el currentDate.
        ui->dateEdit_7->setMaximumDate(QDate::currentDate());
        ui->dateEdit_6->setDate(QDate(2000,01,01)); //le asigno el currentDate.
        ui->dateEdit_6->setMinimumDate(QDate(2000,01,01));

        ui->dateEdit_5->setMaximumDate(QDate::currentDate());
        ui->dateEdit_8->setMaximumDate(QDate::currentDate());
        ui->dateEdit_9->setMaximumDate(QDate::currentDate());

        desactivar_interfaz();
        desactivar_impresion();
    }
    else
    {

        if(fech->value(1).toDate().isNull())
        {
            //Caso que ya se inicio el ciclo
            //hasta
            ui->dateEdit_7->setDate(QDate::currentDate()); // No fue finalizado el ciclo contable asigno fecha actual a fecha hasta.
            ui->dateEdit_7->setMinimumDate(fech->value(0).toDate());
            ui->dateEdit_7->setMaximumDate(QDate::currentDate());

            //desde
            ui->dateEdit_6->setDate(fech->value(0).toDate()); //asigno la fecha_ini a la fecha desde.
            ui->dateEdit_6->setMinimumDate(fech->value(0).toDate());
            ui->dateEdit_6->setMaximumDate(QDate::currentDate());

            //Fecha ingreso contable grupo
            ui->dateEdit_8->setMinimumDate(fech->value(0).toDate());
            ui->dateEdit_8->setMaximumDate(QDate::currentDate());

            //Fecha Ingreso cuenta miembro
            ui->dateEdit_9->setMinimumDate(fech->value(0).toDate());
            ui->dateEdit_9->setMaximumDate(QDate::currentDate());

            //Fecha de ingreso materiales
            ui->dateEdit_5->setDate(QDate::currentDate());
            ui->dateEdit_5->setMaximumDate(QDate::currentDate());

            activar_interfaz();
            activar_impresion();

        }
        else
        {
            //Caso que ya se finalizo el ciclo
            //hasta
            ui->dateEdit_7->setDate(fech->value(1).toDate()); // No fue finalizado el ciclo contable asigno fecha actual a fecha hasta.
            ui->dateEdit_7->setMinimumDate(fech->value(0).toDate());
            ui->dateEdit_7->setMaximumDate(fech->value(1).toDate());

            //desde
            ui->dateEdit_6->setDate(fech->value(0).toDate()); //asigno la fecha_ini a la fecha desde.
            ui->dateEdit_6->setMinimumDate(fech->value(0).toDate());
            ui->dateEdit_6->setMaximumDate(fech->value(1).toDate());

            ui->dateEdit_5->setMaximumDate(QDate::currentDate());
            ui->dateEdit_8->setMaximumDate(QDate::currentDate());
            ui->dateEdit_9->setMaximumDate(QDate::currentDate());

            desactivar_interfaz();
            activar_impresion();

        }

        //QSqlDatabase::database().commit();// termine las asignaciones cierro la transaccion.
    }

}

//Desactivo la interfaz para impedir ingresos e ediciones de contenido
void MainWindow::desactivar_interfaz()
{
    ui->pushButton_4->setEnabled(false); //nuevo miembro
    ui->pushButton_5->setEnabled(false); //editar_ miembro

    //ui->cuenta_miembro->setEnabled(false);//table cuenta miembro
    //ui->pushButton_15->setEnabled(false);//nuevo ingreso cuenta miembro

    //ui->EventosMie->setEnabled(false); //table evento miembro

    ui->pushButton_10->setEnabled(false); // nuevo ingreso contable grupo
    ui->contable_grupo->setEnabled(false); // table contable grupo
}

//Activo la interfaz.
void MainWindow::activar_interfaz()
{
    ui->pushButton_4->setEnabled(true); //nuevo miembro
    ui->pushButton_5->setEnabled(true); //editar_ miembro

    //ui->cuenta_miembro->setEnabled(true);//table cuenta miembro
    //ui->pushButton_15->setEnabled(true);//nuevo ingreso cuenta miembro

    //ui->EventosMie->setEnabled(true); //table evento miembro

    ui->pushButton_10->setEnabled(true); // nuevo ingreso contable grupo
    ui->contable_grupo->setEnabled(true); // table contable grupo
}

//Funcion para desactivar la impresion en el caso de la primera creacion de la base de datos
void MainWindow::desactivar_impresion()
{
    ui->pushButton_14->setEnabled(false);
    ui->pushButton_19->setEnabled(false);
    ui->label_2->setEnabled(false);
    ui->label_109->setEnabled(false);
    ui->comboBox_11->setEnabled(false);
    ui->comboBox_12->setEnabled(false);
    ui->checkBox_14->setEnabled(false);
}

void MainWindow::activar_impresion()
{
    if(CGR->rowCount() == 0)
    {
        ui->pushButton_14->setEnabled(false);
        ui->pushButton_19->setEnabled(false);
        ui->label_2->setEnabled(false);
        ui->label_109->setEnabled(false);
        ui->comboBox_11->setEnabled(false);
        ui->comboBox_12->setEnabled(false);
        ui->checkBox_14->setEnabled(false);
    }
    else
    {
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_19->setEnabled(true);
        ui->label_2->setEnabled(true);
        ui->label_109->setEnabled(true);
        ui->comboBox_11->setEnabled(true);
        ui->comboBox_12->setEnabled(true);
        ui->checkBox_14->setEnabled(true);
    }
}

//----------------------------------Slots Generales-------------------------------------------

//GENERAR PDF's o Impresion
void MainWindow::on_pushButton_pressed()
{
    //Comienzo
    QString Opcion;
    int numero;
    //1
    if (ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "datos";
        numero = 1;
    }
    //2
    if (!ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "familiares";
        numero = 2;
    }
    //3
    if (!ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "fichamedica";
        numero = 3;
    }
    //4
    if (!ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "fichacontable";
        numero = 4;
    }
    //5
    if (ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "datfam";
        numero = 5;
    }
    //6
    if (ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "datfimed";
        numero = 6;
    }
    //7
    if (ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "datficont";
        numero = 7;
    }
    //8
    if (!ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "famfimed";
        numero = 8;
    }
    //9
    if (!ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "famficont";
        numero = 9;
    }
    //10
    if (!ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "fimedficont";
        numero = 10;
    }
    //11
    if (!ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "ninguno";
        numero = 11;
    }
    //12
    if (ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "todos";
        numero = 12;
    }
    //13
    if (ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and !ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "datfamficon";
        numero = 13;
    }
    //14
    if (ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and !ui->checkBox_4->isChecked())
    {
        Opcion = "datfamfimed";
        numero = 14;
    }
    //15
    if (!ui->checkBox->isChecked() and ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "famfimedficon";
        numero = 15;
    }
    //16
    if (ui->checkBox->isChecked() and !ui->checkBox_2->isChecked() and ui->checkBox_3->isChecked() and ui->checkBox_4->isChecked())
    {
        Opcion = "datfimedficon";
        numero = 16;
    }
    //fin

    //Preparo la impresion o el pdf
    //y segun el numero de opcion recurro a el metodo de cada clase que arma el informe

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer,this);
    QTextDocument textDocument;
    QString html;
    if (numero != 11)
    {
        //Ejecuto el dialogo

        printDialog.exec();

        switch (numero)
        {
        case 1:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 2:
            html+="<h3>" "Datos de los Padres del Miembro: " + Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) +"</h3>";
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 3:
            html+="<h3>" "Ficha Medica del Miembro: "+ Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) + "</h3>";
            Aux.grFM(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 4:
            html+="<h3>" "Resumen de Cuenta: " + Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) +"</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 5:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 6:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Ficha Medica" "</h3>";
            Aux.grFM(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 7:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Resumen de Cuenta: " "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 8:
            html+="<h3>" "Datos de los Padres del Miembro "+ Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) +"</h3>";
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            html+="<h3>" "Ficha Medica" "</h3>";
            Aux.grFM(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 9:
            html+="<h3>" "Datos de los Padres del Miembro " + Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) + "</h3>";
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            html+="<h3>" "Resumen de Cuenta: " "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 10:
            html+="<h3>" "Ficha Medica del Miembro " + Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) +"</h3>";
            Aux.grFM(html);
            html+="<h3>" "Resumen de Cuenta: " "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 12:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            html+="<h3>" "Ficha Medica" "</h3>";
            Aux.grFM(html);
            html+="<h3>" "Resumen de Cuenta" "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 13:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            html+="<h3>" "Resumen de Cuenta: " "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 14:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            html+="<h3>" "Ficha Medica" "</h3>";
            Aux.grFM(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 15:
            html+="<h3>" "Datos de los Padres del Miembro " + Aux.rec_nom(Cod_Miembro) + " " + Aux.rec_ape(Cod_Miembro) +"</h3>";
            html+="<h3>" "Datos del Padre" "</h3>";
            Aux.grP(html);
            html+="<h3>" "Datos de la Madre" "</h3>";
            Aux.grM(html);
            html+="<h3>" "Ficha Medica" "</h3>";
            Aux.grFM(html);
            html+="<h3>" "Resumen de Cuenta: " "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        case 16:
            html+="<h3>" "Ficha de Datos" "</h3>";
            Aux.generarreport(html);
            html+="<h3>" "Ficha Medica" "</h3>";
            Aux.grFM(html);
            html+="<h3>" "Resumen de Cuenta: " "</h3>";
            imprimir_cuenta_miembro(html);
            textDocument.setHtml(html);
            textDocument.print(&printer);
            break;
        }

    }
    else
    {
        QMessageBox::information(this,"Cuidado!","Seleccione al menos una opción para generar el Reporte.",QMessageBox::Ok);
    }

    //Elimino la foto temporal para el caso de la impresion de datos
    QFile::remove("Foto.JPG");
}

//Impresion por Rama
void MainWindow::on_pushButton_19_pressed()
{
    //Primero tendria que verificar si hay miembros en la BD para que no me deje imprimir
    QSqlQuery *con_aux= new QSqlQuery();
    con_aux->prepare("SELECT count(*) FROM Miembro");
    con_aux->exec();
    con_aux->next();
    if(con_aux->value(0).toInt() == 0)
    {
        QMessageBox::information(this,"CUIDADO","No hay Miembros Activos cargados en la Base de Datos.",QMessageBox::Ok);
        con_aux->~QSqlQuery();
        return;
    }
    con_aux->~QSqlQuery();

    QString HTML;
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer,this);
    QTextDocument textDocument;
    QSqlQuery *cons = new QSqlQuery();

    if(ui->comboBox_11->itemText(ui->comboBox_11->currentIndex()) == "Resumen de Cuenta")
    {
        cons->prepare("SELECT Cod_Miembro FROM Miembro WHERE Rama = :Ram and Activo == 'Activo'");
        cons->bindValue(":Ram",ui->comboBox_12->itemText(ui->comboBox_12->currentIndex()));
        cons->exec();


        while(cons->next())
        {

            Aux.buscar_BD(cons->value(0).toString());
            //Cargo los modelos para el miembro
            if(cons->value(0).toString() != "NO")
            {
                //tabla ingresos generales
                QString  Filtro_CM ="Cod_Miembro='" + cons->value(0).toString() + "'"+ " and cod_evento is NULL";
                cum->setFilter(Filtro_CM);
                cum->select();

                //tabla evento
                QString  Filtro_CME ="Cod_Miembro='" + cons->value(0).toString() + "'"+ " and cod_evento is not NULL";
                cme->setFilter(Filtro_CME);
                cme->select();

                //Calculo el debe y el haber
                calculos_miembro(cons->value(0).toString());

                //Recien aca podria llamar a los metodos que hacen el append sobre el string
                //Pongo todo adentro de una gran tabla para que no me imprima cortado los informes
                HTML+="<h3>" "Resumen de Cuenta: " + Aux.rec_nom(cons->value(0).toString()) + " " + Aux.rec_ape(cons->value(0).toString()) +"</h3>";
                imprimir_cuenta_miembro(HTML);
            }
        }

        printDialog.exec();
        textDocument.setHtml(HTML);
        textDocument.print(&printer);

        if(!Cod_Miembro.isEmpty() and Cod_Miembro != "NO")
        {
            //Por si voy a usar el Aux despues
            Aux.buscar_BD(Cod_Miembro);

            //tabla ingresos generales
            QString  Filtro_CM ="Cod_Miembro='" + Cod_Miembro + "'"+ "and cod_evento is NULL";
            cum->setFilter(Filtro_CM);
            cum->select();

            //tabla evento
            QString  Filtro_CME ="Cod_Miembro='" + Cod_Miembro + "'"+ "and cod_evento is not NULL";
            cme->setFilter(Filtro_CME);
            cme->select();

            //cum->clear();

            //Calculo el debe y el haber
            calculos_miembro(Cod_Miembro);

        }
    }

    cons->~QSqlQuery();
}

//----------------------------------SECCION MIEMBRO-----------------------------------------

//Configuracion de la busqueda del miembro
void MainWindow::config_busqueda_miembro()
{
    //Uso un tree view para mostrar los resultados de la busqueda
    view = new QTreeView(this);

    //Modelo de datos para asignarle al TreeView

    model = new QSqlQueryModel();

    view->setModel(model);
    view->header()->hide();
    view->setUniformRowHeights(true);

    ui->radioButton_12->setChecked(true); //el apellido como filtro inicial de la busqueda
    ui->dateEdit->setDate(QDate::currentDate());

    //Aqui aplico un cambio estetico en el buscador para que sea coherente a la busqueda
    view->setAlternatingRowColors(true);
    view->hideColumn(4);

    view->setMouseTracking(true);
    ui->lineEdit->setMouseTracking(true);

    //Conecto la Senhal de click o enter en la lista de la busqueda para que haga la busqueda segun el metodo enterdel lineedit
    connect(view, SIGNAL(pressed(QModelIndex)),ui->lineEdit, SIGNAL(returnPressed()));

    //Completer para la tabla de resultado
    ui->radioButton_12->setChecked(true);

    mCompleter = new QCompleter(model);
    if(ui->radioButton_12->isChecked()){mCompleter->setCompletionColumn(0);}
    if(ui->radioButton_13->isChecked()){mCompleter->setCompletionColumn(1);}
    if(ui->radioButton_14->isChecked()){mCompleter->setCompletionColumn(2);}
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mCompleter->setCompletionMode(QCompleter::PopupCompletion);

    mCompleter->setPopup(view);

    ui->lineEdit->setCompleter(mCompleter);
}

//Configuracion de los campos combobox
void MainWindow::camposcombobox()
{
    //Patrulla
    MWlista_pat= new QSqlTableModel();
    MWlista_pat->setTable("Patrulla");
    MWlista_pat->select();
    ui->comboBox_3->setModel(MWlista_pat);

    //Cargo
    MWlista_car= new QSqlTableModel();
    MWlista_car->setTable("Cargo");
    MWlista_car->select();
    ui->comboBox_4->setModel(MWlista_car);

    //Rama
    MWlista_ram= new QSqlTableModel();
    MWlista_ram->setTable("Rama");
    MWlista_ram->select();
    ui->comboBox_2->setModel(MWlista_ram);
    ui->comboBox_12->setModel(MWlista_ram);

    //Cat Pagos
    MWlista_catpag= new QSqlTableModel();
    MWlista_catpag->setTable("Cat_pago");
    MWlista_catpag->select();
    ui->comboBox_10->setModel(MWlista_catpag);

    //Cat Materiales
    MWlista_catmat= new QSqlTableModel();
    MWlista_catmat->setTable("Cat_mat");
    MWlista_catmat->select();
    ui->comboBox_6->setModel(MWlista_catmat);
}

//...........Funciones auxiliares.................................

//Actualiza los modelos de los combobox despues de actualizarse los campos disponibles
void MainWindow::actualizar_modelos()
{
    //Actualizar Contable_Grupo
    CGR->select();

    //Patrulla
    MWlista_pat->select();

    //Cargo
    MWlista_car->select();

    //Rama
    MWlista_ram->select();

    //Cat Pagos
    MWlista_catpag->select();

    //Cat Materiales
    MWlista_catmat->select();

    //Actualizo CuM y CME

    cum->select();
    cme->select();

    asignar_fechas(); // Actualizo la interfaz con las nuevas fechas y bloqueo en caso de ser nesesario.
    //Actualizo los resumenes en el caso que modifique algo en las configuraciones
    calculos_miembro(Cod_Miembro);
    actualizar_resumen_CG();
}

//Para escribir en tiempo real el nombre y apellido son 3 funciones una para recatar el nombre otra el apellido y la otra escribe el label
void MainWindow::escribe_entiemporeal(QString & Nom,QString & Ape)
{
    QString cadenafinal = Ape + ", " + Nom;
    ui->label_3->setText(cadenafinal);
}

//para simplificar funciones para limpiar activar y desactivar campos de los datos de Miembro y de los padres
void MainWindow::limpiarD()
{
    ui->checkBox_13->setChecked(false);
    ui->label_3->setText("Apellido, Nombre");
    ui->dateEdit->setDate(QDate(2000,01,01));
    ui->dateEdit_2->setDate(QDate(2000,01,01));
    ui->Foto->clear();
    ui->Foto->setPixmap(QPixmap(":/icons/male"));
    ui->radioButton->setChecked(true);
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_4->setCurrentIndex(0);
    ui->lineEdit_34->clear();
    ui->lineEdit_35->clear();
    ui->lineEdit_36->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();
    ui->lineEdit_11->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();
    ui->lineEdit_15->clear();
    ui->lineEdit_16->clear();
    ui->plainTextEdit->clear();
    ui->plainTextEdit_2->clear();
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_9->setChecked(false);
    ui->checkBox_10->setChecked(false);
    Aux.borra_foto();

}

void MainWindow::desactivaD()
{
    //Desactivo lo que esta dentro de la pestaÃ±a de datos
    ui->checkBox_13->setEnabled(false);
    ui->Foto->setEnabled(false);
    ui->dateEdit->setEnabled(false);
    ui->dateEdit_2->setEnabled(false);
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_8->setEnabled(false);
    ui->lineEdit_9->setEnabled(false);
    ui->lineEdit_10->setEnabled(false);
    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_12->setEnabled(false);
    ui->lineEdit_13->setEnabled(false);
    ui->lineEdit_15->setEnabled(false);
    ui->lineEdit_16->setEnabled(false);
    ui->lineEdit_34->setEnabled(false);
    ui->lineEdit_35->setEnabled(false);
    ui->lineEdit_36->setEnabled(false);
    ui->checkBox_5->setEnabled(false);
    ui->checkBox_6->setEnabled(false);
    ui->checkBox_7->setEnabled(false);
    ui->checkBox_8->setEnabled(false);
    ui->checkBox_9->setEnabled(false);
    ui->checkBox_10->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->plainTextEdit->setEnabled(false);
    ui->plainTextEdit_2->setEnabled(false);
}

void MainWindow::activaD()
{
    ui->checkBox_13->setEnabled(true);
    ui->Foto->setEnabled(true);
    ui->dateEdit->setEnabled(true);//permito editar la fecha y todos los campos de la ficha
    ui->dateEdit_2->setEnabled(true);
    ui->radioButton->setEnabled(true);
    ui->radioButton_2->setEnabled(true);
    ui->lineEdit_2->setEnabled(true);
    ui->lineEdit_5->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_7->setEnabled(true);
    ui->lineEdit_8->setEnabled(true);
    ui->lineEdit_9->setEnabled(true);
    ui->lineEdit_10->setEnabled(true);
    ui->lineEdit_11->setEnabled(true);
    ui->lineEdit_12->setEnabled(true);
    ui->lineEdit_13->setEnabled(true);
    ui->lineEdit_34->setEnabled(true);
    ui->lineEdit_35->setEnabled(true);
    ui->lineEdit_36->setEnabled(true);
    ui->checkBox_5->setEnabled(true);

    if (ui->checkBox_6->isChecked() == true){
        ui->lineEdit_15->setEnabled(true);
    }

    ui->checkBox_6->setEnabled(true);

    if (ui->checkBox_5->isChecked() == true){
        ui->lineEdit_16->setEnabled(true);
    }

    ui->checkBox_7->setEnabled(true);
    ui->checkBox_8->setEnabled(true);
    ui->checkBox_9->setEnabled(true);
    ui->checkBox_10->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->comboBox_4->setEnabled(true);
    ui->plainTextEdit->setEnabled(true);
    ui->plainTextEdit_2->setEnabled(true);
}

void MainWindow::activaP()
{
    ui->lineEdit_39->setEnabled(true);
    ui->lineEdit_40->setEnabled(true);
    ui->dateEdit_3->setEnabled(true);
    ui->comboBox_5->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_14->setEnabled(true);
    ui->lineEdit_37->setEnabled(true);
    ui->lineEdit_17->setEnabled(true);
    ui->lineEdit_18->setEnabled(true);
    ui->lineEdit_19->setEnabled(true);
    ui->lineEdit_20->setEnabled(true);
    ui->lineEdit_21->setEnabled(true);
    ui->lineEdit_22->setEnabled(true);
    ui->lineEdit_23->setEnabled(true);
    ui->lineEdit_24->setEnabled(true);
}

void MainWindow::desactivaP()
{
    ui->lineEdit_39->setEnabled(false);
    ui->lineEdit_40->setEnabled(false);
    ui->dateEdit_3->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_14->setEnabled(false);
    ui->lineEdit_37->setEnabled(false);
    ui->lineEdit_17->setEnabled(false);
    ui->lineEdit_18->setEnabled(false);
    ui->lineEdit_19->setEnabled(false);
    ui->lineEdit_20->setEnabled(false);
    ui->lineEdit_21->setEnabled(false);
    ui->lineEdit_22->setEnabled(false);
    ui->lineEdit_23->setEnabled(false);
    ui->lineEdit_24->setEnabled(false);
}

void MainWindow::limpiarP()
{
    ui->label_5->setText("Apellido, Nombre");
    ui->lineEdit_39->clear();
    ui->lineEdit_40->clear();
    ui->dateEdit_3->setDate(QDate(2000,01,01));
    ui->comboBox_5->setCurrentIndex(0);
    ui->lineEdit_3->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_37->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_22->clear();
    ui->lineEdit_23->clear();
    ui->lineEdit_24->clear();
}

void MainWindow::activaM()
{
    ui->lineEdit_52->setEnabled(true);
    ui->lineEdit_53->setEnabled(true);
    ui->dateEdit_4->setEnabled(true);
    ui->comboBox_7->setEnabled(true);
    ui->lineEdit_47->setEnabled(true);
    ui->lineEdit_51->setEnabled(true);
    ui->lineEdit_48->setEnabled(true);
    ui->lineEdit_49->setEnabled(true);
    ui->lineEdit_50->setEnabled(true);
    ui->lineEdit_41->setEnabled(true);
    ui->lineEdit_42->setEnabled(true);
    ui->lineEdit_43->setEnabled(true);
    ui->lineEdit_44->setEnabled(true);
    ui->lineEdit_45->setEnabled(true);
    ui->lineEdit_46->setEnabled(true);
}

void MainWindow::desactivaM()
{
    ui->lineEdit_52->setEnabled(false);
    ui->lineEdit_53->setEnabled(false);
    ui->dateEdit_4->setEnabled(false);
    ui->comboBox_7->setEnabled(false);
    ui->lineEdit_47->setEnabled(false);
    ui->lineEdit_51->setEnabled(false);
    ui->lineEdit_48->setEnabled(false);
    ui->lineEdit_49->setEnabled(false);
    ui->lineEdit_50->setEnabled(false);
    ui->lineEdit_41->setEnabled(false);
    ui->lineEdit_42->setEnabled(false);
    ui->lineEdit_43->setEnabled(false);
    ui->lineEdit_44->setEnabled(false);
    ui->lineEdit_45->setEnabled(false);
    ui->lineEdit_46->setEnabled(false);
}

void MainWindow::limpiarM()
{
    ui->label_72->setText("Apellido, Nombre");
    ui->lineEdit_52->clear();
    ui->lineEdit_53->clear();
    ui->dateEdit_4->setDate(QDate(2000,01,01));
    ui->comboBox_7->setCurrentIndex(0);
    ui->lineEdit_47->clear();
    ui->lineEdit_51->clear();
    ui->lineEdit_48->clear();
    ui->lineEdit_49->clear();
    ui->lineEdit_50->clear();
    ui->lineEdit_41->clear();
    ui->lineEdit_42->clear();
    ui->lineEdit_43->clear();
    ui->lineEdit_44->clear();
    ui->lineEdit_45->clear();
    ui->lineEdit_46->clear();
}

void MainWindow::activaFM()
{
    ui->lineEdit_59->setEnabled(true);
    ui->lineEdit_57->setEnabled(true);
    ui->lineEdit_32->setEnabled(true);
    ui->lineEdit_56->setEnabled(true);
    ui->lineEdit_56->setEnabled(true);
    ui->lineEdit_60->setEnabled(true);
    ui->lineEdit_54->setEnabled(true);
    ui->lineEdit_38->setEnabled(true);
    ui->lineEdit_55->setEnabled(true);
    ui->lineEdit_58->setEnabled(true);
    ui->lineEdit_31->setEnabled(true);
    ui->lineEdit_62->setEnabled(true);
    ui->lineEdit_63->setEnabled(true);
    ui->lineEdit_33->setEnabled(true);
    ui->comboBox_9->setEnabled(true);
}

void MainWindow::desactivaFM()
{
    ui->lineEdit_59->setEnabled(false);
    ui->lineEdit_57->setEnabled(false);
    ui->lineEdit_32->setEnabled(false);
    ui->lineEdit_56->setEnabled(false);
    ui->lineEdit_56->setEnabled(false);
    ui->lineEdit_60->setEnabled(false);
    ui->lineEdit_54->setEnabled(false);
    ui->lineEdit_38->setEnabled(false);
    ui->lineEdit_55->setEnabled(false);
    ui->lineEdit_58->setEnabled(false);
    ui->lineEdit_31->setEnabled(false);
    ui->lineEdit_62->setEnabled(false);
    ui->lineEdit_63->setEnabled(false);
    ui->lineEdit_33->setEnabled(false);
    ui->comboBox_9->setEnabled(false);
}

void MainWindow::limpiarFM()
{
    ui->comboBox_9->setCurrentIndex(0);
    ui->lineEdit_59->clear();
    ui->lineEdit_57->clear();
    ui->lineEdit_32->clear();
    ui->lineEdit_56->clear();
    ui->lineEdit_56->clear();
    ui->lineEdit_60->clear();
    ui->lineEdit_54->clear();
    ui->lineEdit_38->clear();
    ui->lineEdit_55->clear();
    ui->lineEdit_58->clear();
    ui->lineEdit_31->clear();
    ui->lineEdit_62->clear();
    ui->lineEdit_63->clear();
    ui->lineEdit_33->clear();
}

//Para activar el reporte de documentos por miembro
void MainWindow::activar_reporte()
{
    ui->checkBox->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ui->checkBox_3->setEnabled(true);
    ui->checkBox_4->setEnabled(true);
    ui->pushButton->setEnabled(true);
}

void MainWindow::desactivar_reporte()
{
    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->checkBox_3->setEnabled(false);
    ui->checkBox_4->setEnabled(false);
    ui->pushButton->setEnabled(false);
}

void MainWindow::limpiar_reporte()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
}

void MainWindow::escribe_entiemporealP(QString & Nom,QString & Ape)
{
    QString cadenafinal = Ape + ", " + Nom;
    ui->label_5->setText(cadenafinal);

}

void MainWindow::escribe_entiemporealM(QString & Nom,QString & Ape)
{
    QString cadenafinal = Ape + ", " + Nom;
    ui->label_72->setText(cadenafinal);

}

//En el caso de que los padres ya estaban en la base de datos los relleno
void MainWindow::rellenopamasa(QString codpa, QString codma)
{
    AuxP = new QSqlTableModel();
    AuxP->setTable("Padre");
    QString filtroP = "Cod_Padre = '" + codpa + "'";
    AuxP->setFilter(filtroP);
    AuxP->select();

    AuxM = new QSqlTableModel();
    AuxM->setTable("Madre");
    QString filtroM = "Cod_Madre = '" + codma + "'";
    AuxM->setFilter(filtroM);
    AuxM->select();

    int indice1,indice2;
    //Datos del Padre
    if (ui->checkBox_11->isChecked())
    {
        if (codpa != "")
        {
            //Apellido del Padre
            ui->lineEdit_39->setText(AuxP->index(0,1).data().toString());
            //Nombre de Padre
            ui->lineEdit_40->setText(AuxP->index(0,0).data().toString());
            //Actualizo el Label
            QString np = AuxP->index(0,0).data().toString(),ap = AuxP->index(0,1).data().toString();
            escribe_entiemporealP(np,ap);
            //Fecha de Nacimiento del Padre
            ui->dateEdit_3->setDate(AuxP->index(0,2).data().toDate());
            //Tipo de Documento del Padre
            indice1 = ui->comboBox_5->findText(AuxP->index(0,4).data().toString());
            ui->comboBox_5->setCurrentIndex(indice1);
            //Numero de Documento del Padre
            ui->lineEdit_3->setText(QString("%1").arg(AuxP->index(0,5).data().toString()));
            //Domicilio del Padre
            ui->lineEdit_14->setText(AuxP->index(0,8).data().toString());
            //Numero del Domicilio del Padre
            ui->lineEdit_37->setText(QString("%1").arg(AuxP->index(0,9).data().toString()));
            //Piso del Padre
            ui->lineEdit_17->setText(AuxP->index(0,10).data().toString());
            //Departamento del Padre
            ui->lineEdit_18->setText(AuxP->index(0,11).data().toString());
            //Telefono del Padre
            ui->lineEdit_19->setText(QString("%1").arg(AuxP->index(0,12).data().toString()));
            //Nacionalidad del Padre
            ui->lineEdit_20->setText(AuxP->index(0,13).data().toString());
            //Provincia del Padre
            ui->lineEdit_21->setText(AuxP->index(0,14).data().toString());
            //Localidad del Padre
            ui->lineEdit_22->setText(AuxP->index(0,15).data().toString());
            //Lugar de Nacimiento del Padre
            ui->lineEdit_23->setText(AuxP->index(0,16).data().toString());
            //Correo electronico del Padre
            ui->lineEdit_24->setText(AuxP->index(0,17).data().toString());
        }
        else
        {
            if(codma != "no" and !ui->checkBox_12->isChecked())
            {
               desactivaP();
               limpiarP();
               ui->checkBox_11->setChecked(false);
            }
        }

    }


    //Datos del Madre
   if (ui->checkBox_12->isChecked())
   {

       if(codma != "")
       {
           //Apellido del Madre
           ui->lineEdit_52->setText(AuxM->index(0,1).data().toString());
           //Nombre de Madre
           ui->lineEdit_53->setText(AuxM->index(0,0).data().toString());
           //Actualizo el Label
           QString nm = AuxM->index(0,0).data().toString(),am = AuxM->index(0,1).data().toString();
           escribe_entiemporealM(nm,am);
           //Fecha de Nacimiento del Madre
           ui->dateEdit_4->setDate(AuxM->index(0,2).data().toDate());
           //Tipo de Documento del Madre
           indice2 = ui->comboBox_7->findText(AuxM->index(0,4).data().toString());
           ui->comboBox_7->setCurrentIndex(indice2);
           //Numero de Documento del Madre
           ui->lineEdit_47->setText(QString("%1").arg(AuxM->index(0,5).data().toString()));
           //Domicilio del Madre
           ui->lineEdit_51->setText(AuxM->index(0,8).data().toString());
           //Numero del Domicilio del Madre
           ui->lineEdit_48->setText(QString("%1").arg(AuxM->index(0,9).data().toString()));
           //Piso del Madre
           ui->lineEdit_49->setText(AuxM->index(0,10).data().toString());
           //Departamento del Madre
           ui->lineEdit_50->setText(AuxM->index(0,11).data().toString());
           //Telefono del Madre
           ui->lineEdit_41->setText(QString("%1").arg(AuxM->index(0,12).data().toString()));
           //Nacionalidad del Madre
           ui->lineEdit_42->setText(AuxM->index(0,13).data().toString());
           //Provincia del Madre
           ui->lineEdit_43->setText(AuxM->index(0,14).data().toString());
           //Localidad del Madre
           ui->lineEdit_44->setText(AuxM->index(0,15).data().toString());
           //Lugar de Nacimiento del Madre
           ui->lineEdit_45->setText(AuxM->index(0,16).data().toString());
           //Correo electronico del Madre
           ui->lineEdit_46->setText(AuxM->index(0,17).data().toString());
       }
       else
       {
           if(codpa != "no" and !ui->checkBox_11->isChecked())
           {
              desactivaM();
              limpiarM();
              ui->checkBox_12->setChecked(false);
           }
       }

    }


   if(ui->checkBox_11->isChecked() and ui->checkBox_12->isChecked())
   {
       if(codpa == "")
       {
           desactivaP();
           limpiarP();
           ui->checkBox_11->setChecked(false);
       }
       if(codma == "")
       {
           desactivaM();
           limpiarM();
           ui->checkBox_12->setChecked(false);
       }

   }

   //Destruyo los modelos para desalocar la memoria y liberar la conexion

   AuxM->~QSqlTableModel();
   AuxP->~QSqlTableModel();

}

//..........Slots..................................................

//Funcion para mostrar la ventana de configuraciones
void MainWindow::abrir_opciones()
{
    conf->desactivarQ();
    conf->desactivarCOS();
    conf->desactivarEVE();
    conf->pes_uno();
    conf->asignar_fechas();

    conf->exec();

}

//Para cuando se tildan que estudia o que trabaja
void MainWindow::on_checkBox_5_stateChanged(int)
{
    if (ui->checkBox_5->isChecked() == true)
    {
        ui->lineEdit_16->setEnabled(true);
    }
    else
    {
        ui->lineEdit_16->setEnabled(false);
        ui->lineEdit_16->clear();
    }

}

void MainWindow::on_checkBox_6_stateChanged(int )
{
    if (ui->checkBox_6->isChecked() == true)
    {
        ui->lineEdit_15->setEnabled(true);
    }
    else
    {
        ui->lineEdit_15->setEnabled(false);
        ui->lineEdit_15->clear();
    }
}

//Menu para la carga de la foto
void MainWindow::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->Foto->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Cargar Foto");
    myMenu.addAction("Quitar Foto");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if (selectedItem->text() == "Cargar Foto")
        {
            //Para cargar la imagen
            QString imagefilename = QFileDialog::getOpenFileName(this,"Image to open","path","Images (*.bmp *.png *.xpm *.jpg)");
            if (imagefilename != "")
            {
                QImage  Imagen;
                Imagen.load (imagefilename);
                Imagen.convertToFormat(QImage::Format_RGB32);
                ui->Foto->setPixmap(QPixmap(QPixmap::fromImage(Imagen).scaled(ui->Foto->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            }

        }
        else
        {
            if (selectedItem->text() == "Quitar Foto")
            {
                //Borro la foto de la interfaz y le asigno al laber una imagen vacia
                ui->Foto->clear();
                if(ui->radioButton->isChecked())
                {
                    ui->Foto->setPixmap(QPixmap(":/icons/male"));
                }
                else
                {
                    ui->Foto->setPixmap(QPixmap(":/icons/female"));
                }

            }
        }
    }


}

//Fotos por defecto
void MainWindow::on_radioButton_pressed()
{
    if(!Aux.tienefoto() or ui->Foto->pixmap()->cacheKey() == QPixmap(":/icons/female").cacheKey()){ui->Foto->setPixmap(QPixmap(":/icons/male"));}

}

void MainWindow::on_radioButton_2_pressed()
{
    if(!Aux.tienefoto() or ui->Foto->pixmap()->cacheKey() == QPixmap(":/icons/male").cacheKey()){ui->Foto->setPixmap(QPixmap(":/icons/female"));}
}

//Buscar en la bases de datos y cargar en los lugares correspondientes

void MainWindow::on_lineEdit_returnPressed ()
{
    //Obtengo el tipo y numero de documento de el miembro seleccionado
    QModelIndex indice = view->currentIndex();

    //Seteo la variable global Cod_Miembro con el valor actual del indice
    Cod_Miembro = indice.sibling(indice.row(),4).data().toString();

    if(!ui->lineEdit->text().isEmpty() and !Cod_Miembro.isEmpty())
    {
        //Tendria que controlar que no haga nada cuando se toca enter sin haber escrito nada aca o si se escribe algo que no esta en la BD
        limpiarD();
        limpiarP();
        limpiarM();

        limpiarFM();
        limpiarCM();

        limpiar_reporte();
        activar_reporte();

        //----------Miembro---------
        ui->checkBox_11->setChecked(false);
        ui->checkBox_12->setChecked(false);
        ui->pushButton_5->setVisible(true);

        //Declaro las variables necesaria para sacar los datos de Aux y ponerlos en los campos Correspondientes
        //Variables para el Miembro
        QString Ape,Nom,S,T_D,Dom,Pi,Dep,Nac,Prov,Loc,Lug_N,C_elec,Sac,Est,Trab,R,Car,P,Obs,Prog; //Cod_Miembro;
        QDate F_N,F_I;
        int Doc,Num,Tel;
        Persona Ma,Pa;

        ficha_medica fichmed;
        //Variables para el Padre
        QString ApeP,NomP,SP,T_DP,DomP,PiP,DepP,NacP,ProvP,LocP,Lug_NP,C_elecP,MiemAct;
        QDate F_NP;
        int DocP,NumP,TelP;
        QPixmap Img;
        //Variables para la Madre
        QString ApeM,NomM,SM,T_DM,DomM,PiM,DepM,NacM,ProvM,LocM,Lug_NM,C_elecM;
        QDate F_NM;
        int DocM,NumM,TelM;
        //Variables para la ficha medica
        QString gruporh,enfer,amb,alim,aler,quir,obs_fm,pad,med;//cod_FM;
        QString medi,dirmed,nomurg;
        int telurg,telmedi;
        // Para cuenta miembro no son nesesarias las variables ya que muestro todos los valores a travez de la tableview

        //Busco el miembro y sus datos asociados en la Base de Datos y los cargo en Aux
        Aux.buscar_BD(Cod_Miembro);

        //Leo los datos de Aux y los paso a las variables declaradas anteriormente. Hago lo mismo con los padres
        Aux.Leer(MiemAct,Img,Ape,Nom,S,F_N,T_D,Doc,Dom,Num,Pi,Dep,Tel,Nac,Prov,Loc,Lug_N,C_elec,Pa,Ma,F_I,Sac,Est,Trab,R,P,Car,Obs,Prog,fichmed); //,Cod_Miembro NO es nesesario en el leer. Ya la tenemos cargada en la variable global.
        Pa.Leer(ApeP,NomP,SP,F_NP,T_DP,DocP,DomP,NumP,PiP,DepP,TelP,NacP,ProvP,LocP,Lug_NP,C_elecP,Cod_Padre);
        Ma.Leer(ApeM,NomM,SM,F_NM,T_DM,DocM,DomM,NumM,PiM,DepM,TelM,NacM,ProvM,LocM,Lug_NM,C_elecM,Cod_Madre);
        fichmed.Leer(Cod_FM,gruporh,enfer,amb,alim,aler,quir,obs_fm,pad,med,medi,dirmed,nomurg,telurg,telmedi);

        //Tengo los datos en las variables, ahora tengo que mostrarlos en pantalla pero que sean no editables

        //Datos del Miembro
        //Nombre
        ui->lineEdit_34->setText(Ape);
        //Apellido
        ui->lineEdit_35->setText(Nom);
        //Actualizo el Label
        escribe_entiemporeal(Nom,Ape);
        //Sexo
        if (S  == "M")
        {
            ui->radioButton->setChecked(true);
        }
        else
        {
            ui->radioButton_2->setChecked(true);
        }
        //Fecha Nacimiento
        ui->dateEdit_2->setDate(F_N);
        //Tipo de Documento
        int indice1 = ui->comboBox->findText(T_D);
        ui->comboBox->setCurrentIndex(indice1);
        //Documento
        ui->lineEdit_2->setText(QString("%1").arg(Doc));
        //Domicilio
        ui->lineEdit_5->setText(Dom);
        //Numero Domicilio
        if(Num>0){ui->lineEdit_36->setText(QString("%1").arg(Num));}
        //Piso
        ui->lineEdit_6->setText(Pi);
        //Departamento
        ui->lineEdit_7->setText(Dep);
        //Telefono
        if(Tel>0){ui->lineEdit_8->setText(QString("%1").arg(Tel));}
        //Nacionalidad
        ui->lineEdit_9->setText(Nac);
        //Provincia
        ui->lineEdit_10->setText(Prov);
        //Localidad
        ui->lineEdit_11->setText(Loc);
        //Lugar de Nacimiento
        ui->lineEdit_12->setText(Lug_N);
        //Correo electronico
        ui->lineEdit_13->setText(C_elec);
        //Fecha de Ingreso al Grupo
        ui->dateEdit->setDate(F_I);
        //Sacreamentos
        if (Sac.contains("Bautismo")){ui->checkBox_7->setChecked(true);}
        if (Sac.contains("Comunión")){ui->checkBox_8->setChecked(true);}
        if (Sac.contains("Confirmación")){ui->checkBox_9->setChecked(true);}
        if (Sac.contains("Matrimonio")){ui->checkBox_10->setChecked(true);}
        //Estudia
        if (Est != "No Estudia")
        {
            ui->checkBox_5->setChecked(true);
            ui->lineEdit_16->setEnabled(false);
            ui->lineEdit_16->setText(Est);
        }
        //Trabaja
        if (Trab != "No Trabaja")
        {
            ui->checkBox_6->setChecked(true);
            ui->lineEdit_15->setEnabled(false);
            ui->lineEdit_15->setText(Trab);
        }
        //Rama
        indice1 = ui->comboBox_2->findText(R);
        ui->comboBox_2->setCurrentIndex(indice1);
        //Patrulla
        indice1 = ui->comboBox_3->findText(P);
        ui->comboBox_3->setCurrentIndex(indice1);
        //Cargo
        indice1 = ui->comboBox_4->findText(Car);
        ui->comboBox_4->setCurrentIndex(indice1);
        //Observaciones
        ui->plainTextEdit->setPlainText(Obs);
        //Progresiones
        ui->plainTextEdit_2->setPlainText(Prog);

        //Miembro Activo
        if (MiemAct  == "Activo")
        {
            ui->checkBox_13->setChecked(true);
            //----------Cuenta_Miembro-------
            activarCM_Asignacion(); //recien aqui permito el ingreso de datos en cuenta Miembro
        }
        else
        {
            ui->checkBox_13->setChecked(false);
            desactivarCM_Asignacion();
        }
        //Foto

        ui->Foto->setPixmap(Img);
        qDebug()<<Img.isNull();
        if(ui->Foto->pixmap()->isNull())
        {
            if(ui->radioButton->isChecked())
            {
                ui->Foto->setPixmap(QPixmap(":/icons/male"));
            }
            else
            {
                ui->Foto->setPixmap(QPixmap(":/icons/female"));
            }
        }

        //----------Ficha_Medica-------

        //telefono urgencias
        if(telurg > 0){
            ui->lineEdit_59->setText(QString("%1").arg(telurg));
        }
        //telefono medico
        if (telmedi > 0){
            ui->lineEdit_57->setText(QString("%1").arg(telmedi));
        }

        //grupo y rh
        int ind2 = ui->comboBox_9->findText(gruporh);
        ui->comboBox_9->setCurrentIndex(ind2);
        //enfermedades
        ui->lineEdit_32->setText(enfer);
        //quirurguicas
        ui->lineEdit_56->setText(quir);
        //alergico
        ui->lineEdit_60->setText(aler);
        //observaciones
        ui->lineEdit_54->setText(obs_fm);
        //padece
        ui->lineEdit_33->setText(pad);
        //medicamentos
        ui->lineEdit_38->setText(med);
        //alimentos
        ui->lineEdit_55->setText(alim);
        //ambientes
        ui->lineEdit_58->setText(amb);
        //nombre urgencias
        ui->lineEdit_31->setText(nomurg);
        //nombre medico
        ui->lineEdit_62->setText(medi);
        //direccion medico
        ui->lineEdit_63->setText(dirmed);

        if(Cod_Miembro != "NO")
        {
            //tabla ingresos generales
            QString  Filtro_CM ="Cod_Miembro='" + Cod_Miembro + "'"+ "and cod_evento is NULL";
            cum->setFilter(Filtro_CM);
            cum->select();

            //tabla evento
            QString  Filtro_CME ="Cod_Miembro='" + Cod_Miembro + "'"+ "and cod_evento is not NULL";
            cme->setFilter(Filtro_CME);
            cme->select();

           //Calculo el debe y el haber
            calculos_miembro(Cod_Miembro);

        }

        //------------Padres-------------

        //Datos del Padre
        if (Cod_Padre != "no")
        {
            //Pongo el tick en el padre
            ui->checkBox_11->setChecked(true);
            ui->checkBox_11->setEnabled(false);
            //Apellido del Padre
            ui->lineEdit_39->setText(ApeP);
            //Nombre de Padre
            ui->lineEdit_40->setText(NomP);
            //Actualizo el Label
            escribe_entiemporealP(NomP,ApeP);
            //Fecha de Nacimiento del Padre
            ui->dateEdit_3->setDate(F_NP);
            //Tipo de Documento del Padre
            indice1 = ui->comboBox_5->findText(T_DP);
            ui->comboBox_5->setCurrentIndex(indice1);
            //Numero de Documento del Padre
            ui->lineEdit_3->setText(QString("%1").arg(DocP));
            //Domicilio del Padre
            ui->lineEdit_14->setText(DomP);
            //Numero del Domicilio del Padre
            if(NumP>0){ui->lineEdit_37->setText(QString("%1").arg(NumP));}
            //Piso del Padre
            ui->lineEdit_17->setText(PiP);
            //Departamento del Padre
            ui->lineEdit_18->setText(DepP);
            //Telefono del Padre
            if(TelP>0){ui->lineEdit_19->setText(QString("%1").arg(TelP));}
            //Nacionalidad del Padre
            ui->lineEdit_20->setText(NacP);
            //Provincia del Padre
            ui->lineEdit_21->setText(ProvP);
            //Localidad del Padre
            ui->lineEdit_22->setText(LocP);
            //Lugar de Nacimiento del Padre
            ui->lineEdit_23->setText(Lug_NP);
            //Correo electronico del Padre
            ui->lineEdit_24->setText(C_elecP);
        }
        //Datos del Madre
        if (Cod_Madre != "no")
        {
            //Pongo el tick en el madre
            ui->checkBox_12->setChecked(true);
            ui->checkBox_12->setEnabled(false);
            //Apellido del Madre
            ui->lineEdit_52->setText(ApeM);
            //Nombre de Madre
            ui->lineEdit_53->setText(NomM);
            //Actualizo el Label
            escribe_entiemporealM(NomM,ApeM);
            //Fecha de Nacimiento del Madre
            ui->dateEdit_4->setDate(F_NM);
            //Tipo de Documento del Madre
            indice1 = ui->comboBox_7->findText(T_DM);
            ui->comboBox_7->setCurrentIndex(indice1);
            //Numero de Documento del Madre
            ui->lineEdit_47->setText(QString("%1").arg(DocM));
            //Domicilio del Madre
            ui->lineEdit_51->setText(DomM);
            //Numero del Domicilio del Madre
            if(NumM){ui->lineEdit_48->setText(QString("%1").arg(NumM));}
            //Piso del Madre
            ui->lineEdit_49->setText(PiM);
            //Departamento del Madre
            ui->lineEdit_50->setText(DepM);
            //Telefono del Madre
            if(TelM){ui->lineEdit_41->setText(QString("%1").arg(TelM));}
            //Nacionalidad del Madre
            ui->lineEdit_42->setText(NacM);
            //Provincia del Madre
            ui->lineEdit_43->setText(ProvM);
            //Localidad del Madre
            ui->lineEdit_44->setText(LocM);
            //Lugar de Nacimiento del Madre
            ui->lineEdit_45->setText(Lug_NM);
            //Correo electronico del Madre
            ui->lineEdit_46->setText(C_elecM);
        }

        //Adapto la interfaz
        //Limpio Los Campos segun corresponda
        if(ui->checkBox_11->isChecked() == false){limpiarP();}
        if(ui->checkBox_12->isChecked() == false){limpiarM();}

    }
    else
    {
       QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Elija un miembro de la lista desplegable.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);
    }

    asignar_fechas(); // Actualizo la interfaz con las nuevas fechas y bloqueo en caso de ser nesesario.

}

void MainWindow::on_lineEdit_35_textEdited(QString Nom)
{
    QString Nombre = Nom;
    QString Apellido = ui->lineEdit_34->text();
    escribe_entiemporeal(Nombre,Apellido);
}

void MainWindow::on_lineEdit_34_textEdited(QString Ape)
{
    QString Apellido = Ape;
    QString Nombre = ui->lineEdit_35->text();
    escribe_entiemporeal(Nombre,Apellido);
}

//Nuevo Miembro - Habilita los campos para completar
void MainWindow::on_pushButton_4_pressed()
{
    limpiarD();
    limpiarP();
    limpiarM();
    limpiarFM();
    activaD();
    activaFM();
    //ui->Foto->setPixmap(QPixmap(":/icons/male"));
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6NM->setVisible(true);
    ui->pushButton_7NM->setVisible(true);
    ui->pushButton_4->setVisible(false);
    ui->checkBox_11->setChecked(false);
    ui->checkBox_12->setChecked(false);
    ui->checkBox_11->setEnabled(true);
    ui->checkBox_12->setEnabled(true);
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->clear();

    // Cuenta_ Miembro - Filtro al presionar nuevo miembro
    ui->cuenta_miembro->setEnabled(false);
    ui->EventosMie->setEnabled(false);
    cme->setFilter("Cod_CON='no'");
    cum->setFilter("Cod_CON='no'");
    cme->select();
    cum->select();

    //Me coloco sobre la pestaña Ficha de datos. Los datos principales de relleno se encuentran sobre esta pestaña

    ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
    ui->tabWidget_2->setCurrentIndex(0); //Pestaña Ficha de Datos
}

//GUARDA Nuevo Miembro - Faltaria validar datos
void MainWindow::on_pushButton_6NM_pressed()
{
    bool control1,control2;

    //Banderas de Control
    control1 = false;
    control2 = false;

    //Controlo que como minimo entre el Nombre Apellido tipo y numero de documento para poder guardarse
    if(!ui->lineEdit_34->text().isEmpty() and !ui->lineEdit_35->text().isEmpty() and !ui->lineEdit_2->text().isEmpty())
    {
        //Guardo en variables los datos cargados

        QString Ape = ui->lineEdit_34->text();
        QString Nom = ui->lineEdit_35->text();

        QString S;
        if (ui->radioButton_2->isChecked() == true)
        {
            S= "F";
        }
        else
        {
            S= "M";
        }
        QDate F_N = ui->dateEdit_2->date();

        int indice1 = ui->comboBox->currentIndex();
        QString T_D  = ui->comboBox->itemText(indice1);

        int Doc = ui->lineEdit_2->text().toInt();
        QString Dom = ui->lineEdit_5->text();
        int Num = ui->lineEdit_36->text().toInt();
        QString Pi = ui->lineEdit_6->text();
        QString D = ui->lineEdit_7->text();
        int Tel = ui->lineEdit_8->text().toInt();
        QString Nac = ui->lineEdit_9->text();
        QString Prov = ui->lineEdit_10->text();
        QString Loc = ui->lineEdit_11->text();
        QString Lug_N = ui->lineEdit_12->text();
        QString C_elec = ui->lineEdit_13->text();

        QDate F_I = ui->dateEdit->date();

        QString Sac;
        if (ui->checkBox_7->isChecked()){Sac+=ui->checkBox_7->text()+",";}
        if (ui->checkBox_8->isChecked()){Sac+=ui->checkBox_8->text()+",";}
        if (ui->checkBox_9->isChecked()){Sac+=ui->checkBox_9->text()+",";}
        if (ui->checkBox_10->isChecked()){Sac+=ui->checkBox_10->text()+",";}

        QString Est;
        if (ui->checkBox_5->isChecked())
        {
            Est = ui->lineEdit_16->text();
        }
        else
        {
            Est = "No Estudia";
        }

        QString Trab;
        if (ui->checkBox_6->isChecked())
        {
            Trab = ui->lineEdit_15->text();
        }
        else
        {
            Trab = "No Trabaja";
        }

        int indice2 = ui->comboBox_2->currentIndex();
        QString R;

        if(ui->comboBox_2->itemText(indice2).isNull())
        {
            R = "Nada";
        }
        else
        {
            R = ui->comboBox_2->itemText(indice2);
        }

        int indice3 = ui->comboBox_3->currentIndex();
        QString P;

        if(ui->comboBox_3->itemText(indice3).isNull())
        {
            P = "Nada";
        }
        else
        {
            P = ui->comboBox_3->itemText(indice3);
        }

        int indice4 = ui->comboBox_4->currentIndex();
        QString Car;

        if(ui->comboBox_4->itemText(indice4).isNull())
        {
            Car = "Nada";
        }
        else
        {
            Car = ui->comboBox_4->itemText(indice4);
        }

        QString Obs = ui->plainTextEdit->toPlainText();
        QString Prog = ui->plainTextEdit_2->toPlainText();

        QString MiemAct;
        if(ui->checkBox_13->isChecked())
        {
            MiemAct = "Activo";

            //Si el miembro se setea como activo automaticamente le permito el ingreso.
            activarCM_Asignacion(); //recien aqui permito el ingreso de datos en cuenta Miembro

        }
        else
        {
            MiemAct = "Inactivo";
            //Desactivo la cuenta miembro. Al no ser un miembro activo no deberia de participar del año scout.
            desactivarCM_Asignacion();
        }

        //Si no se cargo una foto no cargo la foto que esta por defecto

        QPixmap Img=NULL;
        if(ui->Foto->pixmap()->cacheKey() != QPixmap(":/icons/male").cacheKey() and ui->Foto->pixmap()->cacheKey() != QPixmap(":/icons/female").cacheKey()){Img = *ui->Foto->pixmap();}


        // guardo los datos de los Padres en variables para luego cargarlos
        Persona PadreM;
        Persona MadreM;
        ficha_medica ficha;

        //datos ficha medica

        int telurg = ui->lineEdit_59->text().toInt();
        int telmedi = ui->lineEdit_57->text().toInt();
        int indice5 = ui->comboBox_9->currentIndex();
        QString gruporh = ui->comboBox_9->itemText(indice5);
        QString enfer = ui->lineEdit_32->text();
        QString quir = ui->lineEdit_56->text();
        QString aler = ui->lineEdit_60->text();
        QString obs_fm = ui->lineEdit_54->text();
        QString med = ui->lineEdit_38->text();
        QString alim = ui->lineEdit_55->text();
        QString amb = ui->lineEdit_58->text();
        QString nomurg = ui->lineEdit_31->text();
        QString pad = ui->lineEdit_33->text();
        QString medi = ui->lineEdit_62->text();
        QString dirmed = ui->lineEdit_63->text();

        ConectarconBD::cod_setup("Ficha_Medica",Cod_FM);
        ficha.Guardar(Cod_FM,gruporh,enfer,amb,alim,aler,quir,obs_fm,pad,med,medi,dirmed,nomurg,telurg,telmedi);

        if (ui->checkBox_11->isChecked())
        {
            if(!ui->lineEdit_39->text().isEmpty() and !ui->lineEdit_40->text().isEmpty() and !ui->lineEdit_3->text().isEmpty())
            {
                //Banderas de Control
                control1 =true;

                QString ApeP = ui->lineEdit_39->text();
                QString NomP = ui->lineEdit_40->text();
                QString SP = "M";
                QDate F_NP = ui->dateEdit_3->date();
                int indice1 = ui->comboBox_5->currentIndex();
                QString T_DP  = ui->comboBox_5->itemText(indice1);
                int DocP = ui->lineEdit_3->text().toInt();
                QString DomP = ui->lineEdit_14->text();
                int NumP = ui->lineEdit_37->text().toInt();
                QString PiP = ui->lineEdit_17->text();
                QString DP = ui->lineEdit_18->text();
                int TelP = ui->lineEdit_19->text().toInt();
                QString NacP = ui->lineEdit_20->text();
                QString ProvP = ui->lineEdit_21->text();
                QString LocP = ui->lineEdit_22->text();
                QString Lug_NP = ui->lineEdit_23->text();
                QString C_elecP = ui->lineEdit_24->text();

                ConectarconBD::cod_setup("Padre",Cod_Padre);

                PadreM.Guardar(ApeP,NomP,SP,F_NP,T_DP,DocP,DomP,NumP,PiP,DP,TelP,NacP,ProvP,LocP,Lug_NP,C_elecP,Cod_Padre);
            }
            else
            {
                //Redirecciono a la pestaña correspondiente
                ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
                ui->tabWidget_2->setCurrentIndex(1); //Pestaña Ficha de Datos
                QMessageBox::warning(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al ingresar un nuevo Miembro.\n""Complete al menos el Nombre, Apellido y Documento del Padre.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);
            }


        }

        if (ui->checkBox_12->isChecked())
        {

            if(!ui->lineEdit_52->text().isEmpty() and !ui->lineEdit_53->text().isEmpty() and !ui->lineEdit_47->text().isEmpty())
            {
                //Banderas de Control
                control2 =true;


                QString ApeM = ui->lineEdit_52->text();
                QString NomM = ui->lineEdit_53->text();
                QString SM = "F";
                QDate F_NM = ui->dateEdit_4->date();


                int indice1 = ui->comboBox_7->currentIndex();
                QString T_DM  = ui->comboBox_7->itemText(indice1);



                int DocM = ui->lineEdit_47->text().toInt();
                QString DomM = ui->lineEdit_51->text();
                int NumM = ui->lineEdit_48->text().toInt();
                QString PiM = ui->lineEdit_49->text();
                QString DM = ui->lineEdit_50->text();
                int TelM = ui->lineEdit_41->text().toInt();
                QString NacM = ui->lineEdit_42->text();
                QString ProvM = ui->lineEdit_43->text();
                QString LocM = ui->lineEdit_44->text();
                QString Lug_NM = ui->lineEdit_45->text();
                QString C_elecM = ui->lineEdit_46->text();

                ConectarconBD::cod_setup("Madre",Cod_Madre);

                MadreM.Guardar(ApeM,NomM,SM,F_NM,T_DM,DocM,DomM,NumM,PiM,DM,TelM,NacM,ProvM,LocM,Lug_NM,C_elecM,Cod_Madre);
            }
            else
            {
                //Redirecciono a la pestaña correspondiente
                ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
                ui->tabWidget_2->setCurrentIndex(1); //Pestaña Ficha de Datos
                QMessageBox::warning(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al ingresar un nuevo Miembro.\n""Complete al menos Nombre, Apellido y Documento de la Madre.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);
            }


        }

        //Salteo los controles si no se completo lo correcto de los padres

        if (!ui->checkBox_11->isChecked())
        {
            control1 = true;
        }

        if (!ui->checkBox_12->isChecked())
        {
            control2 = true;
        }

        //GUARDO el Nuevo Miembro si pasa los controles

        if(control1 == true and control2 == true)
        {
            ConectarconBD::cod_setup("Miembro",Cod_Miembro);

            Aux.Guardar(MiemAct,Img,Ape,Nom,S,F_N,T_D,Doc,Dom,Num,Pi,D,Tel,Nac,Prov,Loc,Lug_N,C_elec,PadreM,MadreM,F_I,Sac,Est,Trab,R,P,Car,Obs,Prog,ficha,Cod_Miembro);

            // Agrego los metodos transaction t commit para el ingreso nuevo de miembro.
            QSqlDatabase::database().transaction();

            Aux.almacenar_BD();

            QSqlDatabase::database().commit();

            //Acomodo la interfaz para que vuelva al estado anterior

            //limpiarD();
            //limpiarP();
            //limpiarM();
            //limpiarFM();
            desactivaD();
            desactivaP();
            desactivaM();
            desactivaFM();
            ui->lineEdit->setEnabled(true);
            ui->pushButton_4->setVisible(true);
            ui->pushButton_6NM->setVisible(false);
            ui->pushButton_7NM->setVisible(false);
            ui->checkBox_11->setEnabled(false);
            ui->checkBox_12->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            Cod_Padre = "no";
            Cod_Madre = "no";
            Cod_FM = "no";

        }

    }
    else
    {
        //Redirecciono a la pestaña correspondiente
        ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
        ui->tabWidget_2->setCurrentIndex(0); //Pestaña Ficha de Datos
        QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al ingresar un nuevo Miembro.\n""Complete al menos Nombre, Apellido y Documento del Miembro.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);
    }

}

//ACTUALIZA - Guardar cambios de edicion de miembro en la base de datos
void MainWindow::on_pushButton_9E_pressed()
{
    bool control1,control2;

    //Banderas de Control
    control1 = false;
    control2 = false;

    if(!ui->lineEdit_34->text().isEmpty() and !ui->lineEdit_35->text().isEmpty() and !ui->lineEdit_2->text().isEmpty())
    {
        //REVISAR!!!...ME PARECE Q NO ES NESESARIO

        //   QModelIndex indice = view->currentIndex();
        //   Cod_Miembro = indice.sibling(indice.row(),27).data().toString();


        //QString tipodoc = indice.sibling(indice.row(),4).data().toString();
        //int numdoc = indice.sibling(indice.row(),5).data().toInt();

        //Guardo en variables los datos modificados

        QString Ape = ui->lineEdit_34->text();
        QString Nom = ui->lineEdit_35->text();

        QString S;
        if (ui->radioButton_2->isChecked() == true){
            S= "F";
        }
        else
        {
            S= "M";
        }

        QDate F_N = ui->dateEdit_2->date();

        int indice1 = ui->comboBox->currentIndex();
        QString T_D  = ui->comboBox->itemText(indice1);

        int Doc = ui->lineEdit_2->text().toInt();
        QString Dom = ui->lineEdit_5->text();
        int Num = ui->lineEdit_36->text().toInt();
        QString Pi = ui->lineEdit_6->text();
        QString D = ui->lineEdit_7->text();
        int Tel = ui->lineEdit_8->text().toInt();
        QString Nac = ui->lineEdit_9->text();
        QString Prov = ui->lineEdit_10->text();
        QString Loc = ui->lineEdit_11->text();
        QString Lug_N = ui->lineEdit_12->text();
        QString C_elec = ui->lineEdit_13->text();

        QDate F_I = ui->dateEdit->date();

        QString Sac;
        if (ui->checkBox_7->isChecked()){Sac+=ui->checkBox_7->text()+",";}
        if (ui->checkBox_8->isChecked()){Sac+=ui->checkBox_8->text()+",";}
        if (ui->checkBox_9->isChecked()){Sac+=ui->checkBox_9->text()+",";}
        if (ui->checkBox_10->isChecked()){Sac+=ui->checkBox_10->text()+",";}

        QString Est;
        if (ui->checkBox_5->isChecked())
        {
            Est = ui->lineEdit_16->text();
        }
        else
        {
            Est = "No Estudia";
        }

        QString Trab;
        if (ui->checkBox_6->isChecked())
        {
            Trab = ui->lineEdit_15->text();
        }
        else
        {
            Trab = "No Trabaja";
        }

        int indice2 = ui->comboBox_2->currentIndex();
        QString R;

        if(ui->comboBox_2->itemText(indice2).isNull())
        {
            R = "Nada";
        }
        else
        {
            R = ui->comboBox_2->itemText(indice2);
        }

        int indice3 = ui->comboBox_3->currentIndex();
        QString P;

        if(ui->comboBox_3->itemText(indice3).isNull())
        {
            P = "Nada";
        }
        else
        {
            P = ui->comboBox_3->itemText(indice3);
        }

        int indice4 = ui->comboBox_4->currentIndex();
        QString Car;

        if(ui->comboBox_4->itemText(indice4).isNull())
        {
            Car = "Nada";
        }
        else
        {
            Car = ui->comboBox_4->itemText(indice4);
        }

        QString Obs = ui->plainTextEdit->toPlainText();
        QString Prog = ui->plainTextEdit_2->toPlainText();

        QString MiemAct;
        if(ui->checkBox_13->isChecked())
        {
            MiemAct = "Activo";

            //Si el miembro se setea como activo automaticamente le permito el ingreso.
            activarCM_Asignacion(); //recien aqui permito el ingreso de datos en cuenta Miembro

        }
        else
        {
            MiemAct = "Inactivo";
            //Desactivo la cuenta miembro. Al no ser un miembro activo no deberia de participar del año scout.
            desactivarCM_Asignacion();
        }

        //Si no se cargo una foto no cargo la foto que esta por defecto

        QPixmap Img=NULL;
        if(ui->Foto->pixmap()->cacheKey() != QPixmap(":/icons/male").cacheKey() and ui->Foto->pixmap()->cacheKey() != QPixmap(":/icons/female").cacheKey()){Img = *ui->Foto->pixmap();}


        // actualiza los datos de los Padres en variables para luego cargarlos
        Persona PadreM;
        Persona MadreM;
        ficha_medica ficha;

        if (ui->checkBox_11->isChecked())
        {

            if(!ui->lineEdit_39->text().isEmpty() and !ui->lineEdit_40->text().isEmpty() and !ui->lineEdit_3->text().isEmpty())
            {
                //Banderas de Control
                control1 =true;



                QString ApeP = ui->lineEdit_39->text();
                QString NomP = ui->lineEdit_40->text();
                QString SP = "M";
                QDate F_NP = ui->dateEdit_3->date();


                int indice1 = ui->comboBox_5->currentIndex();
                QString T_DP  = ui->comboBox_5->itemText(indice1);



                int DocP = ui->lineEdit_3->text().toInt();
                QString DomP = ui->lineEdit_14->text();
                int NumP = ui->lineEdit_37->text().toInt();
                QString PiP = ui->lineEdit_17->text();
                QString DP = ui->lineEdit_18->text();
                int TelP = ui->lineEdit_19->text().toInt();
                QString NacP = ui->lineEdit_20->text();
                QString ProvP = ui->lineEdit_21->text();
                QString LocP = ui->lineEdit_22->text();
                QString Lug_NP = ui->lineEdit_23->text();
                QString C_elecP = ui->lineEdit_24->text();


                //Aca modificamos un poco la forma de como llama, por que el cod_padre puede estar vacio. Entonces primero preguntamos para luego guardar y almacenar en caso de no estar incluido.



                //En el caso que se completen los datos de los padres posteriormente a la insersion del miembro
                if (PadreM.estaenBD(Cod_Miembro,"M") == false)
                {
                    //qDebug()<<"padre en actualizar"<<Cod_Miembro;
                    ConectarconBD::cod_setup("Padre",Cod_Padre);
                    //ConectarconBD::cod_setup("Padre",Cod_Padre);
                    PadreM.Guardar(ApeP,NomP,SP,F_NP,T_DP,DocP,DomP,NumP,PiP,DP,TelP,NacP,ProvP,LocP,Lug_NP,C_elecP,Cod_Padre);
                    PadreM.almacenar_BD(Cod_Miembro);

                }
                else
                {
                    PadreM.Guardar(ApeP,NomP,SP,F_NP,T_DP,DocP,DomP,NumP,PiP,DP,TelP,NacP,ProvP,LocP,Lug_NP,C_elecP,Cod_Padre);
                }

            }
            else
            {
                //Redirecciono a la pestaña correspondiente
                ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
                ui->tabWidget_2->setCurrentIndex(1); //Pestaña Ficha de Datos
                QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al ingresar un nuevo Miembro.\n""Complete al menos Nombre, Apellido y Documento del Padre.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);

            }

        }

        if (ui->checkBox_12->isChecked())
        {

            if(!ui->lineEdit_52->text().isEmpty() and !ui->lineEdit_53->text().isEmpty() and !ui->lineEdit_47->text().isEmpty())
            {
                //Banderas de Control
                control2 =true;


                QString ApeM = ui->lineEdit_52->text();
                QString NomM = ui->lineEdit_53->text();
                QString SM = "F";
                QDate F_NM = ui->dateEdit_4->date();


                int indice1 = ui->comboBox_7->currentIndex();
                QString T_DM  = ui->comboBox_7->itemText(indice1);

                int DocM = ui->lineEdit_47->text().toInt();
                QString DomM = ui->lineEdit_51->text();
                int NumM = ui->lineEdit_48->text().toInt();
                QString PiM = ui->lineEdit_49->text();
                QString DM = ui->lineEdit_50->text();
                int TelM = ui->lineEdit_41->text().toInt();
                QString NacM = ui->lineEdit_42->text();
                QString ProvM = ui->lineEdit_43->text();
                QString LocM = ui->lineEdit_44->text();
                QString Lug_NM = ui->lineEdit_45->text();
                QString C_elecM = ui->lineEdit_46->text();



                //Igual que en el Padre la comparacion.

                //En el caso que se completen los datos de los padres posteriormente a la insersion del miembro
                if (MadreM.estaenBD(Cod_Miembro,"F") == false)
                {
                    ConectarconBD::cod_setup("Madre",Cod_Madre);
                    //ConectarconBD::cod_setup("Madre",Cod_Madre);
                    MadreM.Guardar(ApeM,NomM,SM,F_NM,T_DM,DocM,DomM,NumM,PiM,DM,TelM,NacM,ProvM,LocM,Lug_NM,C_elecM,Cod_Madre);
                    MadreM.almacenar_BD(Cod_Miembro);
                }

                else
                {
                    MadreM.Guardar(ApeM,NomM,SM,F_NM,T_DM,DocM,DomM,NumM,PiM,DM,TelM,NacM,ProvM,LocM,Lug_NM,C_elecM,Cod_Madre);
                }

            }
            else
            {
                //Redirecciono a la pestaña correspondiente
                ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
                ui->tabWidget_2->setCurrentIndex(1); //Pestaña Ficha de Datos
                QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al ingresar un nuevo Miembro.\n""Complete al menos el Nombre, Apellido y Documento de la Madre.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);

            }

        }

        //Salteo los controles si no se completo lo correcto de los padres

        if (!ui->checkBox_11->isChecked())
        {
            control1 = true;
        }

        if (!ui->checkBox_12->isChecked())
        {
            control2 = true;
        }

        //ACTUALIZO el Nuevo Miembro si pasa los controles

        if(control1 == true and control2 == true)
        {

            //datos ficha medica

            //    QString enfer,amb,alim,aler,quir,obs_fm,pad,med;//cod_FM;
            //    QString medi,dirmed,nomurg;

            int telurg = ui->lineEdit_59->text().toInt();
            int telmedi =ui->lineEdit_57->text().toInt();

            int indc = ui->comboBox_9->currentIndex();
            QString gruporh = ui->comboBox_9->itemText(indc);
            //qDebug()<<"mainwindows.cpp:: gruporh en boton editar"<<gruporh;

            QString enfer = ui->lineEdit_32->text();
            QString quir = ui->lineEdit_56->text();
            QString aler = ui->lineEdit_60->text();
            QString obs_fm = ui->lineEdit_54->text();
            QString med = ui->lineEdit_38->text();
            QString alim = ui->lineEdit_55->text();
            QString amb = ui->lineEdit_58->text();
            QString nomurg = ui->lineEdit_31->text();
            QString pad = ui->lineEdit_33->text();
            QString medi = ui->lineEdit_62->text();
            QString dirmed = ui->lineEdit_63->text();

            ficha.Guardar(Cod_FM,gruporh,enfer,amb,alim,aler,quir,obs_fm,pad,med,medi,dirmed,nomurg,telurg,telmedi);
            //fn.Guardar(Cod_FM,gruporh,enfer,amb,alim,aler,quir,obs_fm,pad,med,medi,dirmed,nomurg,telurg,telmedi);

            //Arreglo para que se pueda modificar ambos padres a la vez
            bool losdos = false;
            if (ui->checkBox_12->isChecked() and ui->checkBox_11->isChecked())
            {
                losdos=true;
            }

            //Elimino los padres si se destildan las casillas
            if (!ui->checkBox_11->isChecked())
            {
                //Lo elimino de la BD
                QSqlQuery *consulta = new QSqlQuery();
                //Si existe la tabla de consulta temporal la elimino para hacer una nueva
                consulta->prepare("DELETE FROM Padre WHERE Cod_Miembro == :cod");
                consulta->bindValue(":cod",Cod_Miembro);
                //qDebug()<<consulta->exec()<<"ELIMINA";
            }
            if (!ui->checkBox_12->isChecked())
            {
                //Lo elimino de la BD
                QSqlQuery *consulta = new QSqlQuery();
                //Si existe la tabla de consulta temporal la elimino para hacer una nueva
                consulta->prepare("DELETE FROM Madre WHERE Cod_Miembro == :cod");
                consulta->bindValue(":cod",Cod_Miembro);
                //qDebug()<<consulta->exec()<<"ELIMINA";
            }

            //Aca de alguna manera me tengo q traer el Cod_Padre antes de guardar en la actualizacion
            Aux.Guardar(MiemAct,Img,Ape,Nom,S,F_N,T_D,Doc,Dom,Num,Pi,D,Tel,Nac,Prov,Loc,Lug_N,C_elec,PadreM,MadreM,F_I,Sac,Est,Trab,R,P,Car,Obs,Prog,ficha,Cod_Miembro);

            QSqlDatabase::database().transaction();

            Aux.actualizar_BD(Cod_Miembro,losdos);

            QSqlDatabase::database().commit();

            //Aca tendria que haber una revision de si hay un error de insersion en la base de datos que muestre un mensaje de error

            //Acomodo la interfaz para que vuelva al estado anterior

            ui->lineEdit->setEnabled(true);
            ui->pushButton_4->setVisible(true);
            ui->pushButton_5->setVisible(true);
            ui->pushButton_8E->setVisible(false);
            ui->pushButton_9E->setVisible(false);

            //limpiarD();
            desactivaD();
            desactivaFM();

            if (ui->checkBox_11->isChecked())
            {
                //limpiarP();
                //Ver esto aca esta para zafarla
                ui->checkBox_11->setChecked(false);
                desactivaP();
                ui->checkBox_11->setEnabled(false);
            }
            if (ui->checkBox_12->isChecked())
            {
                //limpiarM();
                //Ver esto aca esta para zafarla
                ui->checkBox_12->setChecked(false);
                desactivaM();
                ui->checkBox_12->setEnabled(false);
            }
            ui->pushButton_6->setEnabled(false);
            ui->checkBox_11->setEnabled(false);
            ui->checkBox_12->setEnabled(false);

            //Rehago la consulta pra actualizar los datos en la interfaz

            //on_lineEdit_returnPressed();

        }

    }
    else
    {
        //Redirecciono a la pestaña correspondiente
        ui->tabWidget->setCurrentIndex(0); //Pestaña Adm. Miembros
        ui->tabWidget_2->setCurrentIndex(0); //Pestaña Ficha de Datos
        QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al Actualizar el Miembro.\n""Completar correctamente al menos el Nombre, Apellido y Documento del Miembro.",0, QApplication::UnicodeUTF8),QMessageBox::Ok);
    }

}

//Cancela el ingreso de un nuevo miembro
void MainWindow::on_pushButton_7NM_pressed()
{
    desactivaD();
    limpiarD();
    ui->pushButton_6NM->setVisible(false);
    ui->pushButton_7NM->setVisible(false);
    ui->pushButton_4->setVisible(true);
    ui->checkBox_11->setEnabled(false);
    ui->checkBox_12->setEnabled(false);
    ui->checkBox_11->setChecked(false);
    ui->checkBox_12->setChecked(false);
    ui->lineEdit->setEnabled(true);
    ui->pushButton_6->setEnabled(false);
    desactivaP();
    desactivaM();
    limpiarP();
    limpiarM();
    desactivaFM();
    limpiarFM();

}

//Activar o Desactivar Los Campos de los Padres
void MainWindow::on_checkBox_11_pressed()
{
    if (ui->checkBox_11->isChecked())
    {
        desactivaP();
        limpiarP();
        if(ui->checkBox_12->isChecked() == false){ui->pushButton_6->setEnabled(false);}
    }
    else
    {
        activaP();
        ui->pushButton_6->setEnabled(true);

    }
}

void MainWindow::on_checkBox_12_pressed()
{
    if (ui->checkBox_12->isChecked())
    {
        desactivaM();
        limpiarM();
        if(ui->checkBox_11->isChecked() == false){ui->pushButton_6->setEnabled(false);}
    }
    else
    {
        activaM();
        ui->pushButton_6->setEnabled(true);

    }
}

//Escribir en tiempo real el nombre y apellido del padre y de la madre
void MainWindow::on_lineEdit_39_textEdited(QString Ape)
{
    QString Apellido = Ape;
    QString Nombre = ui->lineEdit_40->text();
    escribe_entiemporealP(Nombre,Apellido);
}

void MainWindow::on_lineEdit_40_textEdited(QString Nom)
{
    QString Nombre = Nom;
    QString Apellido = ui->lineEdit_39->text();
    escribe_entiemporealP(Nombre,Apellido);
}

void MainWindow::on_lineEdit_52_textEdited(QString Ape)
{
    QString Apellido = Ape;
    QString Nombre = ui->lineEdit_53->text();
    escribe_entiemporealM(Nombre,Apellido);
}

void MainWindow::on_lineEdit_53_textEdited(QString Nom)
{
    QString Nombre = Nom;
    QString Apellido = ui->lineEdit_52->text();
    escribe_entiemporealM(Nombre,Apellido);
}

//relleno automatico de los campos de los padres en base a los del hijo, reviso que lo escriba en tiempo real en el label tambien
void MainWindow::on_pushButton_6_pressed()
{
    QSqlQuery *consulta = new QSqlQuery();
    //Si existe la tabla de consulta temporal la elimino para hacer una nueva
    consulta->prepare("DELETE FROM PadMa");
    consulta->exec();
    consulta->~QSqlQuery();

    bool result = AuxPer.Llena_siexisten(ui->lineEdit_34->text());
    //Tamanio del Dialogo

    if(result)
    {
        //Aca hago el codigo para buscar los datos de los padres por si se encuentran en la BD - para el caso de Hermanos

        //Muestro el cuadro de dialogo pero primero actualizo la tableview
        RAP->estructura_dialog();
        RAP->exec();
        //En base a la seleccion en la ventana tendria que completar automaticamente los campos de los padres o llenar la clase y destruyo la ventana por la coneccion a la base de datos
        if(!RAP->CodP.isEmpty() or RAP->CodM.isEmpty())
        {
            rellenopamasa(RAP->CodP,RAP->CodM);
        }
        //Desactivo el relleno automatico
        ui->pushButton_6->setEnabled(false);

    }
    else
    {
        //Relleno en Base al Miembro

        QString Vacio = "";
        QString Apellido = ui->lineEdit_34->text();

        if (ui->checkBox_11->isChecked() and ui->checkBox_12->isChecked())
        {
            //Padre


            ui->lineEdit_39->setText(ui->lineEdit_34->text()); //Ape
            ui->lineEdit_14->setText(ui->lineEdit_5->text()); //Direccion
            ui->lineEdit_37->setText(ui->lineEdit_36->text());//Numero
            ui->lineEdit_17->setText(ui->lineEdit_6->text());//Piso
            ui->lineEdit_18->setText(ui->lineEdit_7->text());//Dpto
            ui->lineEdit_19->setText(ui->lineEdit_8->text());//tel
            escribe_entiemporealP(Vacio,Apellido);
            //Madre

            ui->lineEdit_51->setText(ui->lineEdit_5->text()); //Direccion
            ui->lineEdit_48->setText(ui->lineEdit_36->text());//Numero
            ui->lineEdit_49->setText(ui->lineEdit_6->text());//Piso
            ui->lineEdit_50->setText(ui->lineEdit_7->text());//Dpto
            ui->lineEdit_41->setText(ui->lineEdit_8->text());//tel
            escribe_entiemporealM(Vacio,Vacio);
       }
       else
       {
           if(ui->checkBox_11->isChecked())
           {
               //Padre

               ui->lineEdit_39->setText(ui->lineEdit_34->text()); //Ape
               ui->lineEdit_14->setText(ui->lineEdit_5->text()); //Direccion
               ui->lineEdit_37->setText(ui->lineEdit_36->text());//Numero
               ui->lineEdit_17->setText(ui->lineEdit_6->text());//Piso
               ui->lineEdit_18->setText(ui->lineEdit_7->text());//Dpto
               ui->lineEdit_19->setText(ui->lineEdit_8->text());//tel
               escribe_entiemporealP(Vacio,Apellido);
           }
           else
           {
               if (ui->checkBox_12->isChecked())
               {
                   //Madre

                   ui->lineEdit_52->setText(ui->lineEdit_34->text()); //Ape
                   ui->lineEdit_51->setText(ui->lineEdit_5->text()); //Direccion
                   ui->lineEdit_48->setText(ui->lineEdit_36->text());//Numero
                   ui->lineEdit_49->setText(ui->lineEdit_6->text());//Piso
                   ui->lineEdit_50->setText(ui->lineEdit_7->text());//Dpto
                   ui->lineEdit_41->setText(ui->lineEdit_8->text());//tel
                   escribe_entiemporealM(Vacio,Apellido);
               }
           }
       }
    }

}

//Aca actualizo en tiempo real el tablemodel para que la busqueda reconozca los nuevos miembros a medida que se agregan y configuro la palabra filtro para la busqueda en la bd
void MainWindow::on_lineEdit_textEdited(QString)
{
    model->setQuery("select Apellido,Nombre,Rama,Activo,Cod_Miembro FROM Miembro");
    if(ui->radioButton_12->isChecked()){mCompleter->setCompletionColumn(0);}
    if(ui->radioButton_13->isChecked()){mCompleter->setCompletionColumn(1);}
    if(ui->radioButton_14->isChecked()){mCompleter->setCompletionColumn(2);}
}

//Boton de editar datos del miembro presionado
void MainWindow::on_pushButton_5_pressed()
{
    //seteo la interfaz
    ui->lineEdit->setEnabled(false );
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);
    ui->pushButton_8E->setVisible(true);
    ui->pushButton_9E->setVisible(true);
    ui->checkBox_11->setEnabled(true);
    ui->checkBox_12->setEnabled(true);

    activaD();
    activaFM(); // Activo Ficha_Medica

    //Ahora activo los campos de los padres segun esten en la BD, pero tengo que dar la posibilidad de cargar cualquiera de los faltantes
    if (ui->checkBox_11->isChecked())
    {
        activaP();
        ui->checkBox_11->setEnabled(true);
    }
    if (ui->checkBox_12->isChecked())
    {
        activaM();
        ui->checkBox_12->setEnabled(true);
    }

}

//Cancelar edicion de Miembro
void MainWindow::on_pushButton_8E_pressed()
{
    //seteo la interfaz
    ui->lineEdit->setEnabled(true);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setVisible(true);
    ui->pushButton_8E->setVisible(false);
    ui->pushButton_9E->setVisible(false);

    desactivaD();
    desactivaFM();

    if (ui->checkBox_11->isChecked())
    {
        desactivaP();
        ui->checkBox_11->setEnabled(false);
    }
    if (ui->checkBox_12->isChecked())
    {
        desactivaM();
        ui->checkBox_12->setEnabled(false);
    }
    ui->pushButton_6->setEnabled(false);
    ui->checkBox_11->setEnabled(false);
    ui->checkBox_12->setEnabled(false);

    //Tengo que volver atras, es decir mostrar los datos iniciales - Es hacer lo mismo que cuando toco enter

    on_lineEdit_returnPressed();


}


//----------------------------------SECCION MATERIALES--------------------------------------

//Material - Asigno el modelo a la tableview en materiales
void MainWindow::estructura_material()
{

    //cargo la tabla con la info de la BD para material.
    mat = new QSqlTableModel();

    mat->setTable("Material");

    if(!ui->checkBox_15->isChecked())
    {
        mat->setFilter("estado IS NOT 'Dado de Baja'");
    }

    mat->select();

    //verifico que no le permite imprimir si esta vacio.
    if(mat->rowCount() == 0)
    {
        ui->pushButton_3->setEnabled(false);
    }

    mat->setHeaderData(0, Qt::Horizontal, tr("Codigo de Material"));
    mat->setHeaderData(1, Qt::Horizontal, tr("Fecha"));
    mat->setHeaderData(2, Qt::Horizontal, trUtf8("Descripción"));
    mat->setHeaderData(3, Qt::Horizontal, trUtf8("Categoría"));
    mat->setHeaderData(4, Qt::Horizontal, tr("Estado"));
    mat->setHeaderData(5, Qt::Horizontal, tr("Observaciones"));

    //Asigno al tableview el model.
    ui->tabla_material->setModel(mat);
    ui->tabla_material->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tabla_material->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tabla_material->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tabla_material->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter);


    ui->radioButton_5->setChecked(true); //chequeo por default el Filtro Descripcion para poder utilizardo como parametro de comparacion en la busqueda

    ui->dateEdit_5->setDate(QDate::currentDate());

}

//...........Funciones auxiliares.................................

// Material - Para simplificar la limpieza y otras yerbas de Materiales
void MainWindow::limpiarMAT()
{
    ui->lineEdit_4->clear();
    ui->lineEdit_26->clear();
    ui->lineEdit_25->clear();
    ui->spinBox->setValue(1);
    edicion_Material = 0; //Actualizo el valor de Cod_MAT por si hubo una edicion previa.
}

void MainWindow::activarMAT()
{
    ui->pushButton_7->setEnabled(true); //guardar
    ui->pushButton_8->setEnabled(true); //cancelar
    ui->lineEdit_26->setEnabled(true); // descripcion
    ui->lineEdit_25->setEnabled(true); //observaciones
    ui->spinBox->setEnabled(true); //cantidad
    ui->comboBox_6->setEnabled(true); //categoria
    ui->radioButton_3->setEnabled(true); //estado
    ui->radioButton_3->setChecked(true); //estado - chequeo por default el estado del material en "bien"
    ui->radioButton_4->setEnabled(true); //estado -reparar
    ui->dateEdit_5->setEnabled(true); //fecha ingreso

    //label 36,37,40,41,42,43
    ui->label_36->setEnabled(true);
    ui->label_37->setEnabled(true);
    ui->label_40->setEnabled(true);
    ui->label_41->setEnabled(true);
    ui->label_42->setEnabled(true);
    ui->label_43->setEnabled(true);

}

void MainWindow::desactivarMAT()
{
    ui->pushButton_7->setEnabled(false); //guardar
    ui->pushButton_8->setEnabled(false); //cancelar
    ui->pushButton_9->setEnabled(false); //boton borrar
    ui->lineEdit_26->setEnabled(false); // descripcion
    ui->lineEdit_25->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->comboBox_6->setEnabled(false);
    ui->radioButton_3->setEnabled(false);
    ui->radioButton_4->setEnabled(false);
    ui->dateEdit_5->setEnabled(false);
    ui->label_36->setEnabled(false);
    ui->label_37->setEnabled(false);
    ui->label_40->setEnabled(false);
    ui->label_41->setEnabled(false);
    ui->label_42->setEnabled(false);
    ui->label_43->setEnabled(false);

}

//..........Slots..................................................

//Material - Boton nuevo Material
void MainWindow::on_pushButton_2_pressed()
{
    //limpio la interfaz y luego activo los campos para el ingreso
    limpiarMAT();
    activarMAT();
    ui->pushButton_2->setEnabled(false); //boton nuevo_miembro.
    ui->pushButton_9->setEnabled(false); //boton borrar.
    ui->pushButton_3->setEnabled(false); //boton imprimir.

    ui->lineEdit_26->setFocus();

    //Desactivo la tabla
    ui->tabla_material->setEnabled(false);

}

//Material - Boton Guardar en Material
void MainWindow::on_pushButton_7_pressed()
{
    if(!ui->lineEdit_26->text().isEmpty()){

        QString Descripcion,observaciones,categoria,estado;
        QDate Fecha_ingreso;
        int cantidad;
        Material MT;

        //Cargo los valores rellenados en la interfaz en las variables

        Descripcion = ui->lineEdit_26->text();
        observaciones = ui->lineEdit_25->text();
        cantidad = ui->spinBox->value();
        int cat = ui->comboBox_6->currentIndex();
        categoria = ui->comboBox_6->itemText(cat);

        if(ui->comboBox_6->itemText(cat).isNull())
        {
            categoria = "Ninguna";
        }
        else
        {
            categoria = ui->comboBox_6->itemText(cat);
        }
        Fecha_ingreso =ui->dateEdit_5->date();

        if(ui->radioButton_3->isChecked())
        {
            estado = "bien";
        }
        else
        {
            estado = "reparar";
        }

        if(edicion_Material !=1)
        {

            QSqlRecord ingresar = mat->record();

            QSqlField c1("Cod_MAT",QVariant::String);
            QSqlField c2("fecha_ingreso",QVariant::Date);
            QSqlField c3("descripcion",QVariant::String);
            QSqlField c4("categoria",QVariant::String);
            QSqlField c5("estado",QVariant::String);
            QSqlField c6("observaciones",QVariant::String);

            c2.setValue(QVariant(Fecha_ingreso));
            c3.setValue(QVariant(Descripcion));
            c4.setValue(QVariant(categoria));
            c5.setValue(QVariant(estado));
            c6.setValue(QVariant(observaciones));

            QString cod_mat;

            //activo el transaction de la BD para comenzar a insertar en el modelo.
            QSqlDatabase::database().transaction();

            //El For se hace de acuerdo a la cantidad de items iguales que se introducen simultaneamente
            for(int i=0;i<cantidad;i++)
            {
                //Genero el Cod_Material como identificativo

                ConectarconBD::cod_setup("Material",cod_mat);
                c1.setValue(QVariant(cod_mat));

                ingresar.append(c1);
                ingresar.append(c2);
                ingresar.append(c3);
                ingresar.append(c4);
                ingresar.append(c5);
                ingresar.append(c6);

                mat->insertRecord(-1,ingresar);

                //Vuelvo a filtrar
                ui->lineEdit_4->clear();
                mat->setFilter("");
                if(!ui->checkBox_15->isChecked())
                {
                    mat->setFilter("estado IS NOT 'Dado de Baja'");
                }

                mat->select();

                //Scrolldown
                while(mat->canFetchMore()){mat->fetchMore();}
                ui->tabla_material->scrollToBottom();

            }

            if(!QSqlDatabase::database().commit()){

               QMessageBox::critical(this,"Error!","No se a podido insertar los Datos correctamente.""\n Por favor intente nuevamente.");
            }

        }

        else
        {
            //qDebug()<<"Entro Edicion_Material Mainwindows.cpp";
            MT.Guardar(Cod_MAT,Descripcion,categoria,Fecha_ingreso,estado,observaciones);

            QSqlDatabase::database().transaction();

            MT.actualizar_BD_Materiales(Cod_MAT);

            QSqlDatabase::database().commit();

            //Vuelvo a filtrar
            ui->lineEdit_4->clear();
            mat->setFilter("");
            if(!ui->checkBox_15->isChecked())
            {
                mat->setFilter("estado IS NOT 'Dado de Baja'");
            }

            mat->select();

        }

        //Limpio y Desactivo luego de Guardar en la BD
        limpiarMAT();
        desactivarMAT();
        ui->pushButton_2->setEnabled(true); //boton nuevo_miembro.
        ui->pushButton_9->setEnabled(true); //boton borrar.
        ui->pushButton_3->setEnabled(true); //boton imprimir.

        //Activo la tabla
        ui->tabla_material->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"Cuidado!","Complete al menos el campo Descripción.",QMessageBox::Ok);
        ui->lineEdit_26->setFocus();
    }

}

//Material  - Boton cancelar en Material
void MainWindow::on_pushButton_8_pressed()
{
    limpiarMAT();
    desactivarMAT();
    ui->pushButton_2->setEnabled(true); //boton nuevo_miembro.
    ui->pushButton_9->setEnabled(true); //boton borrar.
    ui->pushButton_3->setEnabled(true); //boton imprimir.

    //Activo la tabla
    ui->tabla_material->setEnabled(true);
}

//Material  - Boton borrar en Material
void MainWindow::on_pushButton_9_pressed()
{
    //creo el boton No.
    QPushButton * botonNO = new QPushButton(tr("No"));
    botonNO->setCheckable(true);

    //Creo el boton Si.
    QPushButton * botonSI = new QPushButton(tr("Si"));
    botonSI->setDefault(false);
    botonSI->setAutoDefault(true);

    //Creo el nuevo QMessageBox.
    QMessageBox * boton = new QMessageBox();
    boton->setWindowTitle("Cuidado");
    boton->setText("¿Esta seguro que desea eliminar este elemento?");

    //Asigno los iconos correspondientes
    boton->setWindowIcon(QPixmap(":/icons/gags"));
    boton->setIcon(QMessageBox::Warning);

    //Seteo los botones Si y No en el QMessageBox
    boton->addButton(botonSI,QMessageBox::YesRole);
    boton->addButton(botonNO,QMessageBox::NoRole);

    //Ejecuto el QMessageBox Creado.
    boton->exec();

    //La función clickedButton me devuelve un AbstractButton, de manera tal de poder comparar el botón presionado directamente con el botón de referencia. En este caso BotonSI

    if(boton->clickedButton() == botonSI)
    {
        while(mat->canFetchMore()){mat->fetchMore();}
        //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
        QItemSelection selection( ui->tabla_material->selectionModel()->selection());

        //Genero un array como todos los indices de los elementos seleccionados
        QModelIndexList List = selection.indexes();

        QSqlQuery *aux = new QSqlQuery();
        aux->prepare("UPDATE Material SET estado = 'Dado de Baja' WHERE Cod_MAT = :cod");

        QSqlDatabase::database().transaction(); //Comienzo la transacción.

        while(!List.isEmpty())
        {
            aux->bindValue(":cod",List.first().sibling(List.first().row(),0).data().toString());

            aux->exec();
            List.pop_front();
        }

        QSqlDatabase::database().commit(); //Commiteo la transacción.

        aux->~QSqlQuery();
        //Vuelvo a filtrar
        on_lineEdit_4_returnPressed();


    }

    //Se presiono el botón No.
    ui->pushButton_9->setEnabled(false); //desactivo el boton luego de un borrado.
    boton->~QMessageBox();

}

// MATERIAL - activar boton borrar
void MainWindow::on_tabla_material_clicked(const QModelIndex &)
{
    ui->pushButton_9->setEnabled(true);
}

//Material - Boton Imprimir para el listado
void MainWindow::on_pushButton_3_pressed()
{
    // Aca hay que hacer el metodo para imprimir el listado asignado a tableview, pero con lo copante de imprimir solo el listado filtrado en el lineEdit de Busqueda.
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tabla_material->model()->rowCount();
    const int columnCount = ui->tabla_material->model()->columnCount();

    out <<"<h3>Listado de Materiales - "+ QDate::currentDate().toString() +"</h3><br>";
    out <<"<table border=2 cellspacing=0 cellpadding=2>";

    // headers
    out << "<tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!ui->tabla_material->isColumnHidden(column))
            out << QString("<th align=center >%1</th>").arg(ui->tabla_material->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr>";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->tabla_material->isColumnHidden(column)) {
                QString data = ui->tabla_material->model()->data(ui->tabla_material->model()->index(row, column)).toString().simplified();
                if(column == 1 or column==0)
                {
                    out << QString("<td align=center width=11% bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
                if(column == 2 or column==5)
                {
                    out << QString("<td align=center width=30% bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
                if(column == 3 or column==4)
                {
                    out << QString("<td align=left width=9% bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }

            }
        }
        out << "</tr>";
    }
    out <<  "</table>";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;


}

//Material - LineEdit_4 filtro del buscador
void MainWindow::on_lineEdit_4_returnPressed()
{
    while(mat->canFetchMore()){mat->fetchMore();}
    QString Filtro;
    if(ui->checkBox_15->isChecked())
    {
        if(ui->radioButton_5->isChecked()){Filtro = "descripcion LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_6->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_7->isChecked()){Filtro = "estado LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_8->isChecked()){Filtro = "Cod_MAT LIKE '%" + ui->lineEdit_4->text() + "%'";}
    }
    else
    {
        Filtro = "estado IS NOT 'Dado de Baja'";
        if(ui->radioButton_5->isChecked()){Filtro = "descripcion LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_6->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_7->isChecked()){Filtro = "estado LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_8->isChecked()){Filtro = "Cod_MAT LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}

    }

    mat->setFilter(Filtro);
    mat->select();

    ui->lineEdit_4->clear();

}

//Material - LineEdit_4 filtro del buscador al escribir
void MainWindow::on_lineEdit_4_textEdited(QString)
{
    while(mat->canFetchMore()){mat->fetchMore();}
    QString Filtro;
    if(ui->checkBox_15->isChecked())
    {
        if(ui->radioButton_5->isChecked()){Filtro = "descripcion LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_6->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_7->isChecked()){Filtro = "estado LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_8->isChecked()){Filtro = "Cod_MAT LIKE '%" + ui->lineEdit_4->text() + "%'";}
    }
    else
    {
        Filtro = "estado IS NOT 'Dado de Baja'";
        if(ui->radioButton_5->isChecked()){Filtro = "descripcion LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_6->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_7->isChecked()){Filtro = "estado LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_8->isChecked()){Filtro = "Cod_MAT LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}

    }

    mat->setFilter(Filtro);
    mat->select();
}

//Material - Doubleclick edition mode
void MainWindow::on_tabla_material_doubleClicked(QModelIndex index)
{
    limpiarMAT();
    activarMAT();
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->spinBox->setEnabled(false); // Cantidad
    ui->label_37->setEnabled(false); // Label_Cantidad

    int indice = index.row();

    ui->lineEdit_26->setText(index.sibling(indice,2).data().toString()); //Descripccion

    ui->lineEdit_25->setText(index.sibling(indice,5).data().toString()); //Observaciones

    ui->dateEdit_5->setDate(index.sibling(indice,1).data().toDate()); // Dia

    int ind = ui->comboBox_6->findText(index.sibling(indice,3).data().toString());

    ui->comboBox_8->setCurrentIndex(ind); //Categoria

    if(index.sibling(indice,4).data().toString()=="bien")
    {
        ui->radioButton_3->setChecked(true);
    }
    else
    {
        ui->radioButton_4->setChecked(true);
    }

    Cod_MAT = index.sibling(indice,0).data().toString(); //Cod_MAT

    edicion_Material = 1;

    //Desactivo la tabla
    ui->tabla_material->setEnabled(false);
}

//Material - Dados de baja
void MainWindow::on_checkBox_15_pressed()
{
    while(mat->canFetchMore()){mat->fetchMore();}
    QString Filtro;
    if(!ui->checkBox_15->isChecked())
    {
        if(ui->radioButton_5->isChecked()){Filtro = "descripcion LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_6->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_7->isChecked()){Filtro = "estado LIKE '%" + ui->lineEdit_4->text() + "%'";}
        if(ui->radioButton_8->isChecked()){Filtro = "Cod_MAT LIKE '%" + ui->lineEdit_4->text() + "%'";}
    }
    else
    {
        Filtro = "estado IS NOT 'Dado de Baja'";
        if(ui->radioButton_5->isChecked()){Filtro = "descripcion LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_6->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_7->isChecked()){Filtro = "estado LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}
        if(ui->radioButton_8->isChecked()){Filtro = "Cod_MAT LIKE '%" + ui->lineEdit_4->text() + "%' and estado IS NOT 'Dado de Baja'";}

    }

    mat->setFilter(Filtro);
    mat->select();
}

//----------------------------------SECCION CONTABLE GRUPO----------------------------------

//CONTABLE_GRUPO - //estructura contable para el constructor de la UI
void MainWindow::estructura_contable_grupo()
{
    CGR = new QSqlTableModel();
    CGR->setTable("Contable_Grupo");
    CGR->select();

    CGR->setHeaderData(1, Qt::Horizontal, trUtf8("Descripción"));
    CGR->setHeaderData(2, Qt::Horizontal, tr("Motivo"));
    CGR->setHeaderData(3, Qt::Horizontal, tr("Ingreso"));
    CGR->setHeaderData(4, Qt::Horizontal, tr("Egreso"));
    CGR->setHeaderData(5, Qt::Horizontal, tr("Fecha"));
    CGR->setHeaderData(6, Qt::Horizontal, trUtf8("Categoría"));

    ui->contable_grupo->setModel(CGR);
    ui->contable_grupo->hideColumn(0);
    ui->contable_grupo->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->contable_grupo->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->contable_grupo->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);

    ui->radioButton_9->setChecked(true);

    ui->dateEdit_8->setDate(QDate::currentDate()); //fecha nuevo_ingreso

    ui->contable_grupo->hideColumn(7);
    ui->contable_grupo->hideColumn(8); //Cod_Mie
    ui->contable_grupo->hideColumn(12); //cantidad
    ui->contable_grupo->hideColumn(13); //CON_evento
    ui->contable_grupo->hideColumn(14); //debe_miembro
    ui->contable_grupo->hideColumn(15); //gan_mie
    ui->contable_grupo->hideColumn(16); //gan_grup

    actualizar_resumen_CG();

    limpiarCGR();
    desactivarCGR();

}

//...........Funciones auxiliares.................................

//CONTABLE_GRUPO - //limpiar
void MainWindow::limpiarCGR()
{
    ui->lineEdit_29->clear();//motivo
    ui->comboBox_8->setCurrentIndex(0);//tipo de motivo
    ui->doubleSpinBox->setValue(0); //monto
    ui->lineEdit_28->clear();
    ui->dateEdit_8->setDate(QDate::currentDate());
}

//CONTABLE_GRUPO - //activar
void MainWindow::activarCGR()
{
    ui->pushButton_11->setEnabled(true);// guardar
    ui->pushButton_12->setEnabled(true); //  cancelar

    ui->label_47->setEnabled(true);// label motivo
    ui->lineEdit_29->setEnabled(true);//motivo

    ui->label_80->setEnabled(true);//label tipo_motivo
    ui->comboBox_8->setEnabled(true);//tipo de motivo

    ui->label_79->setEnabled(true);//label monto
    ui->doubleSpinBox->setEnabled(true); //monto

    ui->label_77->setEnabled(true);//label fecha ingreso
    ui->dateEdit_8->setEnabled(true);// fecha ingreso
}

//CONTABLE_GRUPO - //desactivar
void MainWindow::desactivarCGR()
{
    ui->pushButton_11->setEnabled(false);// guardar
    ui->pushButton_12->setEnabled(false); //  cancelar

    ui->label_47->setEnabled(false);// label motivo
    ui->lineEdit_29->setEnabled(false);//motivo

    ui->label_80->setEnabled(false);//label tipo_motivo
    ui->comboBox_8->setEnabled(false);//tipo de motivo

    ui->label_79->setEnabled(false);//label monto
    ui->doubleSpinBox->setEnabled(false); //monto

    ui->label_77->setEnabled(false);//label fecha ingreso
    ui->dateEdit_8->setEnabled(false);// fecha ingreso
}

//CONTABLE_GRUPO - // Resumen haber y deber
void MainWindow::actualizar_resumen_CG()
{
    double haber = 0;
    double deber = 0;

    for( int i = 0;i <= CGR->rowCount();i++)
    {
        haber +=CGR->index(i,3).data().toDouble();
        deber += CGR->index(i,4).data().toDouble();
    }

    ui->label_55->setText("$"+QString::number(haber));
    ui->label_78->setText("$"+QString::number(deber));
    ui->label_111->setText("$"+QString::number(haber - deber));

}

//..........Slots..................................................

//CONTABLE_GRUPO - //nuevo ingreso
void MainWindow::on_pushButton_10_pressed()
{
   //nuevo ingreso
   activarCGR();
   limpiarCGR();
   ui->pushButton_10->setEnabled(false);// boton nuevo_ingreso.
   ui->pushButton_13->setEnabled(false);// boton borrar.
   ui->pushButton_14->setEnabled(false);// boton imprimir.
   ui->lineEdit_29->setFocus();

   //Desactivo la tabla
   ui->contable_grupo->setEnabled(false);
}

//CONTABLE_GRUPO - //guardar
void MainWindow::on_pushButton_11_pressed()
{
    //guardar

    if(!ui->lineEdit_29->text().isEmpty()){

    if(edicion_contable != 1)
    {

        QString Cod_CON,motivo,tipo_motivo,categoria,Cod_Miembro,Rama,Nombre,Apellido,cod_evento;
        QDate fecha_ingreso;
        double Ingreso,Egreso;
        int cantidad;

        ConectarconBD::cod_setup("Contable_Grupo",Cod_CON);

        motivo = ui->lineEdit_29->text();
        int indice = ui->comboBox_8->currentIndex();
        tipo_motivo =ui->comboBox_8->itemText(indice);

        if(tipo_motivo == "Ingreso")
        {
            Ingreso =ui->doubleSpinBox->text().toDouble();
            Egreso = 0;
        }
        else
        {
            Egreso =ui->doubleSpinBox->text().toDouble();
            Ingreso = 0;
        }

        fecha_ingreso = ui->dateEdit_8->date();
        categoria = "General";

        Cod_Miembro = " ";Rama = " "; Nombre = " ";Apellido = " "; cod_evento = " ";
        cantidad = 0;

        contable.guardar(Cod_CON,motivo,tipo_motivo,categoria,fecha_ingreso,Cod_Miembro,QString::null,Rama,Apellido,Nombre,Ingreso,Egreso,cantidad,QString::null,0,0,0);

        QSqlDatabase::database().transaction();

        contable.almacenar_BD();

        //comiteo el ingreso a la BD.
        QSqlDatabase::database().commit();

        //Scrolldown
        while(CGR->canFetchMore()){CGR->fetchMore();}
        ui->contable_grupo->scrollToBottom();

    }
    else
    {

        double monto;
        if(ui->comboBox_8->currentText() == "Ingreso")
        {
            monto = ui->doubleSpinBox->value();
            contable.guardar(Cod_Contable,ui->lineEdit_29->text(),ui->comboBox_8->currentText(),"no",ui->dateEdit_8->date(),"no","no","no","no","no",monto,0,0,"no",0,0,0);

        }
        else
        {
            monto = ui->doubleSpinBox->value();
            contable.guardar(Cod_Contable,ui->lineEdit_29->text(),ui->comboBox_8->currentText(),"no",ui->dateEdit_8->date(),"no","no","no","no","no",0,monto,0,"no",0,0,0);
        }

        QSqlDatabase::database().transaction();

        contable.actualizar_BD_Contable_Grupo();

        //comiteo la actualización.
        QSqlDatabase::database().commit();

        edicion_contable=0;

        //Activo el botón imprimir y nuevo_ingreso nuevamente luego de la edición.
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
    }


    limpiarCGR();
    desactivarCGR();
    ui->pushButton_10->setEnabled(true);// boton nuevo_ingreso.
    ui->pushButton_14->setEnabled(true);// boton imprimir.
    ui->pushButton_13->setEnabled(false);// boton nuevo_ingreso.


    CGR->select();
    cum->select();

    //actualizo el resumen luego de realizar un nuevo ingreso o bien editar un item
    actualizar_resumen_CG();

    //Activo la tabla
    ui->contable_grupo->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"Cuidado!","Complete al menos el campo Motivo.",QMessageBox::Ok);
        ui->lineEdit_29->setFocus();
    }

}

//CONTABLE_GRUPO - //cancelar
void MainWindow::on_pushButton_12_pressed()
{
    limpiarCGR();
    desactivarCGR();
    ui->pushButton_10->setEnabled(true);// boton nuevo_ingreso.
    ui->pushButton_13->setEnabled(false);// boton borrar.
    ui->pushButton_14->setEnabled(true);// boton nuevo_ingreso.

    //Activo la tabla
    ui->contable_grupo->setEnabled(true);

 }

//CONTABLE_GRUPO - //borrar
void MainWindow::on_pushButton_13_pressed()
{

    if(!ui->contable_grupo->currentIndex().sibling(ui->contable_grupo->currentIndex().row(),8).data().toString().isEmpty())
    {

        limpiarCGR();
        desactivarCGR();

        QMessageBox::warning(this,"Cuidado","No se puede borrar este elemento en esta sección.""\n Para hacerlo debe dirigirse a las configuraciones.");
    }
    else
    {
        if(ui->contable_grupo->currentIndex().sibling(ui->contable_grupo->currentIndex().row(),6).data().toString() == "General")
        {
            //borrar

            //creo el boton No.
            QPushButton * botonNO = new QPushButton(tr("No"));
            botonNO->setCheckable(true);

            //Creo el boton Si.
            QPushButton * botonSI = new QPushButton(tr("Si"));
            botonSI->setDefault(false);
            botonSI->setAutoDefault(true);

            //Creo el nuevo QMessageBox.
            QMessageBox * boton = new QMessageBox();
            boton->setWindowTitle("Cuidado");
            boton->setText("¿Esta seguro que desea eliminar este/os elemento/s?");

            //Asigno los iconos correspondientes
            boton->setWindowIcon(QPixmap(":/icons/gags"));
            boton->setIcon(QMessageBox::Warning);

            //Seteo los botones Si y No en el QMessageBox
            boton->addButton(botonSI,QMessageBox::YesRole);
            boton->addButton(botonNO,QMessageBox::NoRole);

            //Ejecuto el QMessageBox Creado.
            boton->exec();

            if(boton->clickedButton() == botonSI)
            {
                //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
                QItemSelection selection( ui->contable_grupo->selectionModel()->selection());

                //Genero un array como todos los indices de los elementos seleccionados
                QList<int> rows;
                foreach( const QModelIndex & index, selection.indexes() ) {
                    rows.append(index.row());
                }

                qSort(rows);


                //elimino
                QSqlDatabase::database().transaction();

                int prev = -1;
                for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
                    int current = rows[i];
                    if( current != prev )
                    {
                        CGR->removeRows(current,1);
                        prev = current;
                    }
                }

                QSqlDatabase::database().commit();

                //Actualizo los resumenes y el modelo
                actualizar_resumen_CG();

                CGR->select();               
            }

            boton->~QMessageBox();

        }
        else
        {
            limpiarCGR();
            desactivarCGR();

            QMessageBox::information(this,"Cuidado","No se puede quitar este elemento en esta sección.""\nPara hacerlo debe ir a la ficha contable del miembro.");
        }
    }

    ui->pushButton_13->setEnabled(false);

}

//CONTABLE_GRUPO - //imprimir
void MainWindow::on_pushButton_14_pressed()
{
    //imprimir

    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->contable_grupo->model()->rowCount();
    const int columnCount = ui->contable_grupo->model()->columnCount();     

    out <<"<h3>Balance de Grupo: - " + QDate::currentDate().toString() +"</h3><br>";
    out <<"<table border=2 cellspacing=0 cellpadding=2>";

    // headers
    out << "<tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++){
        if (!ui->contable_grupo->isColumnHidden(column))
        {

            if(column == 2 or column == 3 or column == 4 or column == 5 or column == 6 or column == 9)
            {
                //Con esta asignacion de 8.5% les alcanza y les sobra para estos campos.
                out << QString("<th width=8.5% align=center%>%1</th>").arg(ui->contable_grupo->model()->headerData(column, Qt::Horizontal).toString());
            }

            if(column ==10 or column ==11)
            {
                //Le asigno al menos 12% al nombre y apellido para que se vean bien en la impresion de la ficha.
                out << QString("<th width=12% align=center%>%1</th>").arg(ui->contable_grupo->model()->headerData(column, Qt::Horizontal).toString());

            }
            if(column ==1)
            {
                //Le asigno un 25% a la descripcion ya que es el texto mas largo de la ficha.
                out << QString("<th width=25% align=center%>%1</th>").arg(ui->contable_grupo->model()->headerData(column, Qt::Horizontal).toString());

            }
        }
    }


    out << "</tr>";

    // data table

    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->contable_grupo->isColumnHidden(column)) {

                QString data = ui->contable_grupo->model()->data(ui->contable_grupo->model()->index(row, column)).toString().simplified();
                if( column== 1 or column == 2 or column == 6 or column == 9 or column ==10 or column ==11)
                {
                    //aliniacion izquierda.
                    out << QString("<td align=left bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }            
                if(column == 5)
                {
                    //aliniacion center.
                    out << QString("<td align=center bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }

                if(column == 3 or column == 4)
                {
                    //aliniacion izquierda.
                    out << QString("<td align=right bkcolor=0>$%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }

            }
        }
        out << "</tr>";
    }

    out <<  "</table>";

    out << "<br>";

    out<<"<table align=right cellpadding=2>";
    out<<"<tr>";
    out<<"<td align=left colspan=2><b>Haber:<b> " + ui->label_55->text() + " </td>";
    out<<"<td align=left colspan=2><b>Deber:<b> " + ui->label_78->text() + " </td>";
    out<<"<td align=left colspan=2><b>Neto:<b> " + ui->label_111->text() + " </td>";
    out<< "</tr>";
    out<< "</table><br>";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;

}

//CONTABLE_GRUPO - // filtro al presionar enter
void MainWindow::on_lineEdit_28_returnPressed()
{
    if(ui->lineEdit_28->text() != NULL)
    {
        QDate auxDesde,auxHasta;
        QString Desde,Hasta;
        auxDesde = ui->dateEdit_6->date();
        auxHasta = ui->dateEdit_7->date();
        Desde = auxDesde.toString("yyyy-MM-dd");
        Hasta = auxHasta.toString("yyyy-MM-dd");

        QString Filtro;
        if(ui->radioButton_9->isChecked()){Filtro = "motivo LIKE '%" + ui->lineEdit_28->text() + "%'" + "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
        if(ui->radioButton_10->isChecked()){Filtro = "tipo_motivo LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
        if(ui->radioButton_11->isChecked()){Filtro = "monto LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
        if(ui->radioButton_16->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
        if(ui->radioButton_15->isChecked()){Filtro = "rama LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
        if(ui->radioButton_18->isChecked()){Filtro = "apellido LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}

        CGR->setFilter(Filtro);
        CGR->select();

        actualizar_resumen_CG();
    }

}

//CONTABLE_GRUPO - // filtro inline
void MainWindow::on_lineEdit_28_textEdited(QString)
{
    QDate auxDesde,auxHasta;
    QString Desde,Hasta;
    auxDesde = ui->dateEdit_6->date();
    auxHasta = ui->dateEdit_7->date();
    Desde = auxDesde.toString("yyyy-MM-dd");
    Hasta = auxHasta.toString("yyyy-MM-dd");

    QString Filtro;
    if(ui->radioButton_9->isChecked()){Filtro = "motivo LIKE '%" + ui->lineEdit_28->text() + "%'" + "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
    if(ui->radioButton_10->isChecked()){Filtro = "tipo_motivo LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
    if(ui->radioButton_11->isChecked()){Filtro = "monto LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
    if(ui->radioButton_16->isChecked()){Filtro = "categoria LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
    if(ui->radioButton_15->isChecked()){Filtro = "rama LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}
    if(ui->radioButton_18->isChecked()){Filtro = "apellido LIKE '%" + ui->lineEdit_28->text() + "%'" "and fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";}

    CGR->setFilter(Filtro);
    CGR->select();

    actualizar_resumen_CG();

}

//CONTABLE_GRUPO - // Cambio Fecha Desde...
void MainWindow::on_dateEdit_6_dateChanged(QDate date) //aclaracion El date que nos provee la funcion es solo de la fecha que cambiamos y no de las 2
{

    if(date <= ui->dateEdit_7->date()){

        QString Desde = date.toString("yyyy-MM-dd");
        QString Hasta = ui->dateEdit_7->date().toString("yyyy-MM-dd");

        QString Filtro = "fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";
        CGR->setFilter(Filtro);
        CGR->select();

        actualizar_resumen_CG();

    }

    else
    {
        ui->dateEdit_6->clear();
        QMessageBox::information(this,"Cuidado!","Fecha Desde no valida. Ingrese una nueva fecha.",QMessageBox::Ok);
    }


}

//CONTABLE_GRUPO - // Cambio Fecha Hasta...
void MainWindow::on_dateEdit_7_dateChanged(QDate date)
{
    if(date >= ui->dateEdit_6->date())
    {
    QString Hasta = date.toString("yyyy-MM-dd");
    QString Desde = ui->dateEdit_6->date().toString("yyyy-MM-dd");

    QString Filtro = "fecha_ingreso BETWEEN '" + Desde + "' and '" + Hasta + "'";
    CGR->setFilter(Filtro);
    CGR->select();

    actualizar_resumen_CG();

    }
    else
    {
        ui->dateEdit_7->clear();
        QMessageBox::information(this,"Cuidado!","Fecha Hasta no valida. Ingrese una nueva fecha.",QMessageBox::Ok);
    }
}

//CONTABLE_GRUPO - // Activar - Desactivar Filtro Fechas
void MainWindow::on_checkBox_14_pressed()
{
    if(ui->checkBox_14->isChecked())
    {
        ui->checkBox_14->setText("Activar");
        ui->dateEdit_6->setEnabled(false);
        ui->label_48->setEnabled(false);
        ui->dateEdit_7->setEnabled(false);
        ui->label_49->setEnabled(false);

        //Actualizo el filtro para que no utilize el anterior en la actualizacion del modelo
        QString Filtro = "fecha_ingreso";
        CGR->setFilter(Filtro);
        CGR->select(); //Hago un nuevo select sin el filtro ya que el mismo fue desactivado

        actualizar_resumen_CG();
    }
    else
    {
        ui->checkBox_14->setText("Desactivar");
        ui->dateEdit_6->setEnabled(true);
        ui->label_48->setEnabled(true);
        ui->dateEdit_7->setEnabled(true);
        ui->label_49->setEnabled(true);
        on_dateEdit_7_dateChanged(ui->dateEdit_7->date());

    }
}

//CONTABLE_GRUPO - // Tabla Contable_Grupo slot onDoubleclick
void MainWindow::on_contable_grupo_doubleClicked(QModelIndex index)
{

    int indice = index.row();
    Fila = indice;

    if(!index.sibling(indice,8).data().toString().isEmpty()){

        limpiarCGR();
        desactivarCGR();
        QMessageBox::warning(this,"Cuidado","No se puede editar este elemento en esta sección.""\n Para hacerlo debe dirigirse a la cuenta miembro.");

    }

    else
    {
        //qDebug()<<!index.sibling(indice,7).data().toString().isEmpty();
        if(index.sibling(indice,6).data().toString() == "General")
        {
            activarCGR();
            limpiarCGR();

            ui->lineEdit_29->setText(index.sibling(indice,1).data().toString()); //motivo

            int ind2 = ui->comboBox_8->findText(index.sibling(indice,2).data().toString());
            ui->comboBox_8->setCurrentIndex(ind2);

            if(ui->comboBox_8->currentText() == "Ingreso")
            {
                ui->doubleSpinBox->setValue(index.sibling(indice,3).data().toDouble());

            }

            else
            {
                ui->doubleSpinBox->setValue(index.sibling(indice,4).data().toDouble());
            }

            ui->dateEdit_8->setDate(index.sibling(indice,5).data().toDate());

            edicion_contable = 1;
            Cod_Contable = index.sibling(indice,0).data().toString();

            //Desactivo la tabla
            ui->contable_grupo->setEnabled(false);

            //Desactivo el botón nuevo_ingreso,borrar e imprimir.
            ui->pushButton_13->setEnabled(false);
            ui->pushButton_14->setEnabled(false);
            ui->pushButton_10->setEnabled(false);
        }

        else
        {
            limpiarCGR();
            desactivarCGR();

            QMessageBox::warning(this,"Cuidado","No se puede editar este elemento en esta sección.""\n Para hacerlo debe dirigirse a la cuenta miembro.");

        }

    }

}

//CONTABLE GRUPO - Contable grupo boton borrar on click
void MainWindow::on_contable_grupo_clicked(const QModelIndex &)
{
    ui->pushButton_13->setEnabled(true);// boton borrar.
}

//----------------------------------SECCION CUENTA MIEMBRO----------------------------------

//CUENTA_MIEMBRO - Estructura_ Cuenta Miembro
void MainWindow::estructura_Cuenta_Miembro()
{
    cum = new QSqlTableModel();
    cum->setTable("Contable_Grupo");
    cum->setFilter("Cod_CON='no'");
    cum->select();

    // seteo los nombres de las columnas para el view de Cuenta Miembro (Ingresos generales)

    cum->setHeaderData(3, Qt::Horizontal, tr("Monto Pagado"));
    cum->setHeaderData(1, Qt::Horizontal, tr("Motivo"));
    cum->setHeaderData(5, Qt::Horizontal, tr("Fecha"));
    cum->setHeaderData(6, Qt::Horizontal, trUtf8("Categoría"));


    ui->cuenta_miembro->setModel(cum);
    ui->cuenta_miembro->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->cuenta_miembro->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    //seteo de columnas
    ui->cuenta_miembro->hideColumn(0); //cod_CON
    ui->cuenta_miembro->hideColumn(2); //tipo_motivo
    ui->cuenta_miembro->hideColumn(4); //egreso
    ui->cuenta_miembro->hideColumn(7); //cod_Miembro
    ui->cuenta_miembro->hideColumn(8); //cod_evento
    ui->cuenta_miembro->hideColumn(9); //rama
    ui->cuenta_miembro->hideColumn(10); //nombre
    ui->cuenta_miembro->hideColumn(11); //apellido
    ui->cuenta_miembro->hideColumn(12); //Cantidad
    ui->cuenta_miembro->hideColumn(13); //CON_evento
    ui->cuenta_miembro->hideColumn(14); //Debe_miembro
    ui->cuenta_miembro->hideColumn(15); //gan_mie
    ui->cuenta_miembro->hideColumn(16); //gan_grup

    //Aca viene la parte del filtro pero para la parte de Eventos
    cme = new QSqlTableModel();
    cme->setTable("Contable_Grupo");
    //Solo para mostrar el modelo sin tener alguien cargado
    cme->setFilter("Cod_CON='no' and cod_evento != 'NULL'");
    cme->select();

    cme->setHeaderData(3, Qt::Horizontal, tr("Ganancia Miembro"));
    cme->setHeaderData(1, Qt::Horizontal, tr("Evento"));
    cme->setHeaderData(14, Qt::Horizontal, tr("Adeuda"));
    cme->setHeaderData(12, Qt::Horizontal, tr("Cantidad"));

    ui->EventosMie->setModel(cme);
    ui->EventosMie->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->EventosMie->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    //seteo de columnas
    ui->EventosMie->hideColumn(0); //cod_CON
    ui->EventosMie->hideColumn(2); //tipo_motivo
    ui->EventosMie->hideColumn(4); //egreso
    ui->EventosMie->hideColumn(5); //Fecha_ingreso
    ui->EventosMie->hideColumn(6); //categoria
    ui->EventosMie->hideColumn(7); //cod_Miembro
    ui->EventosMie->hideColumn(8); //cod_evento
    ui->EventosMie->hideColumn(9); //rama
    ui->EventosMie->hideColumn(10); //nombre
    ui->EventosMie->hideColumn(11); //apellido
    ui->EventosMie->hideColumn(13); //CON_evento
    ui->EventosMie->hideColumn(15); //gan_mie
    ui->EventosMie->hideColumn(16); //gan_grup

   }

//...........Funciones auxiliares.................................

//CUENTA_MIEMBRO - Activar botones y demas luego de asignar un nuevo miembro
void MainWindow::activarCM_Asignacion()
{
    ui->label_97->setEnabled(true);
    ui->label_98->setEnabled(true);
    ui->label_99->setEnabled(true);
    ui->label_100->setEnabled(true);
    ui->label_101->setEnabled(true);
    ui->label_107->setEnabled(true);
    ui->label_108->setEnabled(true);
    ui->cuenta_miembro->setEnabled(true);
    ui->EventosMie->setEnabled(true);
    ui->pushButton_15->setEnabled(true);
}

//CUENTA_MIEMBRO - Desactivar botones y demas luego de asignar un nuevo miembro.
void MainWindow::desactivarCM_Asignacion()
{
    ui->label_97->setEnabled(false);
    ui->label_98->setEnabled(false);
    ui->label_99->setEnabled(false);
    ui->label_100->setEnabled(false);
    ui->label_101->setEnabled(false);
    ui->label_107->setEnabled(false);
    ui->label_108->setEnabled(false);
    ui->cuenta_miembro->setEnabled(false);
    ui->EventosMie->setDisabled(true);
    ui->pushButton_15->setEnabled(false);

}

//CUENTA_MIEMBRO - Activar
void MainWindow::activaCM()
{
    ui->lineEdit_61->setEnabled(true); // motivo
    ui->label_102->setEnabled(true); //label motivo

    ui->doubleSpinBox_2->setEnabled(true); //Monto
    ui->label_103->setEnabled(true); //label monto

    ui->comboBox_10->setEnabled(true); // Categoria
    ui->label_104->setEnabled(true); //label categoria

    ui->pushButton_16->setEnabled(true); //guardar
    ui->pushButton_17->setEnabled(true); //cancelar

    ui->dateEdit_9->setEnabled(true);

}

//CUENTA_MIEMBRO - Desactivar
void MainWindow::desactivaCM()
{
    ui->lineEdit_61->setEnabled(false); // motivo
    ui->label_102->setEnabled(false); //label motivo

    ui->doubleSpinBox_2->setEnabled(false); //Monto
    ui->label_103->setEnabled(false); //label monto

    ui->comboBox_10->setEnabled(false); // Categoria
    ui->label_104->setEnabled(false); //label categoria

    ui->pushButton_16->setEnabled(false); //guardar
    ui->pushButton_17->setEnabled(false); //cancelar
    ui->pushButton_13->setEnabled(false); //boton borrar

    ui->label_105->setEnabled(false); //label cantidad
    ui->spinBox_2->setEnabled(false); //cantidad

    ui->dateEdit_9->setEnabled(false);

}

//CUENTA_MIEMBRO - Limpiar interfaz para evento y para los ingresos
void MainWindow::limpiarCM()
{

    ui->lineEdit_61->clear();
    edicion_CUM = 0;
    ui->doubleSpinBox_2->setValue(0);
    ui->doubleSpinBox_3->setValue(0);
    ui->comboBox_10->setCurrentIndex(0);
    ui->label_101->setText("0.00");
    ui->label_99->setText("0.00");
    ui->label_108->setText("0.00");
    ui->spinBox_2->clear();
    ui->spinBox_2->setEnabled(false);
    ui->label_105->setEnabled(false);
    ui->dateEdit_9->setDate(QDate::currentDate());
    //Calculo el debe y el haber
    calculos_miembro(Cod_Miembro);
}

//CUENTA_MIEMBRO - Activar Evento
void MainWindow::activaME()
{
    ui->doubleSpinBox_3->setEnabled(true); //Monto
    ui->label_106->setEnabled(true); //label monto

    ui->spinBox_2->setEnabled(true);//cantidad
    ui->label_105->setEnabled(true); //label cantidad

    ui->pushButton_16->setEnabled(true); //guardar
    ui->pushButton_17->setEnabled(true); //cancelar
}

//CUENTA_MIEMBRO - Limpiar interfaz Evento
void MainWindow::desactivaME()
{
    ui->doubleSpinBox_3->setEnabled(false); //Monto
    ui->label_106->setEnabled(false); //label monto

    ui->spinBox_2->setEnabled(false);//cantidad
    ui->label_105->setEnabled(false); //label cantidad

    ui->pushButton_16->setEnabled(false); //guardar
    ui->pushButton_17->setEnabled(false); //cancelar
}

//CUENTA_MIEMBRO -  Cuenta Miembro - filtro para aplicar en el model
void MainWindow::filtrar_cuentamiembro(QString &Filtro, QString Cod_Miembro)
{
    CuM.buscar_BD(Filtro,Cod_Miembro);
}

//Calculo el debe y el haber del miembro
void MainWindow::calculos_miembro(QString cod)
{
    //Para calcular el debe sumo todos los montos de los costos cargados en las configuraciones y lo que deba de los eventos
    QSqlQuery *calc = new QSqlQuery();
    calc->prepare("SELECT SUM(monto) FROM Costos");
    calc->exec();
    calc->next();
    double Total_cos=calc->value(0).toDouble();

    calc->prepare("SELECT SUM(debe_miembro) FROM Contable_Grupo WHERE Cod_Miembro = :cod");
    calc->bindValue(":cod",cod);
    calc->exec();
    calc->next();
    double Total_event=calc->value(0).toDouble();

    //Para calcular el haber sumo la columna ingreso

    calc->prepare("SELECT SUM(Ingreso) FROM Contable_Grupo WHERE Cod_Miembro = :cod");
    calc->bindValue(":cod",cod);
    calc->exec();
    calc->next();
    double Ingr=calc->value(0).toDouble();

    //Etiqueta Debe 101
    ui->label_101->setText("$"+QString::number(Total_cos+Total_event));
    //Etiqueta Haber 99
    ui->label_99->setText("$"+QString::number(Ingr));
    //Etiqueta Neto 108
    ui->label_108->setText("$"+QString::number(Ingr - Total_cos - Total_event));

}

// CUENTA_MIEMBRO - Resumen de cuenta Miembro
void MainWindow::imprimir_cuenta_miembro(QString & strStream)
{
    const int rowCount = ui->cuenta_miembro->model()->rowCount();
    const int columnCount = ui->cuenta_miembro->model()->columnCount();

    strStream+= "<font size=3 width=100% align=cente><i> Listado de Pagos Generales </i></font>";

    // tabla cuenta_miembro.
    strStream+="<br>";
    strStream+= "<table border=2 cellspacing=0 cellpadding=1>";
    strStream+= "<tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!ui->cuenta_miembro->isColumnHidden(column))
            strStream+= QString("<th width=25% align=center%><font size=2>%1</font></th>").arg(ui->cuenta_miembro->model()->headerData(column, Qt::Horizontal).toString());
    strStream+= "</tr>";

    // data table cuenta_miembro.

    for (int row = 0; row < rowCount; row++) {
        strStream+="<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->cuenta_miembro->isColumnHidden(column)) {

                QString data = ui->cuenta_miembro->model()->data(ui->cuenta_miembro->model()->index(row, column)).toString().simplified();
                if(column == 1 or column == 6)
                {
                    //alineacion izquierda.
                    strStream+= QString("<td align=left bkcolor=0><font size=2>%1</font></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));

                }
                if(column == 3)
                {
                    //alineacion derecha.
                    strStream+= QString("<td align=right bkcolor=0><font size=2>$%1</font></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
                if(column == 5)
                {
                    //alineacion centrada
                    strStream+= QString("<td align=center bkcolor=0><font size=2>%1</font></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));

                }
            }
        }
        strStream+= "</tr>";
    }

    strStream+=  "</table>";
    strStream+= "<br><br>";
    strStream+= "<font size=3 width=100% align=center><i> Listado de Eventos </i></font>";
    strStream+= "<br>";

    const int rowCount2 = ui->EventosMie->model()->rowCount();
    const int columnCount2 = ui->EventosMie->model()->columnCount();

    // headers tabla EventosMie.

    strStream+="<table border=2 cellspacing=0 cellpadding=1>";
    strStream+= "<tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount2; column++)
        if (!ui->EventosMie->isColumnHidden(column))
            strStream+= QString("<th width=25% align=center%><font size=2>%1</font></th>").arg(ui->EventosMie->model()->headerData(column, Qt::Horizontal).toString());
    strStream+= "</tr>";

    // data table EventosMie.

    for (int row = 0; row < rowCount2; row++) {
        strStream+= "<tr>";
        for (int column = 0; column < columnCount2; column++) {

            if (!ui->EventosMie->isColumnHidden(column)) {

                QString data = ui->EventosMie->model()->data(ui->EventosMie->model()->index(row, column)).toString().simplified();
                if(column == 1)
                {
                    //aliniacion izquierda.
                    strStream+= QString("<td align=left bkcolor=0><font size=2>%1</font></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));

                }

                if(column == 3 or column == 14)
                {
                    //aliniacion derecha.
                    strStream+= QString("<td align=right bkcolor=0><font size=2>$%1</font></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));

                }

                if(column == 12)
                {
                    //aliniacion centrada.
                    strStream+= QString("<td align=center bkcolor=0><font size=2>%1</font></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));

                }

            }
        }
        strStream+= "</tr>";
    }

    strStream+=  "</table>";

    strStream+= "<br>";

    strStream+="<table align=right cellpadding=2>";
    strStream+="<tr>";
    strStream+="<td align=left colspan=2><b>Haber:</b> " + ui->label_99->text() + " </td>";
    strStream+="<td align=left colspan=2><b>Deber:</b> " + ui->label_101->text() + " </td>";
    strStream+="<td align=left colspan=2><b>Neto:</b> " + ui->label_108->text() + " </td>";
    strStream+= "</tr>";
    strStream+= "</table><br>";

}

//..........Slots..................................................

//CUENTA_MIEMBRO - Nuevo Ingreso
void MainWindow::on_pushButton_15_pressed()
{
    limpiarCM();
    activaCM();
    ui->EventosMie->clearSelection();
    ui->cuenta_miembro->clearSelection();
    ui->pushButton_15->setEnabled(false); // lo desactivo para que no me quiera hacer otro ingreso simultaneamente
    ui->pushButton_18->setEnabled(false);
    edicion_CUM = 0;
    ui->lineEdit_61->setFocus();

    //Desactivo las tablas para que no pueda quitar
    ui->cuenta_miembro->setEnabled(false);
    ui->EventosMie->setEnabled(false);
}

//CUENTA_MIEMBRO - Guardar
void MainWindow::on_pushButton_16_pressed()
{
    QString Cod_CON,motivo,categoria;
    double monto;
    QDate dia;

    ConectarconBD::cod_setup("Contable_Grupo",Cod_CON);
    motivo = ui->lineEdit_61->text();

    int indice = ui->comboBox_10->currentIndex();
    categoria = ui->comboBox_10->itemText(indice);
    dia = ui->dateEdit_9->date();

    if(ui->comboBox_10->itemText(indice).isNull())
    {
        categoria = "Ninguna";
    }
    else
    {
        categoria = ui->comboBox_10->itemText(indice);
    }

    monto = ui->doubleSpinBox_2->text().toDouble();

    //qDebug()<<"Mainwindows.cpp - Cuenta Miembro << Cuenta_Miembro"<<Cod_Miembro;

    if(edicion_CUM == 0)
    {

        QSqlDatabase::database().transaction();

        CuM.Guardar(Cod_Miembro,Cod_CON,motivo,monto,categoria,dia,0,0," ");
        CuM.almacenar_BD();        

        QSqlDatabase::database().commit();

        //Scrolldown
        while(cum->canFetchMore()){cum->fetchMore();}
        ui->cuenta_miembro->scrollToBottom();
    }
    else
    {
        if(edicion_CUM == 1)
        {

            //qDebug()<<"Entro en Edicion_CUM 1 - Edición de ingreso General";

            QSqlDatabase::database().transaction();

            CuM.Guardar(Cod_Miembro,Cod_Contable,motivo,monto,categoria,dia,0,0,QString::null);

            CuM.actualizar_BD(edicion_CUM);

            QSqlDatabase::database().commit();
        }
        else
        {

            if(edicion_CUM == 2)
            {
                double debe_aux = ui->spinBox_2->value() * (ganancia_grupo + ganancia_miembro);

                if(debe_aux < ui->doubleSpinBox_3->value())
                {
                    QMessageBox::warning(this,"Cuidado!","El monto ingresado no puede ser mayor al monto adeudado. Ingrese el valor correcto",QMessageBox::Ok);
                }
                else
                {
                    QSqlDatabase::database().transaction();

                    CuM.Guardar(Cod_Miembro,Cod_Contable," ",ui->doubleSpinBox_3->value(),categoria,dia,ui->spinBox_2->value(),0,CON_Evento);
                    CuM.actualizar_BD(edicion_CUM);

                    QSqlDatabase::database().commit();
                }
            }


        }

    }

    CGR->select(); //luego de una nuevo insercion actualizo tanto el tableview de la cuenta miembro
    cum->select();
    cme->select();

    limpiarCM();
    desactivaCM();
    desactivaME();

    //Borro la seleccion
    ui->EventosMie->clearSelection();
    ui->cuenta_miembro->clearSelection();

    ui->pushButton_15->setEnabled(true);
    ui->pushButton_18->setEnabled(false);

    //Vuelvo a actualizar los resumenes
    calculos_miembro(Cod_Miembro);
    actualizar_resumen_CG();

    //Activo las tablas para que no pueda quitar
    ui->cuenta_miembro->setEnabled(true);
    ui->EventosMie->setEnabled(true);
}

//CUENTA_MIEMBRO - Cancelar
void MainWindow::on_pushButton_17_pressed()
{
    limpiarCM();
    desactivaCM();
    desactivaME();

    ui->pushButton_15->setEnabled(true);
    ui->pushButton_18->setEnabled(false);

    //Borro la seleccion
    ui->EventosMie->clearSelection();
    ui->cuenta_miembro->clearSelection();

    //Activo las tablas para que no pueda quitar
    ui->cuenta_miembro->setEnabled(true);
    ui->EventosMie->setEnabled(true);

}

//CUENTA_MIEMBRO - Borrar
void MainWindow::on_pushButton_18_pressed()
{

    //creo el boton No.
    QPushButton * botonNO = new QPushButton(tr("No"));
    botonNO->setCheckable(true);
    botonNO->setAutoDefault(true);

    //Creo el boton Si.
    QPushButton * botonSI = new QPushButton(tr("Si"));
    botonSI->setDefault(false);

    //Creo el nuevo QMessageBox.
    QMessageBox * boton = new QMessageBox();
    boton->setWindowTitle("Cuidado");
    boton->setText("¿Esta seguro que desea eliminar este elemento?");

    //Asigno los iconos correspondientes
    boton->setWindowIcon(QPixmap(":/icons/gags"));
    boton->setIcon(QMessageBox::Warning);

    //Seteo los botones Si y No en el QMessageBox
    boton->addButton(botonSI,QMessageBox::YesRole);
    boton->addButton(botonNO,QMessageBox::NoRole);

    //Ejecuto el QMessageBox Creado.
    boton->exec();

    if(boton->clickedButton() == botonSI)
    {
        //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
        QItemSelection selection( ui->cuenta_miembro->selectionModel()->selection());


        //Genero un array como todos los indices de los elementos seleccionados
        QList<int> rows;
        foreach( const QModelIndex & index, selection.indexes() ) {
            rows.append(index.row());
        }

        qSort(rows);

        //procedo mediante un for a eliminar los elementos selecionados, obviamente asignandole para eso el Model correspondiente a materiales
        int prev = -1;

        QSqlDatabase::database().transaction();

        for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
            int current = rows[i];
            if( current != prev ) {
                cum->removeRows(current,1);
                prev = current;
            }
        }        

        QSqlDatabase::database().commit();

        CGR->select();

        //tabla ingresos generales
        QString  Filtro_CM ="Cod_Miembro='" + Cod_Miembro + "'"+ "and cod_evento is NULL";
        cum->setFilter(Filtro_CM);
        cum->select();

        //tabla evento
        QString  Filtro_CME ="Cod_Miembro='" + Cod_Miembro + "'"+ "and cod_evento is not NULL";
        cme->setFilter(Filtro_CME);
        cme->select();

        limpiarCM();
        desactivaCM();
        desactivaME();

        //Borro la seleccion
        ui->EventosMie->clearSelection();
        ui->cuenta_miembro->clearSelection();

        ui->pushButton_18->setEnabled(false);

        //Vuelvo a actualizar los resumenes
        calculos_miembro(Cod_Miembro);
        actualizar_resumen_CG();
    }

    boton->~QMessageBox();

}

//CUENTA_MIEMBRO - Activar boton borrar en cuenta miembro
void MainWindow::on_cuenta_miembro_pressed(const QModelIndex &)
{
    ui->pushButton_18->setEnabled(true);
}

//CUENTA_MIEMBRO -  Cuenta Miembro - doubleclick edition
void MainWindow::on_cuenta_miembro_doubleClicked(QModelIndex index)
{
    limpiarCM();
    activaCM();

    edicion_CUM = 1;

    int indice = index.row();

    //Evalua si el Cod_Evento es null inicia la edicion de los otros tipos de ingreso
    if(index.sibling(indice,8).data().toString().isEmpty())
    {

        ui->lineEdit_61->setEnabled(true); // motivo
        ui->label_102->setEnabled(true); //label motivo

        ui->doubleSpinBox_2->setEnabled(true); //Monto
        ui->label_103->setEnabled(true); //label monto

        ui->comboBox_10->setEnabled(true); // Categoria
        ui->label_104->setEnabled(true); //label categoria

        ui->pushButton_16->setEnabled(true); //guardar
        ui->pushButton_17->setEnabled(true); //cancelar

        ui->lineEdit_61->setText(index.sibling(indice,1).data().toString()); //motivo

        ui->doubleSpinBox_2->setValue(index.sibling(indice,3).data().toDouble());
    }


    Cod_Contable = index.sibling(indice,0).data().toString();
    ui->pushButton_15->setEnabled(false);

    ui->pushButton_18->setEnabled(false);

    //Desactivo las tablas para que no pueda quitar
    ui->cuenta_miembro->setEnabled(false);
    ui->EventosMie->setEnabled(false);

}

//CUENTA_MIEMBRO -  Cuenta Miembro - doubleclick edition evento
void MainWindow::on_EventosMie_doubleClicked(const QModelIndex &index)
{
    limpiarCM();
    activaME();

    edicion_CUM = 2;
    debe_miembro = 0;

    int indice = index.row();

    //Evalua si el Cod_Evento es null inicia la edicion de los otros tipos de ingreso

    ui->label_102->setEnabled(false); //label_ motivo
    ui->lineEdit_61->setEnabled(false); //motivo

    ui->label_104->setEnabled(false);//label_cat pago
    ui->comboBox_10->setEnabled(false);//categoria de pago
    ui->dateEdit_9->setEnabled(false); //fecha_ingreso

    ui->spinBox_2->setEnabled(true); //Activo la cantidad
    ui->label_105->setEnabled(true); //

    Cod_Evento = index.sibling(indice,8).data().toString();
    CON_Evento = index.sibling(indice,13).data().toString();

    ui->spinBox_2->setValue(index.sibling(indice,12).data().toInt()); //cantidad

    double gan_grup = 0;

    //seteo los valores de las ganancias para hacer el control previo al ingreso de los montos segun la cantidad.

    ganancia_grupo = index.sibling(indice,15).data().toDouble();
    ganancia_miembro = index.sibling(indice,16).data().toDouble();


    CuM.buscar_BD_2(gan_grup,index.sibling(indice,13).data().toString()); //busca la ganancia porcentual del grupo.

    ui->doubleSpinBox_3->setValue(index.sibling(indice,3).data().toDouble() + gan_grup);

    debe_miembro = ui->doubleSpinBox_3->value() + index.sibling(indice,14).data().toDouble(); //total debe_miembro

    if(ui->spinBox_2->value() != 0)
    {
        ui->doubleSpinBox_3->setMaximum(debe_miembro); // seteo el precio max que se debe pagar por evento.
    }

    Cod_Contable = index.sibling(indice,0).data().toString();
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_18->setEnabled(false);

    //Desactivo las tablas para que no pueda quitar
    ui->cuenta_miembro->setEnabled(false);
    ui->EventosMie->setEnabled(false);

}

//CUENTA_MIEMBRO -  Cuenta Miembro - click desactiva el boton borrar
void MainWindow::on_EventosMie_clicked(const QModelIndex &)
{
    ui->pushButton_18->setDisabled(true);
}

//Controlo que no toque en ningun lugar en blanco de la tabla
void MainWindow::on_cuenta_miembro_clicked(const QModelIndex &)
{
    ui->pushButton_18->setEnabled(true);
}

// CUENTA_MIEMBRO - Cuenta Miembro chequeo del monto ingresado en funcion de la cantidad
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    // control monto - cantidad para el evento. Seteo los maximos y minimos por cada evento en funcion de la cantidad.
    double debe_max_aux = (ganancia_grupo + ganancia_miembro) * arg1;
    ui->doubleSpinBox_3->setMaximum(debe_max_aux);
}




