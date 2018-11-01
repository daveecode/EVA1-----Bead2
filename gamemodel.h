#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include "coordinate.h"
#include "newgamedialog.h"
#include <QWidget>
#include <QTimer>
#include <QTime>

class GameModel : public QObject
{
    Q_OBJECT

public:

    enum FieldType { Bear, Basket, Obstacle, Guard, GuardTerritory, Free, None };

    GameModel();

    FieldType getField(int x, int y);
    Coordinate player;
    Coordinate previous;

    int size;
    int baskets;
    int guards;
    int obstacles;
    int basketsHave;
    QTimer *guardstep;
    QTime labelTime;
    QTime _time;

    void newGame();
    void stepPlayer(int x, int y);
    void getBaskets();
    bool checkWin();

signals:

    void gameWon();
    void gameOver();
    void fieldChanged(Coordinate previous, Coordinate current, GameModel::FieldType type);
    void updateTime();

public slots:

    void stepGuard();
    void pauseOrPlay();

private:

    FieldType** gameTable;

    void setGuards();
    void setBaskets();
    void setObstacles();
    void setElements();
    void verticalGuard(int index);
    void horizontalGuard(int index);
    void isGameOver();

    int step;
    int tmp;
     /// ahányat felszedtünk

    QVector <Coordinate> _baskets;
    QVector <Coordinate> _guards;
    QVector <int> steps;
    Coordinate stepTemp;





};

#endif // GAMEMODEL_H
