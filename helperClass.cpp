#include "chess.h"
#include "helperClass.h"
#include <vector>
#include <iostream>
#define NULL 69


unsigned long long white_pawn_end_row = 9259542123273814144ULL;
unsigned long long black_pawn_end_row = 72340172838076673ULL;
//using namespace std;

std::vector<std::vector<int>> helperClass::getPiecesPosition(std::vector<std::vector<char>> board, int white) {
	//white == 1 then white. white == 0, then black


	std::vector<std::vector<int>> getPiecesPositionWhite(16);
	std::vector<std::vector<int>> getPiecesPositionBlack(16);

	int white_counter = 0;
	int black_counter = 0;
	for (int y = 0; y < board.size(); y++) { //board_vector[Height/Row][Width/Col]
		for (int x = 0; x < board[y].size(); x++) {
			char ch = board[y][x];

			if (isalpha(ch)){
				std::vector<int> square = { x,y };
				if (isupper(ch) ) {//white == 
					getPiecesPositionWhite[white_counter] = square;
					white_counter++;
				}
				else {
					getPiecesPositionBlack[black_counter] = square;
					black_counter++;
				}
				
			}

		}
	}
	if (white) {
		getPiecesPositionWhite.resize(white_counter);
		return getPiecesPositionWhite;
	}
	else {
		getPiecesPositionBlack.resize(black_counter);
		return getPiecesPositionBlack;
	}
	

}


std::vector<std::vector<int>> helperClass::get_Moves_Piece(char piece_type, int pos_x, int pos_y) {
	//std::cout << "piece type : " << piece_type << std::endl;
	std::vector<std::vector<int>> moves;

	if (piece_type == 'P') {
		moves = { {pos_x,pos_y+1}, {pos_x,pos_y + 2}, {pos_x + 1, pos_y + 1}, {pos_x - 1, pos_y + 1} };//1 up, 2 up, capture left, capture right
	}
	if (piece_type == 'p') {
		moves = { {pos_x,pos_y - 1}, {pos_x,pos_y - 2}, {pos_x + 1, pos_y - 1}, {pos_x - 1, pos_y - 1} };//1 down, 2 down, capture left, capture right
	}

	if (piece_type == 'N' || piece_type == 'n') {
		moves = { {pos_x + 1, pos_y + 2}, {pos_x - 1, pos_y + 2}, {pos_x + 1, pos_y - 2}, {pos_x - 1, pos_y - 2}, //up down
		{pos_x - 2, pos_y + 1}, {pos_x - 2, pos_y - 1}, {pos_x + 2, pos_y + 1}, {pos_x + 2, pos_y - 1}//left, right
		};
	}

	if (piece_type == 'K' || piece_type == 'k') {
		moves = { {pos_x - 1, pos_y + 1}, {pos_x , pos_y + 1}, {pos_x + 1, pos_y + 1},
			{pos_x - 1, pos_y}, {pos_x + 1, pos_y},
			{pos_x - 1, pos_y - 1}, {pos_x , pos_y - 1}, {pos_x + 1, pos_y - 1}
		};
	}
	std::vector<int> m;
	int min_pos;
	if (piece_type == 'B' || piece_type == 'b') {
		//up right -- needs work, but not now
		//min_pos = std::min(pos_x, pos_y);

		for (int x = -8; x < 8; x++) {
			m = { pos_x + x, pos_y + x };
			moves.insert( moves.begin(), m);
			m = { pos_x + x, pos_y - x };
			moves.insert(moves.begin(), m);
		}
	}

	if (piece_type == 'R' || piece_type == 'r') {
		//up right
		for (int x = 0; x < 8; x++) {
			//rook
			m = { x, pos_y };
			moves.insert(moves.begin(), m);
			m = { pos_x, x };
			moves.insert(moves.begin(), m);
		}
	}

	if (piece_type == 'Q' || piece_type == 'q') {
		//up right
		for (int x = -8; x < 8; x++) {
			//bishop
			m = { pos_x + x, pos_y + x };
			moves.insert(moves.begin(), m);
			m = { pos_x + x, pos_y - x };
			moves.insert(moves.begin(), m);
		}
		for (int x = 0; x < 8; x++) {
			//rook
			m = { x, pos_y };
			moves.insert(moves.begin(), m);
			m = { pos_x, x };
			moves.insert(moves.begin(), m);
		}
	}




	for (int x = moves.size()-1; x > 0; x--) {
		for (int y = 0; y < 2; y++) {
			if (x < moves.size()) {
				if (moves[x][y] > 7 || moves[x][y] < 0) {
					moves.erase(moves.begin() + x);
				}
			}
		}
	}
	return moves;
			

}

