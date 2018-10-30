#include "gamemodel.h"

GameModel::GameModel()
{
    gameTable = new FieldType*[10];

    for(int i = 0; i < 10; ++i) {

        gameTable[i] = new FieldType[10];
    }
}

void GameModel::newGame()
{
    for(int i = 0; i < 10; ++i) {

        for(int j = 0; j < 10; ++j) {

            gameTable[i][j] = Free; // mindegyik mező legyen szabad
        }
    }

    _guards.clear();
    _baskets.clear();

    basketsHave = 0;
    guards = 1;
    baskets = 6;
    obstacles = 8;

    gameTable[0][0] = Bear;
    player.x = 0;
    player.y = 0;
    previous = player;

    setElements();

    stepTemp.x = -1;
    stepTemp.y = -1;

    guardstep = new QTimer;
    guardstep->start(1000);

    connect(guardstep, SIGNAL(timeout()), this, SLOT(stepGuard()));
}

void GameModel::setElements()
{
    setGuards();
    setBaskets();
    setObstacles();
}

void GameModel::setGuards()
{
    int x = qrand() % 10;
    int y = qrand() % 10;

    step = -1;

    while(guards > 0) {

        if(gameTable[x][y] == Free) {

            gameTable[x][y] = Guard;
            Coordinate grd(x,y, 8 - guards);
            _guards.append(grd);
            x = qrand() % 10;
            y = qrand() % 10;
            guards--;
        }

        else {

            x = qrand() % 10;
            y = qrand() % 10;
        }
    }
}

void GameModel::setBaskets()
{
    int x = qrand() % 10;
    int y = qrand() % 10;

    while(baskets > 0) {

        if(gameTable[x][y] == Free) {

            gameTable[x][y] = Basket;
            Coordinate bsk(x,y, 0);
            _baskets.append(bsk);
            x = qrand() % 10;
            y = qrand() % 10;
            baskets--;
        }

        else {

            x = qrand() % 10;
            y = qrand() % 10;
        }
    }
}

void GameModel::setObstacles()
{
    int x = qrand() % 9 + 1;
    int y = qrand() % 10;

    while(obstacles > 0) {

        if(gameTable[x][y] == Free) {

            gameTable[x][y] = Obstacle;
            x = qrand() % 9 + 1;
            y = qrand() % 10;
            obstacles--;
        }

        else {

            x = qrand() % 9 + 1;
            y = qrand() % 10;
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

    if(player.x > 9 || player.x < 0 || player.y > 9 || player.y < 0 || gameTable[player.x][player.y] == Obstacle) {

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
        Coordinate _new(_guards[index].x + step, _guards[index].y, 0);
        fieldChanged(_guards[index], _new, GameModel::Guard);
        _guards[index].x += step;
        isGameOver();

        if(_guards[index].x == -1 || _guards[index].x == 10) {

            step = step * -1;
            _guards[index].x += step;
            Coordinate _new(_guards[index].x + step, _guards[index].y, 0);
            fieldChanged(_guards[index], _new, GameModel::Guard);
            _guards[index].x += step;
        }

        if(getField(_guards[index].x + step, _guards[index].y) == Obstacle) {

            step = step * -1;
            _guards[index].type = Coordinate::Horizontal;
            return;
        }

        if(getField(_guards[index].x + step, _guards[index].y) == Basket) {

            stepTemp.x = _guards[index].x + step;
            stepTemp.y = _guards[index].y;
        }

        if(stepTemp.x == _guards[index].x - step && _baskets.contains(stepTemp)) {

            fieldChanged(stepTemp, stepTemp, GameModel::Basket);
        }
}

void GameModel::horizontalGuard(int index)
{
        isGameOver();
        Coordinate _new(_guards[index].x, _guards[index].y + step, 0);
        fieldChanged(_guards[index], _new, GameModel::Guard);
        _guards[index].y += step;

        if(_guards[index].y == -1 || _guards[index].y == 10) {

                step = step * -1;
                _guards[index].y += step;
                Coordinate _new(_guards[index].x, _guards[index].y + step, 0);
                fieldChanged(_guards[index], _new, GameModel::Guard);
                _guards[index].y += step;
        }

        if(getField(_guards[index].x, _guards[index].y + step) == Obstacle) {

            step = step * -1;
            _guards[index].type = Coordinate::Vertical;
            return;
        }

        if(getField(_guards[index].x, _guards[index].y + step) == Basket) {

            stepTemp.x = _guards[index].x;
            stepTemp.y = _guards[index].y + step;
        }

        if(stepTemp.y == _guards[index].y - step && _baskets.contains(stepTemp)) {

            fieldChanged(stepTemp, stepTemp, GameModel::Basket);

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
    if( x < 0 || x > 9 || y < 0 || y > 9) return None;

    return gameTable[x][y];
}
