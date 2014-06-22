#ifndef GRAFO_H
#define GRAFO_H

#include <QDebug>
#include <limits>
#include <tpl_graph_utils.H>
#include <tpl_graph.H>
#include <tpl_spanning_tree.H>
#include <Dijkstra.H>
#include <QSqlQuery>
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


//para mostrar varios datos en cada nodo en la foto
struct Draw_Node
{
    void operator()(Digrafo & g, Digrafo::Node * node, ofstream & output)
    {
        output << "[label=\"" << node->get_info().nombre << "\\" << 'n' << convertInt(node->get_info().gusto) << "\"]";
    }
};

//para mostrar los arcos en la foto
struct Draw_Arc
{
  void operator()(Digrafo & g, Digrafo::Arc * arc, ofstream & output)
  {
    output << "[label=\"" << arc->get_info().distancia << "\"]";
  }
};

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

//hace la conexion de los nodos especiales Fuente y Sumidero
void insertar_arco(Digrafo & digrafo, const Nodo & src, const Nodo & tgt, const double & distancia)
{
    Digrafo::Node * n1 = digrafo.search_node(src);

    if (n1 == NULL)
        n1 = digrafo.insert_node(src);

    Digrafo::Node * n2 = digrafo.search_node(tgt);

    if (n2 == NULL)
        n2 = digrafo.insert_node(tgt);

    /*if(src.nombre=="Fuente")
        digrafo.insert_arc(n1, n2, distancia);
    if(tgt.nombre=="Sumidero")
        digrafo.insert_arc(n1, n2, distancia);*/
}

//crea el grafo leyendo los datos de un archivo
void insertarNodos(Digrafo & g)
{
    int gust, id;
    string nom;
    Nodo node;
    ifstream entrada1("proteinas.txt");
    ifstream entrada2("carbohidratos.txt");
    ifstream entrada3("ensaladas.txt");

    //Modo Especial Fuente
    node.nombre= "Fuente";
    node.gusto= 100;
    node.tipo= "Desconocido";
    node.id= id= -1;
    typename Digrafo::Node * n1 = g.search_node(node);
        if (n1 == NULL)
            n1 = g.insert_node(node);

    //Modo Especial Sumidero
    node.nombre= "Sumidero";
    node.gusto= 100;
    node.tipo= "Desconocido";
    node.id= id= -1;
    typename Digrafo::Node * n2 = g.search_node(node);
        if (n2 == NULL)
            n2 = g.insert_node(node);

    while(entrada1>>nom>>gust>>id)
    {
        node.nombre= nom;
        node.gusto= gust;
        node.tipo= "Proteinas";
        node.id= id;
        Digrafo::Node * n = g.search_node(node);
        if (n == NULL)
        {
            n = g.insert_node(node);
        }
    }
    while(entrada2>>nom>>gust>>id)
    {
        node.nombre= nom;
        node.gusto= gust;
        node.tipo= "Carbohidratos";
        node.id= id;
        Digrafo::Node * n = g.search_node(node);
        if (n == NULL)
        {
            n = g.insert_node(node);
        }
    }
    while(entrada3>>nom>>gust>>id)
    {
        node.nombre= nom;
        node.gusto= gust;
        node.tipo= "Ensaladas";
        node.id= id;
        Digrafo::Node * n = g.search_node(node);
        if (n == NULL)
        {
            n = g.insert_node(node);
        }
    }
    entrada1.close();
    entrada2.close();
    entrada3.close();
}

typedef Path<Digrafo> Dicamino;

//funcion especial para imprimir Path<Digrafo> en foto
void crear_dot(Dicamino &path, std::ofstream & output)
{
    output << "digraph" << " {" << endl;
    output << "rankdir = \"LR\"" << endl<<endl;
    size_t i=0;
    for (Dicamino::Iterator itor(path); itor.has_current(); itor.next())
    {
        output << i << " [ ";
        output << "label = \"" << itor.get_current_node()->get_info().nombre << "\\" << 'n' << convertInt(itor.get_current_node()->get_info().gusto) << "\"";
        output << " ]" << endl;
        i++;
    }
    output << endl;
    i=0;
    for (Dicamino::Iterator itor2(path); itor2.has_current(); itor2.next())
    {
        //cout << endl << itor2.get_current_arc()->get_info().distancia <<endl;
        if(i < path.size()-1 )
            output << i << " -> " << i+1 << " [label = \"" << itor2.get_current_arc()->get_info().distancia << "\" ]" << endl;
        i++;
    }
    output << "}" << endl;
    output.close();
}

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
    int id1, id2;

    while(entrada>>id1>>id2)
    {
        for(Node_Iterator<Digrafo> it(digrafo); it.has_current(); it.next())
        {
            for(Node_Iterator<Digrafo> it2(digrafo); it2.has_current(); it2.next())
            {
                if( it.get_current()->get_info().id == id1 && it2.get_current()->get_info().id == id2 )
                {
                    if(noRepetirArcos(digrafo, it.get_current()->get_info(), it2.get_current()->get_info()))
                        digrafo.insert_arc(it.get_current(), it2.get_current(), 11);
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
            }
        }
    }
    entrada.close();
}