int* helperClass::get_Moves_Piece_INTARR(char piece_type, int pos_x, int pos_y, int* moves, int idx ) {

	//int* moves = new int[65];//first index contains how many moves in array;
	//int idx = 1;
	if (piece_type == 'P') {
		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 2;
		idx++;

		moves[idx] = pos_x + 1;//{pos_x + 1, pos_y + 1}
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x - 1;//pos_x - 1, pos_y + 1
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

	}
	else if (piece_type == 'p') {
		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 2;
		idx++;

		moves[idx] = pos_x + 1;//{pos_x + 1, pos_y + 1}
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x - 1;//pos_x - 1, pos_y + 1
		idx++;
		moves[idx] = pos_y + 1;
		idx++;
	}

	else if (piece_type == 'N' || piece_type == 'n') {
		moves[idx] = pos_x + 1;
		idx++;
		moves[idx] = pos_y + 2;
		idx++;

		moves[idx] = pos_x - 1;
		idx++;
		moves[idx] = pos_y + 2;
		idx++;

		moves[idx] = pos_x + 1;
		idx++;
		moves[idx] = pos_y - 2;
		idx++;

		moves[idx] = pos_x - 1;
		idx++;
		moves[idx] = pos_y - 2;
		idx++;

		moves[idx] = pos_x - 2;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x - 2;
		idx++;
		moves[idx] = pos_y - 1;
		idx++;

		moves[idx] = pos_x + 2;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x + 2;
		idx++;
		moves[idx] = pos_y - 1;
		idx++;

		
	}

	else if (piece_type == 'K' || piece_type == 'k') {
		moves[idx] = pos_x - 1;
		idx++;
		moves[idx] = pos_y - 1;
		idx++;

		moves[idx] = pos_x - 1;
		idx++;
		moves[idx] = pos_y;
		idx++;

		moves[idx] = pos_x - 1;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y - 1;
		idx++;

		//moves[idx] = pos_x;
		//idx++;
		//moves[idx] = pos_y;
		//idx++;

		moves[idx] = pos_x + 1;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x + 1;
		idx++;
		moves[idx] = pos_y - 1;
		idx++;

		moves[idx] = pos_x + 1;
		idx++;
		moves[idx] = pos_y;
		idx++;

		moves[idx] = pos_x + 1;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

	}
	

	else if (piece_type == 'B' || piece_type == 'b') {
		for (int x = -8; x < 8; x++) {
			//check move is within boudns.
			if (std::max(pos_x + x, pos_y + x) < 8 && std::min(pos_x + x, pos_y + x) >= 0){
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y + x;
				idx++;
			}
			if (std::max(pos_x + x, pos_y - x) < 8 && std::min(pos_x + x, pos_y - x) >= 0) {
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y - x;
				idx++;
			}
		}
	}

	else if (piece_type == 'R' || piece_type == 'r') {
		//up right
		for (int x = 0; x < 8; x++) {
			//rook
			moves[idx] = x;
			idx++;
			moves[idx] = pos_y;
			idx++;

			moves[idx] = pos_x;
			idx++;
			moves[idx] = x;
			idx++;
		}
	}

	else if (piece_type == 'Q' || piece_type == 'q') {
		//bishop
		for (int x = -8; x < 8; x++) {
			//check move is within boudns.
			if (std::max(pos_x + x, pos_y + x) < 8 && std::min(pos_x + x, pos_y + x) >= 0) {
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y + x;
				idx++;
			}
			if (std::max(pos_x + x, pos_y - x) < 8 && std::min(pos_x + x, pos_y - x) >= 0) {
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y - x;
				idx++;
			}
		}
		//rook
		for (int x = 0; x < 8; x++) {
			//rook
			moves[idx] = x;
			idx++;
			moves[idx] = pos_y;
			idx++;
			moves[idx] = pos_x;
			idx++;
			moves[idx] = x;
			idx++;
		}
	}
	//cout << "INDEX" << idx;
	moves[0] = idx;
	//cout << "moves[0]: " << moves[0];

	
	return moves;
}





/* BITBOARD STUFF*/


bool helperClass::IsBitSet(unsigned long long num, int bit)
{
	return 1 == ((num >> bit) & 1);
}

unsigned long long helperClass::SetNthBit(int bit)
{
	unsigned long long num = 0ULL;
	return num |= 1ULL << bit;
}

vector<vector<int>> helperClass::bitboard_to_vector(unsigned long long bitboard) {
	//function that prints a bitboard as an array
	vector<int> width = vector<int>(8, 0);
	vector<vector<int>> vector(8, width);

	int iter;
	for (int col = 0; col < 8; col++) {
		for (int row = 0; row < 8; row++) {
			iter = col * 8 + row;
			vector[col][row] = 0;
			if (helperClass::IsBitSet(bitboard, iter)) {
				vector[col][row] = 1;
			}
		}
	}
	return vector;
}

unsigned long long helperClass::vector_to_bitboard(vector<vector<int>> vector) {
	//function that turns a 2d vector into good stuff
	unsigned long long num = 0ULL;
	int iter;
	for (int col = 0; col < 8; col++) {
		for (int row = 0; row < 8; row++) {
			iter = col * 8 + row;
			if (vector[col][row] == 1) {
				
				num = num | SetNthBit(iter);
				//cout << iter <<  "   number: " << num << endl;
			}
		}
	}
	return num;
}

void helperClass::print_vector(vector<vector<int>> vect) {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			cout << vect[col][row] << " ";
		}
		cout << endl;
	}
}

unsigned long helperClass::GetFirstSetBit(unsigned long long num) {
	//returns the first bit set of a long
	unsigned long bit_pos = NULL;
	bool success = _BitScanForward64(&bit_pos, num);
	return success ? bit_pos : NULL;
}


/* generate moves*/
/* pawns */
unsigned long long helperClass::pawn_forward(unsigned long long current_pos, int amount_forward) {
	//helperClass::pawn_forward(simple, 1) moves white pawns forward by one square
	unsigned long long l = current_pos;
	if (amount_forward > 0)
		l = l << amount_forward;
	else
		l = l >> abs(amount_forward);
	return l;
}
unsigned long long helperClass::king_forward(unsigned long long current_pos, int amount_forward) {
	//moves king forward or backwards & checks if is at edge of board.
	unsigned long long l = current_pos;
	int bit = helperClass::GetFirstSetBit(current_pos);

	if (bit % 8 != 7 && amount_forward > 0) { // when we are at top of board, we do not want to go higher.
		l = l << amount_forward;
	}
	if (bit % 8 != 0 && amount_forward < 0) { // when we are at bottom of board, we do not want to go lower.
		l = l >> abs(amount_forward);
	}
	return l;
}

