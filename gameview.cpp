#include "gameview.h"
#include <QKeyEvent>
#include <QWidget>
#include <QPixmap>
#include <QMessageBox>

GameView::GameView(QWidget *parent) : QWidget(parent)
{

    setWindowTitle("Maci Laci");
    setStyleSheet("background-image: url(:image/free.jpg)");

    grid = new QGridLayout;

    newGame();

    setLayout(grid);

    connect(&_model, SIGNAL(gameWon()), this, SLOT(_modelGameWon()));
    connect(&_model, SIGNAL(gameOver()), this, SLOT(_modelGameOver()));
    connect(&_model, SIGNAL(fieldChanged(Coordinate,Coordinate,GameModel::FieldType)), this, SLOT(_modelFieldChanged(Coordinate,Coordinate,GameModel::FieldType)));
}

void GameView::newGame()
{

    foreach(GameButton *button, buttons) {

        grid->removeWidget(button);
        delete button;
    }

    buttons.clear();
    _model.newGame();

    for(int i = 0; i < 10; ++i) {

        for(int j = 0; j < 10; ++j) {

            GameButton *button = new GameButton(Coordinate(i,j,0), this);
            grid->addWidget(button, i, j);
            buttons.append(button);

            switch (_model.getField(i,j)) {
            case GameModel::Bear:

                button->setBear();
                break;

            case GameModel::Guard:

                button->setGuard();
                break;

            case GameModel::GuardTerritory:

                button->setGuardTerritory();
                break;

            case GameModel::Obstacle:

                button->setObstacle();
                break;

            case GameModel::Basket:

                button->setBasket();
                break;

            case GameModel::Free:

                button->setFree();
                break;

            }
        }
    }
}

void GameView::_modelFieldChanged(Coordinate previous, Coordinate current, GameModel::FieldType type)
{
    foreach(GameButton *button, buttons) {

        if(button->getCoord() == previous) {

            button->setFree();
        }

        if(button->getCoord() == current) {

            switch(type) {

            case GameModel::Bear:

                button->setBear();
                _model.previous = _model.player;
                break;

            case GameModel::Guard:

                button->setGuard();
                break;

            case GameModel::Basket:

                button->setBasket();
                break;

            case GameModel::GuardTerritory:

                button->setGuardTerritory();
                break;

            case GameModel::Free:

                button->setFree();
                break;

            }
        }
    }

    _model.checkWin();
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_S) {

        _model.stepPlayer(1,0);
        _modelFieldChanged(_model.previous, _model.player, GameModel::Bear);

    }

    if(event->key() == Qt::Key_D) {

        _model.stepPlayer(0,1);
        _modelFieldChanged(_model.previous, _model.player, GameModel::Bear);
    }

    if(event->key() == Qt::Key_A) {

        _model.stepPlayer(0, -1);
        _modelFieldChanged(_model.previous, _model.player, GameModel::Bear);
    }

    if(event->key() == Qt::Key_W) {

        _model.stepPlayer(-1, 0);
        _modelFieldChanged(_model.previous, _model.player, GameModel::Bear);
    }
}

void GameView::_modelGameOver()
{
    _modelFieldChanged(_model.previous, _model.player, GameModel::Bear);
    QMessageBox::information(this, trUtf8("Játék vége!"), trUtf8("Vesztettél!"));
}

void GameView::_modelGameWon()
{
    QMessageBox::information(this, trUtf8("Játék vége!"), trUtf8("A játék döntetlen lett!"));
    //QMessageBox::setStyleSheet("background: white");
}
