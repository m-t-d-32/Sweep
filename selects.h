#ifndef SELECTS_H
#define SELECTS_H

#include <QDialog>

namespace Ui {
class Selects;
}

class Selects : public QDialog
{
    Q_OBJECT

public:
    explicit Selects(QWidget *parent = 0);
    ~Selects();

private slots:
    void on_Confirm_clicked();

private:
    Ui::Selects *ui;
};

#endif // SELECTS_H
