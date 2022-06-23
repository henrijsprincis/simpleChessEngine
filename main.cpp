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
#include <ctime>

//---------------------------------------------------------------------------------------
// Global variable
//---------------------------------------------------------------------------------------
Game* current_game = NULL;
char chess_cols[9] = "ABCDEFGH";
char chess_rows[9] = "12345678";
int total_moves = 0;

//hashing
int hash_table_size = 10000000;// 100 000 00 (90mb)
unsigned long long no_element = 123456789ULL;
helperClass::hash_type* hash_table = new helperClass::hash_type[hash_table_size];
unsigned long long random_num_table[64*12+1];//64 squares * 6 pieces * 2 colours :) + white to move.
int hash_collisions = 0;

//minimax
int depth_minimax = 8;
int capture_depth = 0;// after normal depth finished, the computer will return minimax of captures.
int current_minimax = 0;
int STOPPEDEARLY = 1234;

//Timing. we want to cut minimax search short sometimes
auto t1 = chrono::high_resolution_clock::now();
auto t2 = chrono::high_resolution_clock::now();
int max_time_think_pc = 1;// in seconds

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
    current_game->bit_board.white = !current_game->bit_board.white;
    current_game->position_array[current_game->current_move] = current_game->bit_board;

}

int minimaxABCaptures(int depth, int alpha, int beta, int white) {
    total_moves++;
    //calculate all captures until a certain depth.
        
    
    //current_game->vector_board = current_game->bitboardToChar();
    //printBitBoard(*current_game);
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //clearScreen();

    if (current_game->isCheckMate()) {
        return popcount(current_game->bit_board.white_kings) * 1000 - popcount(current_game->bit_board.black_kings) * 1000;

    }
    if (depth == 0) {
        //return heuristic evaluation function
        return helperClass::getValueBitboard(current_game->bit_board);
        //return helperClass::getValueBitboard(current_game->bit_board);
    }
    //move stuff
    int number_elemts = 0;
    int number_of_pieces = Game::getNumberPieces(current_game->bit_board);
    int best_move_value = helperClass::getValueBitboard(current_game->bit_board);//captures can only improve the current value

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
            if (number_of_pieces > Game::getNumberPieces(current_game->bit_board)){
                if (white) {
                    best_move_value = std::max(best_move_value, minimaxABCaptures(depth - 1, alpha, beta, 0));
                    if (best_move_value >= beta) {
                        undoMove();
                        return best_move_value;
                    }
                    alpha = std::max(alpha, best_move_value);
                }
                else {
                    best_move_value = std::min(best_move_value, minimaxABCaptures(depth - 1, alpha, beta, 1));
                    if (best_move_value <= alpha) {
                        undoMove();
                        return best_move_value;
                    }
                    beta = std::min(beta, best_move_value);
                }
            }
            undoMove();
        }
    }

    return best_move_value;
    
}

