#include "graph.h"

//return no of edges
int Graph::edges() { return num_edges; }

//return no of vertices
node Graph::vertex_count() { return V.size(); }

//return vector of vertices
vector<int> Graph::vertices() { return this->V; }


//return edgelist of the vertex
map<node, int>& Graph::getEdgeList(const node v)
{
    return this->E[v];
}

//print the graph
ostream& operator<<(ostream& out, Graph g)
{
    out << "Vertices: " << "{ ";

    for(auto it: g.V)
        out << it << "," ;

    out << "}" << endl; 
    out << "Edges: " << endl;

    int i = 0; 
    for(auto item : g.E)
    {
        out << "E(" << i << "): {";
        for(auto edge : item)
            out << edge << ", ";
        out << "}" << endl;
    }

    return out;
}


//does the graph contain the vertex?
bool Graph::contains_vertex(const node v)
{
    for(auto item : V)
        if (item == v) return true;

    return false;
}

//does the graph contain the edge?
bool Graph::contains_edge(const node from, const node to, const int weight)
{
    pair<const node, int> p(to, weight);
    for(auto item : this->E[from])
        if(p == item) return true;
    return false;
}

//add edge to graph
bool Graph::add_edge(const node from, const node to, const int weight)
{
    if(from == to)
        return false;

    pair<node, int> p(to,weight);

    //if it contains both vertices, add edge
    if(this->contains_vertex(from) && this->contains_vertex(to))
    {
        this->E[from].insert(p);
        this->num_edges++;
        return true;
    }

    return false;
}

//create graph with n vertices
Graph::Graph(int num_nodes) { init(num_nodes); }

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

//default constructor
Graph::Graph()  { this->num_edges = 0; }
