#include "graph.h"
#include "kruskal.h"

#include <fstream>
#include <iomanip>


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

    int num_nodes, num_edges = 0;
    int src, dest, weight;
    //get number of nodes
    fl >> num_nodes;

    //create graph and add edges
    Graph g(num_nodes);
    while(! fl.eof())
    {
        fl >> src >> dest >> weight;
        g.add_edge(src, dest, weight);
        num_edges++;
    }

    //close file
    fl.close();

    cout << "Number of vertices: " << num_nodes << endl;
    cout << "Number of edges: " << num_edges << endl;

    //get MST and print it
    MST_Kruskal mkr(g); 
    vector<tuple<node, node, int>> MST = mkr.get_MST();
    //print
    cout << "Edges in MST: " << num_nodes << endl;
    cout << setw(7) << "source" << setw(7) << "dest" << setw(7) << "weight" << endl;
    for(auto e = MST.begin(); e != MST.end(); e++)
    {
        src = get<0>(*e);
        dest = get<1>(*e);
        weight = get<2>(*e);
        cout << setw(7) << src << setw(7)  << dest << setw(7) << weight << endl; 
    }

    if(mkr.get_weight() != -1)
        cout << "Minimum Weight: " << mkr.get_weight() << endl;
    return 0;
}
