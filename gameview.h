#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>

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

    QPushButton *smallButton;
    QPushButton *mediumButton;
    QPushButton *bigButton;
    QPushButton *pause;
    QLabel *bsk;
    QLabel *time;
    QGridLayout *grid;
    QHBoxLayout *upRow;
    QVBoxLayout *main;
    GameModel _model;
    QVector <GameButton*> buttons;

private slots:

    void _modelFieldChanged(Coordinate previous, Coordinate current, GameModel::FieldType type);
    void _modelGameWon();
    void _modelGameOver();
    void _modelUpdateTime();
    void newGame();
    void small();
    void medium();
    void big();
};

#endif // GAMEVIEW_H