unsigned long long helperClass::pawn_sideways(unsigned long long current_pos, int amount_sideways) {
	//ONLY USED FOR THE KING! CAREFUL
	unsigned long long l = 0ULL;
	l = current_pos;
	int bit = helperClass::GetFirstSetBit(current_pos);
	int col = bit / 8;
	int row = bit % 8;
	if (col == 7 && amount_sideways == 1)
		return current_pos;
	if (col == 0 && amount_sideways == -1)
		return current_pos;

	if (amount_sideways > 0)
		l = l << amount_sideways * 8;
	else
		l = l >> abs(amount_sideways) * 8;
	return l;
}
/* rooks */
unsigned long long helperClass::path_diognal_one(unsigned long long current_pos, bool white_pawn) {
	//if white pawn we can go forwards. if black pawn we can go backwards. if king we or both bitmaps.
	unsigned long long l = current_pos;
	unsigned long long l_1 = 0ULL;
	unsigned long long l_2 = 0ULL;
	unsigned long long l_3 = 0ULL;
	unsigned long long l_4 = 0ULL;
	unsigned long bit;
	//get the row
	bit = helperClass::GetFirstSetBit(current_pos);
	if (bit != NULL) {
		int col = bit / 8;
		int row = bit % 8;
		int col_c;
		int row_c;

		//up and right
		//stop whenever row or col reachers 8
		col_c = col;
		row_c = row;
		if (col_c < 7 && row_c < 7 && (white_pawn) ) {
			l |= l << 9;
			col_c++;
			row_c++;
		}
		l_1 = l;
		l = current_pos;


		col_c = col;
		row_c = row;
		//down and left
		if (col_c > 0 && row_c > 0 && (!white_pawn)) {

			l |= l >> 9;
			col_c--;
			row_c--;
		}
		l_2 = l;
		l = current_pos;

		//down and right
		col_c = col;
		row_c = row;
		if (col_c < 7 && row_c > 0 && (!white_pawn) ) {
			l |= l << 7;
			col_c++;
			row_c--;
		}
		l_3 = l;
		l = current_pos;

		//up and left
		col_c = col;
		row_c = row;
		if (col_c > 0 && row_c < 7 && (white_pawn)) {
			l |= l >> 7;
			col_c--;
			row_c++;
		}
		l_4 = l;
		return l_1 | l_2 | l_3 | l_4;

	}
	else {
		throw ("Error, there is no piece");
	}

}

unsigned long long helperClass::path_L(unsigned long long current_pos) {
	//returns knight move

	unsigned long long l = current_pos;
	unsigned long long l_1 = current_pos;
	unsigned long long l_2 = current_pos;
	unsigned long long l_3 = current_pos;
	unsigned long long l_4 = current_pos;
	unsigned long long l_5 = current_pos;
	unsigned long long l_6 = current_pos;
	unsigned long long l_7 = current_pos;
	unsigned long long l_8 = current_pos;
	unsigned long bit;
	//get the row
	bit = helperClass::GetFirstSetBit(current_pos);
	if (bit != NULL) {
		int col = bit / 8;
		int row = bit % 8;

		//up and right
		if (col + 2 < 8 && row + 1 < 8) {
			l_1 = l_1 << (8 + 8 + 1);
		}
		//up and left
		if (col + 2 < 8 && row - 1 > -1) {
			l_2 = l_2 << (8 + 8 - 1);
		}

		//right and up
		if (col + 1 < 8 && row + 2 < 8) {
			l_3 = l_3 << (8 + 1 + 1);
		}

		//left and up 
		if (col + 1 < 8 && row - 2 > -1) {
			l_4 = l_4 << (8 - 1 - 1);
		}


		//down and right
		if (col - 2 > -1 && row + 1 < 8) {
			l_5 = l_5 >> abs(-8 - 8 + 1);
		}
		//down and left
		if (col - 2 > -1 && row - 1 > -1) {
			l_6 = l_6 >> abs(-8 - 8 - 1);
		}

		//right and down
		if (col - 1 > -1 && row + 2 < 8) {
			l_7 = l_7 >> abs(-8 + 1 + 1);
		}

		//left and down 
		if (col - 1 > -1 && row - 2 > -1) {
			l_8 = l_8 >> abs(-8 - 1 - 1);
		}

		
		return (l_1 | l_2 | l_3 | l_4 | l_5 | l_6 | l_7 | l_8) ^ current_pos;

	}
	else {
		throw ("Error, there is no piece");
	}

}
bool is_square_occupied_white(Game::chess_bitboard current_board, unsigned long long square) {
	//returns true when a white piece occupies a square
	return (current_board.white_pawns | current_board.white_knights | current_board.white_bishops | current_board.white_rooks | current_board.white_queens | current_board.white_kings) & square;
}
Game::chess_bitboard helperClass::capture_square_white(Game::chess_bitboard current_board, unsigned long long square) {
	Game::chess_bitboard after_capture = current_board;

	if (current_board.white_pawns & square) {
		after_capture.white_pawns = current_board.white_pawns & (~square);
	}

	if (current_board.white_knights & square) {
		after_capture.white_knights = current_board.white_knights & (~square);
	}

	if (current_board.white_bishops & square) {
		after_capture.white_bishops = current_board.white_bishops & (~square);
	}

	if (current_board.white_rooks & square) {
		after_capture.white_rooks = current_board.white_rooks & (~square);
	}

	if (current_board.white_queens & square) {
		after_capture.white_queens = current_board.white_queens & (~square);
	}

	if (current_board.white_kings & square) {
		after_capture.white_kings = current_board.white_kings & (~square);
	}

	return after_capture;
}
bool is_square_occupied_black(Game::chess_bitboard current_board, unsigned long long square) {
	//returns true when a black piece occupies a square
	return (current_board.black_pawns | current_board.black_knights | current_board.black_bishops | current_board.black_rooks | current_board.black_queens | current_board.black_kings) & square;
}
Game::chess_bitboard helperClass::capture_square_black(Game::chess_bitboard current_board, unsigned long long square) {
	//destroys black pieces on that square
	Game::chess_bitboard after_capture = current_board;

	if (current_board.black_pawns & square) {
		after_capture.black_pawns = current_board.black_pawns & (~square);
	}

	if (current_board.black_knights & square) {
		after_capture.black_knights = current_board.black_knights & (~square);
	}

	if (current_board.black_bishops & square) {
		after_capture.black_bishops = current_board.black_bishops & (~square);
	}

	if (current_board.black_rooks & square) {
		after_capture.black_rooks = current_board.black_rooks & (~square);
	}

	if (current_board.black_queens & square) {
		after_capture.black_queens = current_board.black_queens & (~square);
	}

	if (current_board.black_kings & square) {
		after_capture.black_kings = current_board.black_kings & (~square);
	}

	return after_capture;
}
bool is_square_occupied(Game::chess_bitboard current_board, unsigned long long square) {
	return (is_square_occupied_black(current_board, square) || is_square_occupied_white(current_board, square));
}
bool is_square_free(Game::chess_bitboard current_board, unsigned long long square) {
	return (!is_square_occupied(current_board, square));
}


