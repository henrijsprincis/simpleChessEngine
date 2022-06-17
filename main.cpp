#include "includes.h"
#include <vector>
#include "user_interface.h"
#include "chess.h"
#include "helperClass.h"
#include "debug.h"
#include <chrono>
#include <thread>

//---------------------------------------------------------------------------------------
// Global variable
//---------------------------------------------------------------------------------------
Game *current_game = NULL;
char chess_cols[9] = "ABCDEFGH";
char chess_rows[9] = "12345678";
int total_moves = 0;
int depth = 5;

//---------------------------------------------------------------------------------------
// Helper
// Auxiliar functions to determine if a move is valid, etc
//---------------------------------------------------------------------------------------
bool isMoveValid(Chess::Position present, Chess::Position future, Chess::EnPassant *S_enPassant, Chess::Castling *S_castling, Chess::Promotion *S_promotion)
{
    bool bValid = false;

    char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);

    // ----------------------------------------------------
    // 1. Is the piece  allowed to move in that direction?
    // ----------------------------------------------------

    switch (toupper(chPiece))
    {
    case 'P':
    {
        cout << "PAWN" << endl;
        // Wants to move forward
        if (future.iColumn == present.iColumn)
        {
            // Simple move forward
            cout << "move forwad";
            if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) ||
                (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
            {
                if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn))
                {
                    bValid = true;
                }
            }

            // Double move forward
            else if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 2) ||
                     (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 2))
            {
                // This is only allowed if the pawn is in its original place
                if (Chess::isWhitePiece(chPiece))
                {
                    if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow - 1, future.iColumn) &&
                        EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn) &&
                        1 == present.iRow)
                    {
                        bValid = true;
                    }
                }
                else // if ( isBlackPiece(chPiece) )
                {
                    if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow + 1, future.iColumn) &&
                        EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn) &&
                        6 == present.iRow)
                    {
                        bValid = true;
                    }
                }
            }
            else
            {
                // This is invalid
                cout << "RETURNED EARLY";
                return false;
            }
        }

        // The "en passant" move
        else if (((Chess::isWhitePiece(chPiece) && 4 == present.iRow && 5 == future.iRow && 1 == abs(future.iColumn - present.iColumn)) ||
                  (Chess::isBlackPiece(chPiece) && 3 == present.iRow && 2 == future.iRow && 1 == abs(future.iColumn - present.iColumn))) &&
                 toupper(current_game->getPieceAtPosition(present.iRow, future.iColumn)) == 'P') // future i row there is a pawn
        {
            cout << "en passant";
            // It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
            string last_move = current_game->getLastMove();

            // Parse the line
            Chess::Position LastMoveFrom;
            Chess::Position LastMoveTo;
            current_game->parseMove(last_move, &LastMoveFrom, &LastMoveTo);

            // First of all, was it a pawn?
            char chLstMvPiece = current_game->getPieceAtPosition(LastMoveTo.iRow, LastMoveTo.iColumn);

            if (toupper(chLstMvPiece) != 'P')
            {
                cout << "FAILED EN PESANT!";
                return false;
            }

            // Did the pawn have a double move forward and was it an adjacent column?
            if (2 == abs(LastMoveTo.iRow - LastMoveFrom.iRow) && 1 == abs(LastMoveFrom.iColumn - present.iColumn))
            {
                bValid = true;
                S_enPassant->bApplied = true;
                S_enPassant->PawnCaptured.iRow = LastMoveTo.iRow;
                S_enPassant->PawnCaptured.iColumn = LastMoveTo.iColumn;
            }
        }

        // Wants to capture a piece
        else if (1 == abs(future.iColumn - present.iColumn))
        {
            cout << "abs col diff" << endl;
            if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) || (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
            {
                cout << "abs row diff" << endl;
                // Only allowed if there is something to be captured in the square
                if (EMPTY_SQUARE != current_game->getPieceAtPosition(future.iRow, future.iColumn))
                {
                    cout << "future square not empty" << endl;
                    bValid = true;
                    cout << "Pawn captured a piece!\n";
                }
            }
        }

        else
        {
            // This is invalid
            return false;
        }

        // If a pawn reaches its eight rank, it must be promoted to another piece
        if ((Chess::isWhitePiece(chPiece) && 7 == future.iRow) ||
            (Chess::isBlackPiece(chPiece) && 0 == future.iRow))
        {
            cout << "Pawn must be promoted!\n";
            S_promotion->bApplied = true;
        }
    }
    break;

    case 'R':
    {
        // Horizontal move
        if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::HORIZONTAL))
            {
                bValid = true;
            }
        }
        // Vertical move
        else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::VERTICAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'N':
    {
        if ((2 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }

        else if ((1 == abs(future.iRow - present.iRow)) && (2 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }
    }
    break;

    case 'B':
    {
        // Diagonal move
        if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::DIAGONAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'Q':
    {
        // Horizontal move
        if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::HORIZONTAL))
            {
                bValid = true;
            }
        }
        // Vertical move
        else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::VERTICAL))
            {
                bValid = true;
            }
        }

        // Diagonal move
        else if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::DIAGONAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'K':
    {
        // Horizontal move by 1
        if ((future.iRow == present.iRow) && (1 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }

        // Vertical move by 1
        else if ((future.iColumn == present.iColumn) && (1 == abs(future.iRow - present.iRow)))
        {
            bValid = true;
        }

        // Diagonal move by 1
        else if ((1 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }

        // Castling
        else if ((future.iRow == present.iRow) && (2 == abs(future.iColumn - present.iColumn)))
        {
            // Castling is only allowed in these circunstances:

            // 1. King is not in check
            if (true == current_game->playerKingInCheck())
            {
                return false;
            }

            // 2. No pieces in between the king and the rook
            if (false == current_game->isPathFree(present, future, Chess::HORIZONTAL))
            {
                return false;
            }

            // 3. King and rook must not have moved yet;
            // 4. King must not pass through a square that is attacked by an enemy piece
            if (future.iColumn > present.iColumn)
            {
                // if future.iColumn is greather, it means king side
                if (false == current_game->castlingAllowed(Chess::Side::KING_SIDE, Chess::getPieceColor(chPiece)))
                {
                    createNextMessage("Castling to the king side is not allowed.\n");
                    return false;
                }
                else
                {
                    // Check if the square that the king skips is not under attack
                    Chess::UnderAttack square_skipped = current_game->isUnderAttack(present.iRow, present.iColumn + 1, current_game->getCurrentTurn());
                    if (false == square_skipped.bUnderAttack)
                    {
                        // Fill the S_castling structure
                        S_castling->bApplied = true;

                        // Present position of the rook
                        S_castling->rook_before.iRow = present.iRow;
                        S_castling->rook_before.iColumn = present.iColumn + 3;

                        // Future position of the rook
                        S_castling->rook_after.iRow = future.iRow;
                        S_castling->rook_after.iColumn = present.iColumn + 1; // future.iColumn -1

                        bValid = true;
                    }
                }
            }
            else // if (future.iColumn < present.iColumn)
            {
                // if present.iColumn is greather, it means queen side
                if (false == current_game->castlingAllowed(Chess::Side::QUEEN_SIDE, Chess::getPieceColor(chPiece)))
                {
                    createNextMessage("Castling to the queen side is not allowed.\n");
                    return false;
                }
                else
                {
                    // Check if the square that the king skips is not attacked
                    Chess::UnderAttack square_skipped = current_game->isUnderAttack(present.iRow, present.iColumn - 1, current_game->getCurrentTurn());
                    if (false == square_skipped.bUnderAttack)
                    {
                        // Fill the S_castling structure
                        S_castling->bApplied = true;

                        // Present position of the rook
                        S_castling->rook_before.iRow = present.iRow;
                        S_castling->rook_before.iColumn = present.iColumn - 4;

                        // Future position of the rook
                        S_castling->rook_after.iRow = future.iRow;
                        S_castling->rook_after.iColumn = present.iColumn - 1; // future.iColumn +1

                        bValid = true;
                    }
                }
            }
        }
    }
    break;

    default:
    {
        cout << "!!!!Should not reach here. Invalid piece: " << char(chPiece) << "\n\n\n";
    }
    break;
    }

    // If it is a move in an invalid direction, do not even bother to check the rest
    if (false == bValid)
    {
        cout << "Piece is not allowed to move to that square\n";
        return false;
    }

    // -------------------------------------------------------------------------
    // 2. Is there another piece of the same color on the destination square?
    // -------------------------------------------------------------------------
    if (current_game->isSquareOccupied(future.iRow, future.iColumn))
    {
        char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);
        if (Chess::getPieceColor(chPiece) == Chess::getPieceColor(chAuxPiece))
        {
            cout << "Position is already taken by a piece of the same color\n";
            return false;
        }
    }

    // ----------------------------------------------
    // 3. Would the king be in check after the move?
    // ----------------------------------------------
    if (true == current_game->wouldKingBeInCheck(chPiece, present, future, S_enPassant))
    {
        cout << "Move would put player's king in check\n";
        return false;
    }

    return bValid;
}

