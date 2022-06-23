#define _HAS_STD_BYTE 0
#include "includes.h"
#include "chess.h"
#include "user_interface.h"
#include <vector> 
#include <map>
#include "helperClass.h"




// -------------------------------------------------------------------
// Chess class
// -------------------------------------------------------------------
int Chess::getPieceColor(char chPiece)
{
   if (isupper(chPiece))
   {
      return WHITE_PIECE;
   }
   else
   {
      return BLACK_PIECE;
   }
}

bool Chess::isWhitePiece(char chPiece)
{
   return getPieceColor(chPiece) == Chess::WHITE_PIECE ? true : false;
}

bool Chess::isBlackPiece(char chPiece)
{
   return getPieceColor(chPiece) == Chess::BLACK_PIECE ? true : false;
}

std::string Chess::describePiece(char chPiece)
   {
      std::string description;

      if (isWhitePiece(chPiece))
      {
         description += "White ";
      }
      else
      {
         description += "Black ";
      }

      switch (toupper(chPiece))
      {
         case 'P':
         {
            description += "pawn";
         }
         break;

         case 'N':
         {
            description += "knight";
         }
         break;

         case 'B':
         {
            description += "bishop";
         }
         break;

         case 'R':
         {
            description += "rook";
         }
         break;

         case 'Q':
         {
            description += "queen";
         }
         break;

         default:
         {
            description += "unknow piece";
         }
         break;
      }

      return description;
   }

 
// -------------------------------------------------------------------
// Game class
// -------------------------------------------------------------------
Game::Game()
{

   // White player always starts
   m_CurrentTurn = WHITE_PLAYER;

   // Game on!
   m_bGameFinished = false;
    

   // Castling is allowed (to each side) until the player moves the king or the rook
   m_bCastlingKingSideAllowed[WHITE_PLAYER]  = true;
   m_bCastlingKingSideAllowed[BLACK_PLAYER]  = true;
   m_bCastlingQueenSideAllowed[WHITE_PLAYER] = true;
   m_bCastlingQueenSideAllowed[BLACK_PLAYER] = true;

   setStartingPosition();
   position_array[current_move] = bit_board;
   vector_board = bitboardToChar();
}

Game::~Game()
{
   white_captured.clear();
   black_captured.clear();
   rounds.clear();
}


bool Game::isCheckMate() {
    return !((bit_board.black_kings > 0ULL) && (bit_board.white_kings > 0ULL));

}

int Game::getCurrentTurn(void)
{
    //returns 1 when white to move and 0 when black to move
    return (current_move+1) % 2;
}

char Game::getPieceAtPosition(int iRow, int iColumn)
{
    return vector_board[iColumn][iRow];
}

//bitboard functions
void Game::setStartingPosition() {
    bool standard_pos = true;
    if(standard_pos){
                                    ////A                  B                  C                D                    E                  F                     G                  H
    chess_init.white_pawns = { {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0} };//{0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}
    chess_init.black_pawns = { {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0} };//{0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,1,0}

    chess_init.white_knights = { {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
    chess_init.black_knights = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0} };//{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}

    chess_init.white_bishops = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
    chess_init.black_bishops = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };//{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}

    chess_init.white_rooks = { {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0} };
    chess_init.black_rooks = { {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1} };

    chess_init.white_queens = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
    chess_init.black_queens = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };

    chess_init.white_kings = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
    chess_init.black_kings = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
    bit_board.black_castle_king = true;
    bit_board.black_castle_queen = true;
    bit_board.white_castle_king = true;
    bit_board.white_castle_queen = true;
    }


    bit_board.pawns_white_start = helperClass::vector_to_bitboard(chess_init.white_pawns);
    bit_board.pawns_black_start = helperClass::vector_to_bitboard(chess_init.black_pawns);
    
    bit_board.white_pawns = helperClass::vector_to_bitboard(chess_init.white_pawns);
    bit_board.black_pawns = helperClass::vector_to_bitboard(chess_init.black_pawns);

    bit_board.white_knights = helperClass::vector_to_bitboard(chess_init.white_knights);
    bit_board.black_knights = helperClass::vector_to_bitboard(chess_init.black_knights);

    bit_board.white_bishops = helperClass::vector_to_bitboard(chess_init.white_bishops);
    bit_board.black_bishops = helperClass::vector_to_bitboard(chess_init.black_bishops);

    bit_board.white_rooks = helperClass::vector_to_bitboard(chess_init.white_rooks);
    bit_board.black_rooks = helperClass::vector_to_bitboard(chess_init.black_rooks);

    bit_board.white_kings = helperClass::vector_to_bitboard(chess_init.white_kings);
    bit_board.black_kings = helperClass::vector_to_bitboard(chess_init.black_kings);

    bit_board.white_queens = helperClass::vector_to_bitboard(chess_init.white_queens);
    bit_board.black_queens = helperClass::vector_to_bitboard(chess_init.black_queens);
}

int Game::getNumberPieces(Game::chess_bitboard b) {
    return popcount(b.white_pawns + b.black_pawns + b.white_knights + b.black_knights + b.white_bishops + b.black_bishops + b.white_rooks + b.black_rooks + b.white_kings + b.black_kings + b.white_queens + b.black_queens);
}

vector<vector<char>> Game::bitboardToChar() {
    vector<vector<char>> board_vector(8, vector<char>(8, ' '));
    
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            int bit = col * 8 + row;
            char c = ' ';
            if (helperClass::IsBitSet(bit_board.white_pawns, bit)) {
                c = 'P';
            }
            if (helperClass::IsBitSet(bit_board.black_pawns, bit)) {
                c = 'p';
            }
            if (helperClass::IsBitSet(bit_board.white_knights, bit)) {
                c = 'N';
            }
            if (helperClass::IsBitSet(bit_board.black_knights, bit)) {
                c = 'n';
            }

            if (helperClass::IsBitSet(bit_board.white_bishops, bit)) {
                c = 'B';
            }
            if (helperClass::IsBitSet(bit_board.black_bishops, bit)) {
                c = 'b';
            }

            if (helperClass::IsBitSet(bit_board.white_rooks, bit)) {
                c = 'R';
            }
            if (helperClass::IsBitSet(bit_board.black_rooks, bit)) {
                c = 'r';
            }
            if (helperClass::IsBitSet(bit_board.white_queens, bit)) {
                c = 'Q';
            }

            if (helperClass::IsBitSet(bit_board.black_queens, bit)) {
                c = 'q';
            }

            if (helperClass::IsBitSet(bit_board.white_kings, bit)) {
                c = 'K';
            }

            if (helperClass::IsBitSet(bit_board.black_kings, bit)) {
                c = 'k';
            }
            board_vector[col][row] = c;
        }
    }
    return board_vector;

}