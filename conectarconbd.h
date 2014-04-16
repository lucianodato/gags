#ifndef CONECTARCONBD_H
#define CONECTARCONBD_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlTableModel>
#include <QtGui/QApplication>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>

class ConectarconBD
{
    public:

    ConectarconBD();
    ~ConectarconBD();

    QSqlDatabase *db;
    bool crearestructura();
    static void config_setup_table();
    static bool check_setup();
    static void cod_setup(QString,QString&);
    static void actualizar_setup(QString,int);

};

#endif // CONECTARCONBD_H
