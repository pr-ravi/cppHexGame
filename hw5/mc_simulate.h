#ifndef MC_SIMULATE_H
#define MC_SIMULATE_H


#include "hex_board.h"

#include <thread>
#include <algorithm>
#include <tuple>
#include <future>


//Monte Carlo simulator
class MC_simulator
{
    private:
        //original hex board
        HexBoard const hB;

        //vector of tuple (position, HexBoard, score)
        vector<tuple<int,HexBoard,int>> trials;

        //"Red" || "Blue"
        string const ai_player;
        string const player;

        //level of ai and thread number
        int const ai_level;
        int const num_threads;

        //get score for given position
        int get_score(int const position, HexBoard board);

        //mark a position, convinience function
        void move(int position, HexBoard& board, string cur_player);

    public:
        //call with the current hexboard, names of players, ai level and num of threads
        MC_simulator(const HexBoard& hB, string ai, string player, int ai_rank, int threads);

        //get move advised by MC simulation
        int get_next_move();
};

//create MC simulator from hexboard
MC_simulator::MC_simulator(const HexBoard& hex, string const ai, string const player, int const ai_rank, int threads) : 
    hB(hex), 
    ai_player(ai), 
    player(player), 
    ai_level(ai_rank),
    num_threads(threads)
{ 
    //reserve vector
    trials.reserve(hB.bSize * hB.bSize - 1);

    //add available positions to trial vector
    int  i = 0;
    for(auto elem : hB.blocks)
    {
        if(elem == HexBoard::occupy::NONE)
            trials.push_back(make_tuple(i, hB, 0));
        i += 1;
    }
}

//convinience function
void inline MC_simulator::move(int const position, HexBoard& board, string const cur_player)
{
    //get row and col from index, mark block on board
    int row = position / board.bSize;
    int col = position % board.bSize;
    board.mark(row, col, cur_player);
}


int MC_simulator::get_score(int const position, HexBoard board)
{ 
    int score = 0;

    //possible moves excluding initial move
    vector<int> moves;
    moves.reserve(trials.size() - 1);

    for(auto t : trials)
        if (get<0>(t) != position)
            moves.push_back(get<0>(t));

    //make initial move
    move(position, board, ai_player);

    //run simulation ai_level times
    for(int i = 0; i < ai_level ; i++)
    {
        //create new board
        HexBoard h = board;        

        //shuffle the moves
        std::random_shuffle(moves.begin(), moves.end());

        //play half of the moves as ai
        for(unsigned long k = 0; k < moves.size() / 2; k++)
            move(moves[k], h, ai_player);

        //did ai win? if so, increase score of position
        if (h.get_winner() == ai_player)
            score++;
    }

    //return final score for the inital position
    return score;
}


//next best move
int MC_simulator::get_next_move()
{
    int trial_size = trials.size();
    int position;

    vector<future<int>> threads;
    threads.reserve(trial_size);

    //sequential computation
    if(num_threads == 0)
    {
        for(auto &trial :trials)
        {
            //get values from tuple
            position = get<0>(trial);
            HexBoard &board = get<1>(trial);
            get<2>(trial) = get_score(position, board);
        }
    }
    else  //threaded computation
    {
        for(int i = 0; i < trial_size; i+= num_threads)
        {

            //create threads for the next available positions
            //e.g. t1,t2,t3,t4 for num_threads = 4
            for( int j = 0; j < num_threads; j++)
            {
                position = get<0>(trials[i + j]);
                HexBoard &board = get<1>(trials[i + j]);
                if( i + j < trial_size)
                    threads.push_back(std::async(std::launch::async,&MC_simulator::get_score, this, position, board));
            }

            //wait for result of threads
            for( int j = 0; j < num_threads; j++)
                if( i + j < trial_size)
                    get<2>(trials[i + j]) = threads[i + j].get();
        }
    }

    //get move with maximum score
    //3rd arg is a lamda, that compares two tuples 
    //decltype is used to infer types
    return get<0>( *std::max_element(trials.begin(), trials.end(),
                [] (tuple<int, HexBoard, int> t1, decltype(t1) t2) -> bool
                { return get<2>(t1) < get<2>(t2); }
                ));
}


#endif