bool isMoveValidComputer(Chess::Position present, Chess::Position future, Chess::EnPassant *S_enPassant, Chess::Castling *S_castling, Chess::Promotion *S_promotion)
{

    bool bValid = false;

    char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);

    // ----------------------------------------------------
    // 1. Is the piece  allowed to move in that direction?
    // ----------------------------------------------------
    switch (toupper(chPiece))
    {
    case 'P':
    {

        // Wants to move forward
        if (future.iColumn == present.iColumn)
        {
            // Simple move forward
            if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) ||
                (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
            {
                if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn))
                {
                    bValid = true;
                }
            }

            // Double move forward
            else if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 2) ||
                     (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 2))
            {
                // This is only allowed if the pawn is in its original place
                if (Chess::isWhitePiece(chPiece))
                {
                    if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow - 1, future.iColumn) &&
                        EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn) &&
                        1 == present.iRow)
                    {
                        bValid = true;
                    }
                }
                else // if ( isBlackPiece(chPiece) )
                {
                    if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow + 1, future.iColumn) &&
                        EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn) &&
                        6 == present.iRow)
                    {
                        bValid = true;
                    }
                }
            }
            else
            {
                // This is invalid
                return false;
            }
        }

        // The "en passant" move
        else if (((Chess::isWhitePiece(chPiece) && 4 == present.iRow && 5 == future.iRow && 1 == abs(future.iColumn - present.iColumn)) ||
                  (Chess::isBlackPiece(chPiece) && 3 == present.iRow && 2 == future.iRow && 1 == abs(future.iColumn - present.iColumn))) &&
                 toupper(current_game->getPieceAtPosition(present.iRow, future.iColumn)) == 'P')
        {

            // It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
            string last_move = current_game->getLastMove();

            // Parse the line
            Chess::Position LastMoveFrom;
            Chess::Position LastMoveTo;
            current_game->parseMove(last_move, &LastMoveFrom, &LastMoveTo);

            // First of all, was it a pawn?
            char chLstMvPiece = current_game->getPieceAtPosition(LastMoveTo.iRow, LastMoveTo.iColumn);

            if (toupper(chLstMvPiece) != 'P')
            {
                return false;
            }

            // Did the pawn have a double move forward and was it an adjacent column?
            if (2 == abs(LastMoveTo.iRow - LastMoveFrom.iRow) && 1 == abs(LastMoveFrom.iColumn - present.iColumn))
            {
                bValid = true;
                S_enPassant->bApplied = true;
                S_enPassant->PawnCaptured.iRow = LastMoveTo.iRow;
                S_enPassant->PawnCaptured.iColumn = LastMoveTo.iColumn;
            }
        }

        // Wants to capture a piece
        else if (1 == abs(future.iColumn - present.iColumn))
        {
            if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) || (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
            {
                // Only allowed if there is something to be captured in the square
                if (EMPTY_SQUARE != current_game->getPieceAtPosition(future.iRow, future.iColumn))
                {
                    bValid = true;
                    // cout << "Pawn captured a piece!\n";
                }
            }
        }
        else
        {
            // This is invalid
            return false;
        }

        // If a pawn reaches its eight rank, it must be promoted to another piece
        if ((Chess::isWhitePiece(chPiece) && 7 == future.iRow) ||
            (Chess::isBlackPiece(chPiece) && 0 == future.iRow))
        {
            // cout << "Pawn must be promoted!\n";
            S_promotion->bApplied = true;
        }
    }
    break;

    case 'R':
    {
        // Horizontal move
        if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFreeComputer(present, future, Chess::HORIZONTAL))
            {
                bValid = true;
            }
        }
        // Vertical move
        else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFreeComputer(present, future, Chess::VERTICAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'N':
    {
        if ((2 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }

        else if ((1 == abs(future.iRow - present.iRow)) && (2 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }
    }
    break;

    case 'B':
    {
        // Diagonal move
        if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFreeComputer(present, future, Chess::DIAGONAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'Q':
    {
        // Horizontal move
        if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFreeComputer(present, future, Chess::HORIZONTAL))
            {
                bValid = true;
            }
        }
        // Vertical move
        else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFreeComputer(present, future, Chess::VERTICAL))
            {
                bValid = true;
            }
        }

        // Diagonal move
        else if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFreeComputer(present, future, Chess::DIAGONAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'K':
    {
        // Horizontal move by 1
        if ((future.iRow == present.iRow) && (1 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }

        // Vertical move by 1
        else if ((future.iColumn == present.iColumn) && (1 == abs(future.iRow - present.iRow)))
        {
            bValid = true;
        }

        // Diagonal move by 1
        else if ((1 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)))
        {
            bValid = true;
        }

        // Castling
        else if ((future.iRow == present.iRow) && (2 == abs(future.iColumn - present.iColumn)))
        {
            // Castling is only allowed in these circunstances:

            // 1. King is not in check
            if (true == current_game->playerKingInCheck())
            {
                return false;
            }

            // 2. No pieces in between the king and the rook
            if (false == current_game->isPathFreeComputer(present, future, Chess::HORIZONTAL))
            {
                return false;
            }

            // 3. King and rook must not have moved yet;
            // 4. King must not pass through a square that is attacked by an enemy piece
            if (future.iColumn > present.iColumn)
            {
                // if future.iColumn is greather, it means king side
                if (false == current_game->castlingAllowed(Chess::Side::KING_SIDE, Chess::getPieceColor(chPiece)))
                {
                    createNextMessage("Castling to the king side is not allowed.\n");
                    return false;
                }
                else
                {
                    // Check if the square that the king skips is not under attack
                    Chess::UnderAttack square_skipped = current_game->isUnderAttack(present.iRow, present.iColumn + 1, current_game->getCurrentTurn());
                    if (false == square_skipped.bUnderAttack)
                    {
                        // Fill the S_castling structure
                        S_castling->bApplied = true;

                        // Present position of the rook
                        S_castling->rook_before.iRow = present.iRow;
                        S_castling->rook_before.iColumn = present.iColumn + 3;

                        // Future position of the rook
                        S_castling->rook_after.iRow = future.iRow;
                        S_castling->rook_after.iColumn = present.iColumn + 1; // future.iColumn -1

                        bValid = true;
                    }
                }
            }
            else // if (future.iColumn < present.iColumn)
            {
                // if present.iColumn is greather, it means queen side
                if (false == current_game->castlingAllowed(Chess::Side::QUEEN_SIDE, Chess::getPieceColor(chPiece)))
                {
                    createNextMessage("Castling to the queen side is not allowed.\n");
                    return false;
                }
                else
                {
                    // Check if the square that the king skips is not attacked
                    Chess::UnderAttack square_skipped = current_game->isUnderAttack(present.iRow, present.iColumn - 1, current_game->getCurrentTurn());
                    if (false == square_skipped.bUnderAttack)
                    {
                        // Fill the S_castling structure
                        S_castling->bApplied = true;

                        // Present position of the rook
                        S_castling->rook_before.iRow = present.iRow;
                        S_castling->rook_before.iColumn = present.iColumn - 4;

                        // Future position of the rook
                        S_castling->rook_after.iRow = future.iRow;
                        S_castling->rook_after.iColumn = present.iColumn - 1; // future.iColumn +1

                        bValid = true;
                    }
                }
            }
        }
    }
    break;

    default:
    {
        // cout << "!!!!Should not reach here. Invalid piece: " << char(chPiece) << "\n\n\n";
    }
    break;
    }

    // If it is a move in an invalid direction, do not even bother to check the rest
    if (false == bValid)
    {
        // cout << "Piece is not allowed to move to that square\n";
        return false;
    }

    // -------------------------------------------------------------------------
    // 2. Is there another piece of the same color on the destination square?
    // -------------------------------------------------------------------------
    if (current_game->isSquareOccupied(future.iRow, future.iColumn))
    {
        char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);
        if (Chess::getPieceColor(chPiece) == Chess::getPieceColor(chAuxPiece))
        {
            // cout << "Position is already taken by a piece of the same color\n";
            return false;
        }
    }

    // ----------------------------------------------
    // 3. Would the king be in check after the move?
    // ----------------------------------------------
    if (true == current_game->wouldKingBeInCheck(chPiece, present, future, S_enPassant))
    {
        // cout << "Move would put player's king in check\n";
        return false;
    }

    return bValid;
}

