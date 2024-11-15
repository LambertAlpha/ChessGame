// mainwindow.cpp
// 该文件实现了棋盘的初始化、玩家的行动逻辑以及对地形和棋子的管理。
// 包含了所有的事件处理器、场景初始化和玩家交互的主逻辑。
// 角色和地形细节被实现为不同的类和枚举，以便实现更清晰的代码结构。
#include "mainwindow.h"
// mainwindow.h: 声明了 MainWindow 类及其成员函数，负责处理游戏的主要逻辑。
// 包括对棋盘、棋子以及游戏状态的管理。
#include "ui_mainwindow.h"
// ui_mainwindow.h: 由 Qt 设计器生成，包含用户界面的元素，
// 例如按钮、视图等。
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QMessageBox>
#include <QBrush>
#include <QPen>
#include <algorithm>
#include <QDebug>
#include <iostream>


// 构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentPlayer(1)
    , selectedPiece(nullptr)
{
    ui->setupUi(this);

    // 创建一个 QGraphicsScene 对象
    scene = new QGraphicsScene(this);

    // 设置棋盘
    setupGameBoard();

    // 添加棋子到备战区
    addPieces();

    // 添加备战区
    addPreparationZones();

    // 设置场景到 Graphics View
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    // 设置窗口标题
    setWindowTitle(QString("Chess Game - 玩家 %1 的回合").arg(currentPlayer));

    // 连接鼠标点击事件
    // 使用事件过滤器处理点击事件
    ui->graphicsView->installEventFilter(this);
}

// 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化棋盘
void MainWindow::setupGameBoard()
{
    const int boardSize = 14;
    const int cellSize = 50;

    // 定义地形区域
    for(size_t i = 0; i < boardSize; ++i){
        for(size_t j = 0; j < boardSize; ++j){
            // 默认地形为 Land
            boardTerrain[i][j] = TerrainType::Land;
        }
    }

    // 添加基地
    // 敌方 Base：纵向 11-13 列，横向 0-1 行
    for(size_t i = 11; i <= 13; ++i){
        for(size_t j = 0; j <= 1; ++j){
            boardTerrain[i][j] = TerrainType::Base;
        }
    }

    // 己方 Base：纵向 11-13 列，横向 12-13 行
    for(size_t i = 11; i <= 13; ++i){
        for(size_t j = 12; j <= 13; ++j){
            boardTerrain[i][j] = TerrainType::Base;
        }
    }

    // 添加 Forest
    // 左上角 Forest：纵向 4-6 列，横向 2-3 行
    for(size_t i = 4; i <= 6; ++i){
        for(size_t j = 2; j <= 3; ++j){
            boardTerrain[i][j] = TerrainType::Forest;
        }
    }

    // 左下角 Forest：纵向 4-6 列，横向 10-11 行
    for(size_t i = 4; i <= 6; ++i){
        for(size_t j = 10; j <= 11; ++j){
            boardTerrain[i][j] = TerrainType::Forest;
        }
    }

    // 添加 Mountain
    for(size_t i = 2; i <= 4; ++i){
        for(size_t j = 5; j <= 8; ++j){
            boardTerrain[i][j] = TerrainType::Mountain;
        }
    }

    // 添加 River
    for(size_t i = 7; i <= 13; ++i){
        for(size_t j = 6; j <= 7; ++j){
            boardTerrain[i][j] = TerrainType::River;
        }
    }

    // 上部 Deserts
    for(size_t i = 11; i <= 13; ++i){
        for(size_t j = 5; j <= 5; ++j){
            boardTerrain[i][j] = TerrainType::Desert;
        }
    }

    // 下部 Deserts
    for(size_t i = 11; i <= 13; ++i){
        for(size_t j = 8; j <= 8; ++j){
            boardTerrain[i][j] = TerrainType::Desert;
        }
    }

    // 绘制棋盘（14x14）
    for (size_t i = 0; i < boardSize; ++i) {
        for (size_t j = 0; j < boardSize; ++j) {
            QColor color;
            switch(boardTerrain[i][j]){
                case TerrainType::Land:
                    color = (i + j) % 2 == 0 ? QColor(255, 255, 255) : QColor(200, 200, 200);
                    break;
                case TerrainType::Forest:
                    color = QColor(34, 139, 34); // Forest Green
                    break;
                case TerrainType::River:
                    color = QColor(30, 144, 255); // Dodger Blue
                    break;
                case TerrainType::Mountain:
                    color = QColor(165, 42, 42); // Brown
                    break;
                case TerrainType::Desert:
                    color = QColor(210, 180, 140); // Tan
                    break;
                case TerrainType::Base:
                    color = QColor(173, 216, 230); // Light Blue for Base
                    break;
                default:
                    color = QColor(255, 255, 255);
            }
            QGraphicsRectItem *cell = scene->addRect(i * cellSize, j * cellSize, cellSize, cellSize, QPen(Qt::black), QBrush(color));
            cell->setZValue(-1); // 确保格子在最底层
        }
    }
}

