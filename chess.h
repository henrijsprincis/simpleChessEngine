#pragma once
#include "includes.h"
#include <list>
class Chess
{
public:
   static int getPieceColor( char chPiece );

   static bool isWhitePiece( char chPiece );

   static bool isBlackPiece( char chPiece );

   static std::string describePiece( char chPiece );

   enum PieceColor
   {
      WHITE_PIECE = 1,
      BLACK_PIECE = 0
   };

   enum Player
   {
      WHITE_PLAYER = 1,
      BLACK_PLAYER = 0
   };

   enum Side
   {
      QUEEN_SIDE = 2,
      KING_SIDE  = 3
   };

   enum Direction
   {
      HORIZONTAL = 0,
      VERTICAL,
      DIAGONAL,
      L_SHAPE
   };

   struct Position
   {
      int iRow;
      int iColumn;
   };

   struct EnPassant
   {
      bool bApplied;
      Position PawnCaptured;
   };

   struct Castling
   {
      bool bApplied;
      Position rook_before;
      Position rook_after;
   };

   struct Promotion
   {
      bool bApplied;
      //Position  pos;
      char chBefore;
      char chAfter;
   };

   struct IntendedMove
   {
      char chPiece;
      Position from;
      Position to;
   };

   struct Attacker
   {
      Position  pos;
      Direction dir;
   };

   struct UnderAttack
   {
      bool bUnderAttack;
      int iNumAttackers;
      Attacker attacker[9]; //maximum theorical number of attackers
   };
   
   const char initial_board[8][8] =
   {
      // This represents the pieces on the board.
      // Keep in mind that pieces[0][0] represents A1
      // pieces[1][1] represents B2 and so on.
      // Letters in CAPITAL are white
      { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
      { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
      { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
      { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
      { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
      { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
      { 'p',  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
      { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
   };
};

class Game : Chess
{
public:
   Game();
   ~Game();


//   void logMove( std::string &to_record );
//     string getLastMove( void );
//     void deleteLastMove( void );

   int getCurrentTurn(void);
   char getPieceAtPosition(int iRow, int iColumn);
   // Save all the moves
   struct Round
   {
      string white_move;
      string black_move;
   };

   //std::deque<std::string> moves;
   std::deque<Round> rounds;

   // Save the captured pieces
   std::vector<char> white_captured;
   std::vector<char> black_captured;

   // Represent the pieces in the board
   vector<vector<char>> vector_board;
   bool was_captured = false;

   //bitboard stuff
   struct chess_bitboard {
       //keep track of starting stuff for pawns
       unsigned long long pawns_white_start;
       unsigned long long pawns_black_start;

       unsigned long long white_pawns;
       unsigned long long black_pawns;

       unsigned long long white_knights;
       unsigned long long black_knights;

       unsigned long long white_bishops;
       unsigned long long black_bishops;

       unsigned long long white_rooks;
       unsigned long long black_rooks;

       unsigned long long white_kings;
       unsigned long long black_kings;

       unsigned long long white_queens;
       unsigned long long black_queens;

       bool white_castle_king = true;
       bool black_castle_king = true;

       bool white_castle_queen = true;
       bool black_castle_queen = true;
       
       bool black_castled = false;
       bool white_castled = false;
       bool white = true;//stores current turn



       bool operator == (const chess_bitboard& rhs) const
       { 
           /* your logic for comparision between "*this" and "rhs" */
           if (this->white_pawns == rhs.white_pawns && this->black_pawns == rhs.black_pawns) {
               if (this->white_knights == rhs.white_knights && this->black_knights == rhs.black_knights) {
                   if (this->white_bishops == rhs.white_bishops && this->black_bishops == rhs.black_bishops) {
                       if (this->white_rooks == rhs.white_rooks && this->black_rooks == rhs.black_rooks) {
                           if (this->white_kings == rhs.white_kings && this->black_kings == rhs.black_kings) {
                               if (this->white_queens == rhs.white_queens && this->black_queens == rhs.black_queens) {
                                   if(this->white == rhs.white){
                                        return true;
                                   }
                               }
                           }
                       }
                   }
               }
           }
           return false;
       }

   } bit_board;
   static   int getNumberPieces(chess_bitboard b);
   
   struct chess_init_arrays {
       vector<vector<int>> white_pawns;
       vector<vector<int>> black_pawns;

       vector<vector<int>> white_knights;
       vector<vector<int>> black_knights;

       vector<vector<int>> white_bishops;
       vector<vector<int>> black_bishops;

       vector<vector<int>> white_rooks;
       vector<vector<int>> black_rooks;

       vector<vector<int>> white_kings;
       vector<vector<int>> black_kings;

       vector<vector<int>> white_queens;
       vector<vector<int>> black_queens;

   } chess_init;

   chess_bitboard position_array[500];
   int current_move = 0;

   vector<vector<char>> bitboardToChar();
   void setStartingPosition();

   bool isCheckMate();

private:

   
   // Castling requirements
   bool m_bCastlingKingSideAllowed[2];
   bool m_bCastlingQueenSideAllowed[2];

   // Holds the current turn
   int  m_CurrentTurn;

   // Has the game finished already?
   bool m_bGameFinished;




   
};

class piece {
public:
    

    struct Position
    {
        int iRow;
        int iColumn;
    };
    char chPiece;




};
