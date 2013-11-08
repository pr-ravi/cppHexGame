#include "graph.h"
#include <tuple>
#include <set>
#include <queue>
#include <algorithm>
#include <iterator>


class MST_Kruskal
{
    public:
    //get MST
    vector<tuple<node,node,int>> get_MST();
    //get the connected component to which the node belongs
    int get_component(int v); 
    //constructor
    MST_Kruskal(Graph &G); 
    
    private:
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

/*
//QuickFind Algorithm
//
int MST_Kruskal::get_component(int v)
{
    while(connected[v] != v)
        v = connected[v];
    return v;
}
*/

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



vector<tuple<node,node,int> > MST_Kruskal::get_MST() 
{
    //priority queue for sorting edges
    priority_queue<tuple<node, node, int>, vector<tuple<node, node, int>>, LessTuple> pq;
    //nodes in MST
    set<node> MST_nodes;
    //edges in MST
    vector<tuple<node, node, int>> MST_edges;
    //minimum weight
    int min_weight = 0;
    /*
    cout << "Initial component list: "<< endl;
    int i =0;
    for(auto it = component.begin();  it != component.end(); it++, i++)
        cout << i << "= " << *it << ", "; 
    cout <<endl;
    */
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
            //cout << "[src=" << src << " dest=" << dest << " weight=" << weight << " ]" << endl;
            MST_edges.push_back(e);
            min_weight += weight;
            //insert into MST_nodes set
            MST_nodes.insert(src);
            MST_nodes.insert(dest);
            //retrieve disjoint trees
            vector<node>& v1 = *component[src];
            vector<node>& v2 = *component[dest];
            //copy to new vector
            vector<node> v3;
            //cout << "v1 :{ ";
            for(auto it = v1.begin(); it != v1.end(); it++)
            {
                v3.push_back(*it);
                //cout << * it << " " ;
            }
            //cout << " }" << endl << "v2 :{ ";
            for(auto it = v2.begin(); it != v2.end(); it++)
            {
                v3.push_back(*it);
                //cout << * it << " " ;
            }
            //cout << " }" << endl;
            //copy(s1.begin(), s1.end(), inserter(s3, s3.begin()));
            //copy(s2.begin(), s2.end(), inserter(s3, s3.begin()));
            //delete disjoint trees
            //erase_tree(v2);
            //erase_tree(v1);
            //insert new tree
            forest.push_back(v3);
            //update component values 
            for(auto it = v3.begin(); it != v3.end(); it++)
            {
                component[*it] = &forest[forest.size() -1 ];
                //cout << "*it = " << *it << " component[*it] = " << component[*it] <<endl;
            }
            /*int i =0;
            for(auto it = component.begin();  it != component.end(); it++, i++)
                cout << i << "= " << *it << ", "; 
            cout <<endl;
            */
            //cout << "MST_nodes: " << MST_nodes.size() << " Forest: " << forest.size() <<endl;
        }
    }
    cout<< "Minimum weight: " << min_weight << endl;
    return MST_edges;
}

void MST_Kruskal::erase_tree(vector<node>& t)
{
    for(auto it = forest.begin(); it != forest.end(); it++)
        if(t == *it)
        {
            forest.erase(it);
            break;
        }
}

MST_Kruskal::MST_Kruskal(Graph &G): g(G) 
{
    //get list of nodes
    V = g.vertices();
    //for perf
    forest.reserve(V.size() * V.size());
    component.reserve(V.size());
    //create a forest of disjoint trees
    int i = 0;
    //set<node> s;
    for(auto it = V.begin(); it != V.end(); i++, it++)
    {
        //component.push_back(i);
        //component[i] = i;
        vector<node> s(0);
        s.push_back(*it);
        for(auto iter = s.begin(); iter != s.end(); iter++)
            ;
        //    cout << *iter << " ";
        //cout << endl;
        forest.push_back(s);
        vector<node>* p = &forest[forest.size() -1 ];
        //cout <<  p << endl;;
        component.push_back(p);
    }
};


