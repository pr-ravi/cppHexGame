#include "graph.h"


node Graph::vertex_count() { return V.size(); }

vector<int> Graph::vertices() 
{
    return this->V;
}


Graph::Graph() {}
map<node, int>& Graph::getEdgeList(const node v)
{
    return this->E[v];
}

ostream& operator<<(ostream& out, Graph g)
{
    out << "Vertices: " << "{ ";

    for(vector<int>::const_iterator it = g.V.begin(); it != g.V.end(); it++)
        out << *it << "," ;

    out << "}" << endl; 
    out << "Edges: " << endl;

    int i = 0; 
    for(vector<map<node, int> >::const_iterator it = g.E.begin(); it != g.E.end(); it++, i++)
    {
        out << "E(" << i << "): {";
        for(map<node, int>::const_iterator e = (*it).begin(); e != (*it).end(); e++)
            out << *e << ", ";
        out << "}" << endl;
    }

    return out;
}




bool Graph::contains_vertex(const node v)
{
    for(vector<int>::iterator it = V.begin(); it != V.end(); it++)
        if (*it == v) return true;

    return false;
}

bool Graph::contains_edge(const node from, const node to, const int weight)
{
    pair<const node, int> p(to, weight);
    for(map<node, int>::iterator it = this->E[from].begin(); it != E[from].end(); it++)
        if(p == *it) return true;
    return false;
}

bool Graph::add_edge(const node from, const node to, const int weight)
{
    if(from == to)
        return false;
   //cout << "size:" <<  this->E.size();
   pair<node, int> p(to,weight);
   if(this->contains_vertex(from) && this->contains_vertex(to))
        {
           this->E[from].insert(p);
           //cout<< Edge(to, weight);
           return true;
        }

   return false;
}

Graph::Graph(int const nVertices) 
{
    size_t sz = static_cast<size_t>(nVertices); 

    V.reserve(sz);
    E.reserve(sz);

    for(int i = 0; i < nVertices; i++)
        this->V.push_back(i);
    
    for(int i =0; i < nVertices; i++)
        this->E.push_back(map<node, int> ());

}

