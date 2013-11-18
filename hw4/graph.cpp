#include "graph.h"

int Graph::edges()
{
    return num_edges;
}

node Graph::vertex_count() { return V.size(); }

vector<int> Graph::vertices() 
{
    return this->V;
}


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

    pair<node, int> p(to,weight);
    if(this->contains_vertex(from) && this->contains_vertex(to))
    {
        this->E[from].insert(p);
        this->num_edges++;
        return true;
    }

    return false;
}

//create graph with n vertices
Graph::Graph(int num_nodes)
{

    init(num_nodes);
}

//initialize edgelist & resreve space
void Graph::init(int const nVertices) 
{
    size_t sz = static_cast<size_t>(nVertices); 
    this->num_edges = 0;

    this->V.reserve(sz);
    this->E.reserve(sz);

    //create vertices
    for(int i = 0; i < nVertices; i++)
        this->V.push_back(i);
    
    //create initial edgelist for each vertex
    for(int i =0; i < nVertices; i++)
        this->E.push_back(map<node, int> ());

}

//create a graph from a file
Graph::Graph(string flname)
{
    //read input file
    ifstream fl(flname, ios::in);
    //could not open file
    if(! fl.is_open())
        cout << "Error opening file " << endl;

    int num_nodes, num_edges = 0;
    int src, dest, weight;
    //get number of nodes
    fl >> num_nodes;
    //initialize and add edges
    init(num_nodes);
    while(! fl.eof())
    {
        fl >> src >> dest >> weight;
        this->add_edge(src, dest, weight);
        num_edges++;
    }

    //close file
    fl.close();
}

Graph::Graph() 
{
    this->num_edges = 0;
}
