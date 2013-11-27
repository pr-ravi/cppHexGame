#include "hex_board.h"
#include "union_find.h"
#include "mc_simulate.h"
#include <time.h>

/* Homework 4 */

int main()
{
    std::srand (unsigned (time(0)));
    int bSize; 
    cout << "Enter the board size: ";
    cin >> bSize;

    cout << "Enter AI difficulty[1..]: ";
    int ai_diff;
    cin >> ai_diff;
    HexBoard hB(bSize);

    int row, col;
    char ch;
    int moves =0;
    int max_moves = bSize * bSize;
    int guess;
    string player = "Blue";
    string ai = "Red";

    cout  << "Blue : (east - west)" << endl;
    cout << "Do you wish to be Blue (y/n)? ";
    cin >> ch;
    if(ch != 'y')
    {
        player = "Red";
        ai = "Blue";
    }
    //begin input till max moves
    for(moves = 0; moves < max_moves; moves++)
    {
        if(moves % 2 == 0)
        {
            cout << "Enter row:";
            cin >> row;
            cout << "Enter col:";
            cin >> col;

            if(!hB.mark(row - 1, col - 1, player))
            {
                cout << "Wrong move.. " << endl ;
                moves--;
                continue;
            }
        }
        else 
        {
            time_t t1 = time(NULL);
            cout << "Computer makes move"  << endl;
            MC_simulator mcarlo(hB, ai, player, 1000, 2);
            guess = mcarlo.get_next_move();
            //cout << "guess = " << guess << endl;
            time_t t2 = time(NULL);
            cout << "Time elapsed : " << difftime(t2,t1) << " secs" << endl;
            hB.mark(guess / bSize, guess % bSize, ai);
        }

        cout << hB;
        cout << endl;

        if(hB.won())
        {
            cout << "Player " << hB.get_winner() << " has won " << endl;
            return 0;
        }
    }
    cout << endl << "Game ends " << endl;
    return 0;
}
