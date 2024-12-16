// piece.cpp
#include "piece.h"
#include <cmath>
#include <QGraphicsScene>
#include <QDebug>
#include <mainwindow.h>
#include <QMessageBox>

Piece::Piece(int x, int y, bool isPlayerOne, QColor color, QGraphicsScene *scene)
    : QGraphicsEllipseItem(0,0, 48, 48), x(x), y(y), isPlayerOne(isPlayerOne)
{
    setBrush(QBrush(color));
    if (isPlayerOne){
        setPen(QPen(Qt::white,3)); // set the boarder of the pieces
    } else {
        setPen(QPen(Qt::black,3));
    }

    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setPos(x * 50, y * 50);
    scene->addItem(this);
}

//move for knight's specialAbility
void Knight::moveTo( QGraphicsScene *scene,int destX, int destY) {
        setPos(destX * 50, destY * 50);
        x = destX;
        y = destY;

}

// MOVE FOR PIECES
void Piece::moveTo(int destX, int destY, QGraphicsScene *scene)
{
    Q_UNUSED(scene);
    error_case = 0;

    bool canMoveToTarget = false;
    MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent());
    bool inForest = mainWindow->getTerrain(this->y, this->x) == TerrainType::Forest;
    bool inMountain = mainWindow->getTerrain(this->y, this->x) == TerrainType::Mountain;

    if (inMountain) {
        // ONLY CAN MOVE 1 IN MOUNTAINS
        bool horizontalMove = (std::abs(destX - this->x) == 1 && destY == this->y);  // move 1 horizontally
        bool verticalMove = (std::abs(destY - this->y) == 1 && destX == this->x);    // move 1 perpendicularly
        bool validMove = horizontalMove || verticalMove;

        if (validMove) {
            canMoveToTarget = true;
        }else{
            error_case = 1;
        }
    }else if (inForest) {
        // IN FOREST
        bool straightMove = (
            (std::abs(destX - this->x) == 0 && std::abs(destY - this->y) <= 2) ||  // move less than 2 perpendicularly
            (std::abs(destX - this->x) <= 2 && std::abs(destY - this->y) == 0)     // move less than 2 horizontally
        );
        bool diagonalMove = (
            std::abs(destX - this->x) == std::abs(destY - this->y) &&  // move less than 2 diagonally
            std::abs(destX - this->x) <= 2
        );

        if (straightMove || diagonalMove) {
            canMoveToTarget = true;
        }else{
            error_case = 2;
        }
    }else{
        if (dynamic_cast<Knight*>(this)) {
            //Checking KINGHT
            Knight* knight = dynamic_cast<Knight*>(this);
            if (knight->isUsingSpecialAbility) {
                canMoveToTarget = true;
            }else{
                bool validMove1 = (std::abs(destX - this->x) == 0 && std::abs(destY - this->y) <= 2);
                bool vaildMove2 = (std::abs(destX - this->x) <= 2 && std::abs(destY - this->y) == 0);
                bool validMove3 = (std::abs(destX - this->x) == 1 && std::abs(destY - this->y) <= 2);
                bool validMove4 = (std::abs(destX - this->x) == 2 && std::abs(destY - this->y) <= 1);
                bool validMove = validMove1 || vaildMove2 || validMove3 || validMove4;
                // move no more than 2
                if (validMove) {
                    canMoveToTarget = true;
                }
            }
        } else if (dynamic_cast<Pawn*>(this)) {
            // checking PAWN
            bool horizontalMove = (std::abs(destX - this->x) == 1 && destY == this->y);  // move 1 horizontally
            bool verticalMove = (std::abs(destY - this->y) == 1 && destX == this->x);    // move 1 perpendicularly
            bool validMove = horizontalMove || verticalMove;
            if (validMove) {
                canMoveToTarget = true;
            }

        } else if (dynamic_cast<Bomb*>(this)) {
            // check BOMB
            bool horizontalMove = (std::abs(destX - this->x) == 1 && destY == this->y);  // move 1 horizontally
            bool verticalMove = (std::abs(destY - this->y) == 1 && destX == this->x);    // move 1 perpendicularly
            bool validMove = horizontalMove || verticalMove;
            // cannot get into the RIVER
            MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent());
            if (validMove && mainWindow->getTerrain(destY, destX) != TerrainType::River) {
                canMoveToTarget = true;
            }else if (mainWindow->getTerrain(destY, destX) == TerrainType::River){
                error_case = 3;
            }
        } else if (dynamic_cast<Queen*>(this)) {
            // checking QUEEN
            bool validMove1 = (std::abs(destX - this->x) == 0 && std::abs(destY - this->y) <= 2);
            bool vaildMove2 = (std::abs(destX - this->x) <= 2 && std::abs(destY - this->y) == 0);
            bool validMove3 = (std::abs(destX - this->x) == std::abs(destY - this->y) && std::abs(destY - this->y) <= 2);
            bool validMove = validMove1 || vaildMove2 || validMove3;
            // cannot get into the RIVER
            MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent());
            if (validMove) {
                 bool hasRiver = false;

                 if (validMove1 || vaildMove2) { // move dierctly
                     int stepX = (destX > this->x) ? 1 : (destX < this->x) ? -1 : 0;
                     int stepY = (destY > this->y) ? 1 : (destY < this->y) ? -1 : 0;

                     int currentX = this->x + stepX;
                     int currentY = this->y + stepY;

                     while (currentX != destX + stepX || currentY != destY + stepY) {
                         if (mainWindow->getTerrain(currentY, currentX) == TerrainType::River) {
                             hasRiver = true;
                             error_case = 4;
                             break;
                         }
                         currentX += stepX;
                         currentY += stepY;
                     }
                 }
                 else if (validMove3) { // move diagonally
                     int stepX = (destX > this->x) ? 1 : -1;
                     int stepY = (destY > this->y) ? 1 : -1;

                     int currentX = this->x + stepX;
                     int currentY = this->y + stepY;

                     while (currentX != destX + stepX && currentY != destY + stepY) {
                         if (mainWindow->getTerrain(currentY, currentX) == TerrainType::River) {
                             hasRiver = true;
                             break;
                         }
                         currentX += stepX;
                         currentY += stepY;
                     }
                 }

                 if (!hasRiver) {
                     canMoveToTarget = true;
                 }
             }
        } else if (dynamic_cast<King*>(this)) {
            // check KING
            bool validMove = (std::abs(destX - this->x) <= 1 && std::abs(destY - this->y) <= 1);  // move 1 only(can diagonally)
            // cannot get into the RIVER
            MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent());
            if (validMove && mainWindow->getTerrain(destY, destX) != TerrainType::River) {
                canMoveToTarget = true;
            }else if (mainWindow->getTerrain(destY, destX) == TerrainType::River){
                error_case = 5;
            }
        } else if (dynamic_cast<Bishop*>(this)) {
        // checking for BISHOP
        bool validMove = (std::abs(destX - this->x) == std::abs(destY - this->y) && std::abs(destY - this->y)<= 2);
        // cannot get into the RIVER
        MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent());
        if (validMove) {
            //cannot get into the RIVER
            int stepX = (destX > this->x) ? 1 : -1;
            int stepY = (destY > this->y) ? 1 : -1;
            bool hasRiver = false;
            int currentX = this->x + stepX;
            int currentY = this->y + stepY;

            while (currentX != destX + stepX && currentY != destY + stepY) {
                if (mainWindow->getTerrain(currentY, currentX) == TerrainType::River) {
                    hasRiver = true;
                    error_case = 6;
                    break;
                }
                currentX += stepX;
                currentY += stepY;
            }

            if (!hasRiver) {
                canMoveToTarget = true;
            }
        }
    }
    }
    if (canMoveToTarget) {
        setPos(destX * 50, destY * 50);
        x = destX;
        y = destY;
    }
}

