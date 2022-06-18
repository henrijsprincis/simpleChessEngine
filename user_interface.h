#pragma once
#include "chess.h"

#define WHITE_SQUARE 'W'
#define BLACK_SQUARE 'B'
#define EMPTY_SQUARE 0x20

void createNextMessage( string msg );
void appendToNextMessage( string msg );
void clearScreen( void );
void printLogo( void );
void printLogo( void );
void printMenu( void );
void printMessage( void );
void printLine( int iLine, int iColor1, int iColor2, Game& game );
void printLineBitboard(int iLine, int iColor1, int iColor2, vector<vector<char>> game_board);
void printSituation( Game& game );
void printBoard(Game& game);
void printBitBoard(Game& game);
