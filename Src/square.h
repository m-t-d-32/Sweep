#ifndef SQUARE
#define SQUARE
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
class Square:public QPushButton
{
    Q_OBJECT

private:
    int x;
    int y;
    bool opened;
    bool hasboom;
    int sign;
    int rightclick;
    bool wasboom;
signals:
    void byclick(int,int,bool);
    void byrightclick();
public:
    Square(QWidget *w):QPushButton(w),opened(false),hasboom(false),sign(0),rightclick(0),wasboom(false){}

    bool getrightclick()
    {
        return rightclick;
    }

    void mousePressEvent(QMouseEvent *event)
    {
        if (event->button()==Qt::RightButton)
        {
            if (!opened)
            {
                if (rightclick==0)
                {
                    setStyleSheet("background-image: url(:/new/prefix1/flag.png);");
                    rightclick=1;
                }
                else
                {
                    setStyleSheet("");
                    rightclick=0;
                }
                emit byrightclick();
            }
        }
        else
        {
            if (!opened && !rightclick)
            {
                opened=true;
                emit byclick(x,y,hasboom);
            }
        }
    }

    void set(int x,int y)
    {
        this->x=x;
        this->y=y;
    }

    void setsign(int x)
    {
        sign=x;
    }

    void showsign()
    {
        this->setFont(QFont("宋体",15));
        this->setText(QString::number(sign));
    }

    int getsign()
    {
        return sign;
    }

    void open()
    {
        if (sign)
            showsign();
        setStyleSheet("border:1px solid blue;");
        opened=true;
    }

    bool getopen()
    {
        return opened;
    }

    void doboom()
    {
        setStyleSheet("background-image: url(:/new/prefix1/bomb.png);");
        wasboom=true;
    }

    bool getwasboom()
    {
        return wasboom;
    }

    void setboom(bool t)
    {
        hasboom=t;
    }

    bool getboom()
    {
        return hasboom;
    }
};

#endif // SQUARE