int minimaxAB(int depth, int alpha, int beta, int white, unsigned long long previous_hash, Game::chess_bitboard previous_board) {
    total_moves++;
    Game::chess_bitboard current_board = current_game->bit_board;
    unsigned long long current_hash = helperClass::hash_board(previous_board, current_board, previous_hash, random_num_table);
    int hash_index = current_hash % hash_table_size;
    if (hash_table[hash_index].board_value == no_element || hash_table[hash_index].exact_hash != current_hash || hash_table[hash_index].depth_evaluated_at < depth)
    {
        if (current_game->isCheckMate()) {
            if (!current_game->bit_board.white) {//white to move but mate
                return 1000;
            }
            else {
                return -1000;//return popcount(current_game->bit_board.white_kings) * 1000 - popcount(current_game->bit_board.black_kings) * 1000;
            }
        }
        if (depth == 0) {
            //return heuristic evaluation function
            //return minimaxABCaptures(capture_depth, alpha, beta, white);
            return helperClass::getValueBitboard(current_game->bit_board);
        }
    //move stuff
    int best_move_value = 1000;
    if (white) {
        best_move_value = -1000;
    }
        Game::chess_bitboard output_array[48];
        for (int c = 0; c < 7; c++) {
            int number_elemts = 0;
            switch (c) {
                case 0://pawn
                    //if(white && current_game->bit_board.white_pawns > 0ULL  || !white && current_game->bit_board.black_pawns > 0ULL)
                    number_elemts = helperClass::get_all_pawn_moves(current_game->bit_board, white, output_array);
                    break;
                case 1:
                    //if (white && current_game->bit_board.white_knights > 0ULL || !white && current_game->bit_board.black_knights > 0ULL)
                    number_elemts = helperClass::get_all_knight_moves(current_game->bit_board, white, output_array);
                    break;
                case 2:
                    //if (white && current_game->bit_board.white_bishops > 0ULL || !white && current_game->bit_board.black_bishops > 0ULL)
                    number_elemts = helperClass::get_all_bishop_moves(current_game->bit_board, white, output_array);
                    break;
                case 3:
                    //if (white && current_game->bit_board.white_rooks > 0ULL || !white && current_game->bit_board.black_rooks > 0ULL)
                    number_elemts = helperClass::get_all_rook_moves(current_game->bit_board, white, output_array);
                    break;
                case 4:
                    //if (white && current_game->bit_board.white_queens > 0ULL || !white && current_game->bit_board.black_queens > 0ULL)
                    number_elemts = helperClass::get_all_queen_moves(current_game->bit_board, white, output_array);
                    break;
                case 5:
                    number_elemts = helperClass::get_all_king_moves(current_game->bit_board, white, output_array);
                    break;
                case 6:
                    //if ( (white && current_game->bit_board.white_castle_king || current_game->bit_board.white_castle_king) || (!white && current_game->bit_board.black_castle_king || current_game->bit_board.black_castle_queen) )
                    number_elemts = helperClass::get_all_castle_moves(current_game->bit_board, white, output_array);
                    break;
                }
            for (int i = 0; i < number_elemts; i++) {
                update_current_board(output_array[i]);
                if (white) {
                    best_move_value = std::max(best_move_value, minimaxAB(depth - 1, alpha, beta, 0, current_hash, current_board));
                    if (best_move_value >= beta) {
                        undoMove();
                        return best_move_value;
                    }
                    alpha = std::max(alpha, best_move_value);
                }
                else {
                    best_move_value = std::min(best_move_value, minimaxAB(depth - 1, alpha, beta, 1, current_hash, current_board));
                    if (best_move_value <= alpha) {
                        undoMove();
                        return best_move_value;
                    }
                    beta = std::min(beta, best_move_value);
                }
                undoMove();
            }
        }
    
        hash_table[hash_index].board_value = best_move_value;
        hash_table[hash_index].depth_evaluated_at = depth;
        hash_table[hash_index].exact_hash = current_hash;
        return best_move_value;
    }
    else
    {
        hash_collisions += 1;
        return hash_table[hash_index].board_value;//move in hash table
    }
}

