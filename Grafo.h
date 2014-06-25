#ifndef GRAFO_H
#define GRAFO_H

#include <QDebug>
#include <limits>
#include <tpl_graph_utils.H>
#include <tpl_graph.H>
#include <tpl_spanning_tree.H>
#include <Dijkstra.H>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QApplication>
#include <QSettings>
using namespace std;

struct Nodo
{
    string nombre;
    int gusto;
    string tipo;
    int id;

    bool operator == (const Nodo & der) const
    {
        return nombre == der.nombre;
    }
};

struct Arco
{
    double distancia;

    Arco() : distancia(numeric_limits<double>::infinity())
            {
                // empty
            }

    Arco(const double & dist) : distancia(dist)
                                {
                                    // empty
                                }

    double & get_distance()
    {
        return distancia;
    }

    operator double & ()
    {
        return distancia;
    }
};

const Arco Arco_Zero(0);

typedef Graph_Node<Nodo> Node_Nodo;

typedef Graph_Arc<Arco> Arco_Arco;

typedef List_Digraph<Node_Nodo, Arco_Arco> Digrafo;

//convierte un entero a un tipo string para usarlo en Arco_String
string convertInt(int number)
{
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

//lleva las cuentas de las distancias para el calculo del camino minimo
struct Distancia
{
    typedef double Distance_Type;

    static const double Max_Distance;

    static const double Zero_Distance;

    Distance_Type & operator () (Digrafo::Arc * a) const
    {
        return a->get_info().distancia;
    }
};

const double Distancia::Max_Distance = numeric_limits<double>::infinity();

const double Distancia::Zero_Distance = 0;

typedef Path<Digrafo> Camino;

//para imprimir el camino por consola
void imprimir_camino(Camino & path)
{
    cout << endl << "Camino: ";
    for(Camino::Iterator itor(path); itor.has_current(); itor.next())
    {
        cout << itor.get_current_node()->get_info().nombre << " " << itor.get_current_node()->get_info().gusto << " " << itor.get_current_node()->get_info().tipo << " --> ";
    }
    cout << endl;
}

//funcion para no tener arcos repetidos en el digrafo
bool noRepetirArcos(Digrafo & digrafo, const Nodo src, const Nodo tgt)
{
    bool band= true;
    for(Digrafo::Arc_Iterator itor(digrafo); itor.has_current(); itor.next())
    {
        Digrafo::Arc * arc = itor.get_current_arc();
        if( (digrafo.get_src_node(arc)->get_info().nombre==src.nombre) && (digrafo.get_tgt_node(arc)->get_info().nombre==tgt.nombre) )
            band= false;
    }
    return band;
}

//funcion que hace la conexion de todos contra todos desde el archivo arcos.txt
void conectarRecetasArchivo(Digrafo & digrafo)
{
    ifstream entrada("arcos.txt");
    int id1, id2, arc;

    while(entrada>>id1>>id2>>arc)
    {
        for(Node_Iterator<Digrafo> it(digrafo); it.has_current(); it.next())
        {
            for(Node_Iterator<Digrafo> it2(digrafo); it2.has_current(); it2.next())
            {
                if( it.get_current()->get_info().id == id1 && it2.get_current()->get_info().id == id2 )
                {
                    if(noRepetirArcos(digrafo, it.get_current()->get_info(), it2.get_current()->get_info()))
                        digrafo.insert_arc(it.get_current(), it2.get_current(), arc);
                }
                if( (it.get_current()->get_info().nombre == "Fuente") && (it2.get_current()->get_info().tipo == "Proteinas") )
                {
                    if(noRepetirArcos(digrafo, it.get_current()->get_info(), it2.get_current()->get_info()))
                        digrafo.insert_arc(it.get_current(), it2.get_current(), 0);
                }
                if( (it.get_current()->get_info().nombre == "Sumidero") && (it2.get_current()->get_info().tipo == "Ensaladas") )
                {
                    if(noRepetirArcos(digrafo, it2.get_current()->get_info(), it.get_current()->get_info()))
                        digrafo.insert_arc(it2.get_current(), it.get_current(), 0);
                }
                //qDebug() << QString::fromStdString(it.get_current()->get_info().nombre) << QString::fromStdString(it2.get_current()->get_info().nombre) << arc;
            }
        }
    }
    entrada.close();
}

//funcion para cuando el usuario inserta recetas
//solo se llama cuando se modifica la lista de recetas en la base de datos
void conectarRecetas(Digrafo & digrafo)
{
    QFile archivo("arcos.txt");
    if(!archivo.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing";
        return;
    }
    QTextStream out(&archivo);
    int c= 5;
    for (Digrafo::Node_Iterator itor(digrafo); itor.has_current(); itor.next())
    {
        for (Digrafo::Node_Iterator itor2(digrafo); itor2.has_current(); itor2.next())
        {
            if( (itor.get_current_node()->get_info().tipo == "Proteinas") && itor2.get_current_node()->get_info().tipo == "Carbohidratos")
            {
                if( noRepetirArcos(digrafo, itor.get_current()->get_info(), itor2.get_current()->get_info()) )
                {
                    digrafo.insert_arc(itor.get_current(), itor2.get_current(), c);
                    out << itor.get_current_node()->get_info().id << " " << itor2.get_current_node()->get_info().id << " " << c << endl;
                }
            }
            if( (itor.get_current_node()->get_info().tipo == "Carbohidratos") && itor2.get_current_node()->get_info().tipo == "Ensaladas")
            {
                if( noRepetirArcos(digrafo, itor.get_current()->get_info(), itor2.get_current()->get_info()) )
                {
                    digrafo.insert_arc(itor.get_current(), itor2.get_current(), c);
                    out << itor.get_current_node()->get_info().id << " " << itor2.get_current_node()->get_info().id << " " << c << endl;
                }
            }
            //para conectar los nodos especiales con sus respectivos niveles
            if( (itor.get_current()->get_info().nombre == "Fuente") && (itor2.get_current()->get_info().tipo == "Proteinas") )
            {
                if(noRepetirArcos(digrafo, itor.get_current()->get_info(), itor2.get_current()->get_info()))
                {
                    digrafo.insert_arc(itor.get_current(), itor2.get_current(), 0);
                    out << itor.get_current_node()->get_info().id << " " << itor2.get_current_node()->get_info().id << " " << c << endl;
                }
            }
            if( (itor.get_current()->get_info().nombre == "Sumidero") && (itor2.get_current()->get_info().tipo == "Ensaladas") )
            {
                if(noRepetirArcos(digrafo, itor2.get_current()->get_info(), itor.get_current()->get_info()))
                {
                    digrafo.insert_arc(itor2.get_current(), itor.get_current(), 0);
                    out << itor2.get_current_node()->get_info().id << " " << itor.get_current_node()->get_info().id << " " << c << endl;
                }
            }
        }
    }
}

typedef Path<Digrafo> Camino2;

//funcion para quitar el efecto de cambio en los arcos cuando se le suman los pesos de los nodos
void normalizarCamino(Camino2 & cam)
{
    int i=0, p1, p2;
    for(Camino2::Iterator itor(cam); itor.has_current(); itor.next())
    {
        if( itor.get_current()->get_info().tipo=="Proteinas" )
            p1= itor.get_current()->get_info().gusto;
        if( itor.get_current()->get_info().tipo=="Carbohidratos" )
            p1+= itor.get_current()->get_info().gusto;
        if( itor.get_current()->get_info().tipo=="Ensaladas" )
            p2= itor.get_current()->get_info().gusto;
    }
    for(Camino2::Iterator itor(cam); itor.has_current(); itor.next())
    {
        if(i < cam.size()-1 )
        {
            switch(i)
            {
                case 1:
                {
                    itor.get_current_arc()->get_info()-= p1;
                    break;
                }
                case 2:
                {
                    itor.get_current_arc()->get_info()-= p2;
                    break;
                }
            }
            //cout << itor.get_current_arc()->get_info().distancia << endl;
        }
        i++;
    }
}

//guarda el id1 id2 y el arco que los conecta
void guardarGrafo(Digrafo &g)
{
    QString dataDir= QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dataDir);

    if(!dir.exists())
    {
        if(!dir.mkpath(dataDir))
        {
            qDebug() << "Error creating directory";
        }
    }
    QString filename= dir.filePath("arcos.txt");
    QFile archivo("arcos.txt");
    if(!archivo.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing";
        return;
    }
    QTextStream out(&archivo);

    //guardo los pares ordenados para los arcos
    for( Digrafo::Arc_Iterator arc_itor(g); arc_itor.has_current(); arc_itor.next() )
    {
        Digrafo::Arc * arc = arc_itor.get_current_arc();
        out << g.get_src_node(arc)->get_info().id << " " << g.get_tgt_node(arc)->get_info().id << " " << arc->get_info().distancia << endl;
    }
    archivo.close();
}

