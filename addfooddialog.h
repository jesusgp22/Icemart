#ifndef ADDFOODDIALOG_H
#define ADDFOODDIALOG_H

#include <QDialog>

namespace Ui {
class AddFoodDialog;
}

class AddFoodDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFoodDialog(QWidget *parent = 0);
    ~AddFoodDialog();

private slots:
    void on_buttonBox_accepted();

    void on_foodComboFox_currentIndexChanged(int index);

private:
    Ui::AddFoodDialog *ui;
};

#endif // ADDFOODDIALOG_H
