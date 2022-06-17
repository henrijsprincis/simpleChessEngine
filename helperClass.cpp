#include "chess.h"
#include "helperClass.h"
#include <vector>
#include <iostream>

// using namespace std;

std::vector<std::vector<int>> helperClass::getPiecesPosition(std::vector<std::vector<char>> board, int white)
{
	// white == 1 then white. white == 0, then black

	std::vector<std::vector<int>> getPiecesPositionWhite(16);
	std::vector<std::vector<int>> getPiecesPositionBlack(16);

	int white_counter = 0;
	int black_counter = 0;
	for (int y = 0; y < board.size(); y++)
	{ // board_vector[Height/Row][Width/Col]
		for (int x = 0; x < board[y].size(); x++)
		{
			char ch = board[y][x];

			if (isalpha(ch))
			{
				std::vector<int> square = {x, y};
				if (isupper(ch))
				{ // white ==
					getPiecesPositionWhite[white_counter] = square;
					white_counter++;
				}
				else
				{
					getPiecesPositionBlack[black_counter] = square;
					black_counter++;
				}
			}
		}
	}
	if (white)
	{
		getPiecesPositionWhite.resize(white_counter);
		return getPiecesPositionWhite;
	}
	else
	{
		getPiecesPositionBlack.resize(black_counter);
		return getPiecesPositionBlack;
	}
}

std::vector<std::vector<int>> helperClass::get_Moves_Piece(char piece_type, int pos_x, int pos_y)
{
	// std::cout << "piece type : " << piece_type << std::endl;
	std::vector<std::vector<int>> moves;

	if (piece_type == 'P')
	{
		moves = {{pos_x, pos_y + 1}, {pos_x, pos_y + 2}, {pos_x + 1, pos_y + 1}, {pos_x - 1, pos_y + 1}}; // 1 up, 2 up, capture left, capture right
	}
	if (piece_type == 'p')
	{
		moves = {{pos_x, pos_y - 1}, {pos_x, pos_y - 2}, {pos_x + 1, pos_y - 1}, {pos_x - 1, pos_y - 1}}; // 1 down, 2 down, capture left, capture right
	}

	if (piece_type == 'N' || piece_type == 'n')
	{
		moves = {
			{pos_x + 1, pos_y + 2}, {pos_x - 1, pos_y + 2}, {pos_x + 1, pos_y - 2}, {pos_x - 1, pos_y - 2}, // up down
			{pos_x - 2, pos_y + 1},
			{pos_x - 2, pos_y - 1},
			{pos_x + 2, pos_y + 1},
			{pos_x + 2, pos_y - 1} // left, right
		};
	}

	if (piece_type == 'K' || piece_type == 'k')
	{
		moves = {{pos_x - 1, pos_y + 1}, {pos_x, pos_y + 1}, {pos_x + 1, pos_y + 1}, {pos_x - 1, pos_y}, {pos_x + 1, pos_y}, {pos_x - 1, pos_y - 1}, {pos_x, pos_y - 1}, {pos_x + 1, pos_y - 1}};
	}
	std::vector<int> m;
	int min_pos;
	if (piece_type == 'B' || piece_type == 'b')
	{
		// up right -- needs work, but not now
		// min_pos = std::min(pos_x, pos_y);

		for (int x = -8; x < 8; x++)
		{
			m = {pos_x + x, pos_y + x};
			moves.insert(moves.begin(), m);
			m = {pos_x + x, pos_y - x};
			moves.insert(moves.begin(), m);
		}
	}

	if (piece_type == 'R' || piece_type == 'r')
	{
		// up right
		for (int x = 0; x < 8; x++)
		{
			// rook
			m = {x, pos_y};
			moves.insert(moves.begin(), m);
			m = {pos_x, x};
			moves.insert(moves.begin(), m);
		}
	}

	if (piece_type == 'Q' || piece_type == 'q')
	{
		// up right
		for (int x = -8; x < 8; x++)
		{
			// bishop
			m = {pos_x + x, pos_y + x};
			moves.insert(moves.begin(), m);
			m = {pos_x + x, pos_y - x};
			moves.insert(moves.begin(), m);
		}
		for (int x = 0; x < 8; x++)
		{
			// rook
			m = {x, pos_y};
			moves.insert(moves.begin(), m);
			m = {pos_x, x};
			moves.insert(moves.begin(), m);
		}
	}

	for (int x = moves.size() - 1; x > 0; x--)
	{
		for (int y = 0; y < 2; y++)
		{
			if (x < moves.size())
			{
				if (moves[x][y] > 7 || moves[x][y] < 0)
				{
					moves.erase(moves.begin() + x);
				}
			}
		}
	}
	return moves;
}

