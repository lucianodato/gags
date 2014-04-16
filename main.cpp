#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QSplashScreen>
#include <QPixmap>
#include <QBitmap>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->addLibraryPath( qApp->applicationDirPath() + "/plugins");

    //SPLASH SCREEN
    QPixmap splash(":/icons/splash");
    QSplashScreen *hola = new QSplashScreen();
    hola->setPixmap(splash.scaled(500,500,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    hola->setMask(splash.scaled(500,500,Qt::KeepAspectRatio,Qt::SmoothTransformation).mask());
    hola->show();
    QTimer::singleShot(2000, hola, SLOT(hide()));

    MainWindow w;
    w.show();
    hola->finish(&w);
    return a.exec();

}
