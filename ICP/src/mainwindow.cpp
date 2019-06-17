/**
 * @file mainwindow.cpp
 * @author xsedla1e
 * @author xsedla1d
 * @date 11 May 2019
 * @brief Soubor k hlavnímu oknu.
 */
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameNumber++;
    ui->tabWidget->addTab(new ChessGame, "Hra " + QString::number(gameNumber));

}

MainWindow::~MainWindow()
{
    delete ui;
}

