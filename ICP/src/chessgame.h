/**
 * @file chessgame.h
 * @author xsedla1e
 * @author xsedla1d
 * @date 11 May 2019
 * @brief Hlavičkový soubor k šachám.
 * 
 * Tento soubor slouží téměr k veškeré logice/vykreslování.
 * Všechna logika šachů je oddělena středníkem a prázdnými řádky.
 * Veškeré vykreslovací prvky, které nám poskytlo Qt je taktéž odděleno středníky.
 */

#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <array>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QRegularExpression>
#include <QTextStream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "ui_chessgame.h"

namespace Ui {
class ChessGame;
}

/// Pomocná proměnná pro šířku políčka (64 px).
constexpr int PIECE_WIDTH = 64;
/// Pomocná proměnná pro výšku políčka (64 px).
constexpr int PIECE_HEIGHT = 64;
/// Pomocná proměnná, počet řádků šachovnice.
constexpr int BOARD_ROWS = 8;
/// Pomocná proměnná, počet sloupců šachovnice.
constexpr int BOARD_COLS = 8;


/**
 * @brief Pomocný enum pro lepší práci s jednotlivými figurkami.
 */
enum class ChessPiece {
    None = 0,
    Pawn = 1,
    Rook = 2,
    Knight = 3,
    Bishop = 4,
    Queen = 5,
    King = 6,
    Count = 7
};

/**
 * @brief Pomocný enum pro usnadnění rozpoznávání dané barvy.
 */
enum class PlayerColor {
    None,
    Black,
    White
};


/**
 * @brief Cesty k jednotlivým obrázkům figurek (černé).
 */
constexpr const char *PIECE_IMAGES_BLACK[] = {
    nullptr,
    ":/resources/b_pawn.png",
    ":/resources/b_rook.png",
    ":/resources/b_knight.png",
    ":/resources/b_bishop.png",
    ":/resources/b_queen.png",
    ":/resources/b_king.png",
};

/**
 * @brief Cesty k jednotlivým obrázkům figurek (bílé).
 */
constexpr const char *PIECE_IMAGES_WHITE[] = {
    nullptr,
    ":/resources/w_pawn.png",
    ":/resources/w_rook.png",
    ":/resources/w_knight.png",
    ":/resources/w_bishop.png",
    ":/resources/w_queen.png",
    ":/resources/w_king.png",
};

/**
 * @brief Class nebo Struct (kdo ví), která uchovává barvu a druh figurky
 * 
 */
struct ChessFigure {
   PlayerColor color = PlayerColor::None;
   ChessPiece piece = ChessPiece::None;
   ChessFigure() {}
   ChessFigure(PlayerColor color, ChessPiece piece) : color(color), piece(piece) {}
   void unset() { color = PlayerColor::None; piece = ChessPiece::None; }
   bool isSet() { return color != PlayerColor::None && piece != ChessPiece::None; }
};

/**
 * @brief Teď se už opravdu jedná o class, dědí z QGraphicsObject
 */
class ChessboardTile : public QGraphicsObject {
    Q_OBJECT
public:
    /// Barva "štětce".
    QColor brushColor;

    ChessFigure figure;
    /// Výchozí hodnota false jestli se jedná o vybrané políčko
    bool selected = false;
    /// Výchozí hodnota false jestli se jedná o validní tah (využíváno u napovídání, kam se může táhnout).
    bool validMove = false;

    ChessboardTile() : QGraphicsObject(nullptr) {}

    /**
     * @brief Získání obrázku pro danou figurku
     *
     * @return Vrací daný načtený obrázek
     */
    QImage getImage()
    {
        assert(figure.isSet());
        if (figure.color == PlayerColor::White) {
            return QImage(PIECE_IMAGES_WHITE[static_cast<int>(figure.piece)]);
        } else {
            return QImage(PIECE_IMAGES_BLACK[static_cast<int>(figure.piece)]);
        }
    }

