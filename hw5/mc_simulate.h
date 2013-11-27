#ifndef MC_SIMULATE_H
#define MC_SIMULATE_H


#include "hex_board.h"

#include <thread>
#include <algorithm>
#include <tuple>
#include <future>
#include <functional>

//typedef position int;

//Monte Carlo simulator
class MC_simulator
{
    private:
        const HexBoard hB;
        // tuple of (position, Hexboard, trial_result)
        vector<tuple<int,HexBoard,int>> trials;
        int num_positions;
        int ai_level;
        const string ai_player;
        const string player;
        int get_score(int const position, HexBoard board);
        void move(int position, HexBoard& board, string cur_player);
        int simulate(HexBoard board, vector<int> moves, int num_times);
    public:
        MC_simulator(const HexBoard& hB, string ai, string player, int ai_rank);
        int get_next_move();
};

MC_simulator::MC_simulator(const HexBoard& hex, string ai, string player, int ai_rank) : 
    hB(hex), 
    ai_player(ai), 
    player(player), 
    ai_level(ai_rank) 
{ 
    //reserve vectors
    trials.reserve(hB.bSize * hB.bSize - 1);

    //add available positions
    int  i = 0;
    for(auto elem : hB.blocks)
    {
        if(elem == HexBoard::occupy::NONE)
            trials.push_back(make_tuple(i, hB, 0));
        i++;
    }    
    num_positions = i - 1;

}

void inline MC_simulator::move(int position, HexBoard& board, string cur_player)
{
    int row = position / board.bSize;
    int col = position % board.bSize;
    board.mark(row, col, cur_player);
}

int MC_simulator::simulate(HexBoard board, vector<int> moves,  int num_times)
{
 //make random moves for both players ai_level times
   int score = 0;
    for(int i = 0; i < num_times ; i++)
    {
        
        HexBoard h = board;
        vector<int> shuffled_moves(moves);
        random_shuffle(shuffled_moves.begin(), shuffled_moves.end());

        int k= 0;
        for(auto pos : shuffled_moves)
        {
            if( k % 2 == 0)
                move(pos, h, player);
            else 
                move(pos, h, ai_player);
            k++;
        }

        if (h.get_winner() == ai_player)
            score++;
    }
    return score;
}


int MC_simulator::get_score(int const position, HexBoard board)
{ 
    int score = 0;
    int num_threads = 2;

    //possilbe moves excluding initial move
    vector<int> moves;
    moves.reserve(trials.size() - 1);

    for(auto t : trials)
        if (get<0>(t) != position)
            moves.push_back(get<0>(t));



    //cout << endl <<"Initial Move :: " << position  << endl;
    //make initial move
    move(position, board, ai_player);
    //cout << endl << "Simulation for " << position << endl;

    if(num_threads == 0)
        score+= simulate(board, moves, ai_level);
    else 
    {
        vector<future<int>> threads;
        threads.reserve(num_threads);

        threads.push_back(std::async(std::launch::async,&MC_simulator::simulate,this,  board, moves, ai_level / num_threads + (ai_level % num_threads)));
            //threads.push_back(std::async([=] () -> int { return simulate(board, moves,  ai_level / num_threads + (ai_level % num_threads)) ; }));
        for(int i = 1; i < num_threads; i++)
            threads.push_back(std::async(std::launch::async,&MC_simulator::simulate,this,  board, moves, ai_level / num_threads));
            //threads.push_back(std::async([=] () -> int { return simulate(board, moves,  ai_level / num_threads) ; }));

        for(int i = 0 ; i < num_threads; i++)
            score += threads[i].get();
        /*
        auto f1 = std::async([=] () -> int { return simulate(board, moves,  ai_level / 2 + ai_level % 2); });
        auto f2 = std::async([=] () -> int { return simulate(board, moves,  ai_level / 2) ; });

        score += f1.get();
        score += f2.get();
        */
    }
    //cout <<"Score: " << score  << endl;
    return score;
}


int MC_simulator::get_next_move()
{
    for(auto &trial : trials)
    {
        //get values from tuple
        int position = get<0>(trial);
        HexBoard &board = get<1>(trial);
        //cout << "Simulating for position " << position <<  endl;

        //run simulation
        get<2>(trial) = get_score(position, board);
    }

    //return get<2>( *std::max_element(trials.begin(), trials.end(), tup_greater));
    return get<0>( *std::max_element(trials.begin(), trials.end(),
                [] (tuple<int, HexBoard, int> t1, decltype(t1) t2) -> bool
                { return get<2>(t1) < get<2>(t2); }
                ));
}


#endif