//complex stuff - diognal
unsigned long long helperClass::path_diognal(unsigned long long current_pos, Game::chess_bitboard current_board, int white) {
	//shift and or to up and shift and or to down
	unsigned long long l = current_pos;
	unsigned long long l_1 = 0ULL;
	unsigned long long l_2 = 0ULL;
	unsigned long long l_3 = 0ULL;
	unsigned long long l_4 = 0ULL;
	unsigned long long current_bit;
	bool exit_early = false;
	unsigned long bit;
	//get the row
	bit = helperClass::GetFirstSetBit(current_pos);
	if (bit != NULL) {
		int col = bit / 8;
		int row = bit % 8;
		int col_c;
		int row_c;

		//up and right
		//stop whenever row or col reachers 8
		col_c = col+1;
		row_c = row+1;
		while (col_c < 8 && row_c < 8) {
			current_bit = SetNthBit(col_c * 8 + row_c);
			if (white && (is_square_occupied_white(current_board, current_bit)) && (col_c != col)) {
				break;
			}
			if (!white && (is_square_occupied_black(current_board, current_bit)) && (col_c != col)) {
				break;
			}
			if (white && (is_square_occupied_black(current_board, current_bit))) {
				exit_early = true;
			}
			if (!white && (is_square_occupied_white(current_board, current_bit))) {
				exit_early = true;
			}
			//do shifting
			l |= l << 9;
			col_c++;
			row_c++;
			//end shifting
			if (exit_early)
				break;
		}
		exit_early = false;
		l_1 = l;
		l = current_pos;


		col_c = col - 1;
		row_c = row - 1;
		//down and left
		while (col_c > -1 && row_c > -1) {
			current_bit = SetNthBit(col_c * 8 + row_c);
			if (white && (is_square_occupied_white(current_board, current_bit)) && (col_c != col)) {
				break;
			}
			if (!white && (is_square_occupied_black(current_board, current_bit)) && (col_c != col)) {
				break;
			}
			if (white && (is_square_occupied_black(current_board, current_bit))) {
				exit_early = true;
			}
			if (!white && (is_square_occupied_white(current_board, current_bit))) {
				exit_early = true;
			}


			l |= l >> 9;
			col_c--;
			row_c--;

			if (exit_early)
				break;
		}
		exit_early = false;
		l_2 = l;
		l = current_pos;



		//up and left
		col_c = col + 1;
		row_c = row - 1;
		while (col_c < 8 && row_c > -1) {
			current_bit = SetNthBit(col_c * 8 + row_c);
			if (white && (is_square_occupied_white(current_board, current_bit))  &&  (col_c!=col)  ) {
				break;
			}
			if (!white && (is_square_occupied_black(current_board, current_bit)) && (col_c != col) ) {
				break;
			}
			if (white && (is_square_occupied_black(current_board, current_bit))) {
				exit_early = true;
			}
			if (!white && (is_square_occupied_white(current_board, current_bit))) {
				exit_early = true;
			}


			l |= l << 7;
			col_c++;
			row_c--;

			if (exit_early)
				break;
		}
		exit_early = false;
		l_3 = l;
		l = current_pos;

		//down and right
		col_c = col - 1;
		row_c = row + 1;
		while (col_c > -1 && row_c < 8) {
			current_bit = SetNthBit(col_c * 8 + row_c);
			if (white && (is_square_occupied_white(current_board, current_bit)) && (col_c != col)) {
				break;
			}
			if (!white && (is_square_occupied_black(current_board, current_bit)) && (col_c != col)) {
				break;
			}
			if (white && (is_square_occupied_black(current_board, current_bit))) {
				exit_early = true;
			}
			if (!white && (is_square_occupied_white(current_board, current_bit))) {
				exit_early = true;
			}


			l |= l >> 7;
			col_c--;
			row_c++;
			if (exit_early)
				break;
		}
		exit_early = false;
		l_4 = l;
		return l_1 | l_2 | l_3 | l_4;

	}
	else {
		throw ("Error, there is no piece");
	}

}


