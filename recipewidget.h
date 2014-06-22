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

    void on_soupButton_clicked();

    void on_allIcon_clicked();

    void on_breakfastButton_clicked();

    void on_fishButton_clicked();

    void on_meatButton_clicked();

    void on_chickenButton_clicked();

    void on_saladButton_clicked();

    void on_riceButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::RecipeWidget *ui;

    void fillRecipeList(QString filter="");

signals:
    void setPage(int index);
};

#endif // RECIPEWIDGET_H