void makeTheMove(Chess::Position present, Chess::Position future, Chess::EnPassant *S_enPassant, Chess::Castling *S_castling, Chess::Promotion *S_promotion)
{
    char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);

    // -----------------------
    // Captured a piece?
    // -----------------------
    if (current_game->isSquareOccupied(future.iRow, future.iColumn))
    {
        char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);

        if (Chess::getPieceColor(chPiece) != Chess::getPieceColor(chAuxPiece))
        {
            createNextMessage(Chess::describePiece(chAuxPiece) + " captured!\n");
        }
        else
        {
            cout << "Error. We should not be making this move\n";
            throw("Error. We should not be making this move");
        }
    }
    else if (true == S_enPassant->bApplied)
    {
        createNextMessage("Pawn captured by \"en passant\" move!\n");
    }

    if ((true == S_castling->bApplied))
    {
        createNextMessage("Castling applied!\n");
    }

    current_game->movePiece(present, future, S_enPassant, S_castling, S_promotion);
}

void makeTheMoveComputer(Chess::Position present, Chess::Position future, Chess::EnPassant *S_enPassant, Chess::Castling *S_castling, Chess::Promotion *S_promotion)
{
    char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
    // -----------------------
    // Captured a piece?
    // -----------------------
    current_game->was_captured = false;
    if (current_game->isSquareOccupied(future.iRow, future.iColumn))
    {
        char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);

        if (Chess::getPieceColor(chPiece) != Chess::getPieceColor(chAuxPiece))
        {
            current_game->was_captured = true;
            // createNextMessage(Chess::describePiece(chAuxPiece) + " captured!\n");
        }
        else
        {
            cout << "Error. We should not be making this move\n";
            throw("Error. We should not be making this move");
        }
    }
    else if (true == S_enPassant->bApplied)
    {
        // createNextMessage("Pawn captured by \"en passant\" move!\n");
    }

    if ((true == S_castling->bApplied))
    {
        // createNextMessage("Castling applied!\n");
    }
    current_game->movePiece(present, future, S_enPassant, S_castling, S_promotion);
}