    /**
     * @brief      Vykreslení daného čtverečku šachovnice a "připlácnutí obrázku figurky".
     *
     * @param      painter  The painter
     * @param[in]  option   The option
     * @param      widget   The widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setBrush(brushColor);
        painter->drawRect(0, 0, PIECE_WIDTH, PIECE_HEIGHT);

        /// Pokud je nastaveno dané políčko, nastaví se obrázek figurky a ten se následně vycentruje
        if (figure.isSet()) {
            QImage image = getImage();
            image = image.scaled(PIECE_WIDTH - 10, PIECE_HEIGHT - 10, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter->drawImage(5, 5, image);
        }

        /// Pokud je dané políčko vybráno zabarvý se do žluta
        if (selected) {
            painter->setBrush(QBrush(QColor(255, 255, 0, 100)));
            painter->drawRect(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
        }

        /// Pokud je dané políčko validním tahem je zabarven do zelené
        if (validMove) {
            painter->setBrush(QBrush(figure.isSet() ? QColor(255, 0, 0, 100) : QColor(0, 255, 0, 100)));
            painter->drawRect(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
        }
    }

    /**
     * @brief      Vymezení plochy pro políčko
     *
     * @return     Vrací objekt na políčko
     */
    QRectF boundingRect() const override
    {
        return QRectF(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
    }


    /**
     * @brief      Zničí objekt.
     */
    ~ChessboardTile() override {}


    /**
     * @brief      Pokud se kliklo levým myšítkem.
     *
     * @param      event  Daný event co se odchytí.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton) {
            emit ChessboardTile::clicked(event);
        }
        //QGraphicsObject::mousePressEvent(event);
    }

/// magie
signals:
    void clicked(QGraphicsSceneMouseEvent *event);
};

/**
 * @brief      Třída která podědí grafickou scénu a následně i destrujue.
 */
class Chessboard : public QGraphicsScene {
public:
    Chessboard() : QGraphicsScene() {}
    ~Chessboard() override {}
};

/**
 * @brief      Pomocný enum na stav jestli je vybráno nebo se udělal tah.
 */
enum class GameState {
    Ready,
    Selected,
};

/**
 * @brief      Struktura, která tvojí snapshot celého herního pole, tak jak je v současném stavu
 */
struct GameSnapshot {
    PlayerColor playerTurn;
    ChessFigure grid[BOARD_ROWS][BOARD_COLS];
};

/**
 * @brief      Skromně řečeno, celá hra.
 */
class ChessGame : public QWidget
{
    Q_OBJECT

public:
    explicit ChessGame(QWidget *parent = nullptr);
    ~ChessGame();

private:
    /// Pomocný _map_ pro převod písmen na dané políčko šachovnice.
    const std::map<std::string, ChessPiece> letterToPiece = {
        { "K", ChessPiece::King },
        { "D", ChessPiece::Queen },
        { "V", ChessPiece::Rook },
        { "S", ChessPiece::Bishop },
        { "J", ChessPiece::Knight },
        { "p", ChessPiece::Pawn },
        { "", ChessPiece::Pawn },
    };

    /// Pomocný _map_ pro převod políčka na dané písmeno.
    const std::map<ChessPiece, QString> pieceToLetter = {
        { ChessPiece::King, "K" },
        { ChessPiece::Queen, "D" },
        { ChessPiece::Rook, "V" },
        { ChessPiece::Bishop, "S" },
        { ChessPiece::Knight, "J" },
        { ChessPiece::Pawn, "p" },
    };

    Ui::ChessGame *ui;

    /// Proměnná jestli se právě automaticky přehrává.
    bool autoplayInProgress = false;
    QTimer *autoplayTimer;

    /// Grid políček šachovnice.
    ChessboardTile *grid[BOARD_ROWS][BOARD_COLS];
    /// Jedna daná šachovnice.
    Chessboard *chessboard;
    /// V jakém stavu je právě daná hra.
    GameState state = GameState::Ready;
    /// Pomocná proměnná na výpis, který hráč hraje.
    PlayerColor playerTurn = PlayerColor::White;
    /// Pomocné proměnné na to, které pole je zrovna vybráno.
    int selectedX, selectedY;
    int historyIndex = -1;
    /// Vektor pro přehrávání historie + undo/redo funkce.
    std::vector<GameSnapshot> history;

    /**
     * @brief      Znova vykreslí šachovnici (po pohybu figurek).
     */
    void updateBoard() {
        ui->chessBoardView->update();
        chessboard->update();
    }

    /**
     * @brief      Uloží vždy po kliknutí danou hru.
     *
     * @return     Vrací daný snapshot daného okamžiku.
     */
    GameSnapshot saveGameState() {
        GameSnapshot snap;
        for (int y = 0; y < BOARD_ROWS; y++) for (int x = 0; x < BOARD_COLS; x++) {
            snap.grid[y][x] = grid[y][x]->figure;
        }
        snap.playerTurn = playerTurn;
        return snap;
    }

    /**
     * @brief      Načte hru, tak jak je zachyceno v snapshotu.
     *
     * @param      snap  Bere ukazatel na jeden daný snapshot daného okamžiku.
     */
    void loadGameState(GameSnapshot &snap) {
        for (int y = 0; y < BOARD_ROWS; y++) for (int x = 0; x < BOARD_COLS; x++) {
            grid[y][x]->figure = snap.grid[y][x];
        }
        updateBoard();
        setPlayerTurn(snap.playerTurn);
    }

    QString getLetterFromCol(int n) { assert(n < BOARD_COLS); return QString(static_cast<char>(n) + 'a'); }

    QString getNumFromRow(int n) { assert(n < BOARD_ROWS); return QString((BOARD_ROWS + '0') - static_cast<char>(n)); }

    /**
     * @brief      Nastaví label na danou barvu hráče co má táhnout (černý/bílý).
     *
     * @param[in]  color  Vstupuje barva hráče.
     */
    void setPlayerTurn(PlayerColor color) {
        playerTurn = color;
        if (playerTurn == PlayerColor::White) { ui->turnLabel->setText("White"); }
        if (playerTurn == PlayerColor::Black) { ui->turnLabel->setText("Black"); }
    }

    /**
     * @brief      Odoznační dané políčko.
     */
    void unmarkMoves() {
        for (int y = 0; y < BOARD_ROWS; y++) {
            for (int x = 0; x < BOARD_COLS; x++) {
                grid[y][x]->validMove = false;
            }
        }
    }

    /**
     * @brief      Označí všechna políčka, která jsou validními tahy.
     *
     * Zde se řeší i validnost daného tahu.
     * To jak se figurka může nebo nemůže pohyhovat.
     * Včetně označení pokud narazí na protivníkovu figurku (červené).
     *
     * @param[in]  selectedX  Označené pole X
     * @param[in]  selectedY  Označené pole Y
     */
    void markValidMoves(int selectedX, int selectedY) {
        auto checkTile = [&](int x, int y) -> bool {
            if (x < 0 || y < 0 || x >= BOARD_COLS || y >= BOARD_ROWS) { return false; }
            if (grid[y][x]->figure.isSet()) {
                if (grid[y][x]->figure.color != grid[selectedY][selectedX]->figure.color) {
                    grid[y][x]->validMove = true;
                }
                return false;
            }
            grid[y][x]->validMove = true;
            return true;
        };

        if (grid[selectedY][selectedX]->figure.piece == ChessPiece::Rook) {
            for (int x = selectedX + 1; x < BOARD_COLS; x++) { if (!checkTile(x, selectedY)) { break; } }
            for (int y = selectedY + 1; y < BOARD_ROWS; y++) { if (!checkTile(selectedX, y)) { break; } }
            for (int x = selectedX - 1; x >= 0; x--) { if (!checkTile(x, selectedY)) { break; } }
            for (int y = selectedY - 1; y >= 0; y--) { if (!checkTile(selectedX, y)) { break; } }

        } else if (grid[selectedY][selectedX]->figure.piece == ChessPiece::Knight) {
            checkTile(selectedX - 1, selectedY - 2);
            checkTile(selectedX - 2, selectedY - 1);
            checkTile(selectedX - 1, selectedY + 2);
            checkTile(selectedX - 2, selectedY + 1);

            checkTile(selectedX + 1, selectedY - 2);
            checkTile(selectedX + 2, selectedY - 1);
            checkTile(selectedX + 1, selectedY + 2);
            checkTile(selectedX + 2, selectedY + 1);
        } else if (grid[selectedY][selectedX]->figure.piece == ChessPiece::Bishop) { // strelec
            for (int x = selectedX + 1, y = selectedY + 1; x < BOARD_COLS && y < BOARD_ROWS; x++, y++) { if (!checkTile(x, y)) { break; } }
            for (int x = selectedX - 1, y = selectedY + 1; x >= 0         && y < BOARD_ROWS; x--, y++) { if (!checkTile(x, y)) { break; } }
            for (int x = selectedX + 1, y = selectedY - 1; x < BOARD_COLS && y >= 0        ; x++, y--) { if (!checkTile(x, y)) { break; } }
            for (int x = selectedX - 1, y = selectedY - 1; x >= 0         && y >= 0        ; x--, y--) { if (!checkTile(x, y)) { break; } }

        } else if (grid[selectedY][selectedX]->figure.piece == ChessPiece::Queen) {
            for (int x = selectedX + 1; x < BOARD_COLS; x++) { if (!checkTile(x, selectedY)) { break; } }
            for (int y = selectedY + 1; y < BOARD_ROWS; y++) { if (!checkTile(selectedX, y)) { break; } }
            for (int x = selectedX - 1; x >= 0; x--) { if (!checkTile(x, selectedY)) { break; } }
            for (int y = selectedY - 1; y >= 0; y--) { if (!checkTile(selectedX, y)) { break; } }
            for (int x = selectedX + 1, y = selectedY + 1; x < BOARD_COLS && y < BOARD_ROWS; x++, y++) { if (!checkTile(x, y)) { break; } }
            for (int x = selectedX - 1, y = selectedY + 1; x >= 0         && y < BOARD_ROWS; x--, y++) { if (!checkTile(x, y)) { break; } }
            for (int x = selectedX + 1, y = selectedY - 1; x < BOARD_COLS && y >= 0        ; x++, y--) { if (!checkTile(x, y)) { break; } }
            for (int x = selectedX - 1, y = selectedY - 1; x >= 0         && y >= 0        ; x--, y--) { if (!checkTile(x, y)) { break; } }

        } else if (grid[selectedY][selectedX]->figure.piece == ChessPiece::King) {
            checkTile(selectedX - 1, selectedY - 1);
            checkTile(selectedX - 1, selectedY    );
            checkTile(selectedX - 1, selectedY + 1);
            checkTile(selectedX    , selectedY - 1);
            checkTile(selectedX    , selectedY + 1);
            checkTile(selectedX + 1, selectedY - 1);
            checkTile(selectedX + 1, selectedY    );
            checkTile(selectedX + 1, selectedY + 1); // to nevypadá jak moje :D on se spis kral chova jako dama, neni to prohozeny? :D jen obrazek jako ? nevim :D

        } else if (grid[selectedY][selectedX]->figure.piece == ChessPiece::Pawn) {
            int direction = grid[selectedY][selectedX]->figure.color == PlayerColor::White ? -1 : 1;
            auto checkForMove = [=](int x, int y) {
                if (!(x >= 0 && y >= 0 && x < BOARD_COLS && y < BOARD_ROWS)) { return; }
                if (!grid[y][x]->figure.isSet()) { grid[y][x]->validMove = true; }
            };
            auto checkForTake = [=](int x, int y) {
                if (!(x >= 0 && y >= 0 && x < BOARD_COLS && y < BOARD_ROWS)) { return; }
                if (grid[y][x]->figure.isSet() && grid[y][x]->figure.color != grid[selectedY][selectedX]->figure.color) {
                    grid[y][x]->validMove = true;
                }
            };

            checkForMove(selectedX, selectedY + direction);
            if ((direction == -1 && selectedY == BOARD_ROWS - 2) || (direction == 1 && selectedY == 1)) {
                if (!grid[selectedY + direction][selectedX]->figure.isSet()) {
                    checkForMove(selectedX, selectedY + direction * 2);
                }
            }
            checkForTake(selectedX - 1, selectedY + direction);
            checkForTake(selectedX + 1, selectedY + direction);
        }
    }

    /**
     * @brief      Zde se řeší "pohyb" figurek.
     * 
     * Podle toho jaký je daný stav hry (ready, clicked).
     * Se vybere daná akce, která může nastat.
     *
     * @param[in]  x     Dané políčko v matici šachovnice X
     * @param[in]  y     Dané políčko v matici šachovnice Y
     */
    void pieceClick(int x, int y) {
        auto *piece = grid[y][x];
        switch (state) {
        case GameState::Ready:
        {
            if (piece->figure.color == playerTurn) {
                piece->selected = true;
                selectedX = x;
                selectedY = y;
                markValidMoves(x, y);
                state = GameState::Selected;
            }

        } break;

        case GameState::Selected:
        {
            if (piece->selected == true) {
              piece->selected = false;
              unmarkMoves();
              state = GameState::Ready;

            } else if (piece->validMove) {
                if (piece->figure.isSet()) {
                    // Sebrani figurky
                }
                grid[y][x]->figure = grid[selectedY][selectedX]->figure;
                grid[selectedY][selectedX]->figure.unset();
                grid[selectedY][selectedX]->selected = false;
                unmarkMoves();
                state = GameState::Ready;
                setPlayerTurn(playerTurn == PlayerColor::White ? PlayerColor::Black : PlayerColor::White);

                int lastItemIndex = history.size() - 1;
                int currentIndex = ui->movesList->currentRow();
                if (currentIndex != lastItemIndex) {
                    for (int i = currentIndex + 1; i <= lastItemIndex; i++) {
                        history.erase(history.begin() + i);
                        delete ui->movesList->item(currentIndex + 1);
                    }
                    qDebug() << "History size: " << history.size() << "\n";
                }
                history.push_back(saveGameState());
                ui->movesList->addItem(pieceToLetter.at(grid[y][x]->figure.piece) + getLetterFromCol(selectedX) + "" + getNumFromRow(selectedY) +
                                       "" + getLetterFromCol(x) + getNumFromRow(y));
                ui->movesList->setCurrentRow(history.size() - 1);

            }
        } break;

        default: assert(!"Unhandled GameState!!!");
        }

        updateBoard();
    }

    /**
     * @brief      Inicializace dané šachové partie.
     * 
     * Možnost přehrávání.
     * Vykreslení šachovnice + sloupce/řádky okolo.
     * Rozestavění figurek na daná políčka dle pravidel.
     * 
     */
    void chessInit()
    {
        autoplayTimer = new QTimer(this);
        connect(autoplayTimer, &QTimer::timeout, [&]() {
            int current = ui->movesList->currentRow();
            if (current >= history.size() - 1) {
                autoplayTimer->stop();
                return;
            }
            loadGameState(history.at(current + 1));
            ui->movesList->setCurrentRow(current + 1);
        });

        ui->chessBoardView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate); // mozna nepotreba?

        chessboard = new Chessboard;
        ui->chessBoardView->setScene(chessboard);

        for (int i = 0; i < BOARD_ROWS; i++) {
            auto textTop = new QGraphicsTextItem;
            textTop->setFont(QFont("Comic Sans MS")); // já chtěl comic sans :( :(
            textTop->setTransformOriginPoint(0.5, 0.5);
            textTop->setPos(i * PIECE_WIDTH + PIECE_WIDTH / 2.5, -30);
            textTop->setPlainText(QString('A' + i));
            chessboard->addItem(textTop);

            auto textBot = new QGraphicsTextItem;
            textBot->setFont(QFont("Comic Sans MS")); // já chtěl comic sans :( :(
            textBot->setTransformOriginPoint(0.5, 0.5);
            textBot->setPos(i * PIECE_WIDTH + PIECE_WIDTH / 2.5, PIECE_HEIGHT * BOARD_ROWS + 2);
            textBot->setPlainText(QString('A' + i));
            chessboard->addItem(textBot);

            auto textLeft = new QGraphicsTextItem;
            textLeft->setFont(QFont("Comic Sans MS")); // já chtěl comic sans :( :(
            textLeft->setTransformOriginPoint(0.5, 0.5);
            textLeft->setPos(-20, i * PIECE_HEIGHT + PIECE_HEIGHT / 4);
            textLeft->setPlainText(QString('8' - i));
            chessboard->addItem(textLeft);

            auto textRight = new QGraphicsTextItem;
            textRight->setFont(QFont("Comic Sans MS")); // já chtěl comic sans :( :(
            textRight->setTransformOriginPoint(0.5, 0.5);
            textRight->setPos(PIECE_WIDTH * BOARD_COLS + 2, i * PIECE_HEIGHT + PIECE_HEIGHT / 4);
            textRight->setPlainText(QString('8' - i));
            chessboard->addItem(textRight);
        }


        bool odd = false;
        for (int y = 0; y < BOARD_ROWS; y++) {
            odd = y % 2 == 0;
            for (int x = 0; x < BOARD_COLS; x++) {
                grid[y][x] = new ChessboardTile;
                grid[y][x]->setPos(x * PIECE_WIDTH, y * PIECE_HEIGHT);
                grid[y][x]->brushColor = odd ? QColor(255,255,255) : QColor(60,60,60);
                odd = !odd;
                chessboard->addItem(grid[y][x]);

                qDebug() << "Piece x: " << x << "y: " << y << "\n";
                auto *piece = grid[y][x];

                connect(piece, &ChessboardTile::clicked, [=](QGraphicsSceneMouseEvent *event) {
                   pieceClick(x, y);
                });
            }
        }

        grid[0][0]->figure = grid[0][7]->figure = { PlayerColor::Black, ChessPiece::Rook };
        grid[0][1]->figure = grid[0][6]->figure = { PlayerColor::Black, ChessPiece::Knight };
        grid[0][2]->figure = grid[0][5]->figure = { PlayerColor::Black, ChessPiece::Bishop };
        grid[0][3]->figure = { PlayerColor::Black, ChessPiece::Queen};
        grid[0][4]->figure = { PlayerColor::Black, ChessPiece::King };

        grid[1][0]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][1]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][2]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][3]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][4]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][5]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][6]->figure = { PlayerColor::Black, ChessPiece::Pawn };
        grid[1][7]->figure = { PlayerColor::Black, ChessPiece::Pawn };

        grid[7][0]->figure = grid[7][7]->figure = { PlayerColor::White, ChessPiece::Rook };
        grid[7][1]->figure = grid[7][6]->figure = { PlayerColor::White, ChessPiece::Knight };
        grid[7][2]->figure = grid[7][5]->figure = { PlayerColor::White, ChessPiece::Bishop };
        grid[7][3]->figure = { PlayerColor::White, ChessPiece::Queen };
        grid[7][4]->figure = { PlayerColor::White, ChessPiece::King };

        grid[6][0]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][1]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][2]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][3]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][4]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][5]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][6]->figure = { PlayerColor::White, ChessPiece::Pawn };
        grid[6][7]->figure = { PlayerColor::White, ChessPiece::Pawn };


        ui->chessBoardView->centerOn(PIECE_WIDTH * BOARD_COLS / 2, PIECE_HEIGHT * BOARD_ROWS / 2);
        setPlayerTurn(PlayerColor::White);

    }

