#include "configuraciones.h"
#include "ui_configuraciones.h"
#include <QSqlRecord>

//Constructor y configuracion de la interfaz
configuraciones::configuraciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configuraciones)
{
    //Interfaz Al INICIO
    ui->setupUi(this);

    //Pestaña por defecto

    ui->Ciclocontable->setCurrentIndex(0);

    op=0;

    //Configuro el mensaje de Warning
    Msg = new QMessageBox(this);
    Msg->setIcon(QMessageBox::Information);
    Msg->setStandardButtons(QMessageBox::Ok);
    Msg->setDefaultButton(QMessageBox::Ok);
    Msg->setWindowTitle("Error al Intentar ingresar un Campo.");
    Msg->hide();


    estructura_costos();
    estructura_modcamposBD();
    estructura_evento();

    asignar_fechas();

}

//Destructor
configuraciones::~configuraciones()
{
    costos->~QSqlTableModel();
    evento->~QSqlTableModel();
    lista_pat->~QSqlTableModel();
    lista_car->~QSqlTableModel();
    lista_ram->~QSqlTableModel();
    lista_catmat->~QSqlTableModel();
    lista_catpag->~QSqlTableModel();

    delete ui;
}

//----------------------------------Funciones Auxiliares Generales----------------------------

//Reglas de Interfaz de los tabs
void configuraciones::on_Ciclocontable_currentChanged(QWidget *)
{
    desactivarQ();
    desactivarCOS();
    desactivarEVE();
    ui->lineEdit_2->clear();
    ui->lineEdit_2->setEnabled(false);
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);
    ui->dateEdit->clearFocus();
    ui->dateEdit_2->clearFocus();
    activarAGR();

    asignar_fechas();
}

//Funcion que verifica si hay categorias cargadas en la BD para advertir al usuario
bool configuraciones::verifica_sihay()
{
    QSqlQuery *cons = new QSqlQuery();

    //Casos que quiero controlar
    cons->prepare("select * from Rama");
    cons->exec();
    cons->next();
    if (!cons->isValid())
    {
        return false;
    }
    cons->prepare("select * from Patrulla");
    cons->exec();
    cons->next();
    if (!cons->isValid())
    {
        return false;
    }
    cons->prepare("select * from Cargo");
    cons->exec();
    cons->next();
    if (!cons->isValid())
    {
        return false;
    }
    cons->prepare("select * from Cat_mat");
    cons->exec();
    cons->next();
    if (!cons->isValid())
    {
        return false;
    }
    cons->prepare("select * from Cat_pago");
    cons->exec();
    cons->next();
    if (!cons->isValid())
    {
        return false;
    }

    cons->~QSqlQuery();
    return true;
}

//----------------------------------PESTAÑA COSTOS--------------------------------------------

void configuraciones::estructura_costos()
{
    //asignacion de los modelos
    costos = new QSqlTableModel();
    costos->setTable("Costos");
    costos->select();

    //Cambio los nombres de las columnas.
    costos->setHeaderData(1, Qt::Horizontal, trUtf8("Descripción"));
    costos->setHeaderData(2, Qt::Horizontal, tr("Monto"));

    //Linkeo el modelo a la tableview
    ui->tableView->setModel(costos);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Muestro el valor calculado del total
    cos.calcular_monto();
    double mon = cos.mostrar_monto();
    QString esc_monto = "$" + QString::number(mon);
    ui->label_5->setText(esc_monto);

}

//...........Funciones auxiliares.................................

void configuraciones::limpiarCOS()
{
    ui->lineEdit->clear();
    ui->doubleSpinBox_2->setValue(0);
}

void configuraciones::activarCOS()
{
    ui->lineEdit->setEnabled(true);
    ui->doubleSpinBox_2->setEnabled(true);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_16->setEnabled(false);
}

void configuraciones::desactivarCOS()
{
    ui->lineEdit->setEnabled(false);
    ui->doubleSpinBox_2->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_16->setEnabled(false);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}

//..........Slots..................................................

//Guardar
void configuraciones::on_pushButton_15_pressed()
{
    if(!ui->lineEdit->text().isEmpty() and ui->doubleSpinBox_2->value() != 0)
    {

        if (Edicion_Costos != 1){
            //Nuevo Ingreso.
            QString costeleta;
            ConectarconBD::cod_setup("Costos",costeleta);

            cos.guardar(ui->lineEdit->text(),ui->doubleSpinBox_2->value(),costeleta);

            QSqlDatabase::database().transaction();

            cos.cargar_bd();

            QSqlDatabase::database().commit();

        }
        else
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
            boton->setText("¿Esta seguro que desea editar este elemento?.");

            //Asigno los iconos correspondientes
            boton->setWindowIcon(QPixmap(":/icons/gags"));
            boton->setIcon(QMessageBox::Warning);

            //Seteo los botones Si y No en el QMessageBox
            boton->addButton(botonSI,QMessageBox::YesRole);
            boton->addButton(botonNO,QMessageBox::NoRole);

            //ejecuto el QMessageBox.
            boton->exec();

            if (boton->clickedButton() == botonSI)

            {
            //Edicion de Costos.
            qDebug()<<"Ingreso en la edicion. Guardar->Costos";

            cos.guardar(ui->lineEdit->text(),ui->doubleSpinBox_2->value(),Cod_Costos);
            qDebug()<<"Valores a actualizar::"<<" Cod_Costos::"<<Cod_Costos<<"Desp"<<ui->lineEdit->text()<<" :: Monto."<<ui->doubleSpinBox_2->value();

            QSqlDatabase::database().transaction();

            cos.actualizar_bd();

            QSqlDatabase::database().commit();

            }

            limpiarCOS();
            desactivarCOS();

        }

        //Actualizo el model
        costos->select();

        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);

        //Muestro el valor calculado del total
        cos.calcular_monto();
        double mon = cos.mostrar_monto();
        QString esc_monto = "$" + QString::number(mon);
        ui->label_5->setText(esc_monto);

        //Activo la tabla
        ui->tableView->setEnabled(true);

        //Limpio y desactivo.
        limpiarCOS();
        desactivarCOS();
    }

    else
    {
        QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al Intentar ingresar un Campo.\n""Escriba los Campos necesarios para Ingresar un nuevo Costo.", 0, QApplication::UnicodeUTF8),QMessageBox::Ok);
    }

}

