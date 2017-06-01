#include <iostream>
using namespace std;

#include "TicTacToeGame.h"
#include "Player.h"
#include "MonteCarloBot.h"

#include <Windows.h>

int main ()
{
    // i want real random!
    SYSTEMTIME st;
    GetSystemTime ( &st );
    srand ( st.wMilliseconds );

    TTTGame::Field field;
    TTTGame::Init ( field );

    Player * player1 = new Bot ();
    player1->SetFigure ( TTT_CROSS );
    Player * player2 = new MonteCarloBot ();
    player2->SetFigure ( TTT_CIRCLE );

    Player * current = player1;

    cout << "Monte-Carlo bot setting up..." << endl;
    for (int i = 0; i < 10; i++ )
    {
        while ( TTTGame::IsPlayable ( field ) )
        {
            current->MakeMove ( field );

            if ( current == player1 )
                current = player2;
            else
                current = player1;
        }

        player1->Reset ();
        player2->Reset ();
        TTTGame::Init ( field );
    }

    system ( "cls" );
    
    player1 = new User ();
    player1->SetFigure ( TTT_CROSS );
    current = player1;

    while ( TTTGame::IsPlayable ( field ) )
    {
        TTTGame::Print ( field );

        cout << "Player " << current->GetFigure () << " turn: ";

        current->MakeMove ( field );

        if ( current == player1 )
            current = player2;
        else
            current = player1;
    }

    TTTGame::Print ( field );
    cout << "Winner is Player " << TTTGame::CheckWin ( field ) << endl;
    system ( "pause" );
}