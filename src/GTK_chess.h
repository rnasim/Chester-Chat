#ifndef GTK_CHESS_H
#define GTK_CHESS_H

#define MAX_GRID_SIZE 8
#define MAX_MSGLEN  100 
#define SQUARE_SIZE 50
#define WINDOW_BORDER 10
#define BOARD_BORDER 10
#define BOARD_WIDTH  (MAX_GRID_SIZE*SQUARE_SIZE)
#define BOARD_HEIGHT (MAX_GRID_SIZE*SQUARE_SIZE)
#define WINDOW_WIDTH  (WINDOW_BORDER + BOARD_BORDER + BOARD_WIDTH)
#define WINDOW_HEIGHT (WINDOW_BORDER + BOARD_BORDER + BOARD_HEIGHT)


typedef struct _Chessboard CHESSBOARD;

typedef enum CHESSICON
{
	BLACK = 1,
	WHITE = 2,
	WPAWN = 3,
	WROOK = 4,
	WBISHOP = 5,
	WKNIGHT = 6,
	WQUEEN = 7,
	WKING = 8,
	BPAWN = 9,
	BROOK = 10,
	BBISHOP = 11,
	BKNIGHT = 12, 
	BQUEEN = 13,
	BKING = 14
}chessicon;

struct _Chessboard
{
  GtkWidget *table;
  GtkWidget *frame;
	int select;
  int sockfd;
	int src_r, src_c, dest_r, dest_c;
  char location;
	GtkWidget *window;
	//GtkWidget *table;
	
};

CHESSBOARD *CreateChessBoard(void);
void CallChess(int sockfd);
int SendMove(CHESSBOARD *chessboard);
void set_event_box_background(GtkWidget* event_box);
void CreateChessWindow(CHESSBOARD *chessboard);
void StoreDest(CHESSBOARD *chessboard, int g_x, int g_y);
void StoreSource(CHESSBOARD *chessboard, int g_x, int g_y);
void MovePiece(CHESSBOARD *chessboard);
void PickPiece(CHESSBOARD *chessboard, int x, int y);
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
void InitBoard();
void DrawBoard(CHESSBOARD *chessboard);
void ReverseGridColor(int g_x, int g_y);
void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y);
void area_click (GtkWidget *widget, gpointer g_data);
char NumtoAlpha(int num);

#endif