unsigned long long helperClass::path_vertical(unsigned long long current_pos, Game::chess_bitboard current_board, int white) {
	//keep shifting current position until a piece is found.
	unsigned long long l = current_pos;
	unsigned long long l_up = current_pos;
	unsigned long bit;
	bool exit_early = false;
	//get the row
	bit = helperClass::GetFirstSetBit(current_pos);
	unsigned long long current_bit;

	if (bit != NULL) {
		int col = bit / 8; //current col.
		int row = bit % 8;
		for (int i = row+1; i < 8; i++) {
			current_bit = SetNthBit(col * 8 + i);
			if (white && (i != row) && is_square_occupied_white(current_board, current_bit))
				break;
			if (!white && (i != row) && is_square_occupied_black(current_board, current_bit))
				break;
			if (white && (i != row) && is_square_occupied_black(current_board, current_bit))
				exit_early = true;
			if (!white && (i != row) && is_square_occupied_white(current_board, current_bit))
				exit_early = true;
			l = l | l << 1;
			if (exit_early)
				break;
		}
		exit_early = false;
		l_up = l;
		l = current_pos;

		for (int i = row-1; i > -1; i--) {
			current_bit = SetNthBit(col * 8 + i);
			if (white && (i != row) && is_square_occupied_white(current_board, current_bit))
				break;
			if (!white && (i != row) && is_square_occupied_black(current_board, current_bit))
				break;
			if (white && (i != row) && is_square_occupied_black(current_board, current_bit))
				exit_early = true;
			if (!white && (i != row) && is_square_occupied_white(current_board, current_bit))
				exit_early = true;
			l = l | l >> 1;
			if (exit_early)
				break;
		}

		return l | l_up;

	}
	else {
		throw ("Error, there is no piece");
	}

}

unsigned long long helperClass::path_horizontal(unsigned long long current_pos, Game::chess_bitboard current_board, int white) {
	//keep shifting current position until a piece is found.
	unsigned long long l = current_pos;
	unsigned long long l_up = current_pos;
	unsigned long bit;
	bool exit_early = false;
	//get the row
	bit = helperClass::GetFirstSetBit(current_pos);
	unsigned long long current_bit;

	if (bit != NULL) {
		int col = bit / 8; //current col.
		int row = bit % 8;
		for (int i = col + 1; i < 8; i++) {
			current_bit = SetNthBit(i * 8 + row);
			if (white && (i != row) && is_square_occupied_white(current_board, current_bit))
				break;
			if (!white && (i != row) && is_square_occupied_black(current_board, current_bit))
				break;
			if (white && (i != row) && is_square_occupied_black(current_board, current_bit))
				exit_early = true;
			if (!white && (i != row) && is_square_occupied_white(current_board, current_bit))
				exit_early = true;
			l = l | l << 8;
			if (exit_early)
				break;
		}
		exit_early = false;
		l_up = l;
		l = current_pos;

		for (int i = col - 1; i > -1; i--) {
			current_bit = SetNthBit(i * 8 + row);
			if (white && (i != row) && is_square_occupied_white(current_board, current_bit))
				break;
			if (!white && (i != row) && is_square_occupied_black(current_board, current_bit))
				break;
			if (white && (i != row) && is_square_occupied_black(current_board, current_bit))
				exit_early = true;
			if (!white && (i != row) && is_square_occupied_white(current_board, current_bit))
				exit_early = true;
			l = l | l >> 8;
			if (exit_early)
				break;
		}

		return l | l_up;

	}
	else {
		throw ("Error, there is no piece");
	}

}

//get all moves stuff

