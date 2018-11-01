#include "gameview.h"
#include "enddialog.h"
#include <QKeyEvent>
#include <QWidget>
#include <QPixmap>
#include <QMessageBox>

GameView::GameView(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Maci Laci");

    grid = new QGridLayout;
    upRow = new QHBoxLayout;
    main = new QVBoxLayout;
    smallButton = new QPushButton("Kis pálya", this);
    mediumButton = new QPushButton("Közepes pálya", this);
    bigButton = new QPushButton("Nagy pálya", this);
    pause = new QPushButton(trUtf8("Játék indítása/megállítása"), this);
    pause->setEnabled(false);
    bsk = new QLabel(trUtf8("Megszerzett kosarak száma: "), this);
    bsk->setText(bsk->text() + QString::number(0));
    time = new QLabel(trUtf8("Eltelt játékidő: "), this);
    time->setText(time->text() + _model._time.toString());

    upRow->addWidget(bsk);
    upRow->addWidget(time);
    upRow->addWidget(smallButton);
    upRow->addWidget(mediumButton);
    upRow->addWidget(bigButton);
    upRow->addWidget(pause);

    //newGame();

    //setLayout(grid);

    main->addLayout(upRow);
    main->addLayout(grid);
    setLayout(main);

    connect(smallButton, SIGNAL(clicked(bool)), this, SLOT(small()));
    connect(mediumButton, SIGNAL(clicked(bool)), this, SLOT(medium()));
    connect(bigButton, SIGNAL(clicked(bool)), this, SLOT(big()));
    connect(&_model, SIGNAL(gameWon()), this, SLOT(_modelGameWon()));
    connect(&_model, SIGNAL(gameOver()), this, SLOT(_modelGameOver()));
    connect(&_model, SIGNAL(fieldChanged(Coordinate,Coordinate,GameModel::FieldType)), this, SLOT(_modelFieldChanged(Coordinate,Coordinate,GameModel::FieldType)));
    connect(&_model, SIGNAL(updateTime()), this, SLOT(_modelUpdateTime()));
    connect(pause, SIGNAL(clicked(bool)), &_model, SLOT(pauseOrPlay()));
}

void GameView::small()
{
    _model.size = 8;
    _model.guards = 2;
    _model.obstacles = 8;
    _model.baskets = 8;

    newGame();
}

void GameView::medium()
{
    _model.size = 9;
    _model.guards = 2;
    _model.obstacles = 9;
    _model.baskets = 9;

    newGame();
}

void GameView::big()
{
    _model.size = 10;
    _model.guards = 2;
    _model.baskets = 10;
    _model.obstacles = 10;

    newGame();
}

void GameView::newGame()
{

    foreach(GameButton *button, buttons) {

        grid->removeWidget(button);
        delete button;
    }

    buttons.clear();
    _model.newGame();
    bsk->setText("A megszerzett kosarak száma: " + QString::number(_model.basketsHave));
    time->setText("Eltelt játékidő: " + _model._time.toString());

    for(int i = 0; i < _model.size; ++i) {

        for(int j = 0; j < _model.size; ++j) {

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

    pause->setEnabled(true);
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
                bsk->setText("A megszerzett kosarak száma: " + QString::number(_model.basketsHave));
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

void GameView::_modelUpdateTime()
{
    time->setText("Eltelt játékidő: " + _model._time.toString());
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
    _model.guardstep->stop();
    _modelFieldChanged(_model.previous, _model.player, GameModel::Bear);
    QMessageBox::information(this, trUtf8("Játék vége!"), trUtf8("Vesztettél! Meglátott a vadőr!"));
}

void GameView::_modelGameWon()
{
    _model.guardstep->stop();
    QMessageBox::information(this, trUtf8("Játék vége!"), trUtf8("Megnyerted a játékot!"));
}

