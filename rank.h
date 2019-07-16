#ifndef RANK_H
#define RANK_H

#include <QDialog>

namespace Ui {
class Rank;
}

class Rank : public QDialog
{
    Q_OBJECT

public:
    explicit Rank(QWidget *parent = 0);
    ~Rank();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Rank *ui;
};

#endif // RANK_H