//---------------------------------------------------------------------------------------
// Commands
// Functions to handle the commands of the program
// New game, Move, Undo, Save game, Load game, etc
//---------------------------------------------------------------------------------------
void newGame(void)
{
    if (NULL != current_game)
    {
        delete current_game;
    }

    current_game = new Game();
}

void undoMove(void)
{
    if (false == current_game->undoIsPossible())
    {
        createNextMessage("Undo is not possible now!\n");
        return;
    }

    current_game->undoLastMove();
    createNextMessage("Last move was undone\n");
}

bool movePieceMain(void)
{
    bool success = false;
    std::string to_record;

    // Get user input for the piece they want to move
    cout << "Input move in standard algebraic notation (SAN): ";

    std::string move_from;
    getline(cin, move_from);

    // if (move_from.length() > 2)
    // {
    //     createNextMessage("You should type only two characters (column and row)\n");
    //     return success;
    // }

    Chess::Position present;
    Chess::Position future;
    future.iColumn = toupper(move_from[move_from.length() - 2]);
    future.iRow = move_from[move_from.length() - 1];

    cout << char(future.iColumn) << char(future.iRow) << endl;

    if (future.iColumn < 'A' || future.iColumn > 'H')
    {
        createNextMessage("Invalid input column.\n");
        return success;
    }

    if (future.iRow < '0' || future.iRow > '8')
    {
        createNextMessage("Invalid row.\n");
        return success;
    }

    // Put in the string to be logged
    to_record += move_from;

    // Convert columns from ['A'-'H'] to [0x00-0x07]
    future.iColumn = future.iColumn - 'A';

    // Convert row from ['1'-'8'] to [0x00-0x07]
    future.iRow = future.iRow - '1';

    // Parse algebraic notation

    // Special cases
    if (move_from == "O-O")
    {
        // Castling kingside
    }
    else if (move_from == "O-O-O")
    {
        // Castling queenside
    }
    else
    {
        // Some move other than castling

        if (move_from.length() == 2)
        {
            // Pawn move
            if (future.iRow + '1' == '4')
            {
                // Check if double move is allowed
                if (current_game->getPieceAtPosition(future.iRow - 2, future.iColumn) == 'P')
                {
                    // Set col and row
                    present.iColumn = toupper(future.iColumn + 'A');
                    present.iRow = (future.iRow - 2) + '1';
                }
                else if (current_game->getPieceAtPosition(future.iRow - 1, future.iColumn) == 'P')
                {
                    // Set col and row
                    present.iColumn = toupper(move_from[move_from.length() - 2]);
                    present.iRow = move_from[move_from.length() - 1] - 1;
                }
            }
            else
            {
                // Check if single move is allowed
                if (current_game->getPieceAtPosition(future.iRow - 1, future.iColumn) == 'P')
                {
                    // Set col and row
                    present.iColumn = toupper(move_from[move_from.length() - 2]);
                    present.iRow = move_from[move_from.length() - 1] - 1;
                }
            }
        }
        else if (move_from.length() == 3)
        {
            // Piece move
            // e.g., Nc4
            char piece = move_from[0];

            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    char pieceAtPosition = current_game->getPieceAtPosition(row, col);
                    if (pieceAtPosition == piece)
                    {
                        // This could be the piece to move
                        // Check if that move would be legal
                        if (isMoveValid())
                        {
                            // Move is valid, make it
                        }
                    }
                }
            }
        }
        else if (move_from.length() == 4)
        {
            // Pawn capture
            // e.g., dxe5
            // Check if regular capture is allowed from left
            // Check if regular capture is allowed from right
            // Clarify ambiguity
            // Check if en passant is allowed from left
            // Check if en passant is allowed from right
            // Clarify ambiguity

            // Or piece capture
            // e.g., Nxc4

            // Or piece move with 1 clarification
            // e.g., Nac4
        }
        else if (move_from.length() == 5)
        {
            // Piece capture with 1 clarification
            // e.g., Naxc4

            // Or piece move with 2 clarifications
            // e.g., Na3c4
        }
        else if (move_from.length() == 6)
        {
            // Piece capture with w clarifications
            // e.g., Na3xc4
        }

        // Check square on board

        // ---------------------------------------------------
        // Did the user pick a valid piece?
        // Must check if:
        // - It's inside the board (A1-H8)
        // - There is a piece in the square
        // - The piece is consistent with the player's turn
        // ---------------------------------------------------

        cout << char(present.iColumn) << char(present.iRow) << endl;

        if (present.iColumn < 'A' || present.iColumn > 'H')
        {
            createNextMessage("Invalid past column.\n");
            return success;
        }

        if (present.iRow < '0' || present.iRow > '8')
        {
            createNextMessage("Invalid row.\n");
            return success;
        }

        // Put in the string to be logged
        to_record += move_from;

        present.iColumn -= 'A';
        present.iRow -= '1';

        char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
        // current_game->printOutBoard();

        cout << "Piece is " << char(chPiece) << "\n";

        if (Chess::WHITE_PIECE == current_game->getCurrentTurn())
        {
            if (Chess::isBlackPiece(chPiece))
            {
                createNextMessage("It is WHITE's turn and you picked a BLACK piece\n");
                return success;
            }
        }
        else
        {
            if (Chess::isWhitePiece(chPiece))
            {
                createNextMessage("It is BLACK's turn and you picked a WHITE piece\n");
                return success;
            }
        }

        if (0x20 == chPiece)
        {
            createNextMessage("You picked an EMPTY square.\n");
            return success;
        }

        // ---------------------------------------------------
        // Get user input for the square to move to
        // ---------------------------------------------------
        // cout << "Move to: ";
        // std::string move_to;
        // getline(cin, move_to);

        // if (move_to.length() > 2)
        // {
        //     createNextMessage("You should type only two characters (column and row)\n");
        //     return success;
        // }

        // ---------------------------------------------------
        // Did the user pick a valid house to move?
        // Must check if:
        // - It's inside the board (A1-H8)
        // - The move is valid
        // ---------------------------------------------------

        // Check if it is not the exact same square
        if (future.iRow == present.iRow && future.iColumn == present.iColumn)
        {
            createNextMessage("[Invalid] You picked the same square!\n");
            return success;
        }

        // Is that move allowed?
        Chess::EnPassant S_enPassant = {0};
        Chess::Castling S_castling = {0};
        Chess::Promotion S_promotion = {0};

        if (false == isMoveValid(present, future, &S_enPassant, &S_castling, &S_promotion))
        {
            createNextMessage("[Invalid] Piece can not move to that square!\n");
            return success;
        }

        // ---------------------------------------------------
        // Promotion: user most choose a piece to
        // replace the pawn
        // ---------------------------------------------------
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

        // ---------------------------------------------------
        // Log the move: do it prior to making the move
        // because we need the getCurrentTurn()
        // ---------------------------------------------------
        current_game->logMove(to_record);

        // ---------------------------------------------------
        // Make the move
        // ---------------------------------------------------
        makeTheMove(present, future, &S_enPassant, &S_castling, &S_promotion);

        // ---------------------------------------------------------------
        // Check if this move we just did put the oponent's king in check
        // Keep in mind that player turn has already changed
        // ---------------------------------------------------------------
        if (true == current_game->playerKingInCheck())
        {
            if (true == current_game->isCheckMate())
            {
                if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
                {
                    appendToNextMessage("Checkmate! Black wins the game!\n");
                }
                else
                {
                    appendToNextMessage("Checkmate! White wins the game!\n");
                }
            }
            else
            {
                // Add to the string with '+=' because it's possible that
                // there is already one message (e.g., piece captured)
                if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
                {
                    appendToNextMessage("White king is in check!\n");
                }
                else
                {
                    appendToNextMessage("Black king is in check!\n");
                }
            }
        }

        return true;
    }
}

