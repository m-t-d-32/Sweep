#include "game.h"
#include "ui_game.h"
#include <QMessageBox>
#include <square.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <QPushButton>
#include <QFile>
#include <QTimer>
#include <QString>
#include <QInputDialog>
#include "selects.h"
#include "rank.h"
int maxX;
int maxY;
int len=25;
int numberofbooms;
int nowis=0;
int countalltime=0;
QString ph_string[3];
QFile *gamefile;
int mintime_used[3];
extern QApplication *extra;
Game::Game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game)
{
    srand(time(NULL));
    ui->setupUi(this);

    gamefile->open(QFile::ReadOnly);
    char temp[100];
    gamefile->readLine(temp,100);
    if (strcmp(temp,"Activated!\n")==0)
    {
            wasActivated=true;
            ui->Active->setText("产品已激活");
            ui->Active->setEnabled(false);
    }
    else
    {
        wasActivated=false;
    }
    gamefile->readLine(temp,100);
    maxX=atoi(temp);
    gamefile->readLine(temp,100);
    maxY=atoi(temp);
    gamefile->readLine(temp,100);
    numberofbooms=atoi(temp);
    gamefile->readLine(temp,100);
    nowis=atoi(temp);

    for (int i=0;i<3;i++)
    {
        gamefile->readLine(temp,100);
        ph_string[i]=QString(temp);
        gamefile->readLine(temp,100);
        mintime_used[i]=atoi(temp);
    }
    gamefile->close();
    this->setWindowTitle("扫雷");
    this->setWindowIcon(QIcon(":/new/prefix1/Main.ico"));
    all_timer=new QTimer();
    lose_timer=new QTimer();
    connect(all_timer,SIGNAL(timeout()),this,SLOT(add1s()));
    connect(lose_timer,SIGNAL(timeout()),this,SLOT(lose()));
    create();
    this->show();
}

Game::~Game()
{
    delete ui;
}

void Game::check(bool doit,int dowhat)
{
    if (dowhat==0)
    {
        ui->Low->setChecked(doit);
    }
    else if (dowhat==1)
    {
        ui->Middle->setChecked(doit);
    }
    else if (dowhat==2)
    {
        ui->High->setChecked(doit);
    }
    else
    {
        ui->Self->setChecked(doit);
    }
}

void Game::torightclick()
{
    int n=numberofbooms;
    for (int i=0;i<maxX;i++)
    {
        for (int j=0;j<maxY;j++)
        {
            if (buttons[i][j]->getrightclick())
                n--;
        }
    }
    ui->Displayer->display(n);
}

void Game::writesomething()
{
    QString temp=QString::number(maxX);
    QByteArray tempSTR=temp.toUtf8();
    gamefile->write(tempSTR);
    gamefile->write("\n");
    temp=QString::number(maxY);
    tempSTR=temp.toUtf8();
    gamefile->write(tempSTR);
    gamefile->write("\n");
    temp=QString::number(numberofbooms);
    tempSTR=temp.toUtf8();
    gamefile->write(tempSTR);
    gamefile->write("\n");
    temp=QString::number(nowis);
    tempSTR=temp.toUtf8();
    gamefile->write(tempSTR);
    gamefile->write("\n");
    for (int i=0;i<3;i++)
    {
        QByteArray temp = ph_string[i].toUtf8();
        gamefile->write(temp);
        temp=(QString::number(mintime_used[i])).toUtf8();
        gamefile->write(temp);
        gamefile->write("\n");
    }
}

void Game::add1s()
{
    countalltime++;
    ui->DisTimer->display(countalltime);
}

void Game::limit(int x,int y)
{
    buttons[x][y]->open();
    if (judge(x,y+1)) limit(x,y+1);
    if (judge(x+1,y+1)) limit(x+1,y+1);
    if (judge(x-1,y+1)) limit(x-1,y+1);
    if (judge(x,y-1)) limit(x,y-1);
    if (judge(x+1,y-1)) limit(x+1,y-1);
    if (judge(x-1,y-1)) limit(x-1,y-1);
    if (judge(x+1,y)) limit(x+1,y);
    if (judge(x-1,y)) limit(x-1,y);
}
bool Game::judge(int x,int y)
{
    if (border(x,y))
        return false;
    if (buttons[x][y]->getboom())
        return false;
    if (buttons[x][y]->getrightclick())
        return false;
    if (buttons[x][y]->getsign())
    {
        buttons[x][y]->open();
        return false;
    }
    if (buttons[x][y]->getopen())
        return false;
    return true;
}
bool Game::border(int x,int y)
{
    if (x<0 || x>=maxX || y<0 || y>=maxY)
        return true;
    return false;
}

