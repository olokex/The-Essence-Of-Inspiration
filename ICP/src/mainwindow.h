/**
 * @file mainwindow.h
 * @author xsedla1e
 * @author xsedla1d
 * @date 11 May 2019
 * @brief Hlavičkový soubor k hlavnímu oknu.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "chessgame.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    /**
     * @brief      Pokud se klikne na novou hru, tak se otevře nový tab.
     */
    void on_newGameButton_clicked() {
        gameNumber++;
        ui->tabWidget->addTab(new ChessGame, "Hra " + QString::number(gameNumber));
    }

    /**
     * @brief      Zavření tabu.
     * 
     * Pokud se klikne na "zavřít", tak se daný tab, který je aktivní uzavře.
     */
    void on_closeGameButton_clicked() {
        int current = ui->tabWidget->currentIndex();
        if (current != -1) {
            ui->tabWidget->removeTab(current);
        }
    }

private:
    Ui::MainWindow *ui;
    int gameNumber = 0;
};

#endif // MAINWINDOW_H
