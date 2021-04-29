//  Conditions.h
//  Chesster Team 3


#ifndef Conditions_h
#define Conditions_h

#include <stdio.h>
#include "Board.h"
#include "Pieces.h"
#include "Movegen.h"

int Check(BOARD *board, PLAYER *player, PLAYER *opponent, int king_row, int king_col);
int Checkmate(BOARD *board, PLAYER *player, PLAYER *opponent);
int Promotion(BOARD *board, PIECE *piece);
void CapturePiece(BOARD *board, PIECE *piece);
void UndoCapture(BOARD *board, PIECE *opponentcapture, int opponent_r, int opponent_c, int opponent_value, char *piecetag);
int CheckNumberofIllegalMoves(BOARD *board, PLAYER *opponent, PIECE *piece, int row_src, int col_src);
int Stalemate(BOARD *board, PLAYER *player, PLAYER *opponent);
int FiftyConsec(MOVELIST *movelist);
int ThreeFoldRep(MOVELIST *movelist);
void GetHints(BOARD *board, PLAYER *player, PLAYER *opponent);

#endif /* Conditions_h */