void sugerir(Digrafo &dg, Camino &path)
{
    //aqui si calcula el camino minimo tomando en cuenta los pesos de los nodos y los arcos
    Digrafo temp;
    for(Arc_Iterator<Digrafo> itor(dg); itor.has_current(); itor.next())
    {
        Digrafo::Arc * arc= itor.get_current();

        Digrafo::Node * n1 = temp.search_node(dg.get_src_node(arc)->get_info());
        if (n1 == NULL)
            n1 = temp.insert_node(dg.get_src_node(arc)->get_info());

        Digrafo::Node * n2 = temp.search_node(dg.get_tgt_node(arc)->get_info());
        if (n2 == NULL)
            n2 = temp.insert_node(dg.get_tgt_node(arc)->get_info());

        temp.insert_arc(n1, n2, arc->get_info().distancia);
    }

    int p;
    for (Arc_Iterator<Digrafo> it(temp); it.has_current(); it.next())
    {
        Digrafo::Arc * a = it.get_current();
        Digrafo::Node * src = temp.get_src_node(a);
        Digrafo::Node * tgt = temp.get_tgt_node(a);
        if( (src->get_info().tipo=="Proteinas") && (tgt->get_info().tipo=="Carbohidratos") )
        {
            p= a->get_info()+src->get_info().gusto+tgt->get_info().gusto;
            a->get_info()= p;
        }
        if( (src->get_info().tipo=="Carbohidratos") && (tgt->get_info().tipo=="Ensaladas") )
        {
            p= a->get_info()+tgt->get_info().gusto;
            a->get_info()= p;
        }
    }

    Path<Digrafo> path3(temp);
    Nodo src, tgt;
    src.nombre= "Fuente";
    tgt.nombre= "Sumidero";
    Dijkstra_Min_Paths<Digrafo, Distancia>()(temp, temp.search_node(src), temp.search_node(tgt), path3);
    normalizarCamino(path3);
    path= path3;
}

