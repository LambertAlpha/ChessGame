// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "piece.h"
#include "terrain.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int currentPlayer; // 1 或 2，表示当前玩家
    Piece *selectedPiece; // 当前选中的棋子

    TerrainType boardTerrain[14][14]; // 棋盘的地形信息
    std::vector<Piece*> player1Pieces; // 玩家1的备战区棋子
    std::vector<Piece*> player2Pieces; // 玩家2的备战区棋子

    void setupGameBoard();          // 初始化棋盘
    void addPreparationZones();     // 添加备战区
    void addPieces();               // 添加初始棋子
    void switchPlayer();            // 切换当前玩家
    void handleMove(int destX, int destY); // 处理棋子移动
    void onGraphicsViewClicked(QPointF point); // 处理点击事件
    void placePiece(int pieceIndex, bool isPlayerOne, int baseX, int baseY); // 放置棋子到基地

    TerrainType getTerrain(int x, int y); // 获取指定位置的地形

    // 检查当前玩家的视野
    bool canSee(Piece* piece);
};

#endif // MAINWINDOW_H
