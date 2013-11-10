#include "graph.h"
#include "dijkstra.h"

#include<ctime>    //for time generation
#include<cstdlib>  //for random generation
//generate random number between [1, max]
int rnd(int max)
{
    return rand() % (max - 1);
}

void test() 
{
    Graph g;

    for(int i=1; i<6; i++)
        g.V.push_back(i);

    map<node, int> m;

    //node 1
    g.E.push_back(m);
    m.insert(pair<node, int>(2,2));
    m.insert(pair<node, int>(3,6));
    g.E.push_back(m);

    m.clear();
    m.insert(pair<node, int>(1,2));
    m.insert(pair<node, int>(4,1));
    g.E.push_back(m);

    m.clear();
    m.insert(pair<node, int>(1,6));
    m.insert(pair<node, int>(5,4));
    m.insert(pair<node, int>(4,1));
    g.E.push_back(m);

    m.clear();
    m.insert(pair<node, int>(2,1));
    m.insert(pair<node, int>(5,6));
    m.insert(pair<node, int>(3,1));
    g.E.push_back(m);
    
    m.clear();
    m.insert(pair<node, int>(3,4));
    m.insert(pair<node, int>(4,6));
    g.E.push_back(m);

    Dijkstra dj(g, 1, 5);

    cout << g << endl;
    vector<int> path ;
    path = dj.get_path();
    
    cout<< "Path: ";
    for(vector<int>::const_reverse_iterator it = path.rbegin(); it != path.rend(); it++)
        if(it != path.rend() -1) cout << " " << *it << " ->";
        else cout << " " << *it;

    if(path.size() != 0)
        cout << endl << "Shortest distance = " << dj.get_weight();
    cout<< endl;
    

}

//parameters : edge density, distance range and number of nodes
//return: graph generated
Graph generate_graph(float density, int range, int num_nodes)
{

    Graph g(num_nodes);
    //seed random generator
    srand(time(0));

    //calculate eges of complete graph
    int max_edges = (num_nodes * (num_nodes - 1) ) / 2;
    //calculate no of edges to isnert
    int num_edges = density * max_edges;

    //insert edges
    for(int i = 0; i < num_edges; )
        if(g.add_edge(rnd(max_edges), rnd(max_edges), rnd(range)))  //was edge insertion successfull?
            i++;

    return g;
}

//main procedure
int main()
{
    vector<float> average(2,0);

    //generate graph with 20% density and calculate average path 
    Graph g = generate_graph(0.2, 10, 50);
    Dijkstra dij_g(g); 
    for( int i = 0; i < 50 ; i++)
    {
        dij_g.get_path(1, i);
        if(dij_g.has_valid_path()) // if the path is valid
            average[0] += dij_g.get_weight(); 
    }
    average[0] /= 50;

    //generate graph with 40% density and calculate average path 
    Graph h = generate_graph(0.4, 10, 50);
    Dijkstra dij_h(h); 
    for( int i = 0; i < 50 ; i++)
    {
        dij_h.get_path(1, i);
        if(dij_h.has_valid_path())
            average[1] += dij_h.get_weight(); 
    }
    average[1] /= 50;

    cout << "Average weight with 20% density:" << average[0] << endl;
    cout << "Average weight with 40% density:" << average[1] << endl;
    cout << "Average weight: " << (average[0] + average[1]) / 2 << endl;
    return 0;
}
