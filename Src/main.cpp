#include "game.h"
#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QMessageBox>
extern QFile *gamefile;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gamefile=new QFile("gamedata.dat");
    if (!gamefile->open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0,"错误","游戏档案文件缺失，加载失败！");
        gamefile->close();
        return 0;
    }
    gamefile->close();
    QTextCodec *c=QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(c);
#ifndef __WINNT
    QTextCodec::setCodecForCStrings(c);
    QTextCodec::setCodecForTr(c);
#endif
    Game w;
    return a.exec();
}