// ---------------------- Knight ----------------------

Knight::Knight(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::blue, scene) {

    name = "Knight";
    specialAbilityText = "Can charge forward up to 5 squares, and kill the first enemy or stop before your teammate.";}


bool Knight::specialAbility(QGraphicsScene *scene)
{

    Q_UNUSED(scene);
    if(hasCharged){
        QMessageBox::information(nullptr,"Invaild ability!","The special ability is already used");
        return false;
    }
    int count = 0;
    int player = 0;
    hasCharged = true;
    int targetY = y;
    this->isUsingSpecialAbility = true;

    // charge forward//
    if(isPlayerOne){
        player = 1;
        int dy = 1;
        for (int i = y + dy; i <= y+5; i += dy) {
            Piece* piece = MainWindow::FindPieceAtXY(x, i, scene);
            if (piece) {
                if (piece->isPlayerOne != this->isPlayerOne) {
                    // eating
                    scene->removeItem(piece);
                    delete piece;
                    targetY = i;
                }else{
                    targetY = i-1;
                }
                break;
            }
            count += 1;
        }
    }else{
        player = 2;
        int dy = -1;
        for (int i = y + dy; i >= y-5; i += dy) {
            Piece* piece = MainWindow::FindPieceAtXY(x, i, scene);
            if (piece) {
                if (piece->isPlayerOne != this->isPlayerOne) {
                   // eating
                   scene->removeItem(piece);
                   delete piece;
                   targetY = i;
                }else{
                   targetY = i + 1;
                }
                break;
            }
            count += 1;
        }

    }

    if(count == 5 && player == 1) targetY += 5;
    else if(count == 5 && player == 2) targetY -= 5;

    if(targetY >= 10 && player == 1)targetY = 10;
    if(targetY <= 0 && player == 2)targetY = 0;


    this->moveTo(scene,x, targetY);
    this->isUsingSpecialAbility = false;
    return true;
}

