#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <map>
#include <vector>
#include <iostream>
#include <string>


typedef int node;    //makes sense
using namespace std;

class Graph 
{
    private:
        //vector of vertices
        vector<int> V;                      
        //vector of maps with node as key and weight as value
        vector<map<node, int> > E;                          
        int num_edges;
        bool contains_vertex(node v);
        bool contains_edge(node from, node to, int weight);

    public:
        //get the map associated with the vertex
        map<node, int>& getEdgeList(node v);
        bool add_edge(node from, node to, int weight);
        //constructors
        Graph(string flname);
        Graph();
        Graph(int num_nodes);
        //prepare edge list & reserve space
        void init(int nVertices);
        //this is for output, debug
        friend ostream& operator<<(ostream& out, Graph g);
        //testcase
        friend void test();
        //number of nodes
        node vertex_count();
        //get list of vertices
        vector<int> vertices();
        //get number of edges
        int edges();
        
};

//print a pair of node, weight
inline ostream& operator<<(ostream& out, pair<node, int> p)
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

#endif