// 添加初始棋子到备战区
void MainWindow::addPieces()
{
    // 玩家1的棋子（蓝色）
    // Knight x2, Bomb x1, Pawn x4, Queen x1, King x1, Bishop x2
    player1Pieces.push_back(new Knight(0, 0, true, scene)); // y坐标暂定，位置后续设置
    player1Pieces.push_back(new Knight(1, 0, true, scene));
    player1Pieces.push_back(new Bomb(2, 0, true, scene));
    player1Pieces.push_back(new Pawn(3, 0, true, scene));
    player1Pieces.push_back(new Pawn(4, 0, true, scene));
    player1Pieces.push_back(new Pawn(5, 0, true, scene));
    player1Pieces.push_back(new Pawn(6, 0, true, scene));
    player1Pieces.push_back(new Queen(7, 0, true, scene));
    player1Pieces.push_back(new King(8, 0, true, scene));
    player1Pieces.push_back(new Bishop(9, 0, true, scene));
    player1Pieces.push_back(new Bishop(10, 0, true, scene));

    // 将玩家1的棋子放置到上方备战区
    for(size_t i = 0; i < player1Pieces.size(); ++i){
        std::cout<<i<<std::endl;
        player1Pieces[i]->setPos(i * 0, -50); // 上方备战区 y = -1 * cellSize
    }

    // 玩家2的棋子（红色）
    // Knight x2, Bomb x1, Pawn x4, Queen x1, King x1, Bishop x2
    player2Pieces.push_back(new Knight(0, 14, false, scene));
    player2Pieces.push_back(new Knight(1, 14, false, scene));
    player2Pieces.push_back(new Bomb(2, 14, false, scene));
    player2Pieces.push_back(new Pawn(3, 14, false, scene));
    player2Pieces.push_back(new Pawn(4, 14, false, scene));
    player2Pieces.push_back(new Pawn(5, 14, false, scene));
    player2Pieces.push_back(new Pawn(6, 14, false, scene));
    player2Pieces.push_back(new Queen(7, 14, false, scene));
    player2Pieces.push_back(new King(8, 14, false, scene));
    player2Pieces.push_back(new Bishop(9, 14, false, scene));
    player2Pieces.push_back(new Bishop(10, 14, false, scene));

    // 将玩家2的棋子放置到下方备战区
    for(size_t i = 0; i < player2Pieces.size(); ++i){
    player2Pieces[i]->setPos(i * 50, 0); // 下方备战区靠近棋盘 y = 0
    }
}

// 添加备战区
void MainWindow::addPreparationZones()
{
    // 已移除未使用的变量 'cellSize'
    // 备战区的棋子已被放置在上方和下方备战区
}

// 切换当前玩家
void MainWindow::switchPlayer()
{
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    setWindowTitle(QString("Chess Game - 玩家 %1 的回合").arg(currentPlayer));

    // 更新所有棋子的可见性
    for (auto item : scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if(piece){
            bool visible = canSee(piece);
            piece->setVisible(visible);
        }
    }
}

