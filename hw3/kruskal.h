#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "graph.h"
#include "union_find.h"

#include <tuple>
#include <queue>


//class implementing Kruskal's Algorithm
class MST_Kruskal
{
    public:
    //get MST
    Graph& get_MST();
    //constructor
    MST_Kruskal(Graph &G); 
    //get weight of MST
    int get_weight();
    
    private:
    //minimum weight
    int weight;
    //is the forest connected?
    bool is_connected();
    //vertices
    vector<node> V;
    //original graph reference
    Graph& g;
    //MST Tree
    Graph mst;
    //union find object
    UnionFind uf;
};

//get MST weight
int MST_Kruskal::get_weight()
{
    return weight;
}

//is the forest connected?
inline bool MST_Kruskal::is_connected()
{
    // edges =  num_vertices - 1
    return static_cast<size_t>(this->mst.edges()) == this->V.size() - 1;
}

//functor for sorting the PQ
class LessTuple
{
    public:
    bool operator() (tuple<node, node, int> t1, tuple<node, node, int> t2)
    {
        //compare 2nd element
        return get<2>(t2) < get<2>(t1);
    }
};



//calulate MST
//return the MST graph
Graph&  MST_Kruskal::get_MST() 
{
    //priority queue for sorting edges
    priority_queue<tuple<node, node, int>, vector<tuple<node, node, int>>, LessTuple> pq;
    //minimum weight
    int min_weight = 0;

    //push all the edges onto the priority queue 
    for(auto it = V.begin(); it != V.end(); it++)
    {
        map<node, int>& elist = g.getEdgeList(*it);
        //tuple of (src, dest, weight)
        for(auto e = elist.begin(); e != elist.end(); e++)
            pq.push(make_tuple(*it, (*e).first, (*e).second));
    }

    int src, dest, weight;

    //until all nodes are in MST, 
    while(!is_connected())
    {
        //get edge with least weight
        tuple<node, node, int> e = pq.top();
        pq.pop();
        src = get<0>(e);
        dest = get<1>(e);
        weight = get<2>(e);

        //are the components disjoint?
        if(! uf.is_connected(src, dest))
        {
            //add edge to MST graph
            mst.add_edge(src, dest, weight);
            //union the two components
            uf.union_node(src, dest);
            min_weight += weight;
        }
    }
    this->weight = min_weight;
    return mst;
}

// constructor
MST_Kruskal::MST_Kruskal(Graph &G): g(G), uf(UnionFind(G.vertex_count()))
{
    mst = Graph(G.vertex_count());
    this->weight = -1;
    this->V = g.vertices();
};

#endif