int *helperClass::get_Moves_Piece_INTARR(char piece_type, int pos_x, int pos_y, int *moves, int idx)
{

	// int* moves = new int[65];//first index contains how many moves in array;
	// int idx = 1;
	if (piece_type == 'P')
	{
		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 2;
		idx++;

		moves[idx] = pos_x + 1; //{pos_x + 1, pos_y + 1}
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x - 1; // pos_x - 1, pos_y + 1
		idx++;
		moves[idx] = pos_y + 1;
		idx++;
	}
	else if (piece_type == 'p')
	{
		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x;
		idx++;
		moves[idx] = pos_y + 2;
		idx++;

		moves[idx] = pos_x + 1; //{pos_x + 1, pos_y + 1}
		idx++;
		moves[idx] = pos_y + 1;
		idx++;

		moves[idx] = pos_x - 1; // pos_x - 1, pos_y + 1
		idx++;
		moves[idx] = pos_y + 1;
		idx++;
	}

	else if (piece_type == 'N' || piece_type == 'n')
	{
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

	else if (piece_type == 'K' || piece_type == 'k')
	{
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

		// moves[idx] = pos_x;
		// idx++;
		// moves[idx] = pos_y;
		// idx++;

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

	else if (piece_type == 'B' || piece_type == 'b')
	{
		for (int x = -8; x < 8; x++)
		{
			// check move is within boudns.
			if (std::max(pos_x + x, pos_y + x) < 8 && std::min(pos_x + x, pos_y + x) >= 0)
			{
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y + x;
				idx++;
			}
			if (std::max(pos_x + x, pos_y - x) < 8 && std::min(pos_x + x, pos_y - x) >= 0)
			{
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y - x;
				idx++;
			}
		}
	}

	else if (piece_type == 'R' || piece_type == 'r')
	{
		// up right
		for (int x = 0; x < 8; x++)
		{
			// rook
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

	else if (piece_type == 'Q' || piece_type == 'q')
	{
		// bishop
		for (int x = -8; x < 8; x++)
		{
			// check move is within boudns.
			if (std::max(pos_x + x, pos_y + x) < 8 && std::min(pos_x + x, pos_y + x) >= 0)
			{
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y + x;
				idx++;
			}
			if (std::max(pos_x + x, pos_y - x) < 8 && std::min(pos_x + x, pos_y - x) >= 0)
			{
				moves[idx] = pos_x + x;
				idx++;
				moves[idx] = pos_y - x;
				idx++;
			}
		}
		// rook
		for (int x = 0; x < 8; x++)
		{
			// rook
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
	// cout << "INDEX" << idx;
	moves[0] = idx;
	// cout << "moves[0]: " << moves[0];

	return moves;
}

int helperClass::iswhitepiece(char c)
{
	// returns 1 when white piece, 0 when black and -1 when empty
	if (isalpha(c))
	{
		if (isupper(c))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

int helperClass::getValueBoard(std::vector<std::vector<char>> board)
{
	int total = 0;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y] == ' ')
			{
				continue;
			}
			if (board[x][y] == 'P')
			{
				total++;
				continue;
			}
			if (board[x][y] == 'p')
			{
				total--;
				continue;
			}

			if (board[x][y] == 'N')
			{
				total += 3;
				continue;
			}
			if (board[x][y] == 'n')
			{
				total -= 3;
				continue;
			}

			if (board[x][y] == 'B')
			{
				total += 4;
				continue;
			}
			if (board[x][y] == 'b')
			{
				total -= 4;
				continue;
			}

			if (board[x][y] == 'R')
			{
				total += 5;
				continue;
			}
			if (board[x][y] == 'r')
			{
				total -= 5;
				continue;
			}
			if (board[x][y] == 'Q')
			{
				total += 9;
				continue;
			}
			if (board[x][y] == 'q')
			{
				total -= 9;
				continue;
			}
		}
	}

	return total;
}

int helperClass::getValueBoardPointer(Game *board)
{
	return board->board_value_array[board->board_value_idx];
	// old code
	/*
	int total = 0;

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (board->board[x][y] == ' ') {
				continue;
			}
			if (board->board[x][y] == 'P') {
				total++;
				continue;
			}
			if (board->board[x][y] == 'p') {
				total--;
				continue;
			}

			if (board->board[x][y] == 'N') {
				total += 3;
				continue;
			}
			if (board->board[x][y] == 'n') {
				total -= 3;
				continue;
			}

			if (board->board[x][y] == 'B') {
				total += 4;
				continue;
			}
			if (board->board[x][y] == 'b') {
				total -= 4;
				continue;
			}

			if (board->board[x][y] == 'R') {
				total += 5;
				continue;
			}
			if (board->board[x][y] == 'r') {
				total -= 5;
				continue;
			}
			if (board->board[x][y] == 'Q') {
				total += 9;
				continue;
			}
			if (board->board[x][y] == 'q') {
				total -= 9;
				continue;
			}

		}
	}

	return total;
	*/
}
