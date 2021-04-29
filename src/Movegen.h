//  Movegen.h
//  Chesster Team 3

#ifndef Movegen_h
#define Movegen_h
#include "Board.h"
#include "Pieces.h"
#include "Conditions.h"

/* Last modified 1/30/2019*/



/*Things that need to be added:
 DeleteMoveList
 DeleteMoveEntry
 
 */

/*CallPiece testcondition:
 *
 * 0 = checks for legality of move
 * 1 = */

typedef struct move MOVE;
#ifndef MOVELIST_DEFINED
typedef struct movelist MOVELIST;
#define MOVELIST_DEFINED
#endif

struct move {
    int src_row;
    int src_col;
    int dst_row;
    int dst_col;
    MOVE *nextentry;
    MOVE *preventry;
    int IsCaptured; // 0 = no, 1 = yes
    PIECE *piece;
    PIECE *opponentcapture;
    int EnPassantStatus;
    int CastlingStatus;
    BOARD *board;
    char *new_board[8][8];
    int score;
    MOVELIST *next_level;
    MOVELIST *prev_level;
    int CheckMove;
    int BadMove;
    int PossibleEzMove;
};

struct movelist {
    MOVE *first;
    MOVE *last;
    int length;
    MOVE *prevmove;
    BOARD *board;
};
/*Adds entries for possible legal moves into a given list by evaluating a given board config and player perspective*/
void getsmartmoves(char *cpy_board[8][8], BOARD *board, PLAYER *player, PLAYER *opponent, MOVELIST *list, int t_guard, int turn_counter, int king, int startguard); // smart AI
void getmoves(char *org_board[8][8], BOARD *board, PLAYER *player, PLAYER *opponent, MOVELIST *list); // backup AI


/*Adds move information into the given list, allocating space and making new entries; stores resulting board from making the move*/
void AddLegalMoves(MOVELIST *list, int src_row, int src_col, int dest_row, int dest_col, BOARD *board, int IsCaptured, PIECE *piece, PIECE *opponentcapture, char *cpy_board[8][8], int CheckMove, int castling);

/*Creates a new move list*/
MOVELIST *NewMoveList(void);

/*Creates a new move*/
MOVE *CreateMove(void);

/*Deletes list of all possible legal moves: frees memory*/
void DeleteMoveList(MOVELIST *list);

/*Deletes a single move entry within the movelist*/
void DeleteMoveEntry(MOVE *entry);

/*Deletes a board*/
void DeleteBoard(BOARD *board);

/* Creates a player */
PLAYER *CreatePlayer(char color, char type);

/*Deletes a player*/
void DeletePlayer(PLAYER *entry);

/*Deletes a piece*/
void DeletePiece(PIECE *piece);

void DeleteInsertedMove(MOVE *entry);

void BadMove(MOVE *move);


#endif
