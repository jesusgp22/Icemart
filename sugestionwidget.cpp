#include "sugestionwidget.h"
#include "ui_sugestionwidget.h"
#include <Grafo.h>
#include <QDebug>

SugestionWidget::SugestionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SugestionWidget)
{
    ui->setupUi(this);
}

SugestionWidget::~SugestionWidget()
{
    delete ui;
}

void SugestionWidget::on_pushButton_4_clicked()
{
    typedef Path<Digrafo> Camino;
    Camino path1;
    sugerir(dg, path1);
    string linea[3];
    int i=0;
    for(Camino::Iterator itor(path1); itor.has_current(); itor.next())
    {
        if( itor.get_current_node()->get_info().nombre != "Fuente" && itor.get_current_node()->get_info().nombre != "Sumidero" )
        {
            linea[i]= itor.get_current_node()->get_info().nombre;
            i++;
        }
    }
    ui->label_2->setText(QString::fromStdString(linea[0]));
    ui->label_3->setText(QString::fromStdString(linea[1]));
    ui->label_4->setText(QString::fromStdString(linea[2]));
    ui->checkBox_2->setText(ui->label_2->text());
    ui->checkBox_3->setText(ui->label_3->text());
    ui->checkBox_4->setText(ui->label_4->text());
}

void SugestionWidget::on_pushButton_6_clicked()
{
    insertarNodos(dg);
    conectarRecetasArchivo(dg);
    ui->pushButton_4->setEnabled(true);
}

void SugestionWidget::on_pushButton_7_clicked()
{
    guardarGrafo(dg);
}

void SugestionWidget::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        qDebug() << "Receta1 y Receta2";
        penalizarCombinacionRecetas1(dg);
        qDebug() << "Se CLICKEO";
    }
    else
    {
        qDebug() << "Receta1 y Receta2";
        premiarCombinacionRecetas1(dg);
        qDebug() << "Se DESCLIKEO";
    }
}

void SugestionWidget::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        qDebug() << "Receta1";
        penalizarReceta(dg, 1);
        qDebug() << "Se CLICKEO";
    }
    else
    {
        qDebug() << "Receta1";
        premiarReceta(dg, 1);
        qDebug() << "Se DESCLIKEO";
    }
}

void SugestionWidget::on_checkBox_3_clicked()
{
    if(ui->checkBox_3->isChecked())
    {
        qDebug() << "Receta2";
        penalizarReceta(dg, 2);
        qDebug() << "Se CLICKEO";
    }
    else
    {
        qDebug() << "Receta2";
        premiarReceta(dg, 2);
        qDebug() << "Se DESCLIKEO";
    }
}

void SugestionWidget::on_checkBox_4_clicked()
{
    if(ui->checkBox_4->isChecked())
    {
        qDebug() << "Receta3";
        penalizarReceta(dg, 3);
        qDebug() << "Se CLICKEO";
    }
    else
    {
        qDebug() << "Receta3";
        premiarReceta(dg, 3);
        qDebug() << "Se DESCLIKEO";
    }
}

void SugestionWidget::on_checkBox_5_clicked()
{
    if(ui->checkBox_5->isChecked())
    {
        qDebug() << "Receta2 y Receta3";
        penalizarCombinacionRecetas2(dg);
        qDebug() << "Se CLICKEO";
    }
    else
    {
        qDebug() << "Receta2 y Receta3";
        premiarCombinacionRecetas2(dg);
        qDebug() << "Se DESCLIKEO";
    }
}