// 处理棋子移动
void MainWindow::handleMove(int destX, int destY)
{
    if (!selectedPiece) return;

    TerrainType terrain = getTerrain(destX, destY);

    // 检查移动是否合法
    if (selectedPiece->canMove(destX, destY, terrain)) {
        // 查找是否有敌方棋子
        for (auto item : scene->items()) {
            Piece *piece = dynamic_cast<Piece*>(item);
            if (piece && piece->x == destX && piece->y == destY) {
                if (piece->isPlayerOne != selectedPiece->isPlayerOne) {
                    // 如果是炸弹，触发自爆
                    if (dynamic_cast<Bomb*>(selectedPiece)) {
                        selectedPiece->specialAbility(scene);
                        scene->removeItem(piece);
                        delete piece;
                        selectedPiece->moveTo(destX, destY, scene);
                        selectedPiece = nullptr;
                        break;
                    }
                    // 如果是国王，检查杀死条件
                    if (dynamic_cast<King*>(piece)) {
                        // 只能被 Pawn 或 Bomb 杀死
                        if (dynamic_cast<Pawn*>(selectedPiece) || dynamic_cast<Bomb*>(selectedPiece)) {
                            scene->removeItem(piece);
                            delete piece;
                            QMessageBox::information(this, "胜利", QString("玩家 %1 获胜！").arg(currentPlayer));
                            // 结束游戏
                            qApp->quit();
                        } else {
                            QMessageBox::information(this, "非法操作", "King 只能被 Pawn 或 Bomb 杀死！");
                            return;
                        }
                    } else {
                        // 普通吃子
                        scene->removeItem(piece);
                        delete piece;
                    }
                }
            }
        }

        // 移动棋子
        selectedPiece->moveTo(destX, destY, scene);

        // 处理 Desert 地形中的特殊规则
        TerrainType currentTerrain = getTerrain(destX, destY);
        if(currentTerrain == TerrainType::Desert){
            // 将棋子降为 Pawn
            Piece *newPawn = new Pawn(destX, destY, selectedPiece->isPlayerOne, scene);
            scene->removeItem(selectedPiece);
            delete selectedPiece;
            selectedPiece = newPawn;
        }

        // 检查是否到达对方基地
        bool reachedBase = false;
        if(currentPlayer == 1 && boardTerrain[destX][destY] == TerrainType::Base && destX >=11 && destX <=13 && destY >=0 && destY <=1){
            reachedBase = true;
        }
        if(currentPlayer == 2 && boardTerrain[destX][destY] == TerrainType::Base && destX >=11 && destX <=13 && destY >=12 && destY <=13){
            reachedBase = true;
        }

        if(reachedBase){
            QMessageBox::information(this, "胜利", QString("玩家 %1 获胜！").arg(currentPlayer));
            // 结束游戏
            qApp->quit();
        }

        // 检查是否有玩家没有棋子
        bool playerOneHasPieces = false;
        bool playerTwoHasPieces = false;
        for (auto item : scene->items()) {
            Piece *piece = dynamic_cast<Piece*>(item);
            if (piece) {
                if (piece->isPlayerOne) playerOneHasPieces = true;
                else playerTwoHasPieces = true;
            }
        }

        if (!playerOneHasPieces) {
            QMessageBox::information(this, "胜利", "玩家2 获胜！");
            qApp->quit();
        }
        if (!playerTwoHasPieces) {
            QMessageBox::information(this, "胜利", "玩家1 获胜！");
            qApp->quit();
        }

        // 切换玩家
        switchPlayer();
    }
}

// 事件过滤器，用于捕捉鼠标点击事件
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsView && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF point = ui->graphicsView->mapToScene(mouseEvent->pos());
        onGraphicsViewClicked(point);
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

