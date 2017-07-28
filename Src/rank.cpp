#include "rank.h"
#include "ui_rank.h"
extern QString ph_string[3];
QString st_string[3]={"初级","中级","高级"};
extern int mintime_used[3];
Rank::Rank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rank)
{
    ui->setupUi(this);
    setWindowTitle("扫雷英雄榜");
    QString temp="\n";
    for (int i=0;i<3;i++)
    {
        temp+=st_string[i]+"  :"+ph_string[i]+"\n用时  "+QString::number(mintime_used[i])+"\n\n";
    }
    ui->Ranker->setText(ui->Ranker->text()+temp);
    this->setWindowIcon(QIcon(":/new/prefix1/Hero.ico"));
}

Rank::~Rank()
{
    delete ui;
}

void Rank::on_pushButton_clicked()
{
    close();
}
