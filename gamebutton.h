#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include <QPushButton>
#include <QWidget>
#include "coordinate.h"
#include "gamemodel.h"

class GameButton : public QPushButton
{
    Q_OBJECT

public:

    GameButton(Coordinate coordinate, QWidget *parent = 0);

    Coordinate getCoord();

    void setBear();
    void setGuard();
    void setGuardTerritory();
    void setBasket();
    void setObstacle();
    void setFree();

    GameModel::FieldType getField();

private:

    Coordinate coord;
    GameModel::FieldType type;

};

#endif // GAMEBUTTON_H
