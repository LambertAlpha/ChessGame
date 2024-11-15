// piece.h
#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include "terrain.h"

// 棋子基类
class Piece : public QGraphicsEllipseItem {
public:
    int x, y;          // 棋子所在的格子坐标
    bool isPlayerOne;  // 判断棋子属于哪个玩家

    Piece(int x, int y, bool isPlayerOne, QColor color, QGraphicsScene *scene);
    virtual bool canMove(int destX, int destY, TerrainType terrain) = 0; // 检查移动是否合法
    virtual void specialAbility(QGraphicsScene *scene) = 0;             // 棋子特殊能力
    void moveTo(int destX, int destY, QGraphicsScene *scene);          // 移动棋子
    virtual ~Piece() {}
};

// 骑士类
class Knight : public Piece {
public:
    Knight(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool canMove(int destX, int destY, TerrainType terrain) override;
    void specialAbility(QGraphicsScene *scene) override;
};

// 兵类
class Pawn : public Piece {
public:
    Pawn(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool canMove(int destX, int destY, TerrainType terrain) override;
    void specialAbility(QGraphicsScene *scene) override;
};

// 炸弹类
class Bomb : public Piece {
public:
    Bomb(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool canMove(int destX, int destY, TerrainType terrain) override;
    void specialAbility(QGraphicsScene *scene) override;
};

// 皇后类
class Queen : public Piece {
public:
    Queen(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool canMove(int destX, int destY, TerrainType terrain) override;
    void specialAbility(QGraphicsScene *scene) override;
};

// 国王类
class King : public Piece {
public:
    King(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool canMove(int destX, int destY, TerrainType terrain) override;
    void specialAbility(QGraphicsScene *scene) override;
};

// 主教类
class Bishop : public Piece {
public:
    Bishop(int x, int y, bool isPlayerOne, QGraphicsScene *scene);
    bool canMove(int destX, int destY, TerrainType terrain) override;
    void specialAbility(QGraphicsScene *scene) override;
};

#endif // PIECE_H
