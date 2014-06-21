#ifndef RECIPEWIDGET_H
#define RECIPEWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class RecipeWidget;
}

class RecipeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecipeWidget(QWidget *parent = 0);
    ~RecipeWidget();

private slots:
    void on_homeButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_backButton_clicked();

private:
    Ui::RecipeWidget *ui;

signals:
    void setPage(int index);
};

#endif // RECIPEWIDGET_H