bool movePieceComputerInt(int from_x, int from_y, int to_x, int to_y, bool debug)
{

    std::string to_record;

    Chess::Position present;
    present.iColumn = chess_cols[from_x];
    present.iRow = chess_rows[from_y];

    // Put in the string to be logged
    to_record += present.iColumn;
    to_record += present.iRow;
    to_record += "-";

    // Convert column from ['A'-'H'] to [0x00-0x07]
    present.iColumn = from_x; // present.iColumn - 'A';

    // Convert row from ['1'-'8'] to [0x00-0x07]
    present.iRow = from_y; // present.iRow - '1';

    char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);

    Chess::Position future;
    future.iColumn = chess_cols[to_x];
    future.iRow = chess_rows[to_y];

    to_record += future.iColumn;
    to_record += future.iRow;

    // Convert columns from ['A'-'H'] to [0x00-0x07]
    future.iColumn = future.iColumn - 'A';

    // Convert row from ['1'-'8'] to [0x00-0x07]
    future.iRow = future.iRow - '1';

    // Check if it is not the exact same square
    if (future.iRow == present.iRow && future.iColumn == present.iColumn)
    {
        // createNextMessage("[Invalid] You picked the same square!\n");
        return false;
    }

    // Is that move allowed?
    Chess::EnPassant S_enPassant = {0};
    Chess::Castling S_castling = {0};
    Chess::Promotion S_promotion = {0};

    if (false == isMoveValidComputer(present, future, &S_enPassant, &S_castling, &S_promotion))
    {
        // createNextMessage("[Invalid] Piece can not move to that square!\n");
        if (debug)
            cout << "You cannot move the piece to that square";
        return false;
    }

    if (S_promotion.bApplied == true)
    {
        char chPromoted = 'Q';

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

    // ---------------------------------------------------
    // Log the move: do it prior to making the move
    // because we need the getCurrentTurn()
    // ---------------------------------------------------
    current_game->logMove(to_record);

    // ---------------------------------------------------
    // Make the move
    // ---------------------------------------------------
    makeTheMoveComputer(present, future, &S_enPassant, &S_castling, &S_promotion);

    // ---------------------------------------------------------------
    // Check if this move we just did put the oponent's king in check
    // Keep in mind that player turn has already changed
    // ---------------------------------------------------------------
    if (true == current_game->playerKingInCheck())
    {
        if (true == current_game->isCheckMate())
        {
            if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
            {
                if (debug)
                    cout << "CHECKMATE BLACK WINS";
                // appendToNextMessage("Checkmate! Black wins the game!\n");
            }
            else
            {
                if (debug)
                    cout << "CHECKMATE WHTIE WINS";
                // appendToNextMessage("Checkmate! White wins the game!\n");
            }
        }
        else
        {
            // Add to the string with '+=' because it's possible that
            // there is already one message (e.g., piece captured)
            if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
            {
                if (debug)
                    cout << "WHITE KING IN CHECK";
                // appendToNextMessage("White king is in check!\n");
            }
            else
            {
                if (debug)
                    cout << "BLACK KING IN CHECK";
                // appendToNextMessage("Black king is in check!\n");
            }
        }
    }

    return true;
}

