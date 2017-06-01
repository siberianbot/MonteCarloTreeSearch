#ifndef TICTACTOE_GAME
#define TICTACTOE_GAME

#include <iostream>
using namespace std;

#define TTT_EMPTY           0
#define TTT_CROSS           1
#define TTT_CIRCLE          2

#define TTT_FIELDSIZE       3

namespace TTTGame
{
    typedef int Field [TTT_FIELDSIZE][TTT_FIELDSIZE];

    void Init ( Field & __field )
    {
        for ( int i = 0; i < TTT_FIELDSIZE; i++ )
            for ( int j = 0; j < TTT_FIELDSIZE; j++ )
                __field[i][j] = TTT_EMPTY;
    }

    void Print ( Field & __field )
    {
        for ( int i = 0; i < TTT_FIELDSIZE; i++ )
        {
            for ( int j = 0; j < TTT_FIELDSIZE; j++ )
            {
                if ( j != 0 ) 
                    cout << '|';
                
                cout << ' ' << __field[i][j] << ' ';
            }
            if ( i != TTT_FIELDSIZE - 1 )
                cout << endl << "-----------" << endl;
            else
                cout << endl;
        }
    }

    bool MakeMove ( Field & __field, int __who, int __wherex, int __wherey )
    {
        if ( __wherex < 0 || __wherex >= TTT_FIELDSIZE )
            return false;
        if ( __wherey < 0 || __wherey >= TTT_FIELDSIZE )
            return false;

        if ( __field[__wherey][__wherex] != TTT_EMPTY )
            return false;

        __field[__wherey][__wherex] = __who;
        return true;
    }

    int CheckWin ( Field & __field )
    {
        /// vertical
        for ( int i = 0; i < TTT_FIELDSIZE; i++ )
        {
            if ( __field[0][i] == __field[1][i] && __field[1][i] == __field[2][i] && __field[1][i] != TTT_EMPTY )
                return __field[1][i];
        }

        /// horizontal
        for ( int i = 0; i < TTT_FIELDSIZE; i++ )
        {
            if ( __field[i][0] == __field[i][1] && __field[i][1] == __field[i][2] && __field[i][1] != TTT_EMPTY )
                return __field[i][1];
        }

        /// diagonal 
        if ( __field[0][0] == __field[1][1] && __field[1][1] == __field[2][2] && __field[1][1] != TTT_EMPTY )
            return __field[1][1];

        if ( __field[0][2] == __field[1][1] && __field[1][1] == __field[2][0] && __field[1][1] != TTT_EMPTY )
            return __field[1][1];

        /// no one wins
        return TTT_EMPTY;
    }

    bool IsPlayable ( Field & __field )
    {
        if ( CheckWin ( __field ) ) return false;

        for ( int i = 0; i < TTT_FIELDSIZE; i++ )
            for ( int j = 0; j < TTT_FIELDSIZE; j++ )
                if ( __field[i][j] == TTT_EMPTY )
                    return true;

        return false;
    }
}

#endif