void Game::toclick(int x,int y,bool hasboom)
{
    if (hasboom)
    {
        buttons[x][y]->doboom();
        lose_timer->start(60);
    }
    else
    {        
        limit(x,y);
        int count=0;
        for (int i=0;i<maxX;i++)
        {
            for (int j=0;j<maxY;j++)
            {
                if (buttons[i][j]->getopen())
                    count++;
            }
        }
        if (count>=(maxX*maxY-numberofbooms))
        {
            win();
        }
    }
}

void Game::create()
{
    check(true,nowis);
    if (maxX<9) maxX=9;
    else if (maxX>30) maxX=30;
    if (maxY<9) maxY=9;
    else if (maxY>24) maxY=24;
    if (numberofbooms<10) numberofbooms=10;
    else if(numberofbooms>(maxX-1)*(maxY-1)) numberofbooms=(maxX-1)*(maxY-1);
    ui->Displayer->display(numberofbooms);
    this->setMaximumSize(maxX*len+60,maxY*len+80);
    this->resize(maxX*len+60,maxY*len+80);


    //ui->Back->setWindowFlags(Qt::WindowStaysOnBottomHint);

    if (gamefile->open(QIODevice::ReadWrite))
    {
        char temp[100];
        gamefile->readLine(temp,100);
        gamefile->resize(0);
        gamefile->write(temp);
        //gamefile->write("\n");
        writesomething();
    }
    else
    {
        QMessageBox::critical(0,"错误","游戏档案文件无法写入！");
    }
    gamefile->close();

    buttons=new Square **[maxX];
    for (int i=0;i<maxX;i++)
    {
        buttons[i]=new Square *[maxY];
        for (int j=0;j<maxY;j++)
        {
            buttons[i][j]=new Square(this);
            buttons[i][j]->setVisible(false);
        }
    }
    for (int i=0;i<maxX;i++)
    {
        for (int j=0;j<maxY;j++)
        {
            buttons[i][j]->setGeometry(len+len*i,len+len*j+10,len,len);
            buttons[i][j]->set(i,j);
            buttons[i][j]->setVisible(true);
            buttons[i][j]->setStyleSheet("");
            connect(buttons[i][j],SIGNAL(byclick(int,int,bool)),this,SLOT(toclick(int,int,bool)));
            connect(buttons[i][j],SIGNAL(byrightclick()),this,SLOT(torightclick()));
        }
    }
    for (int i=0;i<numberofbooms;i++)
    {
        int x=rand()%maxX;
        int y=rand()%maxY;
        if (!(buttons[x][y]->getboom()))
        {
            buttons[x][y]->setboom(true);
        }
        else
        {
            i--;
        }
    }
    for (int i=0;i<maxX;i++)
    {
        for (int j=0;j<maxY;j++)
        {
            int temp=0;
            if (!border(i,j+1) && buttons[i][j+1]->getboom()) temp++;
            if (!border(i+1,j+1) && buttons[i+1][j+1]->getboom()) temp++;
            if (!border(i-1,j+1) && buttons[i-1][j+1]->getboom()) temp++;
            if (!border(i,j-1) && buttons[i][j-1]->getboom()) temp++;
            if (!border(i+1,j-1) && buttons[i+1][j-1]->getboom()) temp++;
            if (!border(i-1,j-1) && buttons[i-1][j-1]->getboom()) temp++;
            if (!border(i+1,j) && buttons[i+1][j]->getboom()) temp++;
            if (!border(i-1,j) && buttons[i-1][j]->getboom()) temp++;
            buttons[i][j]->setsign(temp);
        }
    }
    ui->Displayer->setGeometry(len,maxY*len+20,60,25);
    ui->DisTimer->setGeometry(maxX*len-80,maxY*len+20,60,25);

    countalltime=0;
    all_timer->start(1000);
}

void Game::Delete()
{
    for (int i=0;i<=3;i++)
    {
        check(false,i);
    }
    for (int i=0;i<maxX;i++)
    {
        for (int j=0;j<maxY;j++)
        {
            delete buttons[i][j];
        }
        delete buttons[i];
    }
    delete buttons;
    ui->Displayer->display(0);
    ui->DisTimer->display(0);
}