void undoMoveComputer()
{
    // if (false == current_game->undoIsPossible()) {
    //     cout << "UNDO IS IMPOSSIBLE";
    // }
    current_game->undoLastMoveComputer();
}

void saveGame(void)
{
    string file_name;
    cout << "Type file name to be saved (no extension): ";

    getline(cin, file_name);
    file_name += ".dat";

    std::ofstream ofs(file_name);
    if (ofs.is_open())
    {
        // Write the date and time of save operation
        auto time_now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(time_now);
        ofs << "[Chess console] Saved at: " << std::ctime(&end_time);

        // Write the moves
        for (unsigned i = 0; i < current_game->rounds.size(); i++)
        {
            ofs << current_game->rounds[i].white_move.c_str() << " | " << current_game->rounds[i].black_move.c_str() << "\n";
        }

        ofs.close();
        createNextMessage("Game saved as " + file_name + "\n");
    }
    else
    {
        cout << "Error creating file! Save failed\n";
    }

    return;
}

void loadGame(void)
{
    string file_name;
    cout << "Type file name to be loaded (no extension): ";

    getline(cin, file_name);
    file_name += ".dat";

    std::ifstream ifs(file_name);

    if (ifs)
    {
        // First, reset the pieces
        if (NULL != current_game)
        {
            delete current_game;
        }

        current_game = new Game();

        // Now, read the lines from the file and then make the moves
        std::string line;

        while (std::getline(ifs, line))
        {
            // Skip lines that starts with "[]"
            if (0 == line.compare(0, 1, "["))
            {
                continue;
            }

            // There might be one or two moves in the line
            string loaded_move[2];

            // Find the separator and subtract one
            std::size_t separator = line.find(" |");

            // For the first move, read from the beginning of the string until the separator
            loaded_move[0] = line.substr(0, separator);

            // For the second move, read from the separator until the end of the string (omit second parameter)
            loaded_move[1] = line.substr(line.find("|") + 2);

            for (int i = 0; i < 2 && loaded_move[i] != ""; i++)
            {
                // Parse the line
                Chess::Position from;
                Chess::Position to;

                char chPromoted = 0;

                current_game->parseMove(loaded_move[i], &from, &to, &chPromoted);

                // Check if line is valid
                if (from.iColumn < 0 || from.iColumn > 7 ||
                    from.iRow < 0 || from.iRow > 7 ||
                    to.iColumn < 0 || to.iColumn > 7 ||
                    to.iRow < 0 || to.iRow > 7)
                {
                    createNextMessage("[Invalid] Can't load this game because there are invalid lines!\n");

                    // Clear everything and return
                    current_game = new Game();
                    return;
                }

                // Is that move allowed? (should be because we already validated before saving)
                Chess::EnPassant S_enPassant = {0};
                Chess::Castling S_castling = {0};
                Chess::Promotion S_promotion = {0};

                if (false == isMoveValid(from, to, &S_enPassant, &S_castling, &S_promotion))
                {
                    createNextMessage("[Invalid] Can't load this game because there are invalid moves!\n");

                    // Clear everything and return
                    current_game = new Game();
                    return;
                }

                // ---------------------------------------------------
                // A promotion occurred
                // ---------------------------------------------------
                if (S_promotion.bApplied == true)
                {
                    if (chPromoted != 'Q' && chPromoted != 'R' && chPromoted != 'N' && chPromoted != 'B')
                    {
                        createNextMessage("[Invalid] Can't load this game because there is an invalid promotion!\n");

                        // Clear everything and return
                        current_game = new Game();
                        return;
                    }

                    S_promotion.chBefore = current_game->getPieceAtPosition(from.iRow, from.iColumn);

                    if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
                    {
                        S_promotion.chAfter = toupper(chPromoted);
                    }
                    else
                    {
                        S_promotion.chAfter = tolower(chPromoted);
                    }
                }

                // Log the move
                current_game->logMove(loaded_move[i]);

                // Make the move
                makeTheMove(from, to, &S_enPassant, &S_castling, &S_promotion);
            }
        }

        // Extra line after the user input
        createNextMessage("Game loaded from " + file_name + "\n");

        return;
    }
    else
    {
        createNextMessage("Error loading " + file_name + ". Creating a new game instead\n");
        current_game = new Game();
        return;
    }
}

