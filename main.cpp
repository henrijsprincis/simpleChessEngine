#include "includes.h"
#include <vector> 
#include "user_interface.h"
#include "chess.h"
#include "helperClass.h"
#include "debug.h"
#include <chrono>
#include <thread>
#include <intrin.h>
#include <unordered_map>
#include <bitset>

//THE MOST IMPORTANT HASH FUNCTION
template <>
struct hash<Game::chess_bitboard>
{
    std::size_t operator()(const Game::chess_bitboard& k) const
    {
        //this is not a good hashing algorithm but i dont get paid enough.
        unsigned long long n1 =  699263060593979792ULL;
        unsigned long long n2 =  10280863893657182221ULL;
        unsigned long long n3 =  9444613825643976278ULL;
        unsigned long long n4 =  4960753395245673650ULL;
        unsigned long long n5 =  3348032945078161581ULL;
        unsigned long long n6 =  13452118088618146400ULL;
        unsigned long long n7 =  9385381920577412259ULL;
        unsigned long long n8 =  14555483919324308287ULL;
        unsigned long long n9 =  12268618654937289798ULL;
        unsigned long long n10 = 5212878716342025485ULL;
        unsigned long long n11 = 3219443095174292432ULL;
        unsigned long long n12 = 4006770111060996520ULL;
        return (k.white_pawns + n1) ^ (k.black_pawns + n2) ^ (k.white_knights + n3) ^ (k.black_knights + n4) ^ (k.white_bishops + n5) ^ (k.black_bishops + n6) ^ (k.white_rooks + n7) ^ (k.black_rooks + n8) ^ (k.white_queens + n9) ^ (k.black_queens + n10) ^ (k.white_kings + n11) ^ (k.black_kings + n12) ^ (k.white_castle_king + k.black_castle_king + k.white_castle_queen + k.black_castle_queen);
    }
};

//---------------------------------------------------------------------------------------
// Global variable
//---------------------------------------------------------------------------------------
Game* current_game = NULL;
char chess_cols[9] = "ABCDEFGH";
char chess_rows[9] = "12345678";
int total_moves = 0;
std::unordered_map<Game::chess_bitboard, int> hash_table;

int depth = 5;
int current_minimax = 0;
//---------------------------------------------------------------------------------------
// Helper
// Auxiliar functions to determine if a move is valid, etc
//---------------------------------------------------------------------------------------


void newGame(void)
{
    if (NULL != current_game)
    {
        delete current_game;
    }

    current_game = new Game();
}

void undoMove() {
    current_game->current_move--;
    current_game->bit_board = current_game->position_array[current_game->current_move];
}
void update_current_board(Game::chess_bitboard future_board) {
    //sets current board to future board and saves the board in the array. also increments player's turn
    current_game->bit_board = future_board;
    current_game->current_move++;
    current_game->position_array[current_game->current_move] = current_game->bit_board;
}

int minimaxAB(int depth, int alpha, int beta, int white) {
    if (current_game->isCheckMate()) {
        //cout << "GOT MATE!" << endl;
        if (current_game->getCurrentTurn() == 1) {
            //white to play, but white is in checkmate. so white has lost
            return -1000;
        }
        else {
            return 1000; // otherwise white has won!
        }

    }
    if (depth == 0) {
        //return heuristic evaluation function
        return helperClass::getValueBitboard(current_game->bit_board);
    }
    //move stuff
    int number_elemts = 0;
    int best_move_value = 1000;
    if (white) {
        best_move_value = -1000;
    }
    
    Game::chess_bitboard output_array[48];
    for (int c = 0; c < 7; c++) {
        switch (c) {
        case 0://pawn
            number_elemts = helperClass::get_all_pawn_moves(current_game->bit_board, white, output_array);
            break;
        case 1:
            number_elemts = helperClass::get_all_knight_moves(current_game->bit_board, white, output_array);
            break;
        case 2:
            number_elemts = helperClass::get_all_bishop_moves(current_game->bit_board, white, output_array);
            break;
        case 3:
            number_elemts = helperClass::get_all_rook_moves(current_game->bit_board, white, output_array);
            break;
        case 4:
            number_elemts = helperClass::get_all_queen_moves(current_game->bit_board, white, output_array);
            break;
        case 5:
            number_elemts = helperClass::get_all_king_moves(current_game->bit_board, white, output_array);
            break;
        case 6:
            number_elemts = helperClass::get_all_castle_moves(current_game->bit_board, white, output_array);
            break;
        }

        for (int i = 0; i < number_elemts; i++) {
            update_current_board(output_array[i]);
            total_moves++;
            if (white) {
                best_move_value = std::max(best_move_value, minimaxAB(depth - 1, alpha, beta, 0));
                if (best_move_value >= beta) {
                    undoMove();
                    return best_move_value;
                }
                alpha = std::max(alpha, best_move_value);
            }
            else {
                best_move_value = std::min(best_move_value, minimaxAB(depth - 1, alpha, beta, 1));
                if (best_move_value <= alpha) {
                    undoMove();
                    return best_move_value;
                }
                beta = std::min(beta, best_move_value);
            }
            undoMove();
            }
        }
    
    return best_move_value;
}

