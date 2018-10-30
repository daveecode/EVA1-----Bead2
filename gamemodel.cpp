#include "gamemodel.h"

GameModel::GameModel()
{
    gameTable = new FieldType*[10];

    for(int i = 0; i < 10; ++i) {

        gameTable[i] = new FieldType[10];
    }

    size = 10;
    guards = 2;
    baskets = 6;
    obstacles = 8;

    guardstep = new QTimer;
    connect(guardstep, SIGNAL(timeout()), this, SLOT(stepGuard()));
}

void GameModel::newGame()
{

    gameTable = new FieldType*[size];

    for(int i = 0; i < size; ++i) {

        gameTable[i] = new FieldType[size];
    }
    for(int i = 0; i < size; ++i) {

        for(int j = 0; j < size; ++j) {

            gameTable[i][j] = Free; // mindegyik mező legyen szabad
        }
    }

    if(guardstep->isActive()) {
        guardstep->stop();
    }

    _guards.clear();
    _baskets.clear();
    steps.clear();

    basketsHave = 0;

    gameTable[0][0] = Bear;
    player.x = 0;
    player.y = 0;
    previous = player;
    stepTemp.x = -10;
    stepTemp.y = -10;

    setElements();

    guardstep->start(1000);
}

void GameModel::setElements()
{
    setGuards();
    setBaskets();
    setObstacles();
}

void GameModel::setGuards()
{
    int x = qrand() % size;
    int y = qrand() % size;

    while(guards > 0) {

        if(gameTable[x][y] == Free) {

            gameTable[x][y] = Guard;
            Coordinate grd(x,y, 0);
            _guards.append(grd);
            steps.append(-1);
            x = qrand() % size;
            y = qrand() % size;
            guards--;
        }

        else {

            x = qrand() % size;
            y = qrand() % size;
        }
    }
}

void GameModel::setBaskets()
{
    int x = qrand() % size;
    int y = qrand() % size;

    while(baskets > 0) {

        if(gameTable[x][y] == Free) {

            gameTable[x][y] = Basket;
            Coordinate bsk(x,y, 0);
            _baskets.append(bsk);
            x = qrand() % size;
            y = qrand() % size;
            baskets--;
        }

        else {

            x = qrand() % size;
            y = qrand() % size;
        }
    }
}

void GameModel::setObstacles()
{
    int x = qrand() % size;
    int y = qrand() % size;

    while(obstacles > 0) {

        if(gameTable[x][y] == Free) {

            gameTable[x][y] = Obstacle;
            x = qrand() % size;
            y = qrand() % size;
            obstacles--;
        }

        else {

            x = qrand() % size;
            y = qrand() % size;
        }
    }
}

void GameModel::getBaskets()
{
    foreach(Coordinate coord, _baskets) {

        if(player == coord) {

            _baskets.removeAll(coord);
        }
    }
}

bool GameModel::checkWin()
{
    if(_baskets.size() == 0) {

        gameWon();
        return true;
    }

    else {

        return false;
    }
}

void GameModel::stepPlayer(int x, int y)
{

    gameTable[player.x][player.y] = Free;

    player.x += x;
    player.y += y;

    if(player.x > size - 1 || player.x < 0 || player.y > size - 1 || player.y < 0 || gameTable[player.x][player.y] == Obstacle) {

        player = previous;
    }

    isGameOver();

    gameTable[player.x][player.y] = Bear;

    getBaskets();
}