//Cancelar
void configuraciones::on_pushButton_16_pressed()
{
    limpiarCOS();
    desactivarCOS();
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_16->setEnabled(false);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);

    //Activo la tabla
    ui->tableView->setEnabled(true);
}

//Agregar
void configuraciones::on_pushButton_pressed()
{
    limpiarCOS();
    activarCOS();
    ui->pushButton_15->setEnabled(true);
    ui->pushButton_16->setEnabled(true);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    ui->lineEdit->setFocus();

    //Desactivo la tabla
    ui->tableView->setEnabled(false);

    //Edicion_Costos = 0; Por las dudas.
    Edicion_Costos = 0;
}

//Quitar
void configuraciones::on_pushButton_2_pressed()
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
    boton->setText("¿Esta seguro que desea eliminar este elemento?.");

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
        QItemSelection selection( ui->tableView->selectionModel()->selection());

        //Genero un array como todos los indices de los elementos seleccionados
        QList<int> rows;
        foreach( const QModelIndex & index, selection.indexes() )
        {
            rows.append(index.row());
        }

        qSort(rows);

        QSqlDatabase::database().transaction();

        //elimina elementos seleccionados
        int prev = -1;
        for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
            int current = rows[i];
            if( current != prev ) {
                costos->removeRows(current,1);
                prev = current;
            }
        }

        QSqlDatabase::database().commit();

        //Muestro el valor calculado del total
        cos.calcular_monto();
        double mon = cos.mostrar_monto();
        QString esc_monto = "$" + QString::number(mon);
        ui->label_5->setText(esc_monto);

    }

    boton->~QMessageBox();


}

//Click activa el boton borrar
void configuraciones::on_tableView_clicked(const QModelIndex &)
{
    ui->pushButton_2->setEnabled(true);
}

//Dobleclick - Edición de Costo.
void configuraciones::on_tableView_doubleClicked(const QModelIndex &index)
{
    //Selecciono la fila.
    int fila = index.row();

    //desactivo los botones que no van.
    limpiarCOS();
    activarCOS();

    ui->pushButton_15->setEnabled(true);
    ui->pushButton_16->setEnabled(true);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    //Seteo los datos en los campos correspondientes.
    ui->lineEdit->setText(index.sibling(fila,1).data().toString());
    ui->doubleSpinBox_2->setValue(index.sibling(fila,2).data().toDouble());
    Cod_Costos = index.sibling(fila,0).data().toString();//Lo nesesito para el Update.

    //Desactivo la tabla.
    ui->tableView->setEnabled(false);

    Edicion_Costos = 1;

}

//----------------------------------PESTAÑA CAMPOS BD-----------------------------------------

void configuraciones::estructura_modcamposBD()
{
    //Patrulla

    lista_pat = new QSqlTableModel();
    lista_pat->setTable("Patrulla");
    lista_pat->select();

    ui->tableView_3->setModel(lista_pat);
    ui->tableView_3->horizontalHeader()->hide();
    ui->tableView_3->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Cargo
    lista_car = new QSqlTableModel();
    lista_car->setTable("Cargo");
    lista_car->select();

    ui->tableView_7->setModel(lista_car);
    ui->tableView_7->horizontalHeader()->hide();
    ui->tableView_7->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Rama
    lista_ram = new QSqlTableModel();
    lista_ram->setTable("Rama");
    lista_ram->select();

    ui->tableView_31->setModel(lista_ram);
    ui->tableView_31->horizontalHeader()->hide();
    ui->tableView_31->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Cat Pagos
    lista_catpag= new QSqlTableModel();
    lista_catpag->setTable("Cat_pago");
    lista_catpag->select();

    ui->tableView_8->setModel(lista_catpag);
    ui->tableView_8->horizontalHeader()->hide();
    ui->tableView_8->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    //Cat Materiales
    lista_catmat= new QSqlTableModel();
    lista_catmat->setTable("Cat_mat");
    lista_catmat->select();

    ui->tableView_9->setModel(lista_catmat);
    ui->tableView_9->horizontalHeader()->hide();
    ui->tableView_9->horizontalHeader()->setResizeMode(QHeaderView::Stretch);



}

//...........Funciones auxiliares.................................

void configuraciones::activarAGR()
{
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    ui->tableView_3->setEnabled(true);
    ui->tableView_7->setEnabled(true);
    ui->tableView_31->setEnabled(true);
    ui->tableView_8->setEnabled(true);
    ui->tableView_9->setEnabled(true);
}

void configuraciones::desactivarAGR()
{
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->tableView_3->setEnabled(false);
    ui->tableView_7->setEnabled(false);
    ui->tableView_31->setEnabled(false);
    ui->tableView_8->setEnabled(false);
    ui->tableView_9->setEnabled(false);

}

void configuraciones::activarQ()
{
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_12->setEnabled(true);

}

void configuraciones::desactivarQ()
{
    ui->tableView_3->clearFocus();
    ui->tableView_7->clearFocus();
    ui->tableView_31->clearFocus();
    ui->tableView_9->clearFocus();
    ui->tableView_8->clearFocus();
    ui->tableView_3->clearSelection();
    ui->tableView_7->clearSelection();
    ui->tableView_31->clearSelection();
    ui->tableView_9->clearSelection();
    ui->tableView_8->clearSelection();
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
}

void configuraciones::on_tableView_3_pressed(const QModelIndex &)
{
    ui->pushButton_6->setEnabled(true);
}

void configuraciones::on_tableView_7_pressed(const QModelIndex &)
{

    ui->pushButton_8->setEnabled(true);
}

void configuraciones::on_tableView_31_pressed(const QModelIndex &)
{

    ui->pushButton_4->setEnabled(true);
}

void configuraciones::on_tableView_8_pressed(const QModelIndex &)
{

    ui->pushButton_10->setEnabled(true);
}

