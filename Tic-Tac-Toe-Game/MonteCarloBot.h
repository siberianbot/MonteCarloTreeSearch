#ifndef MONTECARLOBOT
#define MONTECARLOBOT

#include <iostream>
#include <vector>
using namespace std;

#include "Player.h"
#include "TicTacToeGame.h"

typedef struct MCBTreeNode
{
    int                     Player      = 0;

    int                     MoveX       = -1;
    int                     MoveY       = -1;

    int                     Wins        = 0;
    int                     Attempts    = 0;

    MCBTreeNode *           Parent      = nullptr;
    vector <MCBTreeNode *>  Nodes;
} MCBTreeNode;

class MonteCarloBot : public Player
{
private:
    int                     mFigure;

    TTTGame::Field          mPreviousStep;

    void                    FindNewStep ( TTTGame::Field & __field, int & __x, int & __y )
    {
        for (int y = 0; y < TTT_FIELDSIZE; y++)
            for (int x = 0; x < TTT_FIELDSIZE; x++)
                if ( __field[y][x] != this->mPreviousStep[y][x] )
                {
                    __x = x;
                    __y = y;
                }

        for ( int y = 0; y < TTT_FIELDSIZE; y++ )
            for ( int x = 0; x < TTT_FIELDSIZE; x++ )
                this->mPreviousStep[y][x] = __field[y][x];
    }

    MCBTreeNode *           mTree;
    MCBTreeNode *           mCurrent;

public:
    /* ctor */              MonteCarloBot ()
    {
        this->mFigure = 0;
        this->mTree = new MCBTreeNode;
        this->mCurrent = this->mTree;
        TTTGame::Init ( this->mPreviousStep );
    }

    void                    SetFigure ( int __figure )
    {
        this->mFigure = __figure;
    }

    int                     GetFigure ()
    {
        return this->mFigure;
    }

    void                    MakeMove ( TTTGame::Field & __field )
    {
        cout << "MONTE-CARLO BOT MAKING MOVE..." << endl;

        // 0. add node with new move.
        bool exist = false;
        
        int enemyx = -1, enemyy = -1;
        this->FindNewStep ( __field, enemyx, enemyy );

        for ( MCBTreeNode * node : this->mCurrent->Nodes )
        {
            if ( node->MoveX == enemyx && node->MoveY == enemyy )
            {
                exist = true;
                this->mCurrent = node;
            }
        }

        if ( !exist )
        {
            MCBTreeNode * enemymove = new MCBTreeNode;
            enemymove->Parent = this->mCurrent;
            enemymove->MoveX = enemyx;
            enemymove->MoveY = enemyy;
            enemymove->Player = (this->mFigure == TTT_CROSS) ? TTT_CIRCLE : TTT_CROSS;
            this->mCurrent->Nodes.push_back ( enemymove );
            this->mCurrent = enemymove;
        }

        // 1. selection
        // select node with more wins.
        MCBTreeNode * bestnode = this->mCurrent;
        for ( MCBTreeNode * node : this->mCurrent->Nodes )
        {
            if ( ((double) node->Wins / node->Attempts) > ((double) bestnode->Wins / bestnode->Attempts) )
                bestnode = node;
        }

        // 2. expanding
        // create new node.
        MCBTreeNode * newnode = new MCBTreeNode;
        newnode->Parent = bestnode;
        newnode->Player = this->mFigure;
        this->mCurrent->Nodes.push_back ( newnode );

        // 3. simulation
        // simulate game.
        TTTGame::Field field;
        for ( int y = 0; y < TTT_FIELDSIZE; y++ )
            for ( int x = 0; x < TTT_FIELDSIZE; x++ )
                field[y][x] = __field[y][x];

        Player * bot1 = new Bot ();
        bot1->SetFigure ( (this->mFigure == TTT_CROSS) ? TTT_CIRCLE : TTT_CROSS );
        Player * bot2 = new Bot ();
        bot2->SetFigure ( this->mFigure );

        Player * current = bot2;

        while ( TTTGame::IsPlayable ( field ) )
        {
            current->MakeMove ( field );

            if ( newnode->MoveX == -1 && newnode->MoveY == -1 )
                this->FindNewStep ( field, newnode->MoveX, newnode->MoveY );

            if ( current == bot1 )
                current = bot2;
            else
                current = bot1;
        }

        // 4. backpropagation.

        int winner = TTTGame::CheckWin ( field );
        
        MCBTreeNode * currentnode = newnode;
        while ( currentnode != nullptr )
        {
            currentnode->Attempts++;

            if ( currentnode->Player == winner )
                currentnode->Wins++;

            currentnode = currentnode->Parent;
        }

        // make move...
        this->mCurrent = newnode;
        TTTGame::MakeMove ( __field, this->mFigure, mCurrent->MoveX, mCurrent->MoveY );
    }

    void                    Reset ()
    {
        this->mCurrent = this->mTree;
        TTTGame::Init ( this->mPreviousStep );
    }
};

#endif