//modifica el gusto en la base de datos
void aprendizajeReceta(Nodo receta)
{
    QString comando;
    QSqlQuery query;
    //query.exec("UPDATE Recipe SET score=10 WHERE id==1");
    comando= "UPDATE Recipe SET score=";
    comando+=QString::fromStdString(convertInt(receta.gusto))+" WHERE id="+QString::fromStdString(convertInt(receta.id));
    query.exec(comando);
    //qDebug() << comando;
}

//penaliza los gustos de cada receta: +1
//np especifica si es la receta1, receta2 o receta3
void penalizarReceta(Digrafo &dg, int np)
{
    //typedef Path<Digrafo> Camino;
    Camino path1;
    sugerir(dg, path1);
    int i=0;
    for(Camino::Iterator itor(path1); itor.has_current(); itor.next())
    {
        for(Node_Iterator<Digrafo> it(dg); it.has_current(); it.next())
        {
            if(i==np)
            {
                if(itor.get_current_node()->get_info().id == it.get_current_node()->get_info().id)
                {
                    it.get_current_node()->get_info().gusto+=1;
                    if(it.get_current_node()->get_info().gusto<=10)
                        aprendizajeReceta(it.get_current_node()->get_info());
                    else
                    {
                        it.get_current_node()->get_info().gusto=10;
                        aprendizajeReceta(it.get_current_node()->get_info());
                    }
                }
            }
         }
        i++;
    }
}

