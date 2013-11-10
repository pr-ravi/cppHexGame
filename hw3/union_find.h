#include "graph.h"
#include <vector>

#ifndef UNION_FIND_H
#define UNION_FIND_H

//A class that implements the union find algorithm
//to find if two nodes are connected
class UnionFind {
    public:
        UnionFind(int num_nodes);
        void union_node(int src, int dest);
        bool is_connected(int src, int dest);

    private:
        int component(int n);
        vector<int> connected, size;

};

//initialize the component and size array
UnionFind::UnionFind(int num_nodes)
{
    connected.reserve(num_nodes);
    size.reserve(num_nodes);

    //initialize the component array to its index
    //and set size of all trees to 1
    for(auto i = 0 ; i < num_nodes; i++)
    {
        connected.push_back(i);
        size.push_back(1);
    }
}

//return component to which the node belongs to
int UnionFind::component(int n)
{
    //go through list recursively until value is index
    while(connected[n] != n) 
        n = connected[n];
    return n;
}

//are the nodes connected?
bool UnionFind::is_connected(int src, int dest)
{
    return component(src) == component(dest);
}

//connect two components
void UnionFind::union_node(int src, int dest)
{
    int cmp1 = component(src);
    int cmp2 = component(dest);

    //add smaller tree to larger one
    if(size[cmp1] < size[cmp2]) 
    {
        connected[cmp1] = cmp2;
        size[cmp1] += size[cmp2];
    }
    else
    {
        connected[cmp2] = cmp1;
        size[cmp2] += size[cmp1];
    }
}

#endif