int helperClass::get_all_pawn_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]) {
	
	unsigned long long current_pawns;
	unsigned long long current_pawns_iter;
	int counter = 0;
	unsigned long long l_start;
	unsigned long long l_end;
	unsigned long long cptr;
	if (white)
		current_pawns = current_board.white_pawns;
	else
		current_pawns = current_board.black_pawns;
	current_pawns_iter = current_pawns;



	int bit = helperClass::GetFirstSetBit(current_pawns_iter);
	while (bit != NULL) {
		l_start = helperClass::SetNthBit(bit);
		if (white){
			l_end = helperClass::pawn_forward(l_start, 1);
			if (is_square_free(current_board, l_end) ){
				// if the space is clear, allow moving to it
				output_array[counter] = current_board;
				output_array[counter].white_pawns = (current_pawns & (~l_start)) | l_end;//current pawns - starting pos + end position.
				output_array[counter].white_queens = output_array[counter].white_queens | (output_array[counter].white_pawns & white_pawn_end_row);//if pawn makes it to end row, queen.
				output_array[counter].white_pawns = output_array[counter].white_pawns & (~white_pawn_end_row);

				counter++;
				// if also two squares after is free
				l_end = helperClass::pawn_forward(l_start, 2);
				if (is_square_free(current_board, l_end) && (l_start & current_board.pawns_white_start) ) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].white_pawns = (current_pawns & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
			}
			//captures
			l_end = helperClass::path_diognal_one(l_start, true);
			l_end = l_end & (~l_start);
			//one capture is always possible (even on the edge
			//get first capture
			while (std::popcount(l_end)) {
				int first_bit = GetFirstSetBit(l_end);
				cptr = SetNthBit(first_bit);
				if (is_square_occupied_black(current_board, cptr)) {//sometimes == 1, sometimes == 2
					output_array[counter] = capture_square_black(current_board, cptr);
					output_array[counter].white_pawns = (current_pawns & (~l_start)) | cptr;//current pawns - starting pos + end position.
					output_array[counter].white_queens = output_array[counter].white_queens | (output_array[counter].white_pawns & white_pawn_end_row);//if pawn makes it to end row, queen.
					output_array[counter].white_pawns = output_array[counter].white_pawns & (~white_pawn_end_row);
					counter++;
				}
				l_end = l_end & (~cptr);
			}
			
		}
		

		else {//black
			l_end = helperClass::pawn_forward(l_start, -1);
			if (is_square_free(current_board, l_end)) {
				// if the space is clear, allow moving to it
				output_array[counter] = current_board;
				output_array[counter].black_pawns = (current_pawns & (~l_start)) | l_end;//current pawns - starting pos + end position.
				output_array[counter].black_queens = output_array[counter].black_queens | (output_array[counter].black_pawns & black_pawn_end_row);//if pawn makes it to end row, queen.
				output_array[counter].black_pawns = output_array[counter].black_pawns & (~black_pawn_end_row);
				counter++;
				// if also two squares after is free
				l_end = helperClass::pawn_forward(l_start, -2);
				if (is_square_free(current_board, l_end) && (l_start & current_board.pawns_black_start)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].black_pawns = (current_pawns & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
			}
			//captures
			l_end = helperClass::path_diognal_one(l_start, false);
			l_end = l_end & (~l_start);
			//one capture is always possible (even on the edge
			//get first capture
			while (std::popcount(l_end)) {
				int first_bit = GetFirstSetBit(l_end);
				cptr = SetNthBit(first_bit);
				if (is_square_occupied_white(current_board, cptr)) {//sometimes == 1, sometimes == 2
					output_array[counter] = capture_square_white(current_board, cptr);
					output_array[counter].black_pawns = (current_pawns & (~l_start)) | cptr;//current pawns - starting pos + end position.
					output_array[counter].black_queens = output_array[counter].black_queens | (output_array[counter].black_pawns & black_pawn_end_row);//if pawn makes it to end row, queen.
					output_array[counter].black_pawns = output_array[counter].black_pawns & (~black_pawn_end_row);
					counter++;
				}
				l_end = l_end & (~cptr);
			}

		}



		current_pawns_iter = current_pawns_iter & (~l_start);
		bit = helperClass::GetFirstSetBit(current_pawns_iter);
	}

	
	return counter;
}

int helperClass::get_all_knight_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]) {
	unsigned long long current_knights;
	unsigned long long current_knights_iter;
	int counter = 0;
	unsigned long long l_start;
	unsigned long long l_end;
	unsigned long long cptr;
	if (white)
		current_knights = current_board.white_knights;
	else
		current_knights = current_board.black_knights;
	current_knights_iter = current_knights;

	int bit = helperClass::GetFirstSetBit(current_knights_iter);
	while (bit != NULL) {
		l_start = helperClass::SetNthBit(bit);
		if (white) {
			unsigned long long l_end_all = helperClass::path_L(l_start);
			while ( popcount(l_end_all) ){
				l_end = SetNthBit(GetFirstSetBit(l_end_all));

				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].white_knights = (current_knights & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_black(current_board,l_end)) {
					output_array[counter] = capture_square_black(current_board, l_end);
					output_array[counter].white_knights = (current_knights & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}
		else {//black
			unsigned long long l_end_all = helperClass::path_L(l_start);
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));

				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].black_knights = (current_knights & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_white(current_board, l_end)) {
					output_array[counter] = capture_square_white(current_board, l_end);
					output_array[counter].black_knights = (current_knights & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}
		
		current_knights_iter = current_knights_iter & (~l_start);
		bit = helperClass::GetFirstSetBit(current_knights_iter);
	}
	return counter;
}

int helperClass::get_all_bishop_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]) {
	unsigned long long current_bishops;
	unsigned long long current_bishops_iter;
	int counter = 0;
	unsigned long long l_start;
	unsigned long long l_end;
	unsigned long long cptr;
	if (white)
		current_bishops = current_board.white_bishops;
	else
		current_bishops = current_board.black_bishops;
	current_bishops_iter = current_bishops;

	int bit = helperClass::GetFirstSetBit(current_bishops_iter);
	while (bit != NULL) {
		l_start = helperClass::SetNthBit(bit);
		if (white) {
			unsigned long long l_end_all = helperClass::path_diognal(l_start, current_board, white) & (~l_start);
			
			while (popcount(l_end_all)) {
				
				l_end = SetNthBit(GetFirstSetBit(l_end_all));
				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].white_bishops = (current_bishops & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_black(current_board, l_end)) {
					output_array[counter] = capture_square_black(current_board, l_end);
					output_array[counter].white_bishops = (current_bishops & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}
		else {//black
			unsigned long long l_end_all = helperClass::path_diognal(l_start, current_board, white);
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));

				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].black_bishops = (current_bishops & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_white(current_board, l_end)) {
					output_array[counter] = capture_square_white(current_board, l_end);
					output_array[counter].black_bishops = (current_bishops & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}

		current_bishops_iter = current_bishops_iter & (~l_start);
		bit = helperClass::GetFirstSetBit(current_bishops_iter);
	}
	return counter;
}

int helperClass::get_all_rook_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]) {

	Game::chess_bitboard current_board_c = current_board;
	unsigned long long current_rooks;
	unsigned long long current_rooks_iter;
	int counter = 0;
	unsigned long long l_start;
	unsigned long long l_end;
	unsigned long long cptr;
	if (white)
		current_rooks = current_board.white_rooks;
	else
		current_rooks = current_board.black_rooks;
	current_rooks_iter = current_rooks;

	int bit = helperClass::GetFirstSetBit(current_rooks_iter);
	while (bit != NULL) {
		l_start = helperClass::SetNthBit(bit);
		if (white) {
			unsigned long long l_end_all = (helperClass::path_vertical(l_start, current_board, white) | helperClass::path_horizontal(l_start, current_board, white)) & (~l_start);
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));
				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board_c;
					output_array[counter].white_rooks = (current_rooks & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_black(current_board, l_end)) {
					output_array[counter] = capture_square_black(current_board, l_end);
					output_array[counter].white_rooks = (current_rooks & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}
		else {//black
			unsigned long long l_end_all = helperClass::path_vertical(l_start, current_board, white);
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));

				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].black_rooks = (current_rooks & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_white(current_board, l_end)) {
					output_array[counter] = capture_square_white(current_board, l_end);
					output_array[counter].black_rooks = (current_rooks & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}

		current_rooks_iter = current_rooks_iter & (~l_start);
		bit = helperClass::GetFirstSetBit(current_rooks_iter);
	}

	return counter;
}