//premia los gustos de cada receta: -1
//np especifica si es la receta1, receta2 o receta3
void premiarReceta(Digrafo &dg, int np)
{
    //typedef Path<Digrafo> Camino;
    Camino path1;
    sugerir(dg, path1);
    int i=0;
    for(Camino::Iterator itor(path1); itor.has_current(); itor.next())
    {
        for(Node_Iterator<Digrafo> it(dg); it.has_current(); it.next())
        {
            if(i==np)
            {
                if(itor.get_current_node()->get_info().id == it.get_current_node()->get_info().id)
                {
                    it.get_current_node()->get_info().gusto-=1;
                    if(it.get_current_node()->get_info().gusto>=0)
                    {
                        aprendizajeReceta(it.get_current_node()->get_info());
                    }
                    else
                    {
                        it.get_current_node()->get_info().gusto=0;
                        aprendizajeReceta(it.get_current_node()->get_info());
                    }
                }
            }
         }
        i++;
    }
}

//penaliza la combinacion entre la receta1 y la receta2: +1
//para saber la relacion se comparan los tipos: proteinas, carbohidratos y Ensaladas (arreglando)
void penalizarCombinacionRecetas1(Digrafo &dg)
{
    Camino path1;
    sugerir(dg, path1);
    Camino::Iterator it1(path1);
    Camino::Iterator it2(path1);
    it1.next();
    it2.next();
    it2.next();
    for (Arc_Iterator<Digrafo> it(dg); it.has_current(); it.next())
    {
        Digrafo::Arc * arc = it.get_current();
        Digrafo::Node * src = dg.get_src_node(arc);
        Digrafo::Node * tgt = dg.get_tgt_node(arc);
        if( (src->get_info().id == it1.get_current_node()->get_info().id) && (tgt->get_info().id == it2.get_current_node()->get_info().id) )
        {
            arc->get_info()+= 1;
            if(arc->get_info()==11)
                arc->get_info()-=1;
        }
    }
}

//penaliza la combinacion entre la receta2 y la receta3: +1
//para saber la relacion se comparan los tipos: proteinas, carbohidratos y Ensaladas
void penalizarCombinacionRecetas2(Digrafo &dg)
{
    Camino path1;
    sugerir(dg, path1);
    Camino::Iterator it1(path1);
    Camino::Iterator it2(path1);
    it1.next();
    it1.next();
    it2.next();
    it2.next();
    it2.next();
    for (Arc_Iterator<Digrafo> it(dg); it.has_current(); it.next())
    {
        Digrafo::Arc * arc = it.get_current();
        Digrafo::Node * src = dg.get_src_node(arc);
        Digrafo::Node * tgt = dg.get_tgt_node(arc);
        if( (src->get_info().id == it1.get_current_node()->get_info().id) && (tgt->get_info().id == it2.get_current_node()->get_info().id) )
        {
            arc->get_info()+= 1;
            if(arc->get_info()==11)
                arc->get_info()-=1;
        }
    }
}

//premia la combinacion entre la receta2 y la receta3: -1
//para saber la relacion se comparan los tipos: proteinas, carbohidratos y Ensaladas
void premiarCombinacionRecetas1(Digrafo &dg)
{
    Camino path1;
    sugerir(dg, path1);
    Camino::Iterator it1(path1);
    Camino::Iterator it2(path1);
    it1.next();
    it2.next();
    it2.next();
    for (Arc_Iterator<Digrafo> it(dg); it.has_current(); it.next())
    {
        Digrafo::Arc * arc = it.get_current();
        Digrafo::Node * src = dg.get_src_node(arc);
        Digrafo::Node * tgt = dg.get_tgt_node(arc);
        if( (src->get_info().id == it1.get_current_node()->get_info().id) && (tgt->get_info().id == it2.get_current_node()->get_info().id) )
        {
            arc->get_info()-= 1;
            if(arc->get_info()==-1)
                arc->get_info()+=1;
        }
    }
}

