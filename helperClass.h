#pragma once
#include <vector>
//#include <map>

class helperClass
{
public:
	
	static std::vector<std::vector<int>> getPiecesPosition(std::vector<std::vector<char>> board, int white);
	static std::vector<std::vector<int>> get_Moves_Piece(char piece_type, int pos_x, int pos_y);
	static int* get_Moves_Piece_INTARR(char piece_type, int pos_x, int pos_y, int* moves, int idx);

	static int getValueBoard(std::vector<std::vector<char>> board);
	static int getValueBoardPointer(Game* board);
	static int iswhitepiece(char c);
};