int helperClass::get_all_queen_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]) {

	Game::chess_bitboard current_board_c = current_board;
	unsigned long long current_queens;
	unsigned long long current_queens_iter;
	int counter = 0;
	unsigned long long l_start;
	unsigned long long l_end;
	unsigned long long cptr;
	if (white)
		current_queens = current_board.white_queens;
	else
		current_queens = current_board.black_queens;
	current_queens_iter = current_queens;

	int bit = helperClass::GetFirstSetBit(current_queens_iter);
	while (bit != NULL) {
		l_start = helperClass::SetNthBit(bit);
		if (white) {
			unsigned long long l_end_all = (helperClass::path_vertical(l_start, current_board, white) | helperClass::path_horizontal(l_start, current_board, white) | helperClass::path_diognal(l_start, current_board, white) ) & (~l_start);
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));
				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board_c;
					output_array[counter].white_queens = (current_queens & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_black(current_board, l_end)) {
					output_array[counter] = capture_square_black(current_board, l_end);
					output_array[counter].white_queens = (current_queens & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}
		else {//black
			unsigned long long l_end_all = (helperClass::path_vertical(l_start, current_board, white) | helperClass::path_horizontal(l_start, current_board, white) | helperClass::path_diognal(l_start, current_board, white)) & (~l_start);
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));

				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].black_queens = (current_queens & (~l_start)) | l_end;//current pawns - starting pos + end position.
					counter++;
				}
				if (is_square_occupied_white(current_board, l_end)) {
					output_array[counter] = capture_square_white(current_board, l_end);
					output_array[counter].black_queens = (current_queens & (~l_start)) | l_end;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}

		current_queens_iter = current_queens_iter & (~l_start);
		bit = helperClass::GetFirstSetBit(current_queens_iter);
	}

	return counter;
}

int helperClass::get_all_king_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[32]) {

	Game::chess_bitboard current_board_c = current_board;
	unsigned long long current_kings;
	unsigned long long current_kings_iter;
	int counter = 0;
	unsigned long long l_start;
	unsigned long long l_end;
	unsigned long long cptr;
	if (white)
		current_kings = current_board.white_kings;
	else
		current_kings = current_board.black_kings;
	current_kings_iter = current_kings;

	int bit = helperClass::GetFirstSetBit(current_kings_iter);
	while (bit != NULL) {
		l_start = helperClass::SetNthBit(bit);
		//diognal paths hack!
		unsigned long long diognal_paths = (helperClass::path_diognal_one(l_start, true) | helperClass::path_diognal_one(l_start, false)) & helperClass::path_diognal(l_start, current_board, white);

		unsigned long long l_end_all = (diognal_paths |
			helperClass::king_forward(l_start, 1) | helperClass::king_forward(l_start, -1) |
			helperClass::pawn_sideways(l_start, 1) | helperClass::pawn_sideways(l_start, -1)) & (~l_start);
		if (white) {
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));
				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board_c;
					output_array[counter].white_kings = (current_kings & (~l_start)) | l_end;//current pawns - starting pos + end position.
					output_array[counter].white_castle_king = false;
					output_array[counter].white_castle_queen = false;
					counter++;
				}
				if (is_square_occupied_black(current_board, l_end)) {
					output_array[counter] = capture_square_black(current_board, l_end);
					output_array[counter].white_kings = (current_kings & (~l_start)) | l_end;
					output_array[counter].white_castle_king = false;
					output_array[counter].white_castle_queen = false;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}
		else {
			while (popcount(l_end_all)) {
				l_end = SetNthBit(GetFirstSetBit(l_end_all));
				if (is_square_free(current_board, l_end)) {
					// if the space is clear, allow moving to it
					output_array[counter] = current_board;
					output_array[counter].black_kings = (current_kings & (~l_start)) | l_end;//current pawns - starting pos + end position.
					output_array[counter].black_castle_king = false;
					output_array[counter].black_castle_queen = false;
					counter++;
				}
				if (is_square_occupied_white(current_board, l_end)) {
					output_array[counter] = capture_square_white(current_board, l_end);
					output_array[counter].black_kings = (current_kings & (~l_start)) | l_end;
					output_array[counter].black_castle_king = false;
					output_array[counter].black_castle_queen = false;
					counter++;
				}
				l_end_all = l_end_all & (~l_end);
			}
		}

		current_kings_iter = current_kings_iter & (~l_start);
		bit = helperClass::GetFirstSetBit(current_kings_iter);
	}

	return counter;
}

