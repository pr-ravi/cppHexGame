#include "graph.h"

#include <tuple>
#include <queue>
#include <algorithm>
#include <iterator>


//class implementing Kruskal's Algorithm
class MST_Kruskal
{
    public:
    //get MST
    vector<tuple<node,node,int>> get_MST();
    //get the connected component to which the node belongs
    int get_component(int v); 
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
    vector<node> V ;
    //forest of vectors for trees
    vector<vector<node>> forest;
    //connected components
    vector<vector<node>*> component;
    //vector<int> component; 
    Graph& g;
    void erase_tree(vector<node>& t);
};

//get MST weight
int MST_Kruskal::get_weight()
{
    return weight;
}

//is the forest connected?
bool MST_Kruskal::is_connected()
{
    vector<node>* ptr = *component.begin();
    for(auto it = component.begin(); it != component.end(); it++)
        if(ptr != *it)
            return false;
    return true;
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
//return a vector of edges
vector<tuple<node,node,int> > MST_Kruskal::get_MST() 
{
    //priority queue for sorting edges
    priority_queue<tuple<node, node, int>, vector<tuple<node, node, int>>, LessTuple> pq;
    //edges in MST
    vector<tuple<node, node, int>> MST_edges;
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
        if(component[src] != component[dest])
        {
            MST_edges.push_back(e);
            min_weight += weight;

            //retrieve disjoint trees
            vector<node>& v1 = *component[src];
            vector<node>& v2 = *component[dest];

            //copy to new vector
            vector<node> v3;
            v3.reserve(v1.size() + v2.size());
            copy(v1.begin(), v1.end(), back_inserter(v3));
            copy(v2.begin(), v2.end(), back_inserter(v3));

            //insert new tree
            forest.push_back(v3);
            //update component values 
            for(auto it = v3.begin(); it != v3.end(); it++)
                component[*it] = &forest[forest.size() -1 ];
        }
    }
    this->weight = min_weight;
    return MST_edges;
}

// constructor
MST_Kruskal::MST_Kruskal(Graph &G): g(G) 
{
    this->weight = -1;
    //get list of nodes
    this->V = g.vertices();
    //for perf
    this->forest.reserve(V.size() * V.size());
    this->component.reserve(V.size());
    //create a forest of disjoint trees
    int i = 0;
    for(auto it = V.begin(); it != V.end(); i++, it++)
    {
        vector<node> s;
        s.push_back(*it);
        forest.push_back(s);

        //set pointer to the newly added tree
        vector<node>* p = &forest[forest.size() -1 ];
        component.push_back(p);
    }
};