//premia la combinacion entre la receta2 y la receta3: -1
//para saber la relacion se comparan los tipos: proteinas, carbohidratos y Ensaladas
void premiarCombinacionRecetas2(Digrafo &dg)
{
    Camino path1;
    sugerir(dg, path1);
    Camino::Iterator it1(path1);
    Camino::Iterator it2(path1);
    it1.next();
    it1.next();
    it2.next();
    it2.next();
    it2.next();
    for (Arc_Iterator<Digrafo> it(dg); it.has_current(); it.next())
    {
        Digrafo::Arc * arc = it.get_current();
        Digrafo::Node * src = dg.get_src_node(arc);
        Digrafo::Node * tgt = dg.get_tgt_node(arc);
        if( (src->get_info().id == it1.get_current_node()->get_info().id) && (tgt->get_info().id == it2.get_current_node()->get_info().id) )
        {
            arc->get_info()-= 1;
            if(arc->get_info()==-1)
                arc->get_info()+=1;
        }
    }
}

//crea el grafo leyendo los datos de un archivo
void insertarNodos1(Digrafo & g)
{
    Nodo receta;
    //Modo Especial Fuente
    receta.nombre= "Fuente";
    receta.gusto= 0;
    receta.tipo= "Desconocido";
    receta.id= -1;

    Digrafo::Node * n1 = g.search_node(receta);
        if (n1 == NULL)
            n1 = g.insert_node(receta);

    //Modo Especial Sumidero
    receta.nombre= "Sumidero";
    receta.gusto= 0;
    receta.tipo= "Desconocido";
    receta.id= -2;
    Digrafo::Node * n2 = g.search_node(receta);
        if (n2 == NULL)
            n2 = g.insert_node(receta);

    QSqlQuery query;
    query.exec("SELECT id,name,recipe_type,score FROM Recipe");
    while(query.next())
    {
        if( ("Aves y caza" == query.value(2).toString()) || ("Carnes y guisos" == query.value(2).toString()) || ("Pescados y mariscos" == query.value(2).toString()) )
        {
            receta.id= query.value(0).toInt();
            receta.nombre= query.value(1).toString().toUtf8().constData();
            receta.gusto= query.value(3).toInt();
            receta.tipo= "Proteinas";
            Digrafo::Node * n = g.search_node(receta);
            if (n == NULL)
                n = g.insert_node(receta);
        }

        if( ("Pastas y arroces" == query.value(2).toString()) || ("Sopas y cremas" == query.value(2).toString()) )
        {
            receta.id= query.value(0).toInt();
            receta.nombre= query.value(1).toString().toUtf8().constData();
            receta.gusto= query.value(3).toInt();
            receta.tipo= "Carbohidratos";
            Digrafo::Node * n = g.search_node(receta);
            if (n == NULL)
                n = g.insert_node(receta);
        }

        if( ("Ensaladas y verduras" == query.value(2).toString()) )
        {
            receta.id= query.value(0).toInt();
            receta.nombre= query.value(1).toString().toUtf8().constData();
            receta.gusto= query.value(3).toInt();
            receta.tipo= "Ensaladas";
            Digrafo::Node * n = g.search_node(receta);
            if (n == NULL)
                n = g.insert_node(receta);
        }
        //qDebug() << query.value(0).toInt() << query.value(1).toString() << query.value(3).toString();
    }
    QSettings settings;
    settings.remove("arcosInicializados");
    if(!settings.contains("arcosInicializados"))
    {
        conectarRecetas(g);
        settings.setValue("arcosInicializados",true);
    }else
    {
        conectarRecetasArchivo(g);
    }
}

Digrafo dg;

#endif // GRAFO_H
