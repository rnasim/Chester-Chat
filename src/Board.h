//  Board.h
//  Chesster Team 3

#ifndef Board_h
#define Board_h

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>


typedef struct Player PLAYER;
typedef struct Piece PIECE;
typedef struct Board BOARD;
#ifndef MOVELIST_DEFINED
typedef struct movelist MOVELIST;
#define MOVELIST_DEFINED
#endif


typedef enum PieceName {Pawn1, Pawn2, Pawn3, Pawn4, Pawn5, Pawn6, Pawn7, Pawn8, Rook1, Rook2, Bishop1, Bishop2, Knight1, Knight2, Queen, King} pieces; // Pawns = 0-7, Rooks = 8-9, Bishops = 10-11, Knights = 12-13, Queen = 14, King = 15

struct Board {
    PLAYER *white;
    PLAYER *black;
    char *boardarray[8][8];
    MOVELIST *movelist;
};

struct Player {
    char type;  //type of player: human ('h') or AI ('a')
    char color;
    PIECE *piecelist[16];
};


struct Piece{
    PLAYER *player;
    char piecetype;
    int r;
    int c;
    int value;
    int EnPassant; // 1 if eligible, 0 if not
    int castling; // 1 if eligible, 0 if not
};

BOARD *chess(void);
void InitializeBoard(BOARD *global);
void DrawBoard(char *boardarray[8][8]);
int MakeMove(BOARD *board, PLAYER *p, PLAYER *opponent, MOVELIST *movelist);
PIECE *CreatePiece(BOARD *board, int r, int c, char piece, char color, PLAYER *player);
int MovePiece(BOARD *board, PLAYER *opponent, PIECE *piece, int newr, int newc, int CastlingGuard);
int AlphatoNum(char alpha);
char NumtoAlpha(int num);
char *PieceName(char piecename);
PIECE *CheckPiece(PLAYER *p, int r, int c);
int FindEmptySpace(BOARD *board, int r, int c);
BOARD *CreateBoard(PLAYER *human, PLAYER *AI, char *boardarray[8][8]);
BOARD *CreateAIBoard(PLAYER *white, PLAYER *black, char *boardarray[8][8]);
FILE *Log(char color, char piecetype, char destcol, int destrow, int isCaptured, int CheckReturn, char info, int castling);


#endif /* Board_h */
