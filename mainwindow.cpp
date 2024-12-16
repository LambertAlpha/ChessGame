// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "terrain.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QMessageBox>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QTimer>


void checkForVictory(QGraphicsScene *scene,QString & eatMessage, bool & gameOver) {
    if(gameOver) return;
    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece && dynamic_cast<King*>(piece) ) {
            if (piece->isKingCaptured(scene)) {
                if (piece->isPlayerOne) {
                    if(gameOver){
                        return;
                    }

                    eatMessage +=  "Player 1 wins! ";
                } else {
                    if(gameOver){
                        return;
                    }
                    eatMessage +=  "Player 2 wins! ";
                }
            }
            gameOver = true;
            break;
        }
    }
}

void checkForVictory(QGraphicsScene *scene, bool & gameOver,QString & bombMessage) {
    if(gameOver) return;
    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece && dynamic_cast<King*>(piece)) {
            if (piece->isKingCaptured(scene)) {
                if (piece->isPlayerOne) {
                    if(gameOver){
                        return;
                    }
                    bombMessage += "Player 1 wins!";
                } else {
                    if(gameOver){
                        return;
                    }
                    bombMessage += "Player 2 wins!";
                }
            }
            gameOver = true;
        }
    }
}

Piece *MainWindow::FindPieceAtXY(int x, int y, QGraphicsScene *scene){
    Piece *piece = nullptr;
    for (auto item : scene->items()) {
        piece = dynamic_cast<Piece*>(item);
        if(piece) {
            if (piece && piece->x == x && piece->y == y ) {
                return piece;
            }
        }
    }
    return piece;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , gameOver(false)
    , ui(new Ui::MainWindow)
    , currentPlayer(1)
    , selectedPiece(nullptr)
    , terrain(11, 11)

{
    // initialization
    ui->setupUi(this);

    terrain.setupTerrain();

    scene = new QGraphicsScene(0,0,701,701,this);

    setupGameBoard();

    addPieces();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setWindowTitle(QString("Chess Game - PLAYER %1 's Turn").arg(currentPlayer));

    ui->graphicsView->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLegend()
{
    const int legendX = 600;
    const int legendY = 20;
    const int rectSize = 20;
    const int spacing = 10;
    const int textOffset = 25;

    struct LegendItem {
        QColor color;
        QString description;
    };

    // legends of the terrain
    std::vector<LegendItem> terrainLegend = {
        {QColor(34, 139, 34), "Forest"},
        {QColor(30, 144, 255), "River"},
        {QColor(165, 42, 42), "Mountain"},
        {QColor(210, 180, 140), "Desert"},
    };

    // piece's legends
    std::vector<LegendItem> pieceLegend = {
        {QColor(0, 0, 255), "Knight"},
        {QColor(255, 0, 0), "Bomb"},
        {QColor(0, 255, 0), "Pawn"},
        {QColor(204, 0, 204), "Queen"},
        {QColor(255, 255, 0), "King"},
        {QColor(51, 255, 255), "Bishop"},
    };

    int y = legendY;
    scene->addText("Terrain Legend:")->setPos(legendX, y);
    y += rectSize + spacing;
    for (const auto &item : terrainLegend) {
        QGraphicsRectItem *rect = scene->addRect(
            legendX, y, rectSize, rectSize, QPen(Qt::black), QBrush(item.color));
        rect->setZValue(0);

        QGraphicsTextItem *text = scene->addText(item.description);
        text->setDefaultTextColor(Qt::black);
        text->setPos(legendX + textOffset, y - 5);
        y += rectSize + spacing;
    }

    y += spacing;
    scene->addText("Piece Legend:")->setPos(legendX, y);
    y += rectSize + spacing;
    for (const auto &item : pieceLegend) {
        QGraphicsRectItem *rect = scene->addRect(
            legendX, y, rectSize, rectSize, QPen(Qt::black), QBrush(item.color));
        rect->setZValue(0);

        QGraphicsTextItem *text = scene->addText(item.description);
        text->setDefaultTextColor(Qt::black);
        text->setPos(legendX + textOffset, y - 5);
        y += rectSize + spacing;
    }
}

TerrainType MainWindow::getTerrain(int x, int y)
{
    return terrain.getTerrain(x, y);
}


void MainWindow::setupGameBoard() {
    const int cellSize = 50;

    // draw the board
    for (int row = 0; row < terrain.getRows(); ++row) {
        for (int col = 0; col < terrain.getCols(); ++col) {
            QColor color;
            switch (terrain.getTerrain(row, col)) {
            case TerrainType::Land:
                color = (row + col) % 2 == 0 ? QColor(255, 255, 255) : QColor(200, 200, 200);
                break;
            case TerrainType::Forest:
                color = QColor(34, 139, 34);
                break;
            case TerrainType::River:
                color = QColor(30, 144, 255);
                break;
            case TerrainType::Mountain:
                color = QColor(165, 42, 42);
                break;
            case TerrainType::Desert:
                color = QColor(210, 180, 140);
                break;
            }
            QGraphicsRectItem *cell = scene->addRect(col * cellSize, row * cellSize, cellSize, cellSize,
                                                     QPen(Qt::black), QBrush(color));
            cell->setZValue(-1);
        }
    }

    ui->graphicsView->setSceneRect(0, 0, terrain.getCols() * cellSize, terrain.getRows() * cellSize);
    ui->graphicsView->centerOn(terrain.getCols() * cellSize / 2, terrain.getRows() * cellSize / 2);


    addLegend();
}

void MainWindow::addPieces()
{
    // PlayerOne: upwards
    player1Pieces.push_back(new Pawn(0, 0, true, scene));
    player1Pieces.push_back(new Pawn(1, 0, true, scene));
    player1Pieces.push_back(new Knight(2, 0, true, scene));
    player1Pieces.push_back(new Bishop(3, 0, true, scene));
    player1Pieces.push_back(new Queen(4, 0, true, scene));
    player1Pieces.push_back(new King(5, 0, true, scene));
    player1Pieces.push_back(new Bomb(6, 0, true, scene));
    player1Pieces.push_back(new Bishop(7, 0, true, scene));
    player1Pieces.push_back(new Knight(8, 0, true, scene));
    player1Pieces.push_back(new Pawn(9, 0, true, scene));
    player1Pieces.push_back(new Pawn(10, 0, true, scene));

    //PlayerTwo: downwards
    player2Pieces.push_back(new Pawn(0, 10, false, scene));
    player2Pieces.push_back(new Pawn(1, 10, false, scene));
    player2Pieces.push_back(new Knight(2, 10, false, scene));
    player2Pieces.push_back(new Bishop(3, 10, false, scene));
    player2Pieces.push_back(new Queen(4, 10, false, scene));
    player2Pieces.push_back(new King(5, 10, false, scene));
    player2Pieces.push_back(new Bomb(6, 10, false, scene));
    player2Pieces.push_back(new Bishop(7, 10, false, scene));
    player2Pieces.push_back(new Knight(8, 10, false, scene));
    player2Pieces.push_back(new Pawn(9, 10, false, scene));
    player2Pieces.push_back(new Pawn(10, 10, false, scene));
}

void MainWindow::switchPlayer()
{
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    setWindowTitle(QString("Chess Game - Player %1 's Turn").arg(currentPlayer));


    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);

    }
}

