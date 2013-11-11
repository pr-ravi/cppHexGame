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

    //create graph 
    Graph g(argv[1]);
    cout << "Number of vertices: " << g.vertex_count() << endl;
    cout << "Number of edges: " << g.edges() << endl;

    //get MST and print it
    MST_Kruskal mkr(g); 
    Graph  mst = mkr.get_MST();

    //print
    cout << setw(7) << "source" << setw(7) << "dest" << setw(7) << "weight" << endl;
    //for pretty printing
    auto MST_vertices = mst.vertices();
    int num_edges_MST = 0;
    int src, dest, weight;
    for(auto vx = MST_vertices.begin(); vx != MST_vertices.end(); vx++)
    {
        src = *vx;
        auto edges = mst.getEdgeList(src); 
        for(auto e = edges.begin(); e != edges.end(); e++, num_edges_MST++)
        {
            dest = (*e).first;
            weight = (*e).second;
            cout << setw(7) << src << setw(7)  << dest << setw(7) << weight << endl; 
        }
    }
    cout << "Edges in MST: " << num_edges_MST << endl;

    //print weight
    if(mkr.get_weight() != -1)
        cout << "Minimum Weight: " << mkr.get_weight() << endl;
    return 0;
}
