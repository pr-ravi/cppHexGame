#include "hex_board.h"
#include "union_find.h"

/* Homework 4 */

int main()
{
    int bSize;
    cout << "Enter the board size: ";
    cin >> bSize;

    HexBoard hB(bSize);

    int row, col;
    int moves =0;
    int max_moves = bSize * bSize;
    string player = "Blue";

    //begin input till max moves
    for(moves = 0; moves < max_moves; moves++)
    {
        cout << "Enter position for " << player << endl;
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
        
        cout << hB;
        cout << endl;

        if(hB.won())
        {
            cout << "Player " << hB.get_winner() << " has won " << endl;
            return 0;
        }

        //switch player each turn
        if(player == "Blue")
            player = "Red";
        else 
            player = "Blue";
    }
    cout << endl << "Game ends " << endl;
    return 0;
}