void configuraciones::on_tableView_9_pressed(const QModelIndex &)
{

    ui->pushButton_12->setEnabled(true);
}

//..........Slots..................................................

//Agregar Patrulla
void configuraciones::on_pushButton_5_pressed()
{
    ui->lineEdit_2->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
    op=1;
    desactivarAGR();
    desactivarQ();
    ui->lineEdit_2->setFocus();
}

//Quitar Patrulla
void configuraciones::on_pushButton_6_pressed()
{
    //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
    QItemSelection selection( ui->tableView_3->selectionModel()->selection());

    //Genero un array como todos los indices de los elementos seleccionados
    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append(index.row());
    }

    qSort(rows);

    //elimina elementos seleccionados
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            lista_pat->removeRows(current,1);
            prev = current;
        }
    }
    //Actualizo el modelo
    lista_pat->select();
    desactivarQ();
}

//Agregar Cargo
void configuraciones::on_pushButton_7_pressed()
{
    ui->lineEdit_2->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
    op=2;
    desactivarAGR();
    desactivarQ();
    ui->lineEdit_2->setFocus();
}

//Quitar Cargo
void configuraciones::on_pushButton_8_pressed()
{
    //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
    QItemSelection selection( ui->tableView_7->selectionModel()->selection());

    //Genero un array como todos los indices de los elementos seleccionados
    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append(index.row());
    }

    qSort(rows);

    //elimina elementos seleccionadoss
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            lista_car->removeRows(current,1);
            prev = current;
        }
    }
    lista_car->select();
    desactivarQ();


}

//Agregar Rama
void configuraciones::on_pushButton_3_pressed()
{
    ui->lineEdit_2->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
    op=3;
    desactivarAGR();
    desactivarQ();
    ui->lineEdit_2->setFocus();
}

//Quitar Rama
void configuraciones::on_pushButton_4_pressed()
{
    //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
    QItemSelection selection( ui->tableView_31->selectionModel()->selection());

    //Genero un array como todos los indices de los elementos seleccionados
    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append(index.row());
    }

    qSort(rows);

    //elimina elementos seleccionados
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            lista_ram->removeRows(current,1);
            prev = current;
        }
    }
    lista_ram->select();
    desactivarQ();

}

//Agregar Cat Pag
void configuraciones::on_pushButton_9_pressed()
{
    ui->lineEdit_2->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
    op=4;
    desactivarAGR();
    desactivarQ();
    ui->lineEdit_2->setFocus();
}

//Quitar Cat Pag
void configuraciones::on_pushButton_10_pressed()
{
    //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
    QItemSelection selection( ui->tableView_8->selectionModel()->selection());

    //Genero un array como todos los indices de los elementos seleccionados
    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append(index.row());
    }

    qSort(rows);

    //elimina elementos seleccionados
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            lista_catpag->removeRows(current,1);
            prev = current;
        }
    }
    lista_catpag->select();
    desactivarQ();
}

//Agregar Cat Mat
void configuraciones::on_pushButton_11_pressed()
{
    ui->lineEdit_2->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
    op=5;
    desactivarAGR();
    desactivarQ();
    ui->lineEdit_2->setFocus();
}

//Quitar Cat Mat
void configuraciones::on_pushButton_12_pressed()
{
    //El QitemSelection me permite obtener todas las selecciones que realice sobre de la tabla y de la misma obtener los correspondientes indices.... devuelve como referencia un QModelIndex
    QItemSelection selection( ui->tableView_9->selectionModel()->selection());

    //Genero un array como todos los indices de los elementos seleccionados
    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append(index.row());
    }

    qSort(rows);

    //Comienzo la transacción.
    QSqlDatabase::database().transaction();

    //elimina elementos seleccionados
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            lista_catmat->removeRows(current,1);
            prev = current;
        }
    }

    //confirmo la transacción.
    QSqlDatabase::database().transaction();

    lista_catmat->select();
    desactivarQ();
}

//Guardar
void configuraciones::on_pushButton_21_pressed()
{
    QSqlQuery *inserta = new QSqlQuery();

    //Seteo la transacción.
    QSqlDatabase::database().transaction();

    bool res = false;

    switch (op){
    case 1:
        inserta->prepare("insert into Patrulla (Patrulla) values (:campo)");
        inserta->bindValue(":campo",ui->lineEdit_2->text());
        if(!ui->lineEdit_2->text().isEmpty())
        {
            inserta->exec();
            lista_pat->select();
        }
        else
        {
            Msg->setText("Escriba el nombre de una Patrulla para Ingresar");
            res = Msg->exec();
        }
        break;
    case 2:
        inserta->prepare("insert into Cargo (Cargo) values (:campo)");
        inserta->bindValue(":campo",ui->lineEdit_2->text());
        if(!ui->lineEdit_2->text().isEmpty())
        {
            inserta->exec();
            lista_car->select();
        }
        else
        {
            Msg->setText("Escriba el nombre de un Cargo para Ingresar");
            res = Msg->exec();
        }
        break;
    case 3:
        inserta->prepare("insert into Rama (Rama) values (:campo)");
        inserta->bindValue(":campo",ui->lineEdit_2->text());
        if(!ui->lineEdit_2->text().isEmpty())
        {
            inserta->exec();
            lista_ram->select();
        }
        else
        {
            Msg->setText("Escriba el nombre de una Rama para Ingresar");
            res = Msg->exec();
        }
        break;
    case 4:
        inserta->prepare("insert into Cat_pago (Catpago) values (:campo)");
        inserta->bindValue(":campo",ui->lineEdit_2->text());
        if(!ui->lineEdit_2->text().isEmpty())
        {
            inserta->exec();
            lista_catpag->select();
        }
        else
        {
            Msg->setText("Escriba el nombre de una Categoria de Pago para Ingresar");
            res = Msg->exec();
        }
        break;
    case 5:
        inserta->prepare("insert into Cat_mat (Catmat) values (:campo)");
        inserta->bindValue(":campo",ui->lineEdit_2->text());
        if(!ui->lineEdit_2->text().isEmpty())
        {
            inserta->exec();
            lista_catmat->select();
        }
        else
        {
            Msg->setText("Escriba el nombre de una Categoria de Materiales para Ingresar");
            res = Msg->exec();
        }
        break;
    }


    //Vuelvo la interfaz a la situacion normal
    activarAGR();
    desactivarQ();
    ui->lineEdit_2->clear();
    ui->lineEdit_2->setEnabled(false);
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);

    //Confirmo la transacción.
    QSqlDatabase::database().commit();

    inserta->~QSqlQuery();

    if(res)
    {
        qDebug()<<"Entra";
        switch (op){
        case 1:
            on_pushButton_5_pressed();
            break;
        case 2:
            on_pushButton_7_pressed();
            break;
        case 3:
            on_pushButton_3_pressed();
            break;
        case 4:
            on_pushButton_9_pressed();
            break;
        case 5:
            on_pushButton_11_pressed();
            break;
        }
    }



}