void computerPlayMove(int depth, int alpha, int beta, int white) {
    if (current_game->isCheckMate()) {
        if (current_game->getCurrentTurn() == 1) {
            //white to play, but white is in checkmate. so white has lost
            cout << "WHITE WON!" << endl;
            return;
        }
        else {
            cout << "BLACK WON!" << endl;
            return;
        }
    }
    //move stuff
    int number_elemts = 0;
    int best_move_value = 1000;
    if (white) {
        best_move_value = -1000;
    }
    Game::chess_bitboard best_move;
    bool best_move_found = false;

    Game::chess_bitboard output_array[48];
    for (int c = 0; c < 7; c++) {
        switch (c) {
        case 0://pawn
            number_elemts = helperClass::get_all_pawn_moves(current_game->bit_board, white, output_array);
            break;
        case 1:
            number_elemts = helperClass::get_all_knight_moves(current_game->bit_board, white, output_array);
            break;
        case 2:
            number_elemts = helperClass::get_all_bishop_moves(current_game->bit_board, white, output_array);
            break;
        case 3:
            number_elemts = helperClass::get_all_rook_moves(current_game->bit_board, white, output_array);
            break;
        case 4:
            number_elemts = helperClass::get_all_queen_moves(current_game->bit_board, white, output_array);
            break;
        case 5:
            number_elemts = helperClass::get_all_king_moves(current_game->bit_board, white, output_array);
            break;
        case 6:
            number_elemts = helperClass::get_all_castle_moves(current_game->bit_board, white, output_array);
            break;
        }

        for (int i = 0; i < number_elemts; i++) {
            update_current_board(output_array[i]);
            //debug
            /*
            if (c != 0) {
            cout << "Number of moves: " << number_elemts << endl;
            current_game->vector_board = current_game->bitboardToChar();
            printBitBoard(*current_game);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            clearScreen();
            }
            */
            total_moves++;
            if (white) {
                
                int board_value = minimaxAB(depth - 1, alpha, beta, 0);
                if (board_value > best_move_value) {
                    best_move = output_array[i];
                }
                best_move_value = std::max(best_move_value, board_value);
                if (best_move_value >= beta) {
                    best_move = output_array[i];
                    undoMove();
                    break;
                }
                alpha = std::max(alpha, best_move_value);
            }
            else {
                int board_value = minimaxAB(depth - 1, alpha, beta, 1);
                if (board_value < best_move_value) {
                    best_move = output_array[i];
                }
                best_move_value = std::min(best_move_value, board_value);
                if (best_move_value <= alpha) {
                    best_move = output_array[i];
                    undoMove();
                    break;
                }
                beta = std::min(beta, best_move_value);
            }
            undoMove();
        }



    }
    current_minimax = best_move_value;
    //best_move = output_array[0];
    update_current_board(best_move);
}




bool movePieceMain(void)
{
    current_game->vector_board = current_game->bitboardToChar();
    bool success = false;
    std::string to_record;

    // Get user input for the piece they want to move
    cout << "Choose piece to be moved. (example: A1 or b2): ";

    std::string move_from;
    getline(cin, move_from);
    if (move_from == "Q") {
        exit(0);
    }
    if (move_from == "U") {
        
        if (current_game->current_move>2){
            cout << "UNDOING LAST 2 MOVES" << endl;
            undoMove();
            undoMove();
            printBitBoard(*current_game);
        }
        else {
            cout << "Too early in the game to undo" << endl;
            return false;
        }
    }

    if (move_from == "O-O" || move_from == "O-O-O") {
        
        Game::chess_bitboard castle_moves[2];
        int c_moves = helperClass::get_all_castle_moves(current_game->bit_board, current_game->getCurrentTurn(), castle_moves);
        cout << "You tried to castle. " << c_moves << endl;
        for (int i = 0; i < c_moves; i++) {
            if (move_from == "O-O") {
                cout << "Castled short succesfully" << endl;
                //white king is on the kingside castle square after castling and it's white's turn
                if ( (castle_moves[i].white_kings == 281474976710656ULL && current_game->getCurrentTurn()) || (castle_moves[i].black_kings == 36028797018963968ULL && !current_game->getCurrentTurn()) ) {//
                    update_current_board(castle_moves[i]);
                    return true;
                }

                
            }
            if (move_from == "O-O-O") {
                if ( (castle_moves[i].white_kings == 65536ULL && current_game->getCurrentTurn()) || (castle_moves[i].black_kings == 8388608ULL && !current_game->getCurrentTurn()) ) {
                    cout << "Castled long succesfully" << endl;
                    update_current_board(castle_moves[i]);
                    return true;
                }
            }
        }
        cout << "OOoopsss could not castle in this position" << endl;
        return false;
    }

    if (move_from.length() > 2 || move_from.length() == 0)
    {
        createNextMessage("You should type only two characters (column and row)\n");
        return success;
    }

    Chess::Position present;
    present.iColumn = move_from[0];
    present.iRow = move_from[1];

    // ---------------------------------------------------
    // Did the user pick a valid piece?
    // Must check if:
    // - It's inside the board (A1-H8)
    // - There is a piece in the square
    // - The piece is consistent with the player's turn
    // ---------------------------------------------------
    present.iColumn = toupper(present.iColumn);

    if (present.iColumn < 'A' || present.iColumn > 'H')
    {
        createNextMessage("Invalid column.\n");
        return success;
    }

    if (present.iRow < '0' || present.iRow > '8')
    {
        createNextMessage("Invalid row.\n");
        return success;
    }

    // Put in the string to be logged
    to_record += present.iColumn;
    to_record += present.iRow;
    to_record += "-";

    // Convert column from ['A'-'H'] to [0x00-0x07]
    present.iColumn = present.iColumn - 'A';

    // Convert row from ['1'-'8'] to [0x00-0x07]
    present.iRow = present.iRow - '1';

    char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
    //current_game->printOutBoard();

    cout << "Piece is " << char(chPiece) << "\n";

    if (0x20 == chPiece)
    {
        createNextMessage("You picked an EMPTY square.\n");
        return success;
    }

    if (Chess::WHITE_PIECE == current_game->getCurrentTurn())
    {
        if (false == Chess::isWhitePiece(chPiece))
        {
            createNextMessage("It is WHITE's turn and you picked a BLACK piece\n");
            return success;
        }
    }
    else
    {
        if (false == Chess::isBlackPiece(chPiece))
        {
            createNextMessage("It is BLACK's turn and you picked a WHITE piece\n");
            return success;
        }
    }

    // ---------------------------------------------------
    // Get user input for the square to move to
    // ---------------------------------------------------
    cout << "Move to: ";
    std::string move_to;
    getline(cin, move_to);

    if (move_to.length() > 2)
    {
        createNextMessage("You should type only two characters (column and row)\n");
        return success;
    }

    // ---------------------------------------------------
    // Did the user pick a valid house to move?
    // Must check if:
    // - It's inside the board (A1-H8)
    // - The move is valid
    // ---------------------------------------------------
    Chess::Position future;
    future.iColumn = move_to[0];
    future.iRow = move_to[1];

    future.iColumn = toupper(future.iColumn);

    if (future.iColumn < 'A' || future.iColumn > 'H')
    {
        createNextMessage("Invalid column.\n");
        return success;
    }

    if (future.iRow < '0' || future.iRow > '8')
    {
        createNextMessage("Invalid row.\n");
        return success;
    }

    // Put in the string to be logged
    to_record += future.iColumn;
    to_record += future.iRow;

    // Convert columns from ['A'-'H'] to [0x00-0x07]
    future.iColumn = future.iColumn - 'A';

    // Convert row from ['1'-'8'] to [0x00-0x07]
    future.iRow = future.iRow - '1';

    // Check if it is not the exact same square
    if (future.iRow == present.iRow && future.iColumn == present.iColumn)
    {
        createNextMessage("[Invalid] You picked the same square!\n");
        return success;
    }


    //check if move is valid. do this by first getting the piece type, then checking if that piece can move to that square
    int start_square = present.iColumn * 8 + present.iRow;
    int target_square = future.iColumn * 8 + future.iRow;
    unsigned long long start_square_l = helperClass::SetNthBit(start_square);
    unsigned long long future_square_l = helperClass::SetNthBit(target_square);
    Game::chess_bitboard future_bitboard = current_game->bit_board;

        if (toupper(chPiece) == 'P') {
            unsigned long long white_pawn_end_row = 9259542123273814144ULL;
            unsigned long long black_pawn_end_row = 72340172838076673ULL;

            if (current_game->getCurrentTurn()) {
                //white to move //clear current pawn position
                future_bitboard.white_pawns = future_bitboard.white_pawns & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_black(future_bitboard, future_square_l);

                future_bitboard.white_queens = future_bitboard.white_queens | (future_bitboard.white_pawns & white_pawn_end_row);//if queen, queen
                future_bitboard.white_pawns = future_bitboard.white_pawns & (~white_pawn_end_row);


            }
            else {
                //black to move//clear current pawn position
                future_bitboard.black_pawns = future_bitboard.black_pawns & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_white(future_bitboard, future_square_l);
            }
            Game::chess_bitboard output_array[32];
            int number_elemts = helperClass::get_all_pawn_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
            Game::chess_bitboard future_moves_pawns[48];
            for (int i = 0; i < number_elemts; i++) {
                future_moves_pawns[i] = output_array[i];
            }
            for (int i = 0; i < number_elemts; i++) {
                Game::chess_bitboard n = future_moves_pawns[i];
            
                if (n == future_bitboard) {
                    success = true;//we want to move the pawn
                }
            }
        }
        if (toupper(chPiece) == 'N') {
            if (current_game->getCurrentTurn()) {
                //white to move //clear current knight position
                future_bitboard.white_knights = future_bitboard.white_knights & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_black(future_bitboard, future_square_l);
            }
            else {
                //black to move//clear current knight position
                future_bitboard.black_knights = future_bitboard.black_knights & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_white(future_bitboard, future_square_l);
            }
            Game::chess_bitboard output_array[32];
            int number_elemts = helperClass::get_all_knight_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
            Game::chess_bitboard future_moves_knights[48];
            for (int i = 0; i < number_elemts; i++) {
                future_moves_knights[i] = output_array[i];
            }
            for (int i = 0; i < number_elemts; i++) {
                Game::chess_bitboard n = future_moves_knights[i];

                if (n == future_bitboard) {
                    success = true;//we want to move the knight
                }
            }
        }
        if (toupper(chPiece) == 'B') {
            if (current_game->getCurrentTurn()) {
                //white to move //clear current bishop position
                future_bitboard.white_bishops = future_bitboard.white_bishops & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_black(future_bitboard, future_square_l);
            }
            else {
                //black to move//clear current bishop position
                future_bitboard.black_bishops = future_bitboard.black_bishops & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_white(future_bitboard, future_square_l);
            }
            Game::chess_bitboard output_array[32];
            int number_elemts = helperClass::get_all_bishop_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
            Game::chess_bitboard future_moves_bishops[48];
            for (int i = 0; i < number_elemts; i++) {
                future_moves_bishops[i] = output_array[i];
            }
            for (int i = 0; i < number_elemts; i++) {
                Game::chess_bitboard n = future_moves_bishops[i];

                if (n == future_bitboard) {
                    success = true;//we want to move the bishop
                }
            }
        }
        if (toupper(chPiece) == 'R') {
            if (current_game->getCurrentTurn()) {
                //white to move //clear current bishop position
                future_bitboard.white_rooks = future_bitboard.white_rooks & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_black(future_bitboard, future_square_l);
            }
            else {
                //black to move//clear current rook position
                future_bitboard.black_rooks = future_bitboard.black_rooks & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_white(future_bitboard, future_square_l);
            }
            Game::chess_bitboard output_array[32];
            int number_elemts = helperClass::get_all_rook_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
            Game::chess_bitboard future_moves_rooks[48];
            for (int i = 0; i < number_elemts; i++) {
                future_moves_rooks[i] = output_array[i];
            }
            for (int i = 0; i < number_elemts; i++) {
                Game::chess_bitboard n = future_moves_rooks[i];

                if (n == future_bitboard) {
                    success = true;//we want to move the rook
                }
            }
        }
        if (toupper(chPiece) == 'Q') {
            if (current_game->getCurrentTurn()) {
                //white to move //clear current rook position
                future_bitboard.white_queens = future_bitboard.white_queens & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_black(future_bitboard, future_square_l);
            }
            else {
                //black to move//clear current queen position
                future_bitboard.black_queens = future_bitboard.black_queens & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_white(future_bitboard, future_square_l);
            }
            Game::chess_bitboard output_array[32];
            int number_elemts = helperClass::get_all_queen_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
            Game::chess_bitboard future_moves_queens[48];
            for (int i = 0; i < number_elemts; i++) {
                future_moves_queens[i] = output_array[i];
            }
            for (int i = 0; i < number_elemts; i++) {
                Game::chess_bitboard n = future_moves_queens[i];

                if (n == future_bitboard) {
                    success = true;//we want to move the queen
                }
            }
        }
        if (toupper(chPiece) == 'K') {
            if (current_game->getCurrentTurn()) {
                //white to move //clear current queen position
                future_bitboard.white_kings = future_bitboard.white_kings & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_black(future_bitboard, future_square_l);
            }
            else {
                //black to move//clear current king position
                future_bitboard.black_kings = future_bitboard.black_kings & (~start_square_l) | future_square_l;
                future_bitboard = helperClass::capture_square_white(future_bitboard, future_square_l);
            }
            Game::chess_bitboard output_array[32];
            int number_elemts = helperClass::get_all_king_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
            Game::chess_bitboard future_moves_kings[48];
            for (int i = 0; i < number_elemts; i++) {
                future_moves_kings[i] = output_array[i];
            }
            for (int i = 0; i < number_elemts; i++) {
                Game::chess_bitboard n = future_moves_kings[i];

                if (n == future_bitboard) {
                    success = true;//we want to move the king
                }
            }
        }
        



    if (success) {

        update_current_board(future_bitboard);
        printBitBoard(*current_game);
        cout << endl << endl;
    }
    else {
        
    cout << "Unfortunately, the move was found to be invalid" << endl;
    }
    return success;


    // Is that move allowed?
    Chess::Promotion  S_promotion = { 0 };

    

    // ---------------------------------------------------
    // Promotion: user most choose a piece to
    // replace the pawn
    // ---------------------------------------------------
    /*
    if (S_promotion.bApplied == true)
    {
        cout << "Promote to (Q, R, N, B): ";
        std::string piece;
        getline(cin, piece);

        if (piece.length() > 1)
        {
            createNextMessage("You should type only one character (Q, R, N or B)\n");
            return success;
        }

        char chPromoted = toupper(piece[0]);

        if (chPromoted != 'Q' && chPromoted != 'R' && chPromoted != 'N' && chPromoted != 'B')
        {
            createNextMessage("Invalid character.\n");
            return success;
        }

        S_promotion.chBefore = current_game->getPieceAtPosition(present.iRow, present.iColumn);

        if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
        {
            S_promotion.chAfter = toupper(chPromoted);
        }
        else
        {
            S_promotion.chAfter = tolower(chPromoted);
        }

        to_record += '=';
        to_record += toupper(chPromoted); // always log with a capital letter
    }
    */

    // ---------------------------------------------------
    // Log the move: do it prior to making the move
    // because we need the getCurrentTurn()
    // ---------------------------------------------------
    // current_game->logMove(to_record);

}


int main()
{
    bool bRun = true;

    // Clear screen an print the logo
    clearScreen();
    printLogo();

    newGame();
    //cout << current_game->bit_board.black_pawns << endl;
    printBitBoard(*current_game);
    cout << "Useful commands: (Q - quit), (U - Undo), (O-O - short castles), (O-O-O - long castles)" << endl;

    string input = "";
    while (true){
        
        if(movePieceMain()){
        clearScreen();
        printBitBoard(*current_game);
        cout << "Computer thinking" << endl;
        auto t1 = chrono::high_resolution_clock::now();
        computerPlayMove(depth, -1000, 1000, current_game->getCurrentTurn());
        auto t2 = chrono::high_resolution_clock::now();
        clearScreen();
        printBitBoard(*current_game);
        cout << "Computer thought for: " << duration_cast<chrono::seconds>(t2-t1) << endl;
        cout << "Current minimax value: " << current_minimax << endl;
        cout << "Computer looked at " << total_moves << " moves" << endl;
        total_moves = 0;
        }

    }


   /*
   while( bRun )
   {
      printMessage();
      printMenu();

      // Get input from user
      cout << "Type here: ";
      getline(cin, input);

      if (input.length() != 1)
      {
         cout << "Invalid option. Type one letter only\n\n";
         continue;
      }

      try
      {
         switch (input[0])
         {
            case 'N':
            case 'n':
            {
               newGame();
               clearScreen();
               printLogo();
               printSituation(*current_game);
               printBoard(*current_game);
               cout << "Playing as white vs computer at depth: " << depth << ". TODO add option to play as black" << endl;
            }
            break;

            case 'M':
            case 'm':
            {
               if (NULL != current_game)
               {
                  if ( current_game->isFinished() )
                  {
                     cout << "This game has already finished!\n";
                  }
                  else
                  {
                      bool move_succesful = movePieceMain();
                      clearScreen();
                      printSituation(*current_game);
                      printBoard(*current_game);


                      if (false == current_game->isCheckMate() && move_succesful) {
                          total_moves = 0;
                          computer_play_move(current_game->getCurrentTurn());
                          cout << "Computer looked at " << total_moves << " moves" << endl;
                      }
                         
                  }
               }
               else
               {
                  cout << "No game running!\n";
               }
               
            }
            break;

            case 'Q':
            case 'q':
            {
               bRun = false;
            }
            break;

            case 'U':
            case 'u':
            {
               if (NULL != current_game)
               {
                   undoMoveComputer();
                   undoMoveComputer();
                  //clearScreen();
                  printLogo();
                  printSituation(*current_game);
                  printBoard(*current_game);
               }
               else
               {
                  cout << "No game running\n";
               }
            }
            break;

            case 'S':
            case 's':
            {
               if (NULL != current_game)
               {
                  saveGame();
                  clearScreen();
                  printLogo();
                  printSituation(*current_game);
                  printBoard(*current_game);
               }
               else
               {
                  cout << "No game running\n";
               }
            }
            break;

            case 'L':
            case 'l':
            {
               loadGame();
               clearScreen();
               printLogo();
               printSituation(*current_game);
               printBoard(*current_game);
            }
            break;

            default:
            {
               cout << "Option does not exist\n\n";
            }
            break;

         }

      }
      catch (const char* s)
      {
         s;
      }
   }

   */
   return 0;
}
