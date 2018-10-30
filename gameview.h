#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include <QWidget>
#include <QPushButton>
#include <QLayout>

#include "gamemodel.h"
#include "gamebutton.h"

class GameView : public QWidget
{
    Q_OBJECT

public:

    GameView(QWidget *parent = 0);

protected:

    void keyPressEvent(QKeyEvent *event);

private:

    QGridLayout *grid;
    GameModel _model;
    QVector <GameButton*> buttons;

    void newGame();

private slots:

    void _modelFieldChanged(Coordinate previous, Coordinate current, GameModel::FieldType type);
    void _modelGameWon();
    void _modelGameOver();
};

#endif // GAMEVIEW_H