int minimaxAB(int depth, int alpha, int beta, int white)
{

    if (current_game->isFinished() && current_game->isCheckMate())
    {
        // cout << "GOT MATE!" << endl;
        if (current_game->getCurrentTurn() == 1)
        {
            // white to play, but white is in checkmate. so white has lost
            return -1000;
        }
        else
        {
            return 1000; // otherwise white has won!
        }
    }
    if (depth == 0)
    {
        // return heuristic evaluation function
        return helperClass::getValueBoardPointer(current_game);
    }
    // move stuff
    int moves_piece[16 * 20 * 2]; // max theoretical moves = 16(nr pieces)*32(nr moves per piece) *2(coordinates)
    moves_piece[0] = 1;
    int best_move_value = 1000;
    if (white)
    {
        best_move_value = -1000;
    }
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (white == helperClass::iswhitepiece(current_game->board[x][y]))
            {
                int start_idx = moves_piece[0];
                helperClass::get_Moves_Piece_INTARR(current_game->board[x][y], y, x, moves_piece, start_idx);
                for (int a = start_idx; a < moves_piece[0]; a++)
                { // for every move on board
                    // cout << "x: " << x << "y: " << y << "move to: " << moveTo[0] << " " << moveTo[1] << "number elements: " << *moves_piece << endl;
                    bool success = movePieceComputerInt(y, x, moves_piece[a], moves_piece[a + 1], false);
                    if (success)
                    {
                        total_moves++;
                        if (white)
                        {
                            best_move_value = std::max(best_move_value, minimaxAB(depth - 1, alpha, beta, 0));
                            if (best_move_value >= beta)
                            {
                                undoMoveComputer();
                                return best_move_value;
                            }
                            alpha = std::max(alpha, best_move_value);
                        }
                        else
                        {
                            best_move_value = std::min(best_move_value, minimaxAB(depth - 1, alpha, beta, 1));
                            if (best_move_value <= alpha)
                            {
                                undoMoveComputer();
                                return best_move_value;
                            }
                            beta = std::min(beta, best_move_value);
                        }
                        undoMoveComputer();
                    }
                    a++;
                }
            }
        }
    }
    return best_move_value;
}

