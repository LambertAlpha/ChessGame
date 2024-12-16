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
    TerrainType getTerrain(int x, int y);
    void showCaptureMessage(QString & message); // eating message box
    ~MainWindow();
    static Piece * FindPieceAtXY(int x, int y, QGraphicsScene *scene); // find pieces at x & y

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool gameOver;// flag
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int currentPlayer; // 1 or 2; standing for player1 or player 2
    Piece *selectedPiece; // the selected piece currently

    Terrain terrain; // class
    std::vector<Piece*> player1Pieces;
    std::vector<Piece*> player2Pieces;

    void setupGameBoard();
    void addLegend();
    void addPieces();
    void switchPlayer();
    void handleMove(int destX, int destY);
    void onGraphicsViewClicked(QPointF point);

};

#endif // MAINWINDOW_H
