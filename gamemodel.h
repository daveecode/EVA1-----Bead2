#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include "coordinate.h"
#include "newgamedialog.h"
#include <QWidget>
#include <QTimer>

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

    void newGame();
    void stepPlayer(int x, int y);
    void getBaskets();
    bool checkWin();

signals:

    void gameWon();
    void gameOver();
    void fieldChanged(Coordinate previous, Coordinate current, GameModel::FieldType type);

public slots:

    void stepGuard();

private:

    FieldType** gameTable;

    void setGuards();
    void setBaskets();
    void setObstacles();
    void setElements();
    void verticalGuard(int index);
    void horizontalGuard(int index);
    void isGameOver();

    int fstep;  /// léptetéshez kell
    int sstep;
    int tstep;
    int fostep;
    int step;
    int basketsHave; /// ahányat felszedtünk

    QVector <Coordinate> _baskets;
    QVector <Coordinate> _guards;
    QVector <int> steps;
    Coordinate stepTemp;

    QTimer *guardstep;

};

#endif // GAMEMODEL_H
