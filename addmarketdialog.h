#ifndef ADDMARKETDIALOG_H
#define ADDMARKETDIALOG_H

#include <QDialog>

namespace Ui {
class AddMarketDialog;
}

class AddMarketDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMarketDialog(QWidget *parent = 0);
    ~AddMarketDialog();

private slots:
    void on_foodComboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

private:
    Ui::AddMarketDialog *ui;
};

#endif // ADDMARKETDIALOG_H