void MainWindow::showCaptureMessage(QString &message)
{
    // message showcase
    QGraphicsTextItem* textItem = new QGraphicsTextItem(message);

    QFont font("Arial", 16);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::red);

    textItem->setPos(50,520);

    scene->addItem(textItem);

    QTimer::singleShot(3000, [textItem, this]() {
        if (scene->items().contains(textItem)) {
            scene->removeItem(textItem);
            delete textItem;
        }
    });
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsView && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        // left key
        if (mouseEvent->button() == Qt::LeftButton) {
            QPointF point = ui->graphicsView->mapToScene(mouseEvent->pos());
            onGraphicsViewClicked(point);
            return true;
        } if (mouseEvent->button() == Qt::RightButton) {
            if (selectedPiece) {
                QString pieceType;
                if (dynamic_cast<Knight*>(selectedPiece)) {
                    pieceType = "Knight";
                } else if (dynamic_cast<Pawn*>(selectedPiece)) {
                    pieceType = "Pawn";
                } else if (dynamic_cast<Queen*>(selectedPiece)) {
                    pieceType = "Queen";
                } else if (dynamic_cast<King*>(selectedPiece)) {
                    pieceType = "King";
                } else if (dynamic_cast<Bishop*>(selectedPiece)) {
                    pieceType = "Bishop";
                } else if (dynamic_cast<Bomb*>(selectedPiece)) {
                    pieceType = "Bomb";
                }

                // showcase the info.
                QString message = QString("Piece type: %1\nSpecial ability: %2")
                                      .arg(pieceType)
                                      .arg(QString::fromStdString(selectedPiece->getSpecialAbilityText()));
                QMessageBox::information(this, "Piece information", message);

                // checking whether have special ability
                bool hasAbility = (dynamic_cast<Knight*>(selectedPiece) ||
                                  dynamic_cast<Queen*>(selectedPiece) ||
                                  dynamic_cast<King*>(selectedPiece) ||
                                  dynamic_cast<Bishop*>(selectedPiece) ||
                                  dynamic_cast<Bomb*>(selectedPiece));

                if (hasAbility) {
                    int result = QMessageBox::question(this,
                                      selectedPiece->name,
                                      "Do you want to use this piece's special ability?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

                    if (result == QMessageBox::Yes) {
                        bool isBomb = dynamic_cast<Bomb*>(selectedPiece) != nullptr;
                        bool isKnight = dynamic_cast<Knight*>(selectedPiece) != nullptr;
                        bool isQueen = dynamic_cast<Queen*>(selectedPiece) != nullptr;
                        bool isKing = dynamic_cast<King*>(selectedPiece) != nullptr;
                        bool isBishop = dynamic_cast<Bishop*>(selectedPiece) != nullptr;

                        bool abilitySucceeded = selectedPiece->specialAbility(scene);

                        if (abilitySucceeded) {
                            QString abilityMessage;
                            if (isBomb) {
                                abilityMessage = "\nBomb exploded!";
                            } else if (isKnight) {
                                abilityMessage = "\nKnight used Charge!";
                            } else if (isQueen) {
                                abilityMessage = "\nQueen used Royal Command!";
                            } else if (isKing) {
                                abilityMessage = "\nKing used Divine Protection!";
                            } else if (isBishop) {
                                abilityMessage = "\nBishop used Holy Light!";
                            }

                            showCaptureMessage(abilityMessage);

                            // checking the death
                            bool playerOneHasPieces = false;
                            bool playerTwoHasPieces = false;
                            bool playerOneKingAlive = false;
                            bool playerTwoKingAlive = false;

                            for (auto item : scene->items()) {
                                Piece* piece = dynamic_cast<Piece*>(item);
                                if (piece) {
                                    if (piece->isPlayerOne) {
                                        playerOneHasPieces = true;
                                        if (dynamic_cast<King*>(piece)) {
                                            playerOneKingAlive = true;
                                        }
                                    } else {
                                        playerTwoHasPieces = true;
                                        if (dynamic_cast<King*>(piece)) {
                                            playerTwoKingAlive = true;
                                        }
                                    }
                                }
                            }

                            // checking for the winner
                            if (!playerOneKingAlive || !playerOneHasPieces) {
                                QMessageBox::information(this, "WIN!!!", "Player2 win！");
                                qApp->quit();
                            }
                            if (!playerTwoKingAlive || !playerTwoHasPieces) {
                                QMessageBox::information(this, "WIN!!!", "Player1 win！");
                                qApp->quit();
                            }

                            switchPlayer();
                        }
                    }
                } else {
                    QMessageBox::information(this, selectedPiece->name, "This piece has no special ability.");
                }

                selectedPiece = nullptr;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onGraphicsViewClicked(QPointF point)
{
    if(gameOver){
        QMessageBox::information(this, "Game Over", "Have Fun!");
                return;
    }
    const int cellSize = 50;
    int x = static_cast<int>(point.x()) / cellSize;
    int y = static_cast<int>(point.y()) / cellSize;

    int prev_x;
    int prev_y;

    if (x < 0 || x >= 14 || y < 0 || y >= 14) {
        if (selectedPiece != nullptr)
            selectedPiece = nullptr;
        return;
    }

    if (selectedPiece) {
        bool occupied = false;
        Piece* targetPiece = nullptr;

        prev_x = selectedPiece->x;
        prev_y = selectedPiece->y;

        for (auto item : scene->items()) {
            Piece *piece = dynamic_cast<Piece*>(item);
            if (piece && piece->x == x && piece->y == y) {
                occupied = true;
                targetPiece = piece;
                break;
            }
        }

        if (!occupied) {
            // if the target is empty

            // can go over the piece
            Piece * midPiece = nullptr;
            int moveX = std::abs(prev_x - x);
            int moveY = std::abs(prev_y - y);
            if (moveX == moveY && moveX == 2 ){
                midPiece = FindPieceAtXY((prev_x+x)/2, (prev_y + y)/2, scene);
            } else if (moveX == 0 && moveY == 2) {
                midPiece = FindPieceAtXY((prev_x+x)/2, (prev_y + y)/2, scene);
            } else if (moveY == 0 && moveX == 2){
                midPiece = FindPieceAtXY((prev_x+x)/2, (prev_y + y)/2, scene);
            }
            if (midPiece){
                QMessageBox::information(this, "Invalid Movement", "Can go over pieces");
                return;
            }

            selectedPiece->moveTo(x, y, scene);

            if ( prev_x != selectedPiece->x || prev_y != selectedPiece->y){
                switchPlayer();
            }else {
                QString errorMsg;
                switch(selectedPiece->getErrorCase()) {
                    case 1:
                        errorMsg = "Can only move 1 horizontally or perpendicularly in the mountains!";
                        break;
                    case 2:
                        errorMsg = "Can only move 2 in the forest!";
                        break;
                    case 3:
                        errorMsg = "Bomb cannot cross the river!";
                        break;
                    case 4:
                        errorMsg = "Queen cannot cross the river!";
                        break;
                    case 5:
                        errorMsg = "King cannot cross the river!";
                        break;
                    case 6:
                        errorMsg = "Bishop cannot cross the river!";
                        break;
                    default:
                        errorMsg = "Invalid move!";
                }
                QMessageBox::information(this, "Invalid Movement", errorMsg);
            }
            selectedPiece = nullptr;

        } else {

            // if not empty, check for eat
            if (targetPiece->isPlayerOne == selectedPiece->isPlayerOne) {
                // self-piece -> invalid
                QMessageBox::information(this, "Invalid Movement", "Cannot eat self-piece!");
                selectedPiece = nullptr;
            } else {
                // Cannot eat in the desert
                if (getTerrain(selectedPiece->y, selectedPiece->x) == TerrainType::Desert) {
                    QMessageBox::information(this, "Invalid Movement", "Cannot eat in the desert");
                    selectedPiece = nullptr;
                    return;
                }
                // eating----------------------------
                // moving
                selectedPiece->moveTo(x, y, scene);
                if (selectedPiece->x == prev_x && selectedPiece->y == prev_y){
                    QMessageBox::information(this, "Invalid Movement", "Movement Failed!");
                    selectedPiece = nullptr;
                }else {
                    // eating message

                    QString eatMessage = selectedPiece->name + " ate " + targetPiece->name + ". ";
                    if(targetPiece -> name == "King"){
                        checkForVictory(scene,eatMessage,gameOver);
                    }

                    if (selectedPiece->name == "Bomb" && targetPiece->name != "Bomb" ){
                        eatMessage = "Bomb exploded! AND " + targetPiece->name +" died!";
                        scene->removeItem(selectedPiece);
                        scene->removeItem(targetPiece);
                    } else if(targetPiece->name == "Bomb" && selectedPiece->name != "Bomb"){
                        eatMessage = selectedPiece->name + " encountered BOMB and exploded!";
                        scene->removeItem(selectedPiece);
                        scene->removeItem(targetPiece);
                    }else if (targetPiece->name == "Bomb" && selectedPiece->name == "Bomb"){
                        eatMessage = "Two Bombs exploded!";
                        scene->removeItem(selectedPiece);
                        scene->removeItem(targetPiece);
                    }
                    showCaptureMessage(eatMessage);

                    delete targetPiece;
                    selectedPiece = nullptr;
                    switchPlayer();
                }
            }
        }
        return;
    }

    // seeking for piece //
    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if(piece) {
            if (piece && piece->x == x && piece->y == y && piece->isPlayerOne == (currentPlayer == 1)) {
                selectedPiece = piece;
                break;
            }
        }
    }
}


