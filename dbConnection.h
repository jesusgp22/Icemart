#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QSqlError>

#define WIPE false

void createTables(QString &sqlLine)
{
    QSqlQuery query;
    query.exec(sqlLine);
}

void createDatabase()
{
    QFile f(":/sql/schema.sql");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "cannot open resource file schema.sql";
    QTextStream in(&f);
    QString line;
    QString sqlStatement = "";
    while (!in.atEnd()){
        line = in.readLine();
        if(line.trimmed().startsWith('#') || line.isEmpty())
            continue;
        sqlStatement += line;
        if(sqlStatement.trimmed().endsWith(";")){
            createTables(sqlStatement);
            sqlStatement = "";
        }
    }
}

void populateDatabase(){

    QFile file(":/sql/food.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "cannot open resorce file food.txt";
    QTextStream input(&file);
    QSqlQuery query;
    QString line;
    int i=0;
    while(!input.atEnd())
    {
        line = input.readLine();
        int index = line.indexOf(":");
        if(index==-1){
            continue;
        }
        query.prepare("INSERT INTO Food (name, measure_unit) VALUES (?,?);");
        QString name = line.mid(0,index);
        QString measure_unit = line.mid(index+1);
        query.addBindValue(name);
        query.addBindValue(measure_unit);
        if(query.exec()){
            qDebug()<<"inserting"<<name<<measure_unit;
        }
    }

    QFile f(":/sql/recipes.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "cannot open resorce file recipes.txt";
    QTextStream in(&f);
    QString sqlPrefix = "INSERT INTO Recipe (name,recipe_type,preparation,vegetarian,prots,carbs,fats,cals) VALUES";
    QString sqlStatement = "";
    i=0;
    while(!in.atEnd())
    {
        line = in.readLine();
        if(line == "{"){
            sqlStatement+= '(';
            sqlStatement+= "\"" + in.readLine() +"\" ,"; //name
            sqlStatement+= "\"" + in.readLine() +"\" ,"; //recipe_type
            sqlStatement+= "\"" + in.readLine() +"\" ,"; //preparation
            sqlStatement+= in.readLine() +","; //is vegetarian?
            int prots = in.readLine().toInt();
            int carbs = in.readLine().toInt();
            int fats = in.readLine().toInt();
            int cals = prots*4+carbs*4+fats*9;
            sqlStatement+= QString::number(prots) + "," + QString::number(carbs) + ","
                    + QString::number(fats) + "," + QString::number(cals)+ ");";
            if(query.exec(sqlPrefix+sqlStatement)){
                qDebug()<<"sucessfully inserted recipe"<<i;
            }
            else{
                qDebug()<<"failed to insert recipe"<<i;
            }
            sqlStatement="";
            continue;
        }
        if(line == "}")
        {
            i++;
            continue;
        }
        int first_space_index = line.indexOf(" ");
        int equal_index = line.indexOf("=");
        QString values;
        //find out food id
        if(equal_index==-1){
            //nothing to do for this line, continue
            continue;
        }
        QString food_name = line.mid(equal_index+1);
        query.exec("SELECT id FROM Food WHERE name=\""+food_name+"\";");
        if(query.next()){ //get id
             values += query.value(0).toString() + ",";
        }else{ //insert and get id
            if(query.exec("INSERT INTO Food (name,measure_unit) VALUES (\""+food_name+"\",\"unidades\");")){
                qDebug()<<"inserting food:"<<food_name;
                query.exec("SELECT id FROM Food WHERE name=\""+food_name+"\";");
                query.next();
                values += query.value(0).toString() + ",";
            }else{
                qDebug()<<"failed to insert food:"<<food_name;
                qDebug()<<query.lastError().text();
            }
        }
        values += QString::number(i+1)+ ","; //recipe id
        values += "\""+ line.mid(first_space_index+1,equal_index-first_space_index-1) + "\","; //verbose_name
        values += line.mid(0,first_space_index); //amount
        if(query.exec("INSERT INTO Ingredient (food_id,recipe_id,verbose_name,amount) VALUES ("+ values +");"))
        {
            qDebug()<<"Adding ingredient"<<food_name<<"to recipe"<<i;
        }else{
            qDebug()<<"Failed to add ingredient to recipe"<<i;
            qDebug()<<query.lastError().text();
        }
    }
    f.close();

}

static bool createConnection()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dataDir);

    if(!dir.exists())
    {
        if(!dir.mkpath(dataDir))
        {
            qDebug()<<"Error: creating directory";
        }
    }

    QString dbName = dir.filePath("Aplication.sqlite");

    if(WIPE){
        QFile::remove(dbName);
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Error al iniciar la aplicacion"),
                              qApp->tr("No se han encontrado los datos de aplicacion"),
                              QMessageBox::Ok);
        return false;
    }


    QSettings settings;
    if(WIPE){
    settings.remove("dbInitialized"); //uncomment to refill database
    }
    if(!settings.contains("dbInitialized"))
    {
        createDatabase();
        populateDatabase();
        settings.setValue("dbInitialized",true);
    }

//    if(REFILLFOODFILE){
//        if(!query.exec("SELECT name FROM Food")){
//            qDebug()<<"failed to execute sql";
//        }
//        while(query.next()){
//            out << query.value(0).toString()+'\n';
//        }
//    }

    return true;
}

#endif // DBCONNECTION_H
