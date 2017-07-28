#include "selects.h"
#include "ui_selects.h"
#include <QMessageBox>
extern int maxX;
extern int maxY;
extern int numberofbooms;

Selects::Selects(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Selects)
{
    ui->setupUi(this);
    this->setWindowTitle("设置");
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Selects::~Selects()
{
    delete ui;
}

void Selects::on_Confirm_clicked()
{
    maxX=ui->Length->text().toDouble();
    maxY=ui->Width->text().toDouble();
    numberofbooms=ui->Number->text().toDouble();
    this->close();
}