// ---------------------- Pawn ----------------------

Pawn::Pawn(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::green, scene) {
    name = "Pawn";
    specialAbilityText = "NO special ability! ";}

bool Pawn::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // no special ability//
    return false;
}

// ---------------------- Bomb ----------------------

Bomb::Bomb(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::red, scene) {
    name = "Bomb";
    specialAbilityText = "Can kill surrounding pieces";}

// bombing: kill surrounding pieces //
bool Bomb::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene);

    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            if (i >= 0 && i < 14 && j >= 0 && j < 14) {
                for (auto item : scene->items()) {
                    Piece *piece = dynamic_cast<Piece*>(item);

                    if (piece && piece->x == i && piece->y == j) {
                        scene->removeItem(piece);
                        delete piece;
                    }
                }
            }
        }
    }

    return true;
}

// ---------------------- Queen ----------------------

Queen::Queen(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::magenta, scene) {
    name = "Queen";
    specialAbilityText = "Can kill pieces at the four corners of the size-4 square centered at herself";}


bool Queen::specialAbility(QGraphicsScene *scene)
{
    bool team = this->isPlayerOne;
    bool flag = false;

    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece == nullptr){
            continue;
        }
        if(piece->isPlayerOne != team && std::abs(piece->x - x) == 2 && std::abs(piece->y - y) == 2 ) {
            scene->removeItem(piece);
            flag = true;
            }
        }
    if (flag){
        QMessageBox::information(nullptr, "Invalid Operation!", "Queen used her specialAbility");
    }
    return true;
}

// ---------------------- King ----------------------

King::King(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::yellow, scene){
    name = "King";
    specialAbilityText = "Swap positions with a nearest friendly Knight";}


