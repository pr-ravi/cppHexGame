#ifndef HEXBOARD_H
#define HEXBOARD_H

#include "union_find.h"
//#include "mc_simulate.h"
#include <string>
#include <iostream>

using namespace std;
//model the hex board
//A union find structure is used for different players
//A vector of node states is used for modeling the color of a node
class HexBoard
{
    private:
        void add_edge(int index, string player);
        //different states of each node
        enum class occupy{ RED, BLUE, NONE }; 
        //virtual nodes
        struct 
        {
            int TOP;
            int RIGHT;
            int BOTTOM;
            int LEFT;
        } vnodes;
        //vector of the states
        vector<occupy> blocks; 
        //ufB -> union find of blue
        //ufR -> union find of red
        UnionFind ufR, ufB;
        //winner
        string winner;
        //board size, 7 for 7x7 etc
        int bSize;
        void connect_virtual_nodes();
    
    public:
        string get_winner();
        bool won();
        HexBoard(int board_size);
        //mark (row, col) as occupied with color
        bool mark(const int row, const int col, const string player);
        friend ostream& operator<<(ostream& out, HexBoard hb);
        friend class MC_simulator;
        friend class BasicBoard;
};

void HexBoard::connect_virtual_nodes()
{
    int i;

    //set virtual nodes values
    vnodes.TOP = bSize * bSize;
    vnodes.BOTTOM = bSize * bSize + 1;
    vnodes.LEFT = bSize * bSize + 2;
    vnodes.RIGHT = bSize * bSize + 3;

    //conncect the virtual nodes
    //for both players

    //top row
    for(i = 0; i < bSize; i++)
        ufR.union_node(vnodes.TOP, i);

    //bottom row
    for(i = bSize * (bSize - 1); i < bSize * bSize; i++)
        ufR.union_node(vnodes.BOTTOM, i);

    //left column
    for(i = 0; i < bSize * bSize; i+= bSize)
        ufB.union_node(vnodes.LEFT, i);

    //right column
    for(i = bSize - 1; i < bSize * bSize; i+= bSize)
        ufB.union_node(vnodes.RIGHT, i);
}

//initialize graphs, union find structures, color array etc
HexBoard::HexBoard(int board_size) : ufR(UnionFind(board_size * board_size + 4)), 
                                     ufB(UnionFind(board_size * board_size + 4))
{
    bSize = board_size;
    
    connect_virtual_nodes();

    //set all empty blocks to none
    blocks = vector<occupy>(board_size * board_size, occupy::NONE);
    winner = "None";
}

//get name of winner
string HexBoard::get_winner()
{
    return winner;
}

//is there a winnner?
bool HexBoard::won()
{
    return winner != "None";
}

//mark node with selected color
bool HexBoard::mark(const int row, const int col, const string player)
{
    int index = row * bSize + col;
    //check boundary
    if(row >= bSize || row < 0)
        return false;
    if(col >= bSize || col < 0)
        return false;

    //some other player already has node
    if(blocks[index] != occupy::NONE)
        return false;
    else 
    {
        blocks[index] = (player == "Red" ? occupy::RED : occupy::BLUE);
        add_edge(index, player);
        return true;
    }
}

