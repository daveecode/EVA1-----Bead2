#include "gamebutton.h"
#include <QPixmap>

GameButton::GameButton(Coordinate coordinate, QWidget *parent) : QPushButton(parent), coord(coordinate)
{
    setFixedSize(80,80);
    //setStyleSheet("background: white, margin : none, border : none");
}

Coordinate GameButton::getCoord()
{
    return coord;
}

void GameButton::setBear()
{
    setStyleSheet("background-image : url(:image/bear.jpg)");
    type = GameModel::Bear;
}

void GameButton::setBasket()
{
    setStyleSheet("background-image : url(:image/basket.jpg)");
    type = GameModel::Basket;
}

void GameButton::setGuard()
{
    setStyleSheet("background-image : url(:image/guard.jpg)");
    type = GameModel::Guard;
}

void GameButton::setGuardTerritory()
{
    setStyleSheet("background : red");
    type = GameModel::GuardTerritory;
}

void GameButton::setObstacle()
{
    //setStyleSheet("background : black");
    setStyleSheet("background-image: url(:image/obstacle.jpg)");
    type = GameModel::Obstacle;
}

void GameButton::setFree()
{
    setStyleSheet("background-image : url(:image/free.jpg)");
    type = GameModel::Free;
}

GameModel::FieldType GameButton::getField()
{
    return type;
}
