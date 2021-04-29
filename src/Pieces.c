//  Pieces.c
//  Chesster Team 3

#include "Pieces.h"

int CallPiece(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    char piecetype = piece->piecetype;
    switch(piecetype)
    {
        case 'P':
            return(MovePawn(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions));
            break;
        case 'R':
            return(MoveRook(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions));
            break;
        case 'N':
            return(MoveKnight(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions));
            break;
        case 'K':
            return(MoveKing(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions));
            break;
        case 'Q':
            return(MoveQueen(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions));
            break;
        case 'B':
            return(MoveBishop(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions));
            break;
    }
    return 1;
}

int MovePawn(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    if(FindEmptySpace(board, dest_row, dest_col) == 1) // moving pawn to empty space
    {
        if(piece->player->color == 'w')
        {
            if((piece->value == 2) && (src_col == dest_col) && ((dest_row == src_row + 1) || (dest_row == src_row + 2)))
            {
                if(dest_row == src_row + 2 && (CheckPiece(piece->player, src_row+1, src_col) == NULL) && (CheckPiece(opponent, src_row+1, src_col) == NULL))
                {
                    if(test_conditions == 1)
                    {
                        piece->value = 1;
                    }
                    if(CheckPiece(opponent, dest_row, dest_col+1) != NULL || CheckPiece(opponent, dest_row, dest_col-1) != NULL)
                    {
                        piece->EnPassant = 1;
                    }
                    return 0;
                }
                else if(dest_row == src_row + 1)
                {
                    if(test_conditions == 1)
                    {
                        piece->value = 1;
                    }
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else if((piece->value == 1) && (src_col == dest_col) && (dest_row == src_row + 1))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if((piece->value == 2) && (src_col == dest_col) && ((src_row == dest_row + 1) || (src_row == dest_row + 2)))
            {
                if(src_row == dest_row + 2 && (CheckPiece(piece->player, src_row -1, src_col) == NULL) && (CheckPiece(opponent, src_row -1, src_col) == NULL))
                {
                    if(test_conditions == 1)
                    {
                        piece->value = 1;
                    }
                    if(CheckPiece(opponent, dest_row, dest_col+1) != NULL || CheckPiece(opponent, dest_row, dest_col-1) != NULL)
                    {
                        piece->EnPassant = 1;
                    }
                    return 0;
                }
                else if(src_row == dest_row + 1)
                {
                    if(test_conditions == 1)
                    {
                        piece->value = 1;
                    }
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else if((piece->value == 1) && (src_col == dest_col) && (src_row == dest_row + 1))
            {
                return 0;
            }
            else
            {
                return 1;
            }
               
        }
    }
    else if(CheckPiece(piece->player, dest_row, dest_col) != NULL) // moving pawn to a space with your own piece
    {
        return 1;
    }
    else if(CheckPiece(opponent, dest_row, dest_col) != NULL)// otherwise must be moving to a space with a different player's piece (a capture)
    {
        if(piece->player->color == 'w')
        {
            if(((dest_col == src_col + 1) || (dest_col == src_col - 1)) && (dest_row == src_row + 1))
            {
                if(test_conditions == 1)
                {
                    PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                    CapturePiece(board, opponentpiece);
                    if(piece->value == 2)
                    {
                        piece->value = 1;
                    }
                    return 2;
                }
                return 2;
            }
            else if(((dest_col == src_col + 1) || (dest_col == src_col - 1)) && (dest_row == src_row))
            {
                PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                if(opponentpiece != NULL)
                {
                    if(opponentpiece->EnPassant == 1)
                    {
                        if(test_conditions == 1)
                        {
                            CapturePiece(board, opponentpiece);
                            opponentpiece->EnPassant = 0;
                            return 2;
                        }
                        else
                        {
                            return 2;
                        }
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(((dest_col == src_col - 1) || (dest_col == src_col + 1)) && (dest_row == src_row - 1))
            {
                if(test_conditions == 1)
                {
                    PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                    CapturePiece(board, opponentpiece);
                    return 2;
                }
                return 2;
            }
            else if(((dest_col == src_col + 1) || (dest_col == src_col - 1)) && (dest_row == src_row))
            {
                PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                if(opponentpiece != NULL)
                {
                    if(opponentpiece->EnPassant == 1)
                    {
                        if(test_conditions == 1)
                        {
                            CapturePiece(board, opponentpiece);
                            opponentpiece->EnPassant = 0;
                            return 2;
                        }
                        else
                        {
                            return 2;
                        }
                    }
                }
            }
            else
            {
                return 1;
            }
        }
    }
    return 1;
}
int MoveRook(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    if(FindEmptySpace(board, dest_row, dest_col) == 1 || CheckPiece(opponent, dest_row, dest_col) != NULL) // moving rook to empty space or a capture space
    {
        if(src_row == dest_row && src_col != dest_col) // horizontal
        {
            if(src_col < dest_col)
            {
                for(int i = src_col + 1; i < dest_col; i++)
                {
                    if(FindEmptySpace(board, src_row, i) == 0)
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
            if(src_col > dest_col)
            {
                for(int i = src_col - 1; i > dest_col; i--)
                {
                    if(FindEmptySpace(board, src_row, i) == 0)
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
        }
        else if(src_row != dest_row && src_col == dest_col) // vertical
        {
            if(src_row < dest_row)
            {
                for(int i = src_row + 1; i < dest_row; i++)
                {
                    if(FindEmptySpace(board, i, src_col) == 0)
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
            if(src_row > dest_row)
            {
                for(int i = src_row - 1; i > dest_row; i--)
                {
                    if(FindEmptySpace(board, i, src_col) == 0)
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
        }
        else
        {
            return 1;
        }
        return 0;
    }
    else // moving rook to a space with your own piece
    {
        return 1;
        
    }
}
int MoveKnight(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    if(FindEmptySpace(board, dest_row, dest_col) == 1 || CheckPiece(opponent, dest_row, dest_col) != NULL) // moving knight to empty space or a capture space
    {
        if(((dest_row == src_row+1) && (dest_col == src_col+2)) || ((dest_row == src_row+2) && (dest_col == src_col+1)) || ((dest_row == src_row+2) && (dest_col == src_col-1)) || ((dest_row == src_row+1) && (dest_col == src_col-2)) || ((dest_row == src_row-1) && (dest_col == src_col-2)) || ((dest_row == src_row-2) && (dest_col == src_col-1)) || ((dest_row == src_row-2) && (dest_col == src_col+1)) || ((dest_row == src_row-1) && (dest_col == src_col+2)))
        {
           if(CheckPiece(opponent, dest_row, dest_col) != NULL)
            {
                if(test_conditions == 1)
                {
                    PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                    CapturePiece(board, opponentpiece);
                    return 2;
                }
                return 2;
            }
            return 0;
        }
        return 1;
    }
    else // moving knight to a space with your own piece
    {
        return 1;
        
    }
}
int MoveBishop(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    if(FindEmptySpace(board, dest_row, dest_col) == 1 || CheckPiece(opponent, dest_row, dest_col) != NULL) // moving bishop to empty space or a capture space
    {
        int dRow = 0; // change in row value
        int dCol = 0; // change in column value
        if((dest_row < src_row) && (dest_col < src_col)) // moving to bottom left
        {
            dRow = src_row - dest_row;
            dCol = src_col - dest_col;
            if(dRow == dCol)
            {
                for(int i = 1; i < dRow; i++)
                {
                    if(FindEmptySpace(board, src_row-i, src_col-i) == 0) // if there is a piece in the way
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
            return 1;
        }
        else if((dest_row < src_row) && (dest_col > src_col)) // moving to bottom right
        {
            dRow = src_row - dest_row;
            dCol = dest_col - src_col;
            if(dRow == dCol)
            {
                for(int i = 1; i < dRow; i++)
                {
                    if(FindEmptySpace(board, src_row-i, src_col+i) == 0) // if there is a piece in the way
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
            return 1;
        }
        else if((dest_row > src_row) && (dest_col < src_col)) // moving to top left
        {
            dRow = dest_row - src_row;
            dCol = src_col - dest_col;
            if(dRow == dCol)
            {
                for(int i = 1; i < dRow; i++)
                {
                    if(FindEmptySpace(board, src_row+i, src_col-i) == 0) // if there is a piece in the way
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
            return 1;
        }
        else if((dest_row > src_row) && (dest_col > src_col)) // moving to top right
        {
            dRow = dest_row - src_row;
            dCol = dest_col - src_col;
            if(dRow == dCol)
            {
                for(int i = 1; i < dRow; i++)
                {
                    if(FindEmptySpace(board, src_row+i, src_col+i) == 0) // if there is a piece in the way
                    {
                        return 1;
                    }
                }
                if(CheckPiece(opponent, dest_row, dest_col) != NULL)
                {
                    if(test_conditions == 1)
                    {
                        PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                        CapturePiece(board, opponentpiece);
                        return 2;
                    }
                    return 2;
                }
                return 0;
            }
            return 1;
        }
        else
        {
            return 1;
        }
    }
    else // moving bishop to a space with your own piece
    {
        return 1;
        
    }
}
int MoveKing(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    int check3Left = 0;
    int check2Right = 0;
    int check2Left = 0;
    int check1Right = 0;
    int check1Left = 0;
    if(FindEmptySpace(board, dest_row, dest_col) == 1 || CheckPiece(opponent, dest_row, dest_col) != NULL) // moving king to empty space or a capture space
    {
        if(((dest_row == src_row+1) && (dest_col == src_col-1)) || ((dest_row == src_row+1) && (dest_col == src_col)) || ((dest_row == src_row+1) && (dest_col == src_col+1)) || ((dest_row == src_row) && (dest_col == src_col-1)) || ((dest_row == src_row) && (dest_col == src_col+1)) || ((dest_row == src_row-1) && (dest_col == src_col-1)) || ((dest_row == src_row-1) && (dest_col == src_col)) || ((dest_row == src_row-1) && (dest_col == src_col+1)))
            
        {
            if(CheckPiece(opponent, dest_row, dest_col) != NULL)
            {
                if(test_conditions == 1)
                {
                    PIECE *opponentpiece = CheckPiece(opponent, dest_row, dest_col);
                    CapturePiece(board, opponentpiece);
                    return 2;
                }
                return 2;
            }
            return 0;
        }
        //Castling
        if(piece->castling == 1 && test_conditions !=2)
        {
            if((dest_row == src_row) && (dest_col == src_col+2) && piece->player->piecelist[Rook2]->castling == 1) //Castle Kingside
            {
                //Checking Rules
                if(FindEmptySpace(board, src_row, src_col+1) == 0 || FindEmptySpace(board, src_row, src_col+2) == 0) // castling prohibited through other pieces
                {
                    if(piece->player->type == 'h' && test_conditions == 1)
                    {
                        printf("Castling is illegal with other pieces in the way.\n");
                        return 1;
                    }
                }
                if(Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1) == 1) // castling prohibited while in check
                {
                    if(piece->player->type == 'h' && test_conditions == 1)
                    {
                        printf("You cannot castle while in check. \n");
                        return 1;
                    }
                }
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c+2;
                check2Right = Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1); // castling prohibited through check
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c-1;
                check1Right = Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1);
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c-1;
                if(check2Right == 1 || check1Right == 1)
                {
                    if(piece->player->type == 'h' && test_conditions == 1)
                    {
                        printf("You cannot castle through check. \n");
                        return 1;
                    }
                }
                return 0;
                
            }
            
            else if((dest_row == src_row) && (dest_col == src_col-2) && piece->player->piecelist[Rook1]->castling == 1) //Castle Queenside
            {
                //Checking Rules
                if(FindEmptySpace(board, src_row, src_col-1) == 0 || FindEmptySpace(board, src_row, src_col-2) == 0 || FindEmptySpace(board, src_row, src_col-3) == 0) // castling prohibited through other pieces
                {
                    if(piece->player->type == 'h' && test_conditions == 1)
                    {
                        printf("Castling is illegal with other pieces in the way.\n");
                        return 1;
                    }
                }
                if(Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1) == 1) // castling prohibited while in of check
                {
                    if(piece->player->type == 'h' && test_conditions == 1)
                    {
                        printf("You cannot castle while in check. \n");
                        return 1;
                    }
                }
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c-3;
                check3Left = Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1); // castling prohibited through check
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c+1;
                check2Left = Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1);
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c+1;
                check1Left = Check(board, opponent, piece->player, (piece->player->piecelist[King]->r)+1, (piece->player->piecelist[King]->c)+1);
                piece->player->piecelist[King]->c = piece->player->piecelist[King]->c+1;
                if(check3Left == 1 || check2Left == 1 || check1Left == 1)
                {
                    if(piece->player->type == 'h' && test_conditions == 1)
                    {
                        printf("You cannot castle through check. \n");
                        return 1;
                    }
                }
                return 0;
                
            }
        }
        return 1;
    }
    else  // moving king to a space with your own piece
    {
        return 1;
    }
}
int MoveQueen(BOARD *board, PLAYER *opponent, PIECE *piece, int src_row, int src_col, int dest_row, int dest_col, int test_conditions)
{
    int rookreturn = MoveRook(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions);
    int bishopreturn = MoveBishop(board, opponent, piece, src_row, src_col, dest_row, dest_col, test_conditions);
    if(rookreturn == 0 || rookreturn == 2)
    {
        return rookreturn;
    }
    else if(bishopreturn == 0 || bishopreturn == 2)
    {
        return bishopreturn;
    }
    else
    {
        return 1; // moving queen to a space with your own piece
    }
}

