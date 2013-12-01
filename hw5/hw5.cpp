#include "hex_board.h"
#include "union_find.h"
#include "mc_simulate.h"
#include <time.h>
#include <thread>

/* Homework 5 */

//not used
void print_retry()
{
    cout << "Wrong entry. Please try again!! " << endl;
    cout.flush();
}

//sanitise cin to get a character contained in a string
int  get_char(const string prompt,const string valid_ch)
{
    cout << prompt;
    char c;

    while(true)
    {
        if(cin >> c)
        {
            for(auto ch : valid_ch)
                if(ch == c)
                    return c;
        }
        else 
        {
            cin.clear();
            cin.ignore();
        }
    }
}

//sanitise cin to get a number from (0, N]
int  get_number(const string prompt, int upper_bound, bool allow_zero = false)
{
    cout << prompt;
    int n;

    while(true)
    {
        if(cin >> n)
        {
            if( (allow_zero ||  n > 0)  && n <= upper_bound)
                return n;
        }
        else 
        {
            cin.clear();
            cin.ignore();
        }
    }
}

//main entry
int main()
{
    int row, col;    //row, col of board
    char ch;         //for input char
    int moves =0;    //moves already played
    int guess;       //ai guess
    int turn = 0;    //player turn
    int ai_diff;     //ai difficulty
    int size;        //size of board
    int threads = 0; //number of threads

    string player = "Blue";
    string ai = "Red";

    //seed random number
    std::srand (unsigned (time(0)));

    /* input conditions */
    size = get_number("\nEnter the board size: ", 20);

    int max_moves = size * size; //maximum allowable moves

    cout << endl << "Enter AI difficulty" << endl << "Higher AI means more time. [Easy/Medium/Hard]: ";
    ch = get_char("","emhEMH");
    switch(ch)
    {
        case 'e':
        case 'E':
            ai_diff = 1024;
            break;
        case 'm': 
        case 'M':
            ai_diff = 5000;
            break;
        case 'h':
        case 'H':
            ai_diff = 10000;
    }

    //create hex board
    HexBoard hB(size);


    cout  <<  endl << "Blue : (east - west)" << endl;
    ch = get_char( "Do you wish to be Blue (y/n)? " ,"yYnN");
    if(ch != 'y')
    {
        player = "Red";
        ai = "Blue";
    }

    ch = get_char("\nDo you wish to go first (y/n)? ", "yYnN");
    if(ch != 'y')
        turn = 1;

    ch = get_char("\nDo you wish to use threads(y/n)? ", "yYnN");
    if(ch == 'y')
    {
        threads = std::thread::hardware_concurrency();
        cout << "Optimal threads: " << threads << endl;
    }

    cout << endl;

    /*play game */
    for(moves = 0; moves < max_moves; moves++)
    {
        //human player
        if(moves % 2 == turn)
        {
            row = get_number("Enter row:", size);
            col = get_number("Enter col:", size);

            if(!hB.mark(row - 1, col - 1, player))
            {
                cout << endl << "Wrong move.. " << endl ;
                moves--;
                continue;
            }
        }
        //computer player
        else 
        {
            time_t t1 = time(NULL);
            cout << "Computer makes move"  << endl;

            //get guess by monte carlo simulation
            MC_simulator mcarlo(hB, ai, player,ai_diff, threads);
            guess = mcarlo.get_next_move();

            time_t t2 = time(NULL);
            cout << "Time elapsed : " << difftime(t2,t1) << " secs" << endl;
            hB.mark(guess / size, guess % size, ai);
        }

        //print board
        cout << endl << hB;
        cout << endl;

        //do we have a winner?
        if(hB.get_winner() != "None")
        {
            cout << "Player " << hB.get_winner() << " has won " << endl;
            return 0;
        }
    }
    cout << endl << "Game ends " << endl;
    return 0;
}
