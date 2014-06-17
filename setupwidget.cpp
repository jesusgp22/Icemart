#include "setupwidget.h"
#include "ui_setupwidget.h"

#include <QSettings>
#include <QVariant>
#include <QDebug>

SetupWidget::SetupWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SetupWidget)
{
    ui->setupUi(this);
}

SetupWidget::~SetupWidget()
{
    delete ui;
}

void SetupWidget::on_startButton_clicked()
{
    if(ui->nameField->text()!=""){
        QSettings settings;
        settings.setValue("name",ui->nameField->text());
        this->setCurrentWidget(ui->BasicsPage);
    }else{
        //display error message
    }
}

bool SetupWidget::validate(){
    if(ui->weightField->value() == 0){
        return false;
        //display error:select weight
    }
    if(ui->heightField->value() == 0){
        return false;
        //display error: select weight
    }
    if(ui->ageField->value() == 0){
        return false;
        //display error: select age
    }
    if(ui->genderField->currentIndex()==0){
        return false;
        //display error: select gender
    }
    if(ui->activityLevelField->currentIndex()==0){
        return false;
        //display error: select activity level
    }
    return true;
}

void SetupWidget::on_nextPage1Button_clicked()
{
    if(validate()){
        QSettings settings;
        float w = ui->weightField->value();
        float h = ui->heightField->value();
        float IMC = w/(h*h);
        qDebug()<<"IMC is "<<IMC;
        short unsigned int objective;

        settings.setValue("weight",w);
        settings.setValue("height",h);
        settings.setValue("IMC",IMC);

        //determine state of the user
        if(IMC>=40){ //morbid obesity
            settings.setValue("state","morbid_obesity");
            objective = 0;
            this->ui->fitnessStateField->setCurrentIndex(0);
        }else if (IMC>=30 && IMC<40){ //obesity
            settings.setValue("state","obesity");
            objective = 0;
            this->ui->fitnessStateField->setCurrentIndex(1);
        }else if(IMC>=25 && IMC<30){ //overweight
            settings.setValue("state","overweight");
            objective = 0;
            this->ui->fitnessStateField->setCurrentIndex(2);
        }else if(IMC>=18.5 && IMC<25){ //healthy
            objective = 1;
            settings.setValue("state","healthy");
            this->ui->fitnessStateField->setCurrentIndex(3);
        }else if(IMC<18.5){ //underweight
            objective = 2;
            settings.setValue("state","underweight");
            this->ui->fitnessStateField->setCurrentIndex(4);
        }

        //determine caloric ingest
        int age = ui->ageField->value();
        short unsigned int gender = ui->genderField->currentIndex() - 1;

        settings.setValue("age",age);
        settings.setValue("gender",gender);
        float TMB;

        if (gender == 0){
            TMB = w*10 + 6.25*h*100 - 5*age + 5;
        }else{
            TMB = w*10 + 6.25*h*100 - 5*age -161;
        }
        qDebug()<<"TMB is "<<TMB;
        settings.setValue("TMB",TMB);

        float IDC;
        int index = ui->activityLevelField->currentIndex();

        switch(index){
            case 1:
                IDC = TMB*1.2;
                break;
            case 2:
                IDC = TMB*1.375;
                break;
            case 3:
                IDC = TMB*1.55;
                break;
            case 4:
                IDC = TMB*1.725;
                break;
            case 5:
                IDC = TMB*1.9;
                break;
            default:
                IDC = TMB*1.2;
                break;
                //shouldn't happen
        }

        qDebug()<<"IDC is "<<IDC;

        float IDFC;
        if(objective==0){ //perder peso
            ui->objectiveField->setCurrentIndex(0);
            IDFC = IDC - 300;
        }else if(objective==1){
            IDFC = IDC;
            ui->objectiveField->setCurrentIndex(1);
        }else{
            IDFC = IDC +300;
            ui->objectiveField->setCurrentIndex(2);
        }
        ui->IDFCField->setText(QString::number(IDFC));
        settings.setValue("IDFC",IDFC);
        qDebug()<<"IDFC is "<<IDFC;

        this->setCurrentWidget(ui->AdvancedPage);
    }
}

void SetupWidget::on_fitnessStateField_currentIndexChanged(int index)
{
}

void SetupWidget::on_objectiveField_currentIndexChanged(int index)
{
    QSettings settings;
    float IDFC;
    float IDC = settings.value("IDC").toFloat();
    if(index==0){ //perder peso
        IDFC = IDC - 300;
    }else if(index==1){
        IDFC = IDC;
    }else if(index==2){
        IDFC = IDC + 300;
    }
    settings.setValue("IDFC",IDFC);
    ui->IDFCField->setText(QString::number(IDFC));

}

void SetupWidget::on_dietField_currentIndexChanged(int index)
{
    switch (index) {
    case 0: //base diet
        ui->fatsField->setText("20");
        ui->carbsField->setText("55");
        ui->proteinsField->setText("25");
        break;
    case 1: //high carb
        ui->fatsField->setText("20");
        ui->carbsField->setText("60");
        ui->proteinsField->setText("20");
        break;
    case 2: //high prot
        ui->fatsField->setText("35");
        ui->carbsField->setText("25");
        ui->proteinsField->setText("40");
        break;
    case 3: //zone
        ui->fatsField->setText("30");
        ui->carbsField->setText("40");
        ui->proteinsField->setText("30");
        break;
    default:
        break;
    }
    if(index==4){
        ui->fatsField->setEnabled(true);
        ui->carbsField->setEnabled(true);
        ui->proteinsField->setEnabled(true);
    }else{
        ui->fatsField->setEnabled(false);
        ui->carbsField->setEnabled(false);
        ui->proteinsField->setEnabled(false);
    }
}

bool checkPercentages(int a,int b,int c){
    if((a+b+c)!=100){
        return false;
    }
    return true;
}

void SetupWidget::on_endButton_clicked()
{

}

void SetupWidget::on_fatsField_editingFinished()
{
    bool good;
    int a = ui->fatsField->text().toInt(&good);
    int b = ui->carbsField->text().toInt();
    int c = ui->proteinsField->text().toInt();
    if(good){
        if(!checkPercentages(a,b,c)){
            qDebug()<<"error percentages don't add up to 100";
        }
    }else{
        qDebug()<<"error: enter an integer";
    }
}

void SetupWidget::on_proteinsField_editingFinished()
{
    bool good;
    int a = ui->fatsField->text().toInt();
    int b = ui->carbsField->text().toInt();
    int c = ui->proteinsField->text().toInt(&good);
    if(good){
        if(!checkPercentages(a,b,c)){
            qDebug()<<"error percentages don't add up to 100";
        }
    }else{
        qDebug()<<"error: enter an integer";
    }
}

void SetupWidget::on_carbsField_editingFinished()
{
    bool good;
    int a = ui->fatsField->text().toInt();
    int b = ui->carbsField->text().toInt(&good);
    int c = ui->proteinsField->text().toInt();
    if(good){
        if(!checkPercentages(a,b,c)){
            qDebug()<<"error percentages don't add up to 100";
        }
    }else{
        qDebug()<<"error: enter an integer";
    }
}
