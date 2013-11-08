#include "graph.h"
#include "kruskal.h"

#include <fstream>


int main(int argc, char* argv[])
{
    //should provide input graph file
    if (argc != 2)
    {
        cout << "Please supply a input graph file " << endl;
        return -1;
    }


    //read input file
    ifstream fl(argv[1], ios::in);
    //could not open file
    if(! fl.is_open())
        cout << "Error opening file " << endl;
    int num_nodes;
    int src, dest, weight;
    //get number of nodes
    fl >> num_nodes;
    Graph g(num_nodes);
    while(! fl.eof())
    {
        fl >> src >> dest >> weight;
        g.add_edge(src, dest, weight);
    }

    //cout << g << endl;
    MST_Kruskal mkr(g); 

    vector<tuple<node, node, int>> MST = mkr.get_MST();
    for(auto e = MST.begin(); e != MST.end(); e++)
    {
        src = get<0>(*e);
        dest = get<1>(*e);
        weight = get<2>(*e);
        cout << src << " " << dest << " " << weight << endl; 
    }
    //close file
    fl.close();




    return 0;
}