//funcion para cuando el usuario inserta recetas
void conectarRecetas(Digrafo & digrafo)
{
    int c= 10;
    for (Digrafo::Node_Iterator itor(digrafo); itor.has_current(); itor.next())
    {
        for (Digrafo::Node_Iterator itor2(digrafo); itor2.has_current(); itor2.next())
        {
            if( (itor.get_current_node()->get_info().tipo == "Proteinas") && itor2.get_current_node()->get_info().tipo == "Carbohidratos")
            {
                if( noRepetirArcos(digrafo, itor.get_current()->get_info(), itor2.get_current()->get_info()) )
                    digrafo.insert_arc(itor.get_current(), itor2.get_current(), c);
            }
            if( (itor.get_current_node()->get_info().tipo == "Carbohidratos") && itor2.get_current_node()->get_info().tipo == "Ensaladas")
            {
                if( noRepetirArcos(digrafo, itor.get_current()->get_info(), itor2.get_current()->get_info()) )
                    digrafo.insert_arc(itor.get_current(), itor2.get_current(), c);
            }
            //para conectar los nodos especiales con sus respectivos niveles
            if( (itor.get_current()->get_info().nombre == "Fuente") && (itor2.get_current()->get_info().tipo == "Proteinas") )
            {
                if(noRepetirArcos(digrafo, itor.get_current()->get_info(), itor2.get_current()->get_info()))
                    digrafo.insert_arc(itor.get_current(), itor2.get_current(), 0);
            }
            if( (itor.get_current()->get_info().nombre == "Sumidero") && (itor2.get_current()->get_info().tipo == "Ensaladas") )
            {
                if(noRepetirArcos(digrafo, itor2.get_current()->get_info(), itor.get_current()->get_info()))
                    digrafo.insert_arc(itor2.get_current(), itor.get_current(), 0);
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

//guardar grafo (no guarda el grafo)
void guardarGrafo(Digrafo &g)
{
    ofstream salida1("proteinas.txt", ios::trunc);
    ofstream salida2("carbohidratos.txt", ios::trunc);
    ofstream salida3("ensaladas.txt", ios::trunc);
    ofstream salida4("arcos.txt", ios::trunc);

    //guardo los nodos en sus respectivos archivos
    for(Node_Iterator<Digrafo> it(g); it.has_current(); it.next())
    {
        if( it.get_current()->get_info().tipo == "Proteinas" )
            salida1 << it.get_current()->get_info().nombre << " " << it.get_current()->get_info().gusto << " " << it.get_current()->get_info().id << endl;
        if( it.get_current()->get_info().tipo == "Carbohidratos" )
        salida2 << it.get_current()->get_info().nombre << " " << it.get_current()->get_info().gusto << " " << it.get_current()->get_info().id << endl;
        if( it.get_current()->get_info().tipo == "Ensaladas" )
        salida3 << it.get_current()->get_info().nombre << " " << it.get_current()->get_info().gusto << " " << it.get_current()->get_info().id << endl;
    }

    //guardo los pares ordenados para los arcos
    for( Digrafo::Arc_Iterator arc_itor(g); arc_itor.has_current(); arc_itor.next() )
    {
        Digrafo::Arc * arc = arc_itor.get_current_arc();
        if( arc->get_info().distancia != 0 )
            salida4 << g.get_src_node(arc)->get_info().id << " " << g.get_tgt_node(arc)->get_info().id << endl;
    }
    salida1.close();
    salida2.close();
    salida3.close();
    salida4.close();
}

typedef Path<Digrafo> Camino;

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
                if(itor.get_current_node()->get_info().nombre == it.get_current_node()->get_info().nombre)
                {
                    if(it.get_current_node()->get_info().gusto>=0 && it.get_current_node()->get_info().gusto<=10 )
                        it.get_current_node()->get_info().gusto+=1;
                    else
                        it.get_current_node()->get_info().gusto-=1;
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
                if(itor.get_current_node()->get_info().nombre == it.get_current_node()->get_info().nombre)
                {
                    if(it.get_current_node()->get_info().gusto>=0 && it.get_current_node()->get_info().gusto<=10 )
                        it.get_current_node()->get_info().gusto-=1;
                    else
                        it.get_current_node()->get_info().gusto+=1;
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
        if( (src->get_info().nombre == it1.get_current_node()->get_info().nombre) && (tgt->get_info().nombre == it2.get_current_node()->get_info().nombre) )
        {
            if(arc->get_info()>=0 && arc->get_info()<=10 )
            {
                arc->get_info()+= 1;
            }
            else
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
        if( (src->get_info().nombre == it1.get_current_node()->get_info().nombre) && (tgt->get_info().nombre == it2.get_current_node()->get_info().nombre) )
        {
            if(arc->get_info()>=0 && arc->get_info()<=10 )
                arc->get_info()+= 1;
            else
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
        if( (src->get_info().nombre == it1.get_current_node()->get_info().nombre) && (tgt->get_info().nombre == it2.get_current_node()->get_info().nombre) )
        {
            if(arc->get_info()>=0 && arc->get_info()<=10 )
                arc->get_info()-= 1;
            else
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
        if( (src->get_info().nombre == it1.get_current_node()->get_info().nombre) && (tgt->get_info().nombre == it2.get_current_node()->get_info().nombre) )
        {
            if(arc->get_info()>=0 && arc->get_info()<=10 )
                arc->get_info()-= 1;
            else
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
    receta.id= -1;
    Digrafo::Node * n2 = g.search_node(receta);
        if (n2 == NULL)
            n2 = g.insert_node(receta);

    QSqlQuery query;
    query.exec("SELECT id,name,recipe_type,score FROM Recipe");
    while(query.next())
    {
        if( ("Aves y caza" == query.value(2).toString()) || ("Carnes" == query.value(2).toString()) || ("Pescados y mariscos" == query.value(2).toString()) )
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
        conectarRecetas(g);
    }
}

Digrafo dg;

#endif // GRAFO_H