//Cancelar
void configuraciones::on_pushButton_22_pressed()
{
    //Vuelvo la ui a la situacion normal
    activarAGR();
    desactivarQ();
    ui->lineEdit_2->clear();
    ui->lineEdit_2->setEnabled(false);
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);
}


//----------------------------------PESTAÑA EVENTOS--------------------------------------------

//Estructura Evento
void configuraciones::estructura_evento()
{

    //asignacion de los modelos
    evento = new QSqlTableModel();
    evento->setTable("Evento");
    evento->select();

    evento->setHeaderData(0, Qt::Horizontal, tr("Evento"));
    evento->setHeaderData(1, Qt::Horizontal, trUtf8("Descripción"));
    evento->setHeaderData(2, Qt::Horizontal, tr("Monto Unitario"));
    evento->setHeaderData(3, Qt::Horizontal, tr("Ganancia Miembro"));
    evento->setHeaderData(4, Qt::Horizontal, tr("Ganancia Grupo"));
    evento->setHeaderData(5, Qt::Horizontal, tr("Fecha"));

    //Linkeo el modelo a la tableview
    ui->tableView_2->setModel(evento);
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    QSqlQuery *fech = new  QSqlQuery();
    fech->prepare("Select fecha_ini,fecha_fin from fecha_ciclo_contable where cod_fech = 'Actual'");
    //QSqlDatabase::database().transaction();
    fech->exec();
    fech->next();

    ui->dateEdit_3->setMaximumDate(QDate::currentDate());
    if(fech->isValid()){ui->dateEdit_3->setMinimumDate(fech->value(0).toDate());}
    ui->dateEdit_3->setDate(QDate::currentDate());


}

//...........Funciones auxiliares.................................

//LimpiarEVe
void configuraciones::limpiarEVE()
{
    ui->lineEdit_3->clear();
    ui->doubleSpinBox->setValue(0);
    ui->horizontalSlider->setValue(0);
    ui->label_19->setText("0");
    ui->label_21->setText("0");
    QDate dia;
    ui->dateEdit_3->setDate(dia.currentDate());

}

//ActivarEve
void configuraciones::activarEVE()
{
    ui->lineEdit_3->setEnabled(true);
    ui->doubleSpinBox->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->pushButton_13->setEnabled(true);
    ui->pushButton_17->setDisabled(true);
    ui->pushButton_18->setEnabled(true);
    ui->dateEdit_3->setEnabled(true);    
    ui->label_18->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->label_8->setEnabled(true);
    ui->label_20->setEnabled(true);
    ui->label_19->setEnabled(true);
    ui->label_21->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->label_27->setEnabled(true);
}

//DesactivarEve
void configuraciones::desactivarEVE()
{
    ui->lineEdit_3->setEnabled(false);
    ui->doubleSpinBox->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_17->setDisabled(false);
    ui->pushButton_18->setEnabled(false);
    ui->dateEdit_3->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
    ui->label_18->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->label_8->setEnabled(false);
    ui->label_20->setEnabled(false);
    ui->label_19->setEnabled(false);
    ui->label_21->setEnabled(false);
    ui->label_3->setEnabled(false);
    ui->label_27->setEnabled(false);
}

//..........Slots..................................................

//Guardar
void configuraciones::on_pushButton_13_pressed()
{


    if(Edicion_Evento != 1){

        if(ui->doubleSpinBox->value() != 0 and !ui->lineEdit_3->text().isEmpty())
        {
            QString eventual;
            ConectarconBD::cod_setup("Evento",eventual);

            QSqlDatabase::database().transaction();

            eve.guardar(eventual,ui->doubleSpinBox->value(),ui->label_19->text().toDouble(),ui->label_21->text().toDouble(),ui->lineEdit_3->text(),ui->dateEdit_3->date());
            eve.almacenar_BD();

            QSqlDatabase::database().commit();

            limpiarEVE();
            desactivarEVE();

            //Actualizo el modelo
            evento->select();

            //Activo la tabla
            ui->tableView_2->setEnabled(true);

        }

        else
        {
            QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Error al intentar ingresar un Campo.\n""Complete los campos para una correcta edicion del Evento.\n""No pueden quedar campos imcompletos.", 0, QApplication::UnicodeUTF8),QMessageBox::Ok);
        }
    }


    else
    {

        if(ui->doubleSpinBox->value() != 0 and !ui->lineEdit_3->text().isEmpty()){

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
            boton->setText("¿Esta seguro que desea Editar este elemento?.");

            //Asigno los iconos correspondientes
            boton->setWindowIcon(QPixmap(":/icons/gags"));
            boton->setIcon(QMessageBox::Warning);

            //Seteo los botones Si y No en el QMessageBox
            boton->addButton(botonSI,QMessageBox::YesRole);
            boton->addButton(botonNO,QMessageBox::NoRole);

            boton->exec();

            if(boton->clickedButton() == botonSI)
            {

                eve.guardar(Cod_Evento_Actual,ui->doubleSpinBox->value(),ui->label_19->text().toDouble(),ui->label_21->text().toDouble(),ui->lineEdit_3->text(),ui->dateEdit_3->date());

                QSqlDatabase::database().commit();

                eve.actualizar_BD();

                QSqlDatabase::database().transaction();

                limpiarEVE();
                desactivarEVE();

                //actualizo el modelo
                evento->select();

                //Activo la tabla
                ui->tableView_2->setEnabled(true);

            }

            else
            {
                limpiarEVE();
                desactivarEVE();

                //Activo la tabla
                ui->tableView_2->setEnabled(true);
            }

        }

        else
        {
            QMessageBox::information(this,"CUIDADO",QApplication::translate("QDlgLoginForm","Complete los campos para una correcta edicion del Evento.\n""No pueden quedar campos imcompletos.", 0, QApplication::UnicodeUTF8),QMessageBox::Ok);
        }
    }
}