private slots:
    void on_movesList_itemClicked(QListWidgetItem *item) {
        loadGameState(history.at(ui->movesList->currentRow()));
    }

    /**
     * @brief      Undo tlačítko, včetně potvrzení daného rozhodnutí.
     */
    void on_undoButton_clicked() {
        int current = ui->movesList->currentRow();
        if (current > 0) {

            auto result = QMessageBox::information(this, "r u sure", "are you sure you want to klik this?", QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes) {
                result = QMessageBox::information(this, "r u sure", "are you REALLY REALLY sure you want to klik this?", QMessageBox::Yes | QMessageBox::No);
                if (result == QMessageBox::Yes) {
                    QMessageBox::information(this, "ok", "ok");
                    ui->movesList->setCurrentRow(current - 1);
                    loadGameState(history.at(current - 1));
                } else {
                    QMessageBox::information(this, "ok", "malem jsi na to klik");
                }
            } else {
                QMessageBox::information(this, "ok", "malem jsi na to klik");
            }
        } else {
            QMessageBox::critical(this, "cannot do", "no undo :(", QMessageBox::No);
        }
    }

    /**
     * @brief      Redo tlačítko, včetně potvrzení daného rozhodnutí.
     */
    void on_redoButton_clicked() {
        int current = ui->movesList->currentRow();
        if (current < history.size() - 1) {

            auto result = QMessageBox::information(this, "r u sure", "are you sure you want to klik this?", QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes) {
                result = QMessageBox::information(this, "r u sure", "are you REALLY REALLY sure you want to klik this?", QMessageBox::Yes | QMessageBox::No);
                if (result == QMessageBox::Yes) {
                    QMessageBox::information(this, "ok", "ok");
                    ui->movesList->setCurrentRow(current + 1);
                    loadGameState(history.at(current + 1));
                } else {
                    QMessageBox::information(this, "ok", "malem jsi na to klik");
                }
            } else {
                QMessageBox::information(this, "ok", "malem jsi na to klik");
            }
        } else {
            QMessageBox::critical(this, "cannot do", "no redo :(", QMessageBox::No);
        }
    }

    /**
     * @brief      Tlačítko na přehrání dle zvoleného časového úseku.
     */
    void on_playButton_clicked() {
        autoplayTimer->setInterval(ui->autoplayIntervalBox->text().toUInt() * 1000);
        autoplayTimer->start();
    }

    /**
     * @brief      Tlačítko pro zastavení přehrávání.
     */
    void on_stopButton_clicked() {
        autoplayTimer->stop();
    }

    /**
     * @brief      Tlačítko na načtení dané šachové partie.
     * 
     * Automatické načtení dané šachové partie, kde se rozhoduje jestli je v krátké/dlouhé anotaci.
     */
    void on_loadButton_clicked() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open file");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        std::vector<QString> lines;
        QRegularExpression lineNumReg("^(\\d+)\\.");
        while (!file.atEnd()) {
            auto line = file.readLine();
            auto match = lineNumReg.match(line);
            if (!match.hasMatch()) { qDebug() << "Spatny vstupni soubor\n"; continue; }
            int lineNum = match.captured(1).toInt();
            if (lines.size() < lineNum) { lines.resize(lineNum); }
            lines[lineNum - 1] = line;
        }


        QRegularExpression shortReg("^([KDVSJp])?(?:([a-h])|([1-8]))?(x)?([a-h])([1-8])([KDVSJp])?([+#])?$");
        QRegularExpression longReg("^([KDVSJp])?([a-h])([1-8])([a-h])([1-8])$");
        QRegularExpression whitespaceReg("\\s+");
        for (auto &line : lines) {
            auto pieces = line.split(whitespaceReg, QString::SplitBehavior::SkipEmptyParts);
            QRegularExpressionMatch match1, match2;

            auto processShortPattern = [=](QRegularExpressionMatch match) -> bool {
                if (!match.hasMatch()) return false;

                //   1 - oznaceni figurky
                //   2 - sloupec figurky
                //   3 - rada figurky
                //   4 - jestli se bralo
                //   5 - sloupec
                //   6 - rada
                //   > - nezajem
                PlayerColor color = playerTurn;
                std::string s = match.captured(1).toStdString();
                ChessPiece piece = letterToPiece.at(s);
                int col = !match.captured(2).isEmpty() ? match.captured(2).toStdString()[0] - 'a' : -1;
                int row = !match.captured(3).isEmpty() ? '8' - match.captured(3).toStdString()[0] : -1;

                int figureX = -1, figureY = -1;
                for (int y = 0; y < BOARD_ROWS; y++) for (int x = 0; x < BOARD_COLS; x++) {
                    if ((row != -1 && y != row) || (col != -1 && x != col)) { continue; }
                    if (grid[y][x]->figure.color == color && grid[y][x]->figure.piece == piece) {
                        figureX = x; figureY = y; break;
                    }
                }

                if (figureX == -1 || figureY == -1) { qDebug() << "figure not found\n"; return true; }

                int dstX = !match.captured(5).isEmpty() ? match.captured(5).toStdString()[0] - 'a' : -1;
                int dstY = !match.captured(6).isEmpty() ? '8' - match.captured(6).toStdString()[0] : -1;
                if (dstX == -1 || dstY == -1) { qDebug() << "dst not found\n"; return true; }

                pieceClick(figureX, figureY);
                pieceClick(dstX, dstY);

                qDebug() << figureX << ":" << figureY << " -> " << dstX << ":" << dstY << "\n";

                return true;
            };

            auto processLongPattern = [=](QRegularExpressionMatch match) -> bool {
                if (!match.hasMatch()) return false;

                int srcX = !match.captured(2).isEmpty() ? match.captured(2).toStdString()[0] - 'a' : -1;
                int srcY = !match.captured(3).isEmpty() ? '8' - match.captured(3).toStdString()[0] : -1;
                if (srcX == -1 || srcY == -1) { qDebug() << "src not found\n"; return true; }
                int dstX = !match.captured(4).isEmpty() ? match.captured(4).toStdString()[0] - 'a' : -1;
                int dstY = !match.captured(5).isEmpty() ? '8' - match.captured(5).toStdString()[0] : -1;
                if (dstX == -1 || dstY == -1) { qDebug() << "dst not found\n"; return true; }
                pieceClick(srcX, srcY);
                pieceClick(dstX, dstY);
                return true;
            };

            auto error = [&]() -> bool {
                qDebug() << "Neco je spatne\n";
                return true;
            };

            if (pieces.length() > 1) { processShortPattern(shortReg.match(pieces[1])) || processLongPattern(longReg.match(pieces[1])) || error(); }
            if (pieces.length() > 2) { processShortPattern(shortReg.match(pieces[2])) || processLongPattern(longReg.match(pieces[2])) || error(); }
        }
    }

    /**
     * @brief      Tlačítko na uložení dané šachové hry.
     * 
     * Bere se historie z listboxu, která je zobrazována/ukládána v dlouhé notaci.
     */
    void on_saveButton_clicked() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save file");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return;
        QTextStream stream(&file);

        int i;
        for (i = 0; i < ui->movesList->count(); i++) {
            if (i % 2 == 0) {
                stream << (i / 2) + 1 << ". " << ui->movesList->item(i)->text();
            } else {
                stream << " " << ui->movesList->item(i)->text() << "\n";
            }


        }
        if (i % 2 == 0) { stream << "\n"; }
        stream.flush();
        file.close();

    }

};

#endif // CHESSGAME_H