void Game::lose()
{
    all_timer->stop();
    static int x=0;
    static int y=0;
    for (int i=x;i<maxX;i++)
    {
        for (int j=y;j<maxY;j++)
        {
            if (buttons[i][j]->getboom() && (!buttons[i][j]->getwasboom()))
            {
                buttons[i][j]->doboom();
                x=i;
                y=j;
                return;
            }
            buttons[i][j]->setEnabled(false);
        }
        y=0;
    }
    lose_timer->stop();
    x=0;
    y=0;
    QMessageBox::information(0,QString("游戏结束!"),tr("很遗憾，你输了!"));
    //ui->Back->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void Game::win()
{
    all_timer->stop();
    if ( nowis<=2 && mintime_used[nowis]>countalltime)
    {
        if (wasActivated)
        {
            ph_string[nowis]=QInputDialog::getText(0,"破纪录啦！","已破记录，请留大名！")+"\n";
            mintime_used[nowis]=countalltime;

            if (gamefile->open(QIODevice::ReadWrite))
            {
                char temp[100];
                gamefile->readLine(temp,100);
                gamefile->resize(0);
                gamefile->write(temp);
                writesomething();
            }
            else
            {
                QMessageBox::critical(0,"错误","游戏档案文件无法写入！");
            }
            gamefile->close();
        }
        else
        {
            QMessageBox::information(0,"游戏结束","恭喜，你赢了！");
            QMessageBox::warning(0,"需要激活","你已打破纪录，但无法使用英雄榜功能！");
        }
    }
    else
    {
        QMessageBox::information(0,"游戏结束","恭喜，你赢了！");
    }
    for (int i=0;i<maxX;i++)
    {
        for (int j=0;j<maxY;j++)
        {
            buttons[i][j]->setEnabled(false);
        }
    }
}

void Game::on_New_triggered()
{
    Delete();
    create();
}

void Game::on_Low_triggered()
{
    Delete();
    maxX=10;
    maxY=10;
    numberofbooms=10;
    nowis=0;
    create();
}

void Game::on_Middle_triggered()
{
    Delete();
    maxX=16;
    maxY=16;
    numberofbooms=40;
    nowis=1;
    create();
}


void Game::on_High_triggered()
{
    Delete();
    maxX=30;
    maxY=16;
    numberofbooms=99;
    nowis=2;
    create();
}

void Game::on_Self_triggered()
{
    Delete();
    Selects *p=new Selects();
    p->exec();
    delete p;
    nowis=3;
    create();
}

void Game::on_About_triggered()
{
    QMessageBox::information(0,tr("关于 扫雷"),"版本 2.7 Final\n\n基于Action 14内核框架构建\n\n版权所有 保留所有权利");
}

void Game::on_Exit_triggered()
{
    this->close();
}

void Game::on_Help_triggered()
{
    QMessageBox::information(0,"帮助","扫雷：游戏目标是在最短的时间内根据点击格子出现的数字找出所有非雷格子，同时避免踩雷，踩到一个雷即全盘皆输。\n\n\
游戏区包括雷区、地雷计数器（位于左下角，记录剩余地雷数）和计时器（位于右下角，记录游戏时间），确定大小的矩形雷区\
中随机布置一定数量的地雷(初级为9*9个方块10个雷，中级为16*16个方块40个雷，高级为16*30个方块99个雷，自定义级别\
可以自己设定雷区大小和雷数，但是雷区大小不能超过24*30)，玩家需要尽快找出雷区中的所有不是地雷的方块，而不许踩到地雷。\n\n\
游戏的基本操作包括左键单击（Left Click）、右键单击（Right Click）两种。其中左键用于打开安全的格子，推进游戏进度；\
如果方块上出现数字，则该数字表示其周围3×3区域中的地雷数（一般为8个格子，对于边块为5个格子，对于角块为3个格子。所以扫雷中最大的数字为8），\
右键用于标记地雷，以辅助判断。");
}

void Game::on_Active_triggered()
{
    QString temp=QInputDialog::getText(0,"请输入激活码","输入16位激活码激活产品");
    if (temp=="1234567890123456")
    {
        wasActivated=true;

        if (gamefile->open(QIODevice::ReadWrite))
        {
            char temp[100]="Activated!\n";
            gamefile->resize(0);
            gamefile->write(temp);
            writesomething();
            ui->Active->setText("产品已激活");
            ui->Active->setEnabled(false);
        }
        else
        {
            QMessageBox::critical(0,"错误","游戏档案文件无法写入！");
        }
        gamefile->close();
        QMessageBox::information(0,"恭喜","激活成功！");
    }
    else
    {
        QMessageBox::warning(0,"错误","激活失败！");
    }
}

void Game::on_Heros_triggered()
{
    if (!wasActivated)
    {
        QMessageBox::warning(0,"需要激活","英雄榜功能需要激活才可以使用！");
    }
    else
    {
        Rank showline;
        showline.exec();
    }
}