//Activar / Agregar nuevo Evento
void configuraciones::on_pushButton_17_pressed()
{
    activarEVE();
    Edicion_Evento = 0;

    ui->lineEdit_3->setFocus();

    //Desactivo la tabla
    ui->tableView_2->setEnabled(false);

}

//Quitar
void configuraciones::on_pushButton_14_pressed()
{
    limpiarEVE();
    desactivarEVE();

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
    boton->setText("¿Esta seguro que desea eliminar este elemento?.");

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
        QItemSelection selection( ui->tableView_2->selectionModel()->selection());

        //Genero un array como todos los indices de los elementos seleccionados
        QList<int> rows;
        QList<QString> codigo_eventos;
        foreach( const QModelIndex & index, selection.indexes() ) {
            rows.append(index.row());
            codigo_eventos.append(index.sibling(index.row(),0).data().toString());

        }

        qSort(rows);

        QSqlDatabase::database().transaction();

        //elimina elementos seleccionados
        int prev = -1;
        for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
            int current = rows[i];
            if( current != prev ) {
                evento->removeRows(current,1);
                prev = current;

                //Llamo al metodo correspondiente en Evento para borrar en Contable_Grupo todo el resto de los eventos, tanto de miembro como de CG.
                QString borrar_evento = codigo_eventos[i];
                eve.borrar_evento(borrar_evento);

            }
        }

        QSqlDatabase::database().commit();

    }

    boton->~QMessageBox();
}

//Cancelar
void configuraciones::on_pushButton_18_pressed()
{
    limpiarEVE();
    desactivarEVE();

    //Activo la tabla
    ui->tableView_2->setEnabled(true);

}

//Edicion de Evento - Doble_Click
void configuraciones::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    int indice = index.row();

    limpiarEVE();
    activarEVE();

    ui->pushButton_14->setEnabled(false);
    ui->pushButton_17->setEnabled(false);

    Edicion_Evento = 1;
    Cod_Evento_Actual = index.sibling(indice,0).data().toString();

    ui->lineEdit_3->setText(index.sibling(indice,1).data().toString()); //Descripcion

    double monto = index.sibling(indice,2).data().toDouble();
    double ganancia = monto - index.sibling(indice,4).data().toDouble();

    ui->horizontalSlider->setValue(ganancia / monto * 100); //Calculo ganacia miembro y Grupo

    ui->dateEdit_3->setDate(index.sibling(indice,5).data().toDate()); //dia
    ui->doubleSpinBox->setValue(monto); //monto

    //Desactivo la tabla
    ui->tableView_2->setEnabled(false);

}

//Al presionar sobre la tabla se activa el boton quitar
void configuraciones::on_tableView_2_pressed(const QModelIndex &)
{
    ui->pushButton_14->setEnabled(true);
}

//Configuracion del Slider de la pestaña Eventos
void configuraciones::on_horizontalSlider_valueChanged(int value)
{
    double gan_miem, gan_grupo, valor_tot = ui->doubleSpinBox->value();

    //Calculo la ganancia del miembro y por consiguiente la de Grupo
    gan_miem = value * valor_tot / 100;
    gan_grupo = valor_tot - gan_miem;

    ui->label_19->setNum(gan_miem);
    ui->label_21->setNum(gan_grupo);
}

//Retroalimentacion entre el doblespinbox de eventos y el slider
void configuraciones::on_doubleSpinBox_valueChanged(double valor_tot)
{
    ui->tableView_2->setEnabled(false);

    double gan_miem, gan_grupo;
    int value = ui->horizontalSlider->value();

    //Calculo la ganancia del miembro y por consiguiente la de Grupo
    gan_miem = value * valor_tot / 100;
    gan_grupo = valor_tot - gan_miem;

    ui->label_19->setNum(gan_miem);
    ui->label_21->setNum(gan_grupo);
}

//----------------------------------PESTAÑA CICLO CONTABLE-----------------------------------------

void configuraciones::asignar_fechas()
{
    QSqlQuery *fech = new  QSqlQuery();
    fech->prepare("Select fecha_ini,fecha_fin from fecha_ciclo_contable where cod_fech = 'Actual'");
    //QSqlDatabase::database().transaction();
    fech->exec();
    fech->next();

    if(!fech->isValid())
    {
        //QSqlDatabase::database().commit(); //finalizo la transaccion.

        ui->dateEdit->setDate(QDate::currentDate()); //le asigno el currentDate.
        ui->dateEdit_2->setDate(QDate::currentDate()); //le asigno el currentDate.
        desactivar_finciclo();
        desactivar_interfaz();
        desactivar_impresion();

    }
    else
    {
        if(fech->value(1).toDate().isNull())
        {
            ui->dateEdit->setDate(fech->value(0).toDate()); //asigno la fecha_ini a la fecha_ini. (No es nesesario pero bueno queda).
            ui->dateEdit_2->setDate(QDate::currentDate()); // No fue finalizado el ciclo contable asigno fecha actual a fecha_fin.
            activar_finciclo();
            activar_interfaz();
            activar_impresion();

        }
        else
        {
            ui->dateEdit->setDate(QDate::currentDate()); //le asigno el currentDate.
            ui->dateEdit_2->setDate(QDate::currentDate()); //le asigno el currentDate.
            desactivar_finciclo();
            desactivar_interfaz();
            activar_impresion();

        }

        //QSqlDatabase::database().commit();// termine las asignaciones cierro la transaccion.
    }
}

//...........Funciones auxiliares.................................

