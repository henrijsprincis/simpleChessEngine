#pragma once
#include <vector>
#include "chess.h"
//#include <map>

class helperClass
{
public:
	static Game::chess_bitboard capture_square_black(Game::chess_bitboard current_board, unsigned long long square);
	static Game::chess_bitboard capture_square_white(Game::chess_bitboard current_board, unsigned long long square);
	static std::vector<std::vector<int>> getPiecesPosition(std::vector<std::vector<char>> board, int white);
	static std::vector<std::vector<int>> get_Moves_Piece(char piece_type, int pos_x, int pos_y);
	static int* get_Moves_Piece_INTARR(char piece_type, int pos_x, int pos_y, int* moves, int idx);

	static int getValueBitboard(Game::chess_bitboard c);


	//bitboard stuff
	static void print_vector(vector<vector<int>> vect);
	static vector<vector<int>> bitboard_to_vector(unsigned long long bitboard);
	static bool IsBitSet(unsigned long long num, int bit);
	static unsigned long long SetNthBit(int bit);
	static unsigned long long vector_to_bitboard(vector<vector<int>> vector);
	static unsigned long GetFirstSetBit(unsigned long long num);

	//moves general
	static unsigned long long path_horizontal(unsigned long long current_pos, Game::chess_bitboard current_board, int white);
	static unsigned long long path_vertical(unsigned long long current_pos, Game::chess_bitboard current_board, int white);
	static unsigned long long path_diognal(unsigned long long current_pos, Game::chess_bitboard current_board, int white);
	static unsigned long long path_diognal_one(unsigned long long current_pos, bool is_king);
	static unsigned long long path_L(unsigned long long current_pos);
	
	//moves specific
	static unsigned long long pawn_forward(unsigned long long current_pos, int amount_forward);
	static unsigned long long king_forward(unsigned long long current_pos, int amount_forward);
	static unsigned long long pawn_sideways(unsigned long long current_pos, int amount_sideways);
	static int get_all_pawn_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]);
	static int get_all_knight_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]);
	static int get_all_bishop_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]);
	static int get_all_rook_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]);
	static int get_all_queen_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]);
	static int get_all_king_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[8]);
	static int get_all_castle_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[2]);

	//hash function
	static unsigned long long hash_board(Game::chess_bitboard board, unsigned long long zobrist_numbers[64 * 12]);
	static unsigned long long hash_board(Game::chess_bitboard board_previous, Game::chess_bitboard board_current, unsigned long long hash, unsigned long long zobrist_numbers[64 * 12 + 1]);
	
	struct hash_type {
		int board_value;
		int depth_evaluated_at;
		unsigned long long exact_hash;
		//int alpha;
		//int beta;
		//Game::chess_bitboard exact_pos;
	};


	static unsigned long long get_random_num();
};