/**
 * @file main.cpp
 * @author xsedla1e
 * @author xsedla1d
 * @date 11 May 2019
 * @brief Main soubor.
 */

#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /// Načtení fontu comic sans.
    QFile fontFile(":/resources/COMIC.TTF");
    fontFile.open(QFile::ReadOnly);
    int id = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
    fontFile.close();

    /// Vyvolání daného okna pro celou aplikaci.
    MainWindow w;
    w.show();
    return app.exec();
}