void GameModel::isGameOver()
{
    for(int i = 0; i < _guards.size(); ++i) {

        if(_guards[i].x + 1 == player.x && _guards[i].y == player.y) {
            gameOver();
        }

        if(_guards[i].x + 1 == player.x && _guards[i].y + 1 == player.y) {
            gameOver();
        }

        if(_guards[i].x + 1 == player.x && _guards[i].y - 1 == player.y) {
            gameOver();
        }

        if(_guards[i].x - 1 == player.x && _guards[i].y == player.y) {
            gameOver();
        }

        if(_guards[i].x - 1 == player.x && _guards[i].y - 1 == player.y) {
            gameOver();
        }

        if(_guards[i].x - 1 == player.x && _guards[i].y + 1 == player.y) {
            gameOver();
        }

        if(_guards[i].x == player.x && _guards[i].y + 1 == player.y) {
            gameOver();
        }

        if(_guards[i].x == player.x && _guards[i].y - 1 == player.y) {
            gameOver();
        }
    }
}

void GameModel::verticalGuard(int index)
{
    if(getField(_guards[index].x + steps[index], _guards[index].y) == Obstacle) {

        steps[index] = steps[index] * -1;
        _guards[index].type = Coordinate::Horizontal;
        return;
    }

    Coordinate _new(_guards[index].x + steps[index], _guards[index].y, 0);
    fieldChanged(_guards[index], _new, GameModel::Guard);
    _guards[index].x += steps[index];
    isGameOver();

    if(_guards[index].x == -1 || _guards[index].x == size) {

        steps[index] = steps[index] * -1;
        _guards[index].x += steps[index];
        Coordinate _new(_guards[index].x + steps[index], _guards[index].y, 0);
        fieldChanged(_guards[index], _new, GameModel::Guard);
        _guards[index].x += steps[index];
    }



    if(getField(_guards[index].x + steps[index], _guards[index].y) == Basket) {

        stepTemp.x = _guards[index].x + steps[index];
        stepTemp.y = _guards[index].y;
    }

    if(stepTemp.x == _guards[index].x - steps[index] && _baskets.contains(stepTemp)) {

        fieldChanged(stepTemp, stepTemp, GameModel::Basket);
    }
}

void GameModel::horizontalGuard(int index)
{

    if(getField(_guards[index].x, _guards[index].y + steps[index]) == Obstacle) {

        steps[index] = steps[index] * -1;
        _guards[index].type = Coordinate::Vertical;
        return;
    }

    if(getField(_guards[index].x, _guards[index].y + steps[index]) == Basket) {

        stepTemp.x = _guards[index].x;
        stepTemp.y = _guards[index].y + steps[index];
    }

    if(stepTemp.y == _guards[index].y - steps[index] && _baskets.contains(stepTemp)) {

        fieldChanged(stepTemp, stepTemp, GameModel::Basket);

    }

    isGameOver();
    Coordinate _new(_guards[index].x, _guards[index].y + steps[index], 0);
    fieldChanged(_guards[index], _new, GameModel::Guard);
    _guards[index].y += steps[index];  /// ez van rossz helyen, le kell vizgsálni a _new után az obstaclet!!!

    if(_guards[index].y == -1 || _guards[index].y == size) {


        if(stepTemp.y == _guards[index].y - steps[index] && _baskets.contains(stepTemp)) {

            fieldChanged(stepTemp, stepTemp, GameModel::Basket);
            steps[index] = steps[index] * -1;
            _guards[index].y += steps[index];
            Coordinate _new(_guards[index].x, _guards[index].y + steps[index], 0);
            fieldChanged(_guards[index], _new, GameModel::Guard);
            _guards[index].y += steps[index];
        }

        else {
            steps[index] = steps[index] * -1;
            _guards[index].y += steps[index];
            Coordinate _new(_guards[index].x, _guards[index].y + steps[index], 0);
            fieldChanged(_guards[index], _new, GameModel::Guard);
            _guards[index].y += steps[index];
        }
    }


}

void GameModel::stepGuard()
{
    for(int i = 0; i < _guards.size(); ++i) {

        if(_guards[i].type == Coordinate::Vertical) {

            verticalGuard(i);
        }

        else {
            horizontalGuard(i);
        }
    }
}

GameModel::FieldType GameModel::getField(int x, int y)
{
    if( x < 0 || x >= size || y < 0 || y >= size) return None;

    return gameTable[x][y];
}