// change the position with the KINGHT
bool King::specialAbility(QGraphicsScene *scene) {
    Q_UNUSED(scene);

    // checking the validation //
    if (hasUsedAbility) {
        if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
            QMessageBox::warning(mainWindow,
                               QStringLiteral("CANNOT USE"),
                               QStringLiteral("The swaping skill is used!"),
                               QMessageBox::Ok);
        }
        return false;
    }

    Knight* nearestKnight = nullptr;
    int minDistance = INT_MAX;

    // seeking for nearset knight
    for (auto item : scene->items()) {
        Piece* piece = dynamic_cast<Piece*>(item);
        if (piece && dynamic_cast<Knight*>(piece) && piece->isPlayerOne == this->isPlayerOne) {
            int distance = std::abs(piece->x - this->x) + std::abs(piece->y - this->y);
            if (distance < minDistance) {
                minDistance = distance;
                nearestKnight = dynamic_cast<Knight*>(piece);
            }
        }
    }

    // if found, then change
    if (nearestKnight) {
        int tempX = this->x;
        int tempY = this->y;

        this->setPos(nearestKnight->x * 50, nearestKnight->y * 50);
        this->x = nearestKnight->x;
        this->y = nearestKnight->y;

        nearestKnight->setPos(tempX * 50, tempY * 50);
        nearestKnight->x = tempX;
        nearestKnight->y = tempY;

        hasUsedAbility = true;

        if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
            return true;
        }
        return true;
    }

    // if not found
    if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
        QMessageBox::warning(mainWindow,
                           QStringLiteral("CANNOT USE!"),
                           QStringLiteral("Cannot find the knight!"),
                           QMessageBox::Ok);
    }
    return false;
}

// ---------------------- Bishop ----------------------

Bishop::Bishop(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::cyan, scene) {
    name = "Bishop";
    specialAbilityText = "Places a Pawn in front. Usable twice.";}


bool Bishop::specialAbility(QGraphicsScene *scene)
{
    if (abilityUsesLeft <= 0) {
        if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
            QMessageBox::warning(mainWindow,
                               QStringLiteral("CANNOT USE!"),
                               QStringLiteral("Run out of the chances!"),
                               QMessageBox::Ok);
        }
        return false;
    }

    // calculate the coordinates depends on the player //
    int frontX = x;
    int frontY = isPlayerOne ? y + 1 : y - 1;

    // within the board
    if (frontY < 0 || frontY >= 14) {
        if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
            QMessageBox::warning(mainWindow,
                               QStringLiteral("CANNOT USE!"),
                               QStringLiteral("Cannot spawn outside the board"),
                               QMessageBox::Ok);
        }
        return false;
    }
    // checking the emptyness//
    Piece* targetPiece = nullptr;
    for (auto item : scene->items()) {
        if (Piece* piece = dynamic_cast<Piece*>(item)) {
            if (piece->x == frontX && piece->y == frontY) {
                targetPiece = piece;
                break;
            }
        }
    }

    if (targetPiece) {
        if (targetPiece->isPlayerOne == isPlayerOne) {
            if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
                QMessageBox::warning(mainWindow,
                                   QStringLiteral("CANNOT USE!"),
                                   QStringLiteral("Cannot spawn upon your own piece!"),
                                   QMessageBox::Ok);
            }
            return false;
        } else {
            // kill the piece if it belongs to the opponent.
            scene->removeItem(targetPiece);
            delete targetPiece;
        }
    }

    // create new pawn
    Pawn* newPawn = new Pawn(frontX, frontY, isPlayerOne, scene);
    abilityUsesLeft--;

    // message box
    if (MainWindow* mainWindow = qobject_cast<MainWindow*>(scene->parent())) {
        QString message = targetPiece
            ? QStringLiteral("Kill & spawn a new pawn（Left:%1）").arg(abilityUsesLeft)
            : QStringLiteral("Spawn a new pawn！（Left:%1）").arg(abilityUsesLeft);
        mainWindow->showCaptureMessage(message);
    }

    return true;
}

// checking the king is alive//
bool Piece::isKingCaptured(QGraphicsScene *scene)
{
    bool kingCaptured = false;
    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece && dynamic_cast<King*>(piece) && piece->name == "King") {
            if (piece->x == this->x && piece->y == this->y) {
                kingCaptured = true;
                break;
            }
        }
    }
    return kingCaptured;
}