//Funcion para desactivar el inicio de ciclo y activar el fin
void configuraciones::activar_finciclo()
{
    //Activo el fin de ciclo
    ui->dateEdit_2->setEnabled(true);
    ui->pushButton_20->setEnabled(true);
    ui->label_17->setEnabled(true);
    //Desactivo el inicio del ciclo
    ui->dateEdit->setEnabled(false);
    ui->pushButton_19->setEnabled(false);
    ui->label_16->setEnabled(false);

}

//Funcion para activar el inicio de ciclo y desactivar el fin
void configuraciones::desactivar_finciclo()
{
    //Activo el fin de ciclo
    ui->dateEdit_2->setEnabled(false);
    ui->pushButton_20->setEnabled(false);
    ui->label_17->setEnabled(false);
    //Desactivo el inicio del ciclo
    ui->dateEdit->setEnabled(true);
    ui->pushButton_19->setEnabled(true);
    ui->label_16->setEnabled(true);
}

void configuraciones::nuevo_ciclo()
{
    //Calculo y guardo los saldos en los miembros y los cambio a miembros inactivos pero borro el anterior
    QSqlQuery *nuevociclo = new QSqlQuery();

    QSqlDatabase::database().transaction();   //Inicio el proceso para un nuevo_ciclo.

    //Primero trunco el saldo en todos los miembros Inactivos
    nuevociclo->prepare("UPDATE Miembro SET Saldo = 0 WHERE Activo IS NOT 'Activo'");
    nuevociclo->exec();

    //Recalculo los saldos
    nuevociclo->prepare("SELECT distinct(Cod_Miembro) FROM Miembro WHERE Activo IS 'Activo'");
    nuevociclo->exec();

    while(nuevociclo->next())
    {
        QString cod=nuevociclo->value(0).toString();

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

        //Sumo el saldo del año anterior
        calc->prepare("SELECT Saldo FROM Miembro WHERE Cod_Miembro = :cod");
        calc->bindValue(":cod",cod);
        calc->exec();
        calc->next();

        double Saldo_ant=calc->value(0).toDouble();

        double Saldo=Ingr+Saldo_ant-Total_event-Total_cos;

        //Actualizo el Saldo
        calc->prepare("UPDATE Miembro SET Saldo = :sal WHERE Cod_Miembro = :cod");
        calc->bindValue(":sal",Saldo);
        calc->bindValue(":cod",cod);
        calc->exec();

        //Pongo inactivo el miembro
        calc->prepare("UPDATE Miembro SET Activo = 'Inactivo' WHERE Cod_Miembro = :cod");
        calc->bindValue(":cod",cod);
        calc->exec();

    }

    //Trunco la tabla Historico_contable.
    nuevociclo->prepare("DELETE FROM Historico_Contable_Grupo");
    nuevociclo->exec();

    //Guardo la tabla de contable grupo en la de historico
    nuevociclo->prepare("INSERT INTO Historico_Contable_Grupo SELECT * FROM Contable_Grupo");
    nuevociclo->exec();

    //Reinicio los codigos en la tabla setup para contable grupo evento y costos

    nuevociclo->prepare("UPDATE Setup SET ultimo_cod = 1 where nom_tabla = 'Contable_Grupo' and nom_tabla = 'Costos' and nom_tabla = 'Evento'");
    nuevociclo->exec();

    //Trunco las tablas
    nuevociclo->prepare("DELETE FROM Contable_Grupo");
    nuevociclo->exec();
    nuevociclo->prepare("DELETE FROM Costos");
    nuevociclo->exec();
    nuevociclo->prepare("DELETE FROM Evento");
    nuevociclo->exec();

    //Guardo los numeros finales del año scout anterior
    nuevociclo->prepare("SELECT SUM(Ingreso),SUM(Egreso) FROM Historico_Contable_Grupo"); //Sumo la totalidad de haber y deber del año anterior sino seria las cosas del grupo WHERE Rama IS 'GRUPO'
    nuevociclo->exec();
    nuevociclo->next();

    double Ing_ant=nuevociclo->value(0).toDouble();
    double Egr_ant=nuevociclo->value(1).toDouble();

    QString Cod_CON;
    ConectarconBD::cod_setup("Contable_Grupo",Cod_CON);

    nuevociclo->prepare("INSERT INTO Contable_Grupo (Cod_CON,Ingreso,Egreso,Rama,motivo,tipo_motivo,categoria,fecha_ingreso) values(:cod,:ing,:egr,'Grupo','Saldo Ciclo Contable Anterior','Ingreso','General',:fecha)");
    nuevociclo->bindValue(":fecha",QDate::currentDate());
    nuevociclo->bindValue(":cod",Cod_CON);
    nuevociclo->bindValue(":ing",Ing_ant);
    nuevociclo->bindValue(":egr",Egr_ant);
    nuevociclo->exec();

    QSqlDatabase::database().commit(); //Finalizo el ciclo de un nuevo_ciclo.

    //Luego de iniciar hago un select sobre las tablas costos y evento. Deben de aparecen vacios instántaneamente.
    costos->select();
    evento->select();

    nuevociclo->~QSqlQuery();
}

void configuraciones::desactivar_interfaz()
{
    ui->tableView_2->setEnabled(false);//table evento
    ui->pushButton_17->setEnabled(false);//nuevo evento
    ui->tableView->setEnabled(false);// table costos
    ui->pushButton->setEnabled(false);//agregar costo
    ui->pushButton_2->setEnabled(false);//quitar costo
    ui->label->setEnabled(false); //label descripcion
    ui->label_2->setEnabled(false); // label monto
}

void configuraciones::activar_interfaz()
{
    ui->tableView_2->setEnabled(true);//table evento
    ui->pushButton_17->setEnabled(true);//nuevo evento
    ui->tableView->setEnabled(true);// table costos
    ui->pushButton->setEnabled(true);//agregar costo
    ui->pushButton_2->setEnabled(true);//quitar costo
    ui->label->setEnabled(true); //label descripcion
    ui->label_2->setEnabled(true); // label monto
}

void configuraciones::desactivar_impresion()
{
    ui->label_26->setEnabled(false);
    ui->pushButton_23->setEnabled(false);
    ui->pushButton_24->setEnabled(false);

}

