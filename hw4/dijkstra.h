#ifndef DIGKSTRA_H
#define DIGKSTRA_H

#include "graph.h"
#include <queue>   //for priority queue
#include <limits>  //for getting max values of a datatype
#include <set>     //for set
#include <tuple>   //for typle ie (1,2,....) values

using namespace std;

//functor for sorting the priority queue
//funtors are classes that can be called with the () operator, ie lesserTuple(t1,t2)
struct lesserTuple
{
    bool operator()(tuple<node, node, int> t1, tuple<node, node, int> t2)
    {
        return get<2>(t1) > get<2>(t2);
    }
};

//shortest path calculator
class Dijkstra
{
    public:
    //the first constructor was used mostly to test another simulation
    Dijkstra(Graph& G, node src, node dest) : g(G), dest(dest), src(src), min_weight(-1) {}; 
    Dijkstra(Graph& G) : g(G), min_weight(-1) {}
    
    vector<int> get_path(int src, int dest);
    vector<int> get_path(); 
    inline int get_weight();
    inline bool has_valid_path();


    private:
    node src;        //source node
    node dest;       //destination node
    int min_weight;  //shortest path
    Graph& g;        //graph ;)
};


//has the path been computed?
bool Dijkstra::has_valid_path()
{
    return this->min_weight != -1;
}

//return minumum weight
int Dijkstra::get_weight()
{
    return this->min_weight;
}

//set src, dest and then call get_path()
vector<int> Dijkstra::get_path(int src, int dest)
{
    this->src = src;
    this->dest = dest;
    return get_path();
}

//find shortest path
vector<int> Dijkstra::get_path()
{
    // if same source and destination return empty path
    if(src == dest)
        return vector<int>();

    //shortest path
    vector<int> path; 
    //openset of (node, node, weight) pairs
    priority_queue<tuple<node, node, int>, vector<tuple<node, node, int> >, lesserTuple> openSet; 
    //visited set
    set<node> visited;
    //unvisited set
    set<node> unvisited;
    //distanceSet, is a map with the dest as key, and (src, weight) as value
    map<node, pair<node, int> > distanceSet; 
    //node under consideration
    tuple<node, node, int> cur_node; 

    
    //set initial distances of all nodes to max values and initial node to 0
    //mark all nodes as unvisited
    vector<node> nodes = g.vertices(); 
    int imax = numeric_limits<int>::max();
    for(vector<node>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        openSet.push(make_tuple(src, *it, imax));
        unvisited.insert(*it);
    }
    openSet.push(make_tuple(src, src, 0));

    
    map<node, int> edges;
    int cur_weight, cur_vertex;
    int prev_vertex = src;
    int new_dest, new_weight;
    set<node>::iterator found;
     while(true)
    {
        //get the node with lowest distance
        cur_node = openSet.top();
        cur_vertex = get<1>(cur_node);
        prev_vertex = get<0>(cur_node);
        cur_weight = get<2>(cur_node);
        //remove top element
        openSet.pop();

        //add current node to visited & remove from unvisited
        visited.insert(cur_vertex);
        found = unvisited.find(cur_vertex);
        if(found != unvisited.end())
            unvisited.erase(unvisited.find(cur_vertex));

        //debug
        /*
        cout<< "Visited: " << "{" ;
        for(auto it = visited.begin(); it != visited.end(); it++)
            cout << *it << ",";
        cout << "}" << endl;

        cout<< "Unvisited: " << "{" ;
        for(auto it = unvisited.begin(); it != unvisited.end(); it++)
            cout << *it << ",";
        cout << "}" << endl;
        */

        //add the node to the distance set 
        distanceSet.insert(make_pair(cur_vertex, make_pair(prev_vertex, cur_weight)));

        //if the dest has been reached or the distance is infinity
        if(cur_weight == imax)
            return vector<int>();
        else if(cur_vertex == dest)
            break;

        //calculate tentative distances and add to openset
        edges = g.getEdgeList(cur_vertex);
        for(map<node, int>::const_iterator it = edges.begin(); it != edges.end(); it++)
        {
          new_dest = (*it).first;
          new_weight = cur_weight + (*it).second;
          //if node is unvisited add to openset
          if(unvisited.find(new_dest) != unvisited.end())
              openSet.push(make_tuple(cur_vertex, new_dest, new_weight));
        }
    }

     //store minimum weight
     this->min_weight = cur_weight;

     //store dest
     path.push_back(dest);
     pair<node, pair<node, int> > dvector; 

     //compute path by backtracking
     while(cur_vertex != src)
     {
        dvector = *distanceSet.find(cur_vertex);
        cur_vertex = dvector.second.first;
        path.push_back(cur_vertex);
     }

    return path;
}

#endif
