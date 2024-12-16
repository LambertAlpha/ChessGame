// piece.h
#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include "terrain.h"

// base for piece
class Piece : public QGraphicsEllipseItem {
public:
    int x, y;          // grid location
    bool isPlayerOne;  // the belonging of the piece
    virtual ~Piece() {}
    virtual bool specialAbility(QGraphicsScene *scene) = 0;  // whether the special ability used successfully
    std::string getSpecialAbilityText() const {
            return specialAbilityText;
        }
    QString name = "Piece";
    bool isKingCaptured(QGraphicsScene *scene);
    Piece(int x, int y, bool isPlayerOne, QColor color, QGraphicsScene *scene);
    void moveTo(int destX, int destY, QGraphicsScene *scene);
    int getErrorCase() const { return error_case; };

protected:
    std::string specialAbilityText; // description of the special ability
private:
    int error_case = 0;
};


class Knight : public Piece {
public:
    Knight(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool specialAbility(QGraphicsScene *scene) override;
    bool isUsingSpecialAbility = false;
    void moveTo(QGraphicsScene *scene,int destX,int destY);
private:
    bool hasCharged = false;


};


class Pawn : public Piece {
public:
    Pawn(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool specialAbility(QGraphicsScene *scene) override;

};


class Bomb : public Piece {
public:
    Bomb(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool specialAbility(QGraphicsScene *scene) override;

};


class Queen : public Piece {
public:
    Queen(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool specialAbility(QGraphicsScene *scene) override;

};


class King : public Piece {
public:
    King(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool specialAbility(QGraphicsScene *scene) override;
    bool hasUsedAbility = false;

};


class Bishop : public Piece {
public:
    Bishop(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool specialAbility(QGraphicsScene *scene) override;

private:
    int abilityUsesLeft = 2;
};

#endif // PIECE_H