//adds all possible edges form the node
//first, we add all neighbouring edges to a vector
//then, we union the trees
void HexBoard::add_edge(int index, const string player)
{
    occupy state;
    if(player == "Blue")
        state = occupy::BLUE;
    else 
        state = occupy::RED;
    vector<pair<int, int>> edges;
    edges.reserve(6);

   //long conditions upcoming,
   //i = index
   //sz  = size of hex board eg 7 for 7x7
   // top left corner
   if(index == 0)
   {
       //two neighbours, i + 1 and i + sz
       if(blocks[index +         1] == state) edges.push_back(make_pair(index, index +         1));
       if(blocks[index +     bSize] == state) edges.push_back(make_pair(index, index +     bSize));
   }
   //top right corner
   else if(index == bSize - 1)
   {
       // i - 1, i + sz, i + sz - 1
       if(blocks[index -         1] == state) edges.push_back(make_pair(index, index -         1));
       if(blocks[index +     bSize] == state) edges.push_back(make_pair(index, index +     bSize));
       if(blocks[index + bSize - 1] == state) edges.push_back(make_pair(index, index + bSize - 1));
   }
   //bottom left corner
   else if(index == bSize * (bSize -1))
   {
       //i - sz, i + 1, i - sz + 1
       if(blocks[index +         1] == state) edges.push_back(make_pair(index, index +         1));
       if(blocks[index -     bSize] == state) edges.push_back(make_pair(index, index -     bSize));
       if(blocks[index - bSize + 1] == state) edges.push_back(make_pair(index, index - bSize + 1));
   }
   //bottom right corner
   else if(index == bSize * bSize - 1)
   {
       //i - 1, i - sz
       if(blocks[index -         1] == state) edges.push_back(make_pair(index, index -         1));
       if(blocks[index -     bSize] == state) edges.push_back(make_pair(index, index -     bSize));
   }
   //top row
   else if(index < bSize)
   {
       //i + 1, i - 1, i + sz, i + sz - 1
       if(blocks[index -         1] == state) edges.push_back(make_pair(index, index -         1));
       if(blocks[index +         1] == state) edges.push_back(make_pair(index, index +         1));
       if(blocks[index +     bSize] == state) edges.push_back(make_pair(index, index +     bSize));
       if(blocks[index + bSize - 1] == state) edges.push_back(make_pair(index, index + bSize - 1));
   }
   //bottom row
   else if(index > bSize * (bSize - 1))
   {
       //i + 1, i - 1, i - sz, i - sz + 1
       if(blocks[index -         1] == state) edges.push_back(make_pair(index, index -         1));
       if(blocks[index +         1] == state) edges.push_back(make_pair(index, index +         1));
       if(blocks[index -     bSize] == state) edges.push_back(make_pair(index, index -     bSize));
       if(blocks[index - bSize + 1] == state) edges.push_back(make_pair(index, index - bSize + 1));
   }
   //leftmost column
   else if(index % bSize == 0)
   {
       //i + 1, i + sz, i - sz, i - sz + 1
       if(blocks[index +         1] == state) edges.push_back(make_pair(index, index +         1));
       if(blocks[index +     bSize] == state) edges.push_back(make_pair(index, index +     bSize));
       if(blocks[index -     bSize] == state) edges.push_back(make_pair(index, index -     bSize));
       if(blocks[index - bSize + 1] == state) edges.push_back(make_pair(index, index - bSize + 1));
   }
   //rightmost column
   else if(index % bSize == bSize - 1)
   {
       //i - 1, i + sz, i - sz, i + sz - 1
       if(blocks[index -         1] == state) edges.push_back(make_pair(index, index -         1));
       if(blocks[index +     bSize] == state) edges.push_back(make_pair(index, index +     bSize));
       if(blocks[index -     bSize] == state) edges.push_back(make_pair(index, index -     bSize));
       if(blocks[index + bSize - 1] == state) edges.push_back(make_pair(index, index + bSize - 1));
   }
   //internal nodes
   else
   {
       //i + 1, i - 1, i + sz, i - sz, i + sz - 1, i - sz + 1
       if(blocks[index -         1] == state) edges.push_back(make_pair(index, index -         1));
       if(blocks[index +         1] == state) edges.push_back(make_pair(index, index +         1));
       if(blocks[index +     bSize] == state) edges.push_back(make_pair(index, index +     bSize));
       if(blocks[index -     bSize] == state) edges.push_back(make_pair(index, index -     bSize));
       if(blocks[index + bSize - 1] == state) edges.push_back(make_pair(index, index + bSize - 1));
       if(blocks[index - bSize + 1] == state) edges.push_back(make_pair(index, index - bSize + 1));
   }

   //Union the trees
   if(player == "Red")
    for(auto item : edges)
        ufR.union_node(item.first, item.second);
   else 
    for(auto item : edges)
        ufB.union_node(item.first, item.second);

   //did blue win?
   if(player == "Blue" && ufB.is_connected(vnodes.LEFT, vnodes.RIGHT))
       winner = "Blue";
   else if(player == "Red" && ufR.is_connected(vnodes.TOP, vnodes.BOTTOM))
       winner = "Red";

   
/*
   //did blue win ?
   if(player == "Blue")
   {
       //check if any in left most column is in same tree as the node
       for(int i = 0; i < bSize * bSize; i+=bSize)
           if(ufB.is_connected(i, index))
           {
               //then check right most column
               for(int j = bSize - 1; j < bSize * bSize; j += bSize)
                   if(ufB.is_connected(j, index))
                   {
                       winner = "Blue";
                       return;
                   }
               break;
           }
   }
   //did red win?
   else
       //check if any in top column is in same tree as the node
       for(int i = 0; i < bSize; i++)
           if(ufR.is_connected(i, index))
           {
               //then check bottom most column
               for(int j = bSize * (bSize - 1); j < bSize * bSize; j++)
                   if(ufR.is_connected(j, index))
                   {
                       winner = "Red";
                       return;
                   }
               break;
           }

*/
  }


//print hex board, mostly hacked and still not right, 
//so not bothering to comment it:(
ostream& operator<<(ostream& out, HexBoard hb)
{   
    int cur_index;
    for(int i = 0; i < hb.bSize; i++)
    {
       for(int k =0; k < i; k++) { cout << " "; }
       for(int j = 0 ; j < hb.bSize; j++)
       {
           cur_index = i * hb.bSize + j;
           if(hb.blocks[cur_index] == HexBoard::occupy::RED)
               cout << "R";
           else if(hb.blocks[cur_index] == HexBoard::occupy::BLUE)
               cout << "B";
           else
               cout << ".";
           if(j != hb.bSize - 1)
               cout << " - ";
       }
       cout << endl;
       for(int k =0; k < i ; k++) { cout << " "; }
       cout<< " ";
       for(int k =0; k < hb.bSize && i != hb.bSize - 1; k++)
       {
           if(k ==0)
               cout << "\\";
           else cout << " / \\";
           if( k == hb.bSize - 1)
               cout << endl;
       }
    }

    return out;
}

#endif