// 处理 GraphicsView 的点击事件
void MainWindow::onGraphicsViewClicked(QPointF point)
{
    // 输出点击的坐标
    qDebug() << "Clicked coordinates: (" << point.x() << ", " << point.y() << ")";
    const int cellSize = 50;
    int x = static_cast<int>(point.x()) / cellSize;
    std::cout<< x<< std::endl;
    int y = static_cast<int>(point.y()) / cellSize;
    std::cout<< y<< std::endl;

    // 检查是否点击在备战区
    // 上方备战区：y == -1
    if(y == -1 && currentPlayer == 1){
        std::cout<<"is in Player1's set"<<std::endl;
        std::cout<<x<<std::endl;
        size_t pieceIndex = static_cast<size_t>(x);
        if(pieceIndex < player1Pieces.size()){
            Piece *piece = player1Pieces[pieceIndex];
            std::cout<<888888888<<std::endl;
            if(piece){
                // 选择棋子并高亮可放置的位置
                std::cout<<8888888<<std::endl;
                selectedPiece = piece;

                // 高亮基地中的可放置位置
                // 玩家1的基地：纵向 11-13 列，横向 0-1 行
                for(size_t i =11; i <=13; ++i){
                    std::cout<<8888888<<std::endl;
                    for(size_t j =0; j <=1; ++j){
                        if(boardTerrain[i][j] == TerrainType::Base){
                            QGraphicsRectItem *highlight = scene->addRect(i * cellSize, j * cellSize, cellSize, cellSize, QPen(Qt::red), QBrush(QColor(255,0,0,100)));
                            highlight->setData(0, 999);
                        }
                    }
                }
                return;
            }
        }
    }

    // 下方备战区：y == 13
    if(y == 0 && currentPlayer == 2){
        size_t pieceIndex = static_cast<size_t>(x);
        if(pieceIndex < player2Pieces.size()){
            Piece *piece = player2Pieces[pieceIndex];
            if(piece){
                // 选择棋子并高亮可放置的位置
                selectedPiece = piece;

                // 高亮基地中的可放置位置
                // 玩家2的基地：纵向 11-13 列，横向 12-13 行
                for(size_t i =11; i <=13; ++i){
                    for(size_t j =12; j <=13; ++j){
                        if(boardTerrain[i][j] == TerrainType::Base){
                            QGraphicsRectItem *highlight = scene->addRect(i * cellSize, j * cellSize, cellSize, cellSize, QPen(Qt::red), QBrush(QColor(255,0,0,100)));
                            highlight->setData(0, 999);
                        }
                    }
                }
                return;
            }
        }
    }

    // 如果选中了棋子, 尝试放置到点击的位置
    if (selectedPiece) {
        // 检查是否点击在自己的基地
        bool validPlacement = false;
        std::cout<<"current Player"<<currentPlayer<<std::endl;
        if (currentPlayer == 1 && x >= 11 && x <= 13 && y >= 0 && y <= 1) {
            validPlacement = true;
        }
        if (currentPlayer == 2 && x >= 11 && x <= 13 && y >= 12 && y <= 13) {
            validPlacement = true;
        }

        if(validPlacement){
            // 检查目标位置是否已有棋子
            bool occupied = false;
            for(auto item : scene->items()){
                Piece *piece = dynamic_cast<Piece*>(item);
                if(piece && piece->x == x && piece->y == y){
                    occupied = true;
                    break;
                }
            }

            if(!occupied){
                // 放置棋子
                std::cout<< "move to" << x << std::endl;
                std::cout<< "move to" << y << std::endl;

                selectedPiece->moveTo(x, y, scene);
                // 从备战区移除棋子
                if(currentPlayer ==1){
                    player1Pieces.erase(std::remove(player1Pieces.begin(), player1Pieces.end(), selectedPiece), player1Pieces.end());
                }
                else{
                    player2Pieces.erase(std::remove(player2Pieces.begin(), player2Pieces.end(), selectedPiece), player2Pieces.end());
                }
                selectedPiece = nullptr;

                // 移除高亮
                std::vector<QGraphicsItem*> itemsToRemove;
                for(auto highlight : scene->items()){
                    QGraphicsRectItem *rect = dynamic_cast<QGraphicsRectItem*>(highlight);
                    if(rect && rect->data(0).toInt() == 999){
                        itemsToRemove.push_back(rect);
                    }
                }
                for(auto item : itemsToRemove){
                    scene->removeItem(item);
                    delete item;
                }

                // 切换玩家
                switchPlayer();
                return;
            }
            else{
                QMessageBox::information(this, "非法操作", "目标位置已有棋子！");
                return;
            }
        }
        else{
            QMessageBox::information(this, "非法操作", "请选择正确的基地位置放置棋子！");
            return;
        }
    }
}

// 获取指定位置的地形
TerrainType MainWindow::getTerrain(int x, int y)
{
    if(x >=0 && x <14 && y >=0 && y <14){
        return boardTerrain[x][y];
    }
    return TerrainType::Land;
}

// 检查当前玩家的视野
bool MainWindow::canSee(Piece* piece)
{
    if(getTerrain(piece->x, piece->y) != TerrainType::Forest){
        return true; // 非 Forest 地形，无视野限制
    }

    // 检查是否有敌方棋子在同一 Forest 中
    bool enemyInSameForest = false;
    for(auto item : scene->items()){
        Piece* other = dynamic_cast<Piece*>(item);
        if(other && other->isPlayerOne != piece->isPlayerOne && getTerrain(other->x, other->y) == TerrainType::Forest){
            enemyInSameForest = true;
            break;
        }
    }

    if(enemyInSameForest){
        return true; // 有敌方棋子在同一 Forest 中，可以看到
    }

    // 否则，视野受限
    return false;
}
