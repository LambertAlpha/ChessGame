// piece.cpp
#include "piece.h"
#include <cmath>
#include <QGraphicsScene>

// 基类构造函数
Piece::Piece(int x, int y, bool isPlayerOne, QColor color, QGraphicsScene *scene)
    : QGraphicsEllipseItem(x * 50, y * 50, 50, 50), x(x), y(y), isPlayerOne(isPlayerOne)
{
    setBrush(QBrush(color));                   // 设置棋子的颜色
    setPen(QPen(Qt::black));                   // 设置棋子边框颜色
    setFlag(QGraphicsItem::ItemIsSelectable); // 使棋子可选
    scene->addItem(this);                      // 将棋子添加到场景中
}

// 棋子移动
void Piece::moveTo(int destX, int destY, QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

    // 更新位置
    setPos(destX * 50, destY * 50);
    x = destX;
    y = destY;
}

// ---------------------- Knight ----------------------

Knight::Knight(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::blue, scene) {}

// 骑士的移动规则
bool Knight::canMove(int destX, int destY, TerrainType terrain)
{
    int dx = std::abs(destX - x);
    int dy = std::abs(destY - y);
    bool validMove = (dx == 2 && dy == 1) || (dx == 1 && dy == 2);

    if(!validMove){
        return false;
    }

    // 地形限制
    switch(terrain){
        case TerrainType::Forest:
            // 棋子在 Forest 中的视野限制，需要在 MainWindow 中实现
            return true;
        case TerrainType::River:
            // Knight 无法过河
            return false;
        case TerrainType::Mountain:
            // 在 Mountain 上，Knight 只能像 Pawn 一样直走一格
            // 假设 Pawn 只能直走一格
            return (dx <=1 && dy <=1);
        case TerrainType::Desert:
            // 穿过 Desert 的角色会降为 Pawn，移动一格
            return (dx <=1 && dy <=1);
        case TerrainType::Land:
            return true;
        default:
            return false;
    }
}

// 骑士没有特殊能力
void Knight::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

    // 无特殊能力
}

// ---------------------- Pawn ----------------------

Pawn::Pawn(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::green, scene) {}

// 兵的移动规则
bool Pawn::canMove(int destX, int destY, TerrainType terrain)
{
    // 兵只能走一格直线
    bool validMove = (std::abs(destX - x) == 1 && std::abs(destY - y) == 0);

    if(!validMove){
        return false;
    }

    // 地形限制
    switch(terrain){
        case TerrainType::Forest:
            // 兵在 Forest 中视野受限，具体逻辑需要进一步实现
            // 这里简单返回 true
            return true;
        case TerrainType::Mountain:
            // 兵可以在 Mountain 上移动
            return true;
        case TerrainType::Desert:
            // 兵穿过 Desert 不受影响
            return true;
        case TerrainType::River:
            // 兵可以过河
            return true;
        case TerrainType::Land:
            return true;
        default:
            return false;
    }
}

// 兵没有特殊能力
void Pawn::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

    // 无特殊能力
}

// ---------------------- Bomb ----------------------

Bomb::Bomb(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::red, scene) {}

// 炸弹的移动规则
bool Bomb::canMove(int destX, int destY, TerrainType terrain)
{
    // 炸弹只能走一格
    if (!(std::abs(destX - x) <= 1 && std::abs(destY - y) <= 1)) {
        return false;
    }

    // 地形限制
    switch(terrain){
        case TerrainType::Forest:
            return true;
        case TerrainType::River:
            // Bomb 无法过河
            return false;
        case TerrainType::Mountain:
            // Bomb 无法在 Mountain 上移动
            return false;
        case TerrainType::Desert:
            // Bomb 穿过 Desert 降为 Pawn，移动一格
            return (std::abs(destX - x) <=1 && std::abs(destY - y) <=1);
        case TerrainType::Land:
            return true;
        default:
            return false;
    }
}

// 炸弹的特殊能力：自爆，炸掉周围3x3区域的棋子
void Bomb::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

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
}

// ---------------------- Queen ----------------------

Queen::Queen(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::yellow, scene) {}

// 皇后的移动规则
bool Queen::canMove(int destX, int destY, TerrainType terrain)
{
    int dx = std::abs(destX - x);
    int dy = std::abs(destY - y);
    bool validMove = (dx == 2 && dy == 0) || (dx == 0 && dy == 2) ||
                     (dx == 2 && dy == 2);

    if(!validMove){
        return false;
    }

    // 地形限制
    switch(terrain){
        case TerrainType::Forest:
            return true;
        case TerrainType::River:
            // Queen 无法过河
            return false;
        case TerrainType::Mountain:
            // Queen 无法在 Mountain 上移动
            return false;
        case TerrainType::Desert:
            // Queen 穿过 Desert 降为 Pawn，移动一格
            return false; // 因为当前移动是符合 Queen 的移动规则
        case TerrainType::Land:
            return true;
        default:
            return false;
    }
}

// 皇后没有特殊能力
void Queen::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

    // 无特殊能力
}

// ---------------------- King ----------------------

King::King(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::magenta, scene) {}

// 国王的移动规则
bool King::canMove(int destX, int destY, TerrainType terrain)
{
    // 国王每次可以走一格，直线或对角线
    if (!(std::abs(destX - x) <= 1 && std::abs(destY - y) <= 1)) {
        return false;
    }

    // 地形限制
    switch(terrain){
        case TerrainType::Forest:
            return true;
        case TerrainType::River:
            return true; // 假设国王可以过河
        case TerrainType::Mountain:
            return true;
        case TerrainType::Desert:
            return true;
        case TerrainType::Land:
            return true;
        default:
            return false;
    }
}

// 国王没有特殊能力
void King::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

    // 无特殊能力
}

// ---------------------- Bishop ----------------------

Bishop::Bishop(int x, int y, bool isPlayerOne, QGraphicsScene *scene)
    : Piece(x, y, isPlayerOne, Qt::cyan, scene) {}

// 主教的移动规则
bool Bishop::canMove(int destX, int destY, TerrainType terrain)
{
    int dx = std::abs(destX - x);
    int dy = std::abs(destY - y);
    bool validMove = (dx == 2 && dy == 2);

    if(!validMove){
        return false;
    }

    // 地形限制
    switch(terrain){
        case TerrainType::Forest:
            return true;
        case TerrainType::River:
            // Bishop 无法过河
            return false;
        case TerrainType::Mountain:
            // Bishop 无法在 Mountain 上移动
            return false;
        case TerrainType::Desert:
            // Bishop 穿过 Desert 降为 Pawn，移动一格
            return false; // 因为当前移动是符合 Bishop 的移动规则
        case TerrainType::Land:
            return true;
        default:
            return false;
    }
}

// 主教没有特殊能力
void Bishop::specialAbility(QGraphicsScene *scene)
{
    Q_UNUSED(scene); // 标记参数为未使用

    // 无特殊能力
}