int helperClass::get_all_castle_moves(Game::chess_bitboard current_board, int white, Game::chess_bitboard output_array[2]) {
	int counter = 0;
	if (white) {
		if (current_board.white_castle_king) {//white is allowed to castle kingside
			//check that the two squares next to the king are empty
			unsigned long long square_1 = current_board.white_kings;
			square_1 = square_1 << 8;
			unsigned long long square_2 = square_1;
			square_2 = square_2 << 8;
			//current_board = capture_square_white(current_board, square_1);
			//current_board = capture_square_white(current_board, square_2);
			if (is_square_free(current_board, square_1) && is_square_free(current_board, square_2)) {//the two squares are free. I will allow castleing through check for now.
				output_array[counter] = current_board;
				output_array[counter].white_kings = square_2;
				output_array[counter].white_rooks = output_array[counter].white_rooks & (~72057594037927936ULL) | square_1;
				output_array[counter].white_castle_king = false;
				output_array[counter].white_castle_queen = false;
				counter++;
			}
		}
		if (current_board.white_castle_queen) {//white is allowed to queen
			unsigned long long square_1 = current_board.white_kings;
			square_1 = square_1 >> 8;
			unsigned long long square_2 = square_1;
			square_2 = square_2 >> 8;
			unsigned long long square_3 = square_2;
			square_3 = square_3 >> 8;
			if (is_square_free(current_board, square_1) && is_square_free(current_board, square_2) && is_square_free(current_board, square_3)) {//the two squares are free. I will allow castleing through check for now.
				output_array[counter] = current_board;
				output_array[counter].white_kings = square_2;
				output_array[counter].white_rooks = output_array[counter].white_rooks & (~1ULL) | square_1;
				output_array[counter].white_castle_king = false;
				output_array[counter].white_castle_queen = false;
				counter++;
			}
		}
	}
	else {
		if (current_board.black_castle_king) {//black is allowed to castle kingside
			//check that the two squares next to the king are empty
			unsigned long long square_1 = current_board.black_kings;
			square_1 = square_1 << 8;
			unsigned long long square_2 = square_1;
			square_2 = square_2 << 8;
			//current_board = capture_square_black(current_board, square_1);
			//current_board = capture_square_black(current_board, square_2);
			if (is_square_free(current_board, square_1) && is_square_free(current_board, square_2)) {//the two squares are free. I will allow castleing through check for now.
				output_array[counter] = current_board;
				output_array[counter].black_kings = square_2;
				output_array[counter].black_rooks = output_array[counter].black_rooks & (~9223372036854775808ULL) | square_1;
				output_array[counter].black_castle_king = false;
				output_array[counter].black_castle_queen = false;
				counter++;
			}
		}
		if (current_board.black_castle_queen) {//black is allowed to queen
			unsigned long long square_1 = current_board.black_kings;
			square_1 = square_1 >> 8;
			unsigned long long square_2 = square_1;
			square_2 = square_2 >> 8;
			unsigned long long square_3 = square_2;
			square_3 = square_3 >> 8;
			if (is_square_free(current_board, square_1) && is_square_free(current_board, square_2) && is_square_free(current_board, square_3)) {//the two squares are free. I will allow castleing through check for now.
				output_array[counter] = current_board;
				output_array[counter].black_kings = square_2;
				output_array[counter].black_rooks = output_array[counter].black_rooks & (~128ULL) | square_1;
				output_array[counter].black_castle_king = false;
				output_array[counter].black_castle_queen = false;
				counter++;
			}
		}

	}

	return counter;
}


// Heuristic eval
//cols										A					B					C				D					E					F					G				H
vector<vector<int>> good_knights = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,1,1,1,1,0,0}, {0,0,1,1,1,1,0,0}, {0,0,1,1,1,1,0,0}, {0,0,1,1,1,1,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
int long long good_knight_bitboard = helperClass::vector_to_bitboard(good_knights);

vector<vector<int>> good_bishops = { {0,0,0,0,0,0,0,0}, {0,1,1,1,1,1,1,0}, {0,1,1,1,1,1,1,0}, {0,1,1,1,1,1,1,0}, {0,1,1,1,1,1,1,0}, {0,1,1,1,1,1,1,0}, {0,1,1,1,1,1,1,0}, {0,0,0,0,0,0,0,0} };
int long long good_bishops_bitboard = helperClass::vector_to_bitboard(good_bishops);

vector<vector<int>> good_queens = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,1,1,1,1,0,0}, {0,0,1,1,1,1,0,0}, {0,0,1,1,1,1,0,0}, {0,0,1,1,1,1,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
int long long good_queens_bitboard = helperClass::vector_to_bitboard(good_queens);



int helperClass::getValueBitboard(Game::chess_bitboard c) {
	int total = 0;
	//basic
	total += popcount(c.white_pawns) - popcount(c.black_pawns);
	total += popcount(c.white_knights) * 30 - popcount(c.black_knights) * 30;
	total += popcount(c.white_bishops) * 40 - popcount(c.black_bishops) * 40;
	total += popcount(c.white_rooks) * 50 - popcount(c.black_rooks) * 50;
	total += popcount(c.white_kings) * 1000 - popcount(c.black_kings) * 1000;
	total += popcount(c.white_queens) * 90 - popcount(c.black_queens) * 90;
	//advanced
	total += popcount(c.white_knights & good_knight_bitboard) * 3 - popcount(c.black_knights & good_knight_bitboard) * 3;
	total += popcount(c.white_bishops & good_bishops_bitboard) * 2 - popcount(c.black_bishops & good_bishops_bitboard) * 2;
	total += popcount(c.white_queens & good_queens_bitboard) * 1 - popcount(c.black_queens & good_queens_bitboard) * 1;

	return total;
}