Game::chess_bitboard computerGetMove(int depth, int alpha, int beta, int white, unsigned long long previous_hash, Game::chess_bitboard previous_board) {
    total_moves++;
    if (current_game->isCheckMate()) {
        if (!current_game->bit_board.white) {//white to move but mate
            cout << "CHECKMATE WHITE WINS!" << endl;
            return previous_board;
        }
        else {
            cout << "CHECKMATE BLACK WINS!" << endl;
            return previous_board;
        }
    }
    if (depth < 1) {
        cout << "Lowest depth = 1. Setting depth to 1!" << endl;
        depth = 1;
    }
    //move stuff
    int number_elemts = 0;
    int best_move_value = 1000;
    if (white) {
        best_move_value = -1000;
    }
    Game::chess_bitboard current_board = current_game->bit_board;
    unsigned long long current_hash = helperClass::hash_board(previous_board, current_board, previous_hash, random_num_table);
    int hash_index = current_hash % hash_table_size;
    Game::chess_bitboard best_move;
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
            t2 = chrono::high_resolution_clock::now();
            if (duration_cast<chrono::seconds>(t2 - t1) > std::chrono::seconds{ max_time_think_pc }) {
                
                return output_array[0];//chance of having the final evaluation finish is basically zero.
                //This means we can always take the previously best move as the best move :).
            }
            update_current_board(output_array[i]);
            if (white) {
                int board_value = minimaxAB(depth - 1, alpha, beta, 0, current_hash, current_board);
                if (board_value > best_move_value)
                    best_move = output_array[i];
                best_move_value = std::max(best_move_value, board_value);
                if (best_move_value >= beta) {
                    undoMove();
                    break;
                }
                alpha = std::max(alpha, best_move_value);
            }
            else {
                int board_value = minimaxAB(depth - 1, alpha, beta, 1, current_hash, current_board);
                if (board_value < best_move_value)
                    best_move = output_array[i];
                best_move_value = std::min(best_move_value, board_value);
                if (best_move_value <= alpha) {
                    undoMove();
                    break;
                }
                beta = std::min(beta, best_move_value);
            }
            undoMove();

        }
    }
    current_minimax = best_move_value;
    //update_current_board(best_move);
    return best_move;
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
    Game::chess_bitboard output_array[32];
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
            future_bitboard.black_queens = future_bitboard.black_queens | (future_bitboard.black_pawns & black_pawn_end_row);//if queen, queen
            future_bitboard.black_pawns = future_bitboard.black_pawns & (~black_pawn_end_row);
        }
        int number_elemts = helperClass::get_all_pawn_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
        for (int i = 0; i < number_elemts; i++) {
            if (output_array[i] == future_bitboard) {
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
        int number_elemts = helperClass::get_all_knight_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
        for (int i = 0; i < number_elemts; i++) {
            if (output_array[i] == future_bitboard) {
                success = true;//we want to move the pawn
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
        int number_elemts = helperClass::get_all_bishop_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
        for (int i = 0; i < number_elemts; i++) {
            if (output_array[i] == future_bitboard) {
                success = true;//we want to move the pawn
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
        int number_elemts = helperClass::get_all_rook_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
        for (int i = 0; i < number_elemts; i++) {
            if (output_array[i] == future_bitboard) {
                success = true;//we want to move the pawn
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
        int number_elemts = helperClass::get_all_queen_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
        for (int i = 0; i < number_elemts; i++) {
            if (output_array[i] == future_bitboard) {
                success = true;//we want to move the pawn
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
        int number_elemts = helperClass::get_all_king_moves(current_game->bit_board, current_game->getCurrentTurn(), output_array);
        for (int i = 0; i < number_elemts; i++) {
            if (output_array[i] == future_bitboard) {
                success = true;//we want to move the pawn
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
}

void reset_hash_table() {
    for (int i = 0; i < hash_table_size; i++) {
        hash_table[i].board_value = no_element;
        //Game::chess_bitboard b;
        //hash_table[i].exact_pos = b;
        //hash_table[i].exact_hash = 0ULL;

    }
}

void initilize_hash_table() {
    reset_hash_table();
    for (int piece = 0; piece < 12; piece++) {//zobrist hash table
        for (int square = 0; square < 64; square++) {
            //srand(time(0));
            random_num_table[64 * piece + square] = helperClass::get_random_num();//~0ULL;//
        }
    }
    random_num_table[64 * 12] = helperClass::get_random_num();
}

int main()
{
    bool bRun = true;

    // Clear screen an print the logo
    clearScreen();
    printLogo();
    initilize_hash_table();
    newGame();
    //cout << current_game->bit_board.black_pawns << endl;
    printBitBoard(*current_game);
    cout << "Useful commands: (Q - quit), (U - Undo), (O-O - short castles), (O-O-O - long castles)" << endl;

    string input = "";
    cout << "PC will think for more than: " << max_time_think_pc << " seconds" << endl;
    bool checkmate_found = false;
    while (true){
        
        if(movePieceMain()){
            clearScreen();
            printBitBoard(*current_game);
            cout << "Computer thinking" << endl;
            t1 = chrono::high_resolution_clock::now();// start timer
            int i = 1;
            unsigned long long starting_hash = helperClass::hash_board(current_game->bit_board, random_num_table);
            Game::chess_bitboard previous_best_move = computerGetMove(1, -2000, 2000, current_game->getCurrentTurn(), starting_hash, current_game->bit_board);// this move will come from a good evaluation.
            Game::chess_bitboard current_best_move;// this move will come from incomplete evaluation that returned early because time expired.
            

            for (i = 2; i < 20; i++) {
                if (checkmate_found) {
                    reset_hash_table();//reset the hash table. This avoids ai being stuck. this is terrible terrible fix, but it works.
                    checkmate_found = false;
                }
                current_best_move = computerGetMove(i, -2000, 2000, current_game->getCurrentTurn(), starting_hash, current_game->bit_board);
                if (abs(current_minimax) > 500) {
                    cout << "computer found checkmate in: " << i << " moves.";
                    checkmate_found = true;
                    break;
                }
                if (duration_cast<chrono::seconds>(t2 - t1) > std::chrono::seconds{ max_time_think_pc }) {
                    //if time expired, make current best move previous best move & break
                    current_best_move = previous_best_move;
                    break;
                }
                else {
                    previous_best_move = current_best_move;
                }
            }
            update_current_board(current_best_move);

            //reset_hash_table();
            
            clearScreen();
            printBitBoard(*current_game);
            cout << "Computer thought for: " << duration_cast<chrono::seconds>(t2-t1) << ". Depth: " << i - 1 << endl;
            cout << "Position value: " << current_minimax << endl;
            cout << "Computer looked at " << total_moves << " moves" << endl;
            cout << "Hash collisions or transpositions: " << hash_collisions << endl;
            total_moves = 0;
            hash_collisions = 0;
            
            
        }

    }

    return 0;
}
