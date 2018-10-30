#include "newgamedialog.h"
#include "gameview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameView *g;
    g = new GameView;

    g->show();

    return a.exec();
}