void configuraciones::activar_impresion()
{

    QSqlQuery *fech = new  QSqlQuery();
    fech->prepare("Select fecha_ini from fecha_ciclo_contable where cod_fech = 'Anterior'");
    QSqlDatabase::database().transaction();
    fech->exec();
    fech->next();

    if(fech->isValid())
    {
    ui->label_26->setEnabled(true);
    ui->pushButton_23->setEnabled(true);
    ui->pushButton_24->setEnabled(true);
    QSqlDatabase::database().commit();
    fech->~QSqlQuery();

    }
    else
    {
        ui->label_26->setEnabled(false);
        ui->pushButton_23->setEnabled(false);
        ui->pushButton_24->setEnabled(false);
    }
}

void configuraciones::pes_uno()
{
    ui->Ciclocontable->setCurrentIndex(0);
}

//..........Slots..................................................

//Boton de inicio de ciclo
void configuraciones::on_pushButton_19_pressed()
{
    //Inserto en la tabla de fechas
    QSqlQuery *fech = new QSqlQuery();

    //primero verifico si no existe ninguna fila.
    fech->prepare("select * from fecha_ciclo_contable");

    QSqlDatabase::database().transaction();
    fech->exec();
    fech->next();


    if(!fech->isValid())
    {
        QSqlDatabase::database().commit();
        //No existe insercion previa. Inserto nuevo valor

        fech->prepare("INSERT INTO fecha_ciclo_contable (cod_fech,fecha_ini) values('Actual',:fecha)");
        fech->bindValue(":fecha",ui->dateEdit->date());

        QSqlDatabase::database().transaction();
        fech->exec();
        QSqlDatabase::database().commit();       

    }
    else
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
        boton->setText("¿Esta seguro que desea iniciar un nuevo Año Scout?.\n""Se van a borrar datos con esta operación.");

        //Asigno los iconos correspondientes
        boton->setWindowIcon(QPixmap(":/icons/gags"));
        boton->setIcon(QMessageBox::Critical);

        //Seteo los botones Si y No en el QMessageBox
        boton->addButton(botonSI,QMessageBox::YesRole);
        boton->addButton(botonNO,QMessageBox::NoRole);

        //Ejecuto el QMessageBox Creado.
        boton->exec();

        if(boton->clickedButton() == botonSI)
        {
            //Existe una fila. Borro preventivamente el valor con codigo cod_fech = 'Anterior' si existe.

            fech->prepare("DELETE from fecha_ciclo_contable where cod_fech = 'Anterior'");

            QSqlDatabase::database().transaction();
            fech->exec();
            QSqlDatabase::database().commit();

            //Actualizo el cod_fech = 'Actual' ---> cod_fech = 'Anterior'.

            fech->prepare("UPDATE fecha_ciclo_contable set cod_fech ='Anterior' where cod_fech = 'Actual'");

            QSqlDatabase::database().transaction();
            fech->exec();
            QSqlDatabase::database().commit();

            //Ingreso la fecha del nuevo ciclo contable.

            fech->prepare("INSERT INTO fecha_ciclo_contable (cod_fech,fecha_ini) values('Actual',:fecha)");
            fech->bindValue(":fecha",ui->dateEdit->date());

            QSqlDatabase::database().transaction();
            fech->exec();
            QSqlDatabase::database().commit();


            //Aca tendria que hacer los truncate en las tablas desactivar todos los miembros y hacer los calculos de el año anterior
            nuevo_ciclo(); //reinicio el nuevo ciclo.

            activar_interfaz();            
        }

    }

    //Activo el fin de ciclo
    activar_finciclo();
    asignar_fechas();
    fech->~QSqlQuery();

    //Aca van los metodos para inicializar el nuevo ciclo contable. Truncado, copiado y actualizacion de tablas.
}

//Boton de fin de ciclo
void configuraciones::on_pushButton_20_pressed()
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
    boton->setText("¿Esta seguro que desea finalizar el Año Scout actual?.\n""Con esta acción no se podran realizar nuevas inserciones al mismo.");

    //Asigno los iconos correspondientes
    boton->setWindowIcon(QPixmap(":/icons/gags"));
    boton->setIcon(QMessageBox::Critical);

    //Seteo los botones Si y No en el QMessageBox
    boton->addButton(botonSI,QMessageBox::YesRole);
    boton->addButton(botonNO,QMessageBox::NoRole);

    //Ejecuto el QMessageBox Creado.
    boton->exec();

    if(boton->clickedButton() == botonSI)
    {
        //Activo el inicio
        desactivar_finciclo();

        //Inserto en la tabla de fechas
        QSqlQuery *fech = new QSqlQuery();
        fech->prepare("UPDATE fecha_ciclo_contable set fecha_fin = :fecha_fin where cod_fech is 'Actual'");
        fech->bindValue(":fecha_fin",ui->dateEdit_2->date());

        QSqlDatabase::database().transaction();
        fech->exec();
        QSqlDatabase::database().commit();

        desactivar_interfaz();

        fech->~QSqlQuery();

    }

    boton->~QMessageBox();
}

