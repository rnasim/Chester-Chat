//  Conditions.c
//  Chesster Team 3

#include "Conditions.h"

int Check(BOARD *board, PLAYER *player, PLAYER *opponent, int king_row, int king_col)
{
    int piecereturn = 0;
    for(int i = Pawn1; i <= King; i++)
    {
        if(player->piecelist[i]->r == 9)
        {
            continue;
        }
        piecereturn = CallPiece(board, opponent, player->piecelist[i], (player->piecelist[i]->r)+1, (player->piecelist[i]->c)+1, king_row, king_col, 0);
        if(piecereturn == 2)
        {
            return 1; // king is in check automatically
        }
    }
    return 0;
}
int Checkmate(BOARD *board, PLAYER *player, PLAYER *opponent)
{
    PIECE *piece = NULL;
    int originalR = 0;
    int originalC = 0;
    char *originalpiecetag = "nm";
    int original_piece_value = 0;
    int movereturn = 0;
    PIECE *opponentcapture = NULL;
    int opponent_r = 0;
    int opponent_c = 0;
    int opponent_value = 0;
    char *piecetag = "nm";
    for(int i = Pawn1; i <= King; i++)
    {
        piece = opponent->piecelist[i];
        if(piece->value == 0) // captured piece cannot be moved to save the king
        {
            continue;
        }
        originalR = piece->r;
        originalC = piece->c;
        originalpiecetag = board->boardarray[piece->r][piece->c];
        original_piece_value = piece->value;
        movereturn = 0;
        opponentcapture = NULL;
        opponent_r = 0;
        opponent_c = 0;
        opponent_value = 0;
        piecetag = NULL;
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                int IsLegalMove = CallPiece(board, player, piece, (piece->r)+1, (piece->c)+1, j+1, k+1, 0); // can the opponent make a legal move?
                if(IsLegalMove == 1)
                {
                    continue;
                }
                else // if so, can that legal move get them out of check?
                {
                    char *originaldesttag = board->boardarray[j][k];
                    if(IsLegalMove == 2)
                    {
                        opponentcapture = CheckPiece(player, j+1, k+1);
                        opponent_r = opponentcapture->r;
                        opponent_c = opponentcapture->c;
                        piecetag = board->boardarray[opponent_r][opponent_c];
                        opponent_value = opponentcapture->value;
                        opponentcapture->r = 9;
                        opponentcapture->c = 9;
                        opponentcapture->value = 0;
                    }
                    movereturn = MovePiece(board, player, piece, j, k, 0);
                    if(movereturn == 1) // if the test move made puts/leaves the opponent in check
                    {
                        if(IsLegalMove == 2) // if move is a potential capture move, reset captures and value updates
                        {
                            UndoCapture(board, opponentcapture, opponent_r, opponent_c, opponent_value, piecetag);
                        }
                        if(piece->value == 1 && original_piece_value == 2)
                        {
                            piece->value = 2;
                        }
                    }
                    else
                    {
                        if(opponentcapture != NULL)
                        {
                            opponentcapture->r = opponent_r;
                            opponentcapture->c = opponent_c;
                            opponentcapture->value = opponent_value;
                            board->boardarray[opponent_r][opponent_c] = piecetag;
                        }
                        board->boardarray[j][k] = originaldesttag;
                        board->boardarray[originalR][originalC] = originalpiecetag;
                        piece->r = originalR;
                        piece->c = originalC;
                        return 0;
                    }
                    
                }
                
            }
        }
    }
    return 1;
}
int Promotion(BOARD *board, PIECE *piece)
{
    char choice = '\0';
    if(piece->player->type == 'a')
    {
        if(piece->player->color == 'w' &&  piece->r == 7)
        {
            piece->value = 9;
            piece->piecetype = 'Q';
            board->boardarray[piece->r][piece->c] = "wQ";
            return 1;
        }
        else if(piece->player->color == 'b' && piece->r == 0)
        {
            piece->value = 9;
            piece->piecetype = 'Q';
            board->boardarray[piece->r][piece->c] = "bQ";
            return 1;
        }
    }
    if(piece->player->color == 'w')
    {
        if(piece->r == 7)
        {
            printf("Pawn Promotion: Please select a piece to promote this pawn to. Enter Q for queen, N for knight, B for bishop, or R for rook.\n");
            scanf(" %c", &choice);
        }
    }
    else if(piece->player->color == 'b')
    {
        if(piece->r == 0)
        {
            printf("Pawn Promotion: Please select a piece to promote this pawn to. Enter Q for queen, N for knight, B for bishop, or R for rook.\n");
            scanf(" %c", &choice);
        }
    }
    if(choice == 'Q' || choice == 'q')
    {
        piece->value = 9;
        piece->piecetype = 'Q';
        if(piece->player->color == 'w')
        {
            board->boardarray[piece->r][piece->c] = "wQ";
        }
        else
        {
            board->boardarray[piece->r][piece->c] = "bQ";
        }
        return 1;
    }
    else if(choice == 'N' || choice == 'n')
    {
        piece->value = 3;
        piece->piecetype = 'N';
        if(piece->player->color == 'w')
        {
            board->boardarray[piece->r][piece->c] = "wN";
        }
        else
        {
            board->boardarray[piece->r][piece->c] = "bN";
        }
        return 1;
    }
    else if(choice == 'B' || choice == 'b')
    {
        piece->value = 3;
        piece->piecetype = 'B';
        if(piece->player->color == 'w')
        {
            board->boardarray[piece->r][piece->c] = "wB";
        }
        else
        {
            board->boardarray[piece->r][piece->c] = "bB";
        }
        return 1;
    }
    else if(choice == 'R' || choice == 'r')
    {
        piece->value = 5;
        piece->piecetype = 'R';
        if(piece->player->color == 'w')
        {
            board->boardarray[piece->r][piece->c] = "wR";
        }
        else
        {
            board->boardarray[piece->r][piece->c] = "bR";
        }
        return 1;
    }
    return 0;
}
void CapturePiece(BOARD *board, PIECE *piece)
{
    if(piece->piecetype != 'K')
    {
        board->boardarray[piece->r][piece->c] = "  ";
        piece->r = 9; // 9 = off board
        piece->c = 9; // 9 = off board
        piece->value = 0;
        piece = NULL;
    }
    
}
void UndoCapture(BOARD *board, PIECE *opponentcapture, int opponent_r, int opponent_c, int opponent_value, char *piecetag)
{
    board->boardarray[opponent_r][opponent_c] = piecetag;
    opponentcapture->r = opponent_r;
    opponentcapture->c = opponent_c;
    opponentcapture->value = opponent_value;
}
int CheckNumberofIllegalMoves(BOARD *board, PLAYER *opponent, PIECE *piece, int row_src, int col_src)
{
    int counter = 0;
    for(int i = 1; i <= 8; i++)
    {
        for(int j = 1; j <= 8; j++)
        {
            if(CallPiece(board, opponent, piece, row_src, col_src, i, j, 2) == 1)
            {
                counter++;
            }
        }
    }
    return counter;
}
int Stalemate(BOARD *board, PLAYER *player, PLAYER *opponent)
{
    if(Check(board, opponent, player, (player->piecelist[King]->r)+1, (player->piecelist[King]->c)+1)== 0)
    {
        if(Checkmate(board, opponent, player) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int FiftyConsec (MOVELIST *movelist){
	MOVE *CurrentMove = movelist -> last;
	for (int i = 0; i < 50 ; i++){

		if (CurrentMove -> IsCaptured == 1 || CurrentMove -> piece -> piecetype == 'P'){
			return 0;
		}		
		CurrentMove = CurrentMove -> preventry;
}
		return 1;

}

int ThreeFoldRep (MOVELIST *movelist){
    MOVE *CurrentMove = movelist -> last;
    MOVE *ReferenceMove = movelist -> last -> preventry -> preventry;
    int MoveCounter = 0;
    while (ReferenceMove != NULL){
        
        if (CurrentMove -> piece -> piecetype == ReferenceMove -> piece -> piecetype){
            if (CurrentMove -> dst_col == ReferenceMove -> dst_col){
                if (CurrentMove -> dst_row == ReferenceMove -> dst_row){
                    
                    MoveCounter += 1;
                }
            }
        }
        if (MoveCounter == 2){
            return 1;
        }
        if(ReferenceMove->preventry == NULL)
        {
            return 0;
        }
        ReferenceMove = ReferenceMove -> preventry -> preventry;
    }
    return 0;    
}
void GetHints(BOARD *board, PLAYER *player, PLAYER *opponent)
{
    printf("\nHints:\n");
    MOVELIST *hints = NewMoveList();
    getmoves(board->boardarray, board, player, opponent, hints);
    MOVE *current = hints->first;
    char *piece =  NULL;
    int movecounter = 0;
    char src_col = '\0';
    char dst_col = '\0';
    while(current != NULL && movecounter < 3)
    {
        if(current->IsCaptured == 1)
        {
            src_col = NumtoAlpha(current->src_col+1);
            dst_col = NumtoAlpha(current->dst_col+1);
            piece = PieceName(current->piece->piecetype);
            printf("Capture! Move %s from %c%d to %c%d \n", piece, src_col, current->src_row+1, dst_col, current->dst_row+1);
            movecounter++;
        }
        current = current->nextentry;
    }
    if(movecounter < 3)
    {
        current = hints->first;
        while(current != NULL && movecounter < 3)
        {
            if(current->IsCaptured == 0)
            {
                src_col = NumtoAlpha(current->src_col+1);
                dst_col = NumtoAlpha(current->dst_col+1);
                piece = PieceName(current->piece->piecetype);
                printf("Move %s from %c%d to %c%d \n", piece, src_col, current->src_row+1, dst_col, current->dst_row+1);
                movecounter++;
            }
            current = current->nextentry;
        }
    }
    printf("\n");
}
