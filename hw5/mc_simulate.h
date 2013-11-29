#ifndef MC_SIMULATE_H
#define MC_SIMULATE_H


#include "hex_board.h"
#include "basic_board.h"

#include <thread>
#include <algorithm>
#include <tuple>
#include <future>
#include <functional>
#include <random>


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

        int const ai_level;
        int const num_threads;

        //get score for given position
        int get_score(int const position, HexBoard board);
        //mark a position, convinience function
        void move(int position, HexBoard& board, string cur_player);

        void move(int const position, BasicBoard& board);
    public:
        //call with the current hexboard, names of players, ai level and num of threads
        MC_simulator(const HexBoard& hB, string ai, string player, int ai_rank);
        //get move advised by MC simulation
        int get_next_move();
};

MC_simulator::MC_simulator(const HexBoard& hex, string const ai, string const player, int const ai_rank) : 
    hB(hex), 
    ai_player(ai), 
    player(player), 
    ai_level(ai_rank),
    num_threads(std::thread::hardware_concurrency())
{ 
    //reserve vectors
    trials.reserve(hB.bSize * hB.bSize - 1);

    //add available positions
    int  i = 0;
    for(auto elem : hB.blocks)
    {
        if(elem == HexBoard::occupy::NONE)
            trials.push_back(make_tuple(i, hB, 0));
        i += 1;
    }
}
//
    //num_threads(std::thread::hardware_concurrency())

void inline MC_simulator::move(int const position, HexBoard& board, string const cur_player)
{
    //get row and col from index, mark block on board
    int row = position / board.bSize;
    int col = position % board.bSize;
    board.mark(row, col, cur_player);
}
void inline MC_simulator::move(int const position, BasicBoard& board)
{
    //get row and col from index, mark block on board
    int row = position / board.bSize;
    int col = position % board.bSize;
    board.mark(row, col);
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

        //std::random_device rd;
        //std::mt19937 g(rd());
    //run simulation ai_level times
    for(int i = 0; i < ai_level ; i++)
    {
        //for each simulation, create a board
        //shuffle the available moves
        //and play till end
        //HexBoard h = board;        
       // std::shuffle(moves.begin(), moves.end(), g);
 
        BasicBoard h(board, ai_player);
        std::random_shuffle(moves.begin(), moves.end());

        //int k= 0;
        //for(auto pos : moves);
        cout << endl << "sim " << i  << " for position : " << position << endl;
        for(unsigned long j = 0; j < (moves.size() /2) + 1; j++)
        {
            //if(h.won())
             //   break;
            move(moves[j], h);
            cout << moves[j] << " ";
        }
        cout << endl;
        /*
        int k= 0;
        for(auto pos : moves)
        {
            if(h.won())
                break;
            //alternate players
            if( k % 2 == 0)
                move(pos, h, player);
            else 
                move(pos, h, ai_player);
            k++;
        }
        */
        //cout << endl <<h << endl;
        //If winner is AI, increase score
        if (h.get_winner() == ai_player)
            score++;
    }
    //cout << "Score for position " << position << " : " << score << endl;
    //return final score for the inital position
    return score;
}


int MC_simulator::get_next_move()
{
    int trial_size = trials.size();
    int position;

    vector<future<int>> threads;
    threads.reserve(trial_size);
    //for(int i = 0; i < num_threads; i++)
     //   threads[i] = nullptr;
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