//Boton de Impresion
void configuraciones::on_pushButton_23_pressed()
{
    //Hay que hacer una modificacion sobre el codigo de la funcion impresion de contable grupo
    QSqlTableModel *hist=new QSqlTableModel();
    hist->setTable("Historico_Contable_Grupo");
    hist->select();

    hist->setHeaderData(1, Qt::Horizontal, tr("Descripcion"));
    hist->setHeaderData(2, Qt::Horizontal, tr("Motivo"));
    hist->setHeaderData(3, Qt::Horizontal, tr("Ingreso"));
    hist->setHeaderData(4, Qt::Horizontal, tr("Egreso"));
    hist->setHeaderData(5, Qt::Horizontal, tr("Fecha"));
    hist->setHeaderData(6, Qt::Horizontal, tr("Categoria"));


    QTableView *Aux = new QTableView();
    Aux->setModel(hist);

    Aux->hideColumn(0);
    Aux->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    Aux->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);

    //oculto dateEdit_6 las columnas que no me son relevantes en el modelo Contable_grupo

    Aux->hideColumn(7);
    Aux->hideColumn(8);
    Aux->hideColumn(12); //cantidad
    Aux->hideColumn(13); //CON_evento
    Aux->hideColumn(14); //debe_miembro
    Aux->hideColumn(15); //gan_mie
    Aux->hideColumn(16); //gan_grup

    //Impresion en Si

    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = hist->rowCount();
    const int columnCount = hist->columnCount();


    QSqlDatabase::database().transaction(); // Busco las fechas desde y hasta definido para el ciclo anterior.

    QSqlQuery *fecha = new QSqlQuery();
    fecha->prepare("Select fecha_ini,fecha_fin from fecha_ciclo_contable where cod_fech = 'Anterior'");
    fecha->exec();
    fecha->next();

    out << "<h3>Informe Ciclo Contable Anterior</h3>";
    out << "<h4>Desde : " + fecha->value(0).toDate().toString() + " - Hasta : " + fecha->value(1).toDate().toString() + "</h4><br>";
    out << "<table border=2 cellspacing=0 cellpadding=1>";

     QSqlDatabase::database().commit();

    // headers
    out << "<tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++){
        if (!Aux->isColumnHidden(column))
        {

            if(column == 2 or column == 3 or column == 4 or column == 5 or column == 6 or column == 9)
            {
                //Con esta asignacion de 8.5% les alcanza y les sobra para estos campos.
                out << QString("<th width=8.5% align=center%>%1</th>").arg(hist->headerData(column, Qt::Horizontal).toString());
            }

            if(column ==10 or column ==11)
            {
                //Le asigno al menos 12% al nombre y apellido para que se vean bien en la impresion de la ficha.
                out << QString("<th width=12% align=center%>%1</th>").arg(hist->headerData(column, Qt::Horizontal).toString());

            }
            if(column ==1)
            {
                //Le asigno un 25% a la descripcion ya que es el texto mas largo de la ficha.
                out << QString("<th width=25% align=center%>%1</th>").arg(hist->headerData(column, Qt::Horizontal).toString());

            }
        }
    }

    out << "</tr>";

    // data table

    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!Aux->isColumnHidden(column)) {

                QString data = hist->data(hist->index(row, column)).toString().simplified();
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

    QSqlDatabase::database().transaction();

    fecha->prepare("select sum(Ingreso),sum(Egreso) from historico_contable_grupo");
    fecha->exec();
    fecha->next();

    out<<"<h4><table align=right cellpadding=2>";
    out<<"<tr>";
    out<<"<td align=left colspan=2><b>Haber:<b> " + QString::number(fecha->value(0).toDouble()) + " </td>";
    out<<"<td align=left colspan=2><b>Deber:<b> " + QString::number(fecha->value(1).toDouble()) + " </td>";
    out<<"<td align=left colspan=2><b>Neto:<b> " + QString::number(fecha->value(0).toDouble() - fecha->value(1).toDouble()) + " </td>";
    out<< "</h4></tr>";
    out<< "</table><br>";

    QSqlDatabase::database().commit(); //finalizo para liberar memoria.
    fecha->finish();

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;

    hist->~QSqlTableModel();
    Aux->~QTableView();

}

//Boton de exportar a CVS
void configuraciones::on_pushButton_24_pressed()
{
    //modelo y table view auxiliares

    QSqlTableModel *hist=new QSqlTableModel();
    hist->setTable("Historico_Contable_Grupo");
    hist->select();

    hist->setHeaderData(1, Qt::Horizontal, tr("Descripcion"));
    hist->setHeaderData(2, Qt::Horizontal, tr("Motivo"));
    hist->setHeaderData(3, Qt::Horizontal, tr("Ingreso"));
    hist->setHeaderData(4, Qt::Horizontal, tr("Egreso"));
    hist->setHeaderData(5, Qt::Horizontal, tr("Fecha"));
    hist->setHeaderData(6, Qt::Horizontal, tr("Categoria"));


    QTableView *Aux = new QTableView();
    Aux->setModel(hist);

    Aux->hideColumn(0);
    Aux->hideColumn(7);
    Aux->hideColumn(8); //Cod_Mie
    Aux->hideColumn(12); //cantidad
    Aux->hideColumn(13); //CON_evento
    Aux->hideColumn(14); //debe_miembro
    Aux->hideColumn(15); //gan_mie
    Aux->hideColumn(16); //gan_grup

    //Funcion sacada de los foros de QT
    QString linki = QFileDialog::getSaveFileName(this, tr("Export CSV"),
                                                 QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)+"/Historico Contable.csv",
                                                 tr("Comma Separated Values (*.csv)"));

    int x = 0;
    QString exportdata;

    int counthidden=0, jint = 0;

    while (jint < hist->columnCount()) {

        counthidden+=Aux->isColumnHidden(jint);
        jint++;
    }


    int w = 1;
    while (x < hist->columnCount()){

        if (!Aux->isColumnHidden(x)) {


            exportdata.append(hist->headerData(x,Qt::Horizontal,Qt::DisplayRole).toString());


            if (hist->columnCount() - w > counthidden)
                exportdata.append(";");
            else {
                exportdata.append("\n");

            }
            w++;
        }
        x++;

    }

    int z = 0;

    w = 1;
    while (z < hist->rowCount()) {

        x = 0;

        w = 1;
        while (x < hist->columnCount()) {
            if (!Aux->isColumnHidden(x)) {

                if(hist->data(hist->index(z,x),Qt::DisplayRole).isNull())
                {
                    exportdata.append("-");
                }
                else
                {
                    exportdata.append(hist->data(hist->index(z,x),Qt::DisplayRole).toString());
                }


                if (hist->columnCount() - w > counthidden)
                    exportdata.append(";");
                else
                    exportdata.append("\n");

                w++;
            }
            x++;

        }

        z++;
    }





    QFile file;
    if (!linki.isEmpty()) {
        file.setFileName(linki);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    }
    QByteArray ttext;
    ttext.append(exportdata);
    file.write(ttext);

    hist->~QSqlTableModel();
}