void computer_play_move(int white)
{
    if (depth < 1)
    {
        cout << "Error, depth must be at least 1!" << endl
             << "Depth set to 1!" << endl;
        depth = 1;
    }
    // the top minmax function that does not undo the best move it's
    vector<vector<char>> board_vector = current_game->getAllPieces();
    vector<vector<int>> Pieces = helperClass::getPiecesPosition(board_vector, white); // get black pieces
    vector<int> best_from;
    vector<int> best_to;
    vector<int> PiecePos;
    vector<int> moveTo;
    int best_value = (white * -2000) + 1000; //-1000 if white, +1000 if white == 1.
    int moves_considered = 0;
    auto start = std::chrono::system_clock::now();
    for (int x = 0; x < Pieces.size(); x++)
    {
        PiecePos = Pieces[x];
        vector<vector<int>> moves_piece = helperClass::get_Moves_Piece(board_vector[PiecePos[1]][PiecePos[0]], PiecePos[0], PiecePos[1]);
        for (int a = 0; a < moves_piece.size(); a++)
        { // for every move on board
            moveTo = moves_piece[a];
            bool success = movePieceComputerInt(PiecePos[0], PiecePos[1], moveTo[0], moveTo[1], false);
            // debug
            // if (current_game->board[PiecePos[1]][PiecePos[0]] == 'k') cout << "move from col: " << PiecePos[0] << " row: " << PiecePos[1] << "Move to: " << moveTo[0] << moveTo[1] << "SUCCESS: " << success << endl;
            if (success)
            {
                moves_considered++;
                int board_value = minimaxAB(depth - 1, -1000, 1000, abs(white - 1));
                if (white)
                {
                    if (board_value > best_value)
                    {
                        best_value = board_value;
                        best_from = PiecePos;
                        best_to = moveTo;
                    }
                }
                else
                {
                    if (board_value < best_value)
                    {
                        best_value = board_value;
                        best_from = PiecePos;
                        best_to = moveTo;
                    }
                }

                undoMoveComputer();
            }
        }
    }
    // do the move
    cout << "Number of legal moves black: " << moves_considered << endl;
    bool success = movePieceComputerInt(best_from[0], best_from[1], best_to[0], best_to[1], true);
    if (success == false)
    {
        throw("Something bad");
    }
    bool debug = true;
    if (debug)
    {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        clearScreen(); // these should be in the main function
        printSituation(*current_game);
        printBoard(*current_game);
        cout << "elapsed time: " << elapsed_seconds.count() << "s" << endl;
        cout << "Heuristic value: " << best_value << endl;
    }
}

int main()
{
    bool bRun = true;

    // Clear screen an print the logo
    clearScreen();
    printLogo();

    string input = "";

    while (bRun)
    {
        printMessage();
        printMenu();

        // Get input from user
        cout << "Type here: ";
        getline(cin, input);

        if (input.length() != 1)
        {
            // Check if input is valid algebraic notation
            if (input.length() == 2)
            {
                // Pawn move, e.g. d4
            }
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
                    if (current_game->isFinished())
                    {
                        cout << "This game has already finished!\n";
                    }
                    else
                    {
                        bool move_succesful = movePieceMain();
                        clearScreen();
                        printSituation(*current_game);
                        printBoard(*current_game);

                        if (false == current_game->isCheckMate() && move_succesful)
                        {
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
                    // clearScreen();
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
        catch (const char *s)
        {
            s;
        }
    }

    return 0;
}
