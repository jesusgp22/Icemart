#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QStackedWidget>

namespace Ui {
class SetupWidget;
}

class SetupWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit SetupWidget(QWidget *parent = 0);
    ~SetupWidget();

private slots:
    void on_startButton_clicked();

    void on_nextPage1Button_clicked();

    void on_fitnessStateField_currentIndexChanged(int index);

    void on_objectiveField_currentIndexChanged(int index);

    void on_dietField_currentIndexChanged(int index);

    void on_endButton_clicked();

    void on_fatsField_editingFinished();

    void on_proteinsField_editingFinished();

    void on_carbsField_editingFinished();

private:
    Ui::SetupWidget *ui;
    bool validate();

signals:
    void setPage(int index);
};

#endif // SETUPWIDGET_H
