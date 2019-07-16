#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "square.h"
namespace Ui {
class Game;
}

class Game : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();
    void check(bool,int);
    void limit(int,int);
    bool border(int,int);
    bool judge(int,int);
    void Delete();
    void create();
    void writesomething();
private slots:

    void on_New_triggered();

    void on_Low_triggered();

    void on_Middle_triggered();

    void on_High_triggered();

    void on_Self_triggered();

    void on_About_triggered();

    void on_Exit_triggered();

    void on_Help_triggered();

    void on_Active_triggered();

    void on_Heros_triggered();

public slots:

    void toclick(int,int,bool);

    void torightclick();

    void lose();

    void win();

    void add1s();
private:
    Square ***buttons;
    bool wasActivated;
    Ui::Game *ui;

    QTimer *all_timer;
    QTimer *lose_timer;
};

#endif // GAME_H
