/**
 * @file chessgame.cpp
 * @author xsedla1e
 * @author xsedla1d
 * @date 11 May 2019
 * @brief Šachy...
 */
#include "chessgame.h"
#include "ui_chessgame.h"

/**
 * @brief      Vytvoření jedné šachové partie.
 *
 * To se potom naskládá do "tabů", tak aby bylo možno hrát více naráz.
 *
 * @param      parent  Ukazatel na rodiče dané šachové hry.
 */
ChessGame::ChessGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessGame)
{
    ui->setupUi(this);
    ui->turnLabel->setFont(QFont("Comic Sans MS", 24));
    chessInit();
}

ChessGame::~ChessGame()
{
    delete ui;
}

