#include <gtk/gtk.h>
#include <stdlib.h>
#include <assert.h>
#include "GTK_chess.h"



char Board[MAX_GRID_SIZE][MAX_GRID_SIZE] = {{0}};
int selected = 0;
int tempSource[2];
int tempDest[2];
static int count = 0;
GtkWidget *frame_Chess;
GtkWidget *table;
GtkWidget *window_Chess;

void CallChess(int sockfd)
{
	//GtkWidget *window;
	//GtkWidget *fixed;

	CHESSBOARD *chessboard = g_malloc(sizeof(*chessboard));
	
	//gtk_init(&argc, &argv);
	chessboard->sockfd = sockfd;
  chessboard->location = 'h';
  printf("Socket = %d\n", chessboard->sockfd);
  printf("ADDRESS OF CHESS = %p\n", &chessboard);
  CreateChessWindow(chessboard);

  /*while(1){
 	  while(gtk_events_pending()){
		gtk_main_iteration();
   }
  }*/
	//gtk_main();
	
	//return 0;
}

/*void set_event_box_background(GtkWidget* event_box)
{
    GError* error = NULL;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("./chess_icon/Chessboard.png", &error);
    GdkPixmap *pixmap = NULL;
    GdkPixmap *mask = NULL;

    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &mask, 255);
    GtkStyle* orig_style = gtk_widget_get_style(event_box);
    GtkStyle* style = gtk_style_copy(orig_style);
    style->bg_pixmap[GTK_STATE_NORMAL] = pixmap;
    gtk_widget_set_style(event_box, style);
}*/

void CreateChessWindow(CHESSBOARD *chessboard){
  // Declare Widgets
  //GtkWidget *window_Chess;
  GtkWidget *event_box;
  GtkWidget  *frame_buttons;
  GtkWidget *hbox, *vbox;
  GtkWidget *button;
  //GtkWidget *table;
  
  // Initialize Widgets //
  
  // Window
  window_Chess = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //chessboard->window = window_Chess;
  //gtk_widget_set_size_request(window_Chess, WINDOW_WIDTH, WINDOW_HEIGHT) ;  
  gtk_container_set_border_width (GTK_CONTAINER (window_Chess), WINDOW_BORDER);
  g_signal_connect_swapped (window_Chess, "delete-event", G_CALLBACK (gtk_widget_destroy), window_Chess);
  g_signal_connect(window_Chess, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
  gtk_window_set_position(GTK_WINDOW(window_Chess), GTK_WIN_POS_CENTER) ; 
	gtk_window_set_title(GTK_WINDOW(window_Chess), "Chesster") ; 
	gtk_window_set_resizable(GTK_WINDOW(window_Chess), FALSE) ; 
  
  //HBOX
  hbox = gtk_hbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(window_Chess), hbox);
  
  /* Create an EventBox and add it to our toplevel window */
  event_box = gtk_event_box_new ();
  //set_event_box_background(event_box);
  gtk_box_pack_start(GTK_BOX(hbox), event_box, FALSE, TRUE, 0);
  gtk_widget_show (event_box);
  
  // Frame 
  //chessboard->frame = gtk_frame_new(NULL);
  //frame_Chess = chessboard->frame;
  frame_Chess = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(event_box), frame_Chess);
  
  frame_buttons = gtk_frame_new(NULL);
  gtk_box_pack_start(GTK_BOX(hbox), frame_buttons, FALSE, TRUE, 10);
 
	/*Click Events*/
	g_signal_connect(window_Chess, "delete-event", G_CALLBACK(delete_event), NULL);
  gtk_widget_set_events(event_box, GDK_BUTTON_PRESS_MASK) ; 	
	g_signal_connect(event_box, "button_press_event", G_CALLBACK(area_click), (gpointer) chessboard) ; 
 printf("Socket = %d\n", chessboard->sockfd);
 
  gtk_widget_realize (event_box);
  gdk_window_set_cursor (event_box->window, gdk_cursor_new (GDK_HAND1));
  
   //VBOX
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame_buttons), vbox);
  //gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, TRUE, 0);
  
  // Buttons
  button = gtk_button_new_with_label("Hey");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, TRUE, 0);
  
  // Table
  table = gtk_table_new(MAX_GRID_SIZE, MAX_GRID_SIZE, TRUE);
  gtk_widget_set_size_request(table, BOARD_WIDTH, BOARD_HEIGHT) ;
  
  InitBoard();
  DrawBoard(chessboard);
  
  gtk_container_add(GTK_CONTAINER(frame_Chess), table);
  gtk_widget_show_all(window_Chess);

}

void InitBoard(void)
{
	/* Initialize Board colors */
	int j = 0;
	while(j < 8){
		if(j%2 == 0){	
			for(int i = 0; i < MAX_GRID_SIZE; i++){
				if(i%2 == 0){Board[i][j] = WHITE;}
				else{Board[i][j] = BLACK;}
			}
		}
		else{
			for(int i = 0; i < MAX_GRID_SIZE; i++){
				if(i%2 == 0){Board[i][j] = BLACK;}
				else{Board[i][j] = WHITE;}
			}
		}
	j++;	//Increment to next row
	}
	for(int pawn = 0; pawn < MAX_GRID_SIZE; pawn++)
    {
		Board[pawn][6] = BPAWN;
		Board[pawn][1] = WPAWN;
    }
  //Black pieces 
	Board[0][7] = BROOK;
	Board[1][7] = BKNIGHT;
	Board[2][7] = BBISHOP;
	Board[3][7] = BQUEEN;
	Board[4][7] = BKING;
	Board[5][7] = BBISHOP;
	Board[6][7] = BKNIGHT;
	Board[7][7] = BROOK;
	
	//White Pieces 
	Board[0][0] = WROOK;
	Board[1][0] = WKNIGHT;
	Board[2][0] = WBISHOP;
	Board[3][0] = WQUEEN;
	Board[4][0] = WKING;
	Board[5][0] = WBISHOP;
	Board[6][0] = WKNIGHT;
	Board[7][0] = WROOK;
    
 /*//Black pieces 
	Board[0][0] = BROOK;
	Board[1][0] = BKNIGHT;
	Board[2][0] = BBISHOP;
	Board[3][0] = BQUEEN;
	Board[4][0] = BKING;
	Board[5][0] = BBISHOP;
	Board[6][0] = BKNIGHT;
	Board[7][0] = BROOK;
	
	//White Pieces 
	Board[0][7] = WROOK;
	Board[1][7] = WKNIGHT;
	Board[2][7] = WBISHOP;
	Board[3][7] = WQUEEN;
	Board[4][7] = WKING;
	Board[5][7] = WBISHOP;
	Board[6][7] = WKNIGHT;
	Board[7][7] = WROOK;*/
	
}

void ReverseGridColor(int g_x, int g_y)
{
	if(Board[g_x][g_y] == BLACK)
	{
		Board[g_x][g_y] = WHITE;
	}
	else
	{
		Board[g_x][g_y] = BLACK;
	}
}

static gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data)
{
	g_print("delete event occurred\n");
	gtk_main_quit();
	return TRUE;
}

void DrawBoard(CHESSBOARD *chessboard)
{
	GtkWidget *chess_icon;
	int i, j;
	  printf("ADDRESS OF CHESS 3 = %p \n", &chessboard);
	/* Draw Checker Board */
    for(i = 0; i < 8; i++)
	{
		for(j = 7; j >= 0; j--)
		{
			switch(Board[i][j])
			{
				case BLACK:
          //chess_icon = NULL;
					chess_icon = gtk_image_new_from_file("./../src/chess_icon/Bsquare.jpg") ;
					break;
				case WHITE:
          //chess_icon = NULL;        
					chess_icon = gtk_image_new_from_file("./../src/chess_icon/Wsquare.jpg") ;
					break;
				case WPAWN:
          if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WPawnW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WPawnB.jpg");}
            }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WPawnB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WPawnW.jpg");}
          }
					break;
				case BPAWN:
				  if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BPawnW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BPawnB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BPawnB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BPawnW.jpg");}
          }
					break;
				case WROOK:
          if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WRookW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WRookB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WRookB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WRookW.jpg");}
          }
					break;
				case BROOK:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BRookW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BRookB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BRookB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BRookW.jpg");}
          }
					break;
				case WBISHOP:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WBishopW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WBishopB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WBishopB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WBishopW.jpg");}
          }
					break;
				case BBISHOP:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BBishopW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BBishopB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BBishopB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BBishopW.jpg");}
          }
					break;
				case WKNIGHT:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKnightW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKnightB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKnightB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKnightW.jpg");}
          }
					break;
				case BKNIGHT:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKnightW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKnightB.jpg");}
          }  
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKnightB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKnightW.jpg");}
          }
					break;
				case WQUEEN:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WQueenW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WQueenB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WQueenB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WQueenW.jpg");}
          }
					break;
				case BQUEEN:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BQueenW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BQueenB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BQueenB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BQueenW.jpg");}
          }
					break;
				case WKING:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKingW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKingB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKingB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/WKingW.jpg");}
          }
					break;
				case BKING:
					if(j%2 == 0){
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKingW.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKingB.jpg");}
          }
          else{
            if(i%2 == 0){chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKingB.jpg");}
            else{chess_icon = gtk_image_new_from_file("./../src/chess_icon/BKingW.jpg");}
          }
					break;
				default:
					break;
			}
			gtk_table_attach(GTK_TABLE(table), chess_icon, i, i + 1, 7-j, 7-j + 1, GTK_FILL, GTK_FILL, 0, 0) ;
		}
	}
		gtk_widget_show(table);

}


void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y)
{
	int x = (c_x - BOARD_BORDER) / SQUARE_SIZE;
	int y = (c_y - BOARD_BORDER) / SQUARE_SIZE;
	
	if(x < 0){ x = 0; }
	else if(x >= MAX_GRID_SIZE){ x = MAX_GRID_SIZE - 1; }
	
	if(y < 0){ y = 0; }
	else if(y >= MAX_GRID_SIZE){ y = MAX_GRID_SIZE - 1; }
	
	*g_x = x;
	*g_y = 7-y;
}

void area_click (GtkWidget *widget, gpointer g_data)
{
  CHESSBOARD* chessboard = (CHESSBOARD*)g_data;
  assert(chessboard);
  int sockfd = chessboard->sockfd;
  printf("ADDRESS OF g_data = %p\n", &g_data);
  printf("ADDRESS OF CHESS 3 = %p \n", &chessboard);
  printf("Socket = %d\n", sockfd);
  printf("Location = %c\n", chessboard->location);
	int coord_x, coord_y, grid_x, grid_y;
	
	/* Find coordinate on mouse click */
	GdkModifierType state ; 
	gdk_window_get_pointer(widget->window, &coord_x, &coord_y, &state) ; 
	if(coord_x > 0 && coord_x < WINDOW_WIDTH && coord_y > 0 && coord_y < WINDOW_HEIGHT){
		CoordToGrid(coord_x, coord_y, &grid_x, &grid_y);
	}
	printf("coord_x = %d, coord_y = %d, grid_x = %d, grid_y = %d \n", coord_x, coord_y, grid_x, grid_y);
	//printf("PieceNumber = %d\n", chessboard->PieceNumber);
	
	/* Checks to see if click is inside board */
	if(grid_x < 0 || grid_x > MAX_GRID_SIZE || grid_y < 0 || grid_y > MAX_GRID_SIZE){
		g_print("Must click inside board.\n");
		return;
	}
	
  if(selected == 0){
		if(Board[grid_x][grid_y] == WHITE || Board[grid_x][grid_y] == BLACK){
			selected = 0;
		}
		else{
			selected = 1;
			//printf("Selected = %d\n", selected);
		} 
 }
  if(count == 0 && selected == 1)
  {
    StoreSource(chessboard, grid_x, grid_y);
    PickPiece(chessboard, grid_x, grid_y);
    //printf("Stored %d %d \n", chessboard->src_r , chessboard->src_c);
    count = 1;
  }
  else if(count == 1){
      StoreDest(chessboard, grid_x, grid_y);
      int isValid = SendMove(chessboard);
      count = 0;
      printf("moving Piece\n");
      MovePiece(chessboard);
      gtk_container_remove(GTK_CONTAINER(frame_Chess), table);
      table = gtk_table_new(MAX_GRID_SIZE, MAX_GRID_SIZE, TRUE);
      gtk_widget_set_size_request(table, BOARD_WIDTH, BOARD_HEIGHT);
      DrawBoard(chessboard);
      gtk_container_add(GTK_CONTAINER(frame_Chess), table);
      gtk_widget_show_all(frame_Chess);
    
  }
		selected = 0;
		//printf("Selected = %d\n", selected);

}

int SendMove(CHESSBOARD *chessboard){
  char send[1000];
  int sockfd = chessboard->sockfd;
  printf("Socket = %d\n", sockfd);
  int col_src, row_src, col_dest, row_dest;
  col_src = tempSource[0]+1;
  row_src = tempSource[1]+1;
  col_dest = tempDest[0]+1;
  row_dest = tempDest[1]+1;
  
  
  char move[4];
  move[0] = NumtoAlpha(col_src);
  move[1] = (char)row_src + 48;
  move[2] = NumtoAlpha(col_dest);
  move[3] = (char)row_dest + 48;
  
  /*strcpy(send, move);
  u_int32_t len = htonl(strlen(send));
  write(sockfd, &len, sizeof(len));
	if(write(sockfd, send, strlen(send)) < 0)
    {
		puts("Text could not reach the server.");
        exit(0);
    }
  memset(send,'\0',sizeof(send));
  */
  g_print("Moved from %c%c to %c%c\n", move[0], move[1], move[2], move[3]);
  return 1;
}

void StoreSource(CHESSBOARD *chessboard, int g_x, int g_y){
	/*chessboard->dest_r = g_x;
	chessboard->dest_c = g_y;
	printf("Dest %d %d \n", chessboard->dest_r , chessboard->dest_c);*/
  tempSource[0] = g_x;
  tempSource[1] = g_y;
}

void StoreDest(CHESSBOARD *chessboard, int g_x, int g_y){
	/*chessboard->src_r = g_x;
	chessboard->src_c = g_y;
	printf("Stored %d %d \n", chessboard->src_r , chessboard->src_c);*/
  tempDest[0] = g_x;
  tempDest[1] = g_y;
}

void MovePiece(CHESSBOARD *chessboard){
	int src_r, src_c, dest_r, dest_c;
	/*src_r = chessboard->src_r;
	src_c = chessboard->src_c;
	dest_r = chessboard->dest_r;
	dest_c = chessboard->dest_c;*/
  src_r = tempSource[0];
  src_c = tempSource[1];
  dest_r = tempDest[0];
  dest_c = tempDest[1];
	
	int PieceSource = Board[src_r][src_c];
	Board[dest_r][dest_c] = PieceSource;
	
	if(src_c%2 == 0){
		if(src_r%2 == 0){Board[src_r][src_c] = WHITE;}
		else{Board[src_r][src_c] = BLACK;}
	}
	else{
		if(src_r%2 == 0){Board[src_r][src_c] = BLACK;}
		else{Board[src_r][src_c] = WHITE;}
	}
	
	//printf("Moved from %d %d to %d %d\n", src_r, src_c, dest_r, dest_c);
	//Board[dest_r][dest_c] = Board[src_r][src_c];
}

void CheckSpace(int x, int y){
	int grid = Board[x][y];
	if(grid == WHITE || grid == BLACK){
		
	}
}

void PickPiece(CHESSBOARD *chessboard, int x, int y){
	if(Board[x][y] != WHITE || Board[x][y] != BLACK){
		switch(Board[x][y])
		{
			case WPAWN:
				printf("Piece selected: White Pawn.\n");
				break;
			case WROOK:
				printf("Piece selected: White Rook.\n");
				break;
			case WBISHOP:
				printf("Piece selected: White Bishop.\n");
				break;
			case WKNIGHT:
				printf("Piece selected: White Knight.\n");
				break;
			case WQUEEN:
				printf("Piece selected: White Queen.\n");
				break;
			case WKING:
				printf("Piece selected: White King.\n");
				break;
				
			case BPAWN:
				printf("Piece selected: Black Pawn.\n");
				break;
			case BROOK:
				printf("Piece selected: Black Rook.\n");
				break;
			case BBISHOP:
				printf("Piece selected: Black Bishop.\n");
				break;
			case BKNIGHT:
				printf("Piece selected: Black Kinght.\n");
				break;
			case BQUEEN:
				printf("Piece selected: Black Queen.\n");
				break;
			case BKING:
				printf("Piece selected: Black King.\n");
				break;
			default:
				break;
		}
	}
}

CHESSBOARD *CreateChessBoard(void){
	
	CHESSBOARD *chessboard = g_malloc(sizeof(CHESSBOARD));
  //BOARD *global = chess();
	return chessboard;
}

char NumtoAlpha(int num)
{
    char alpha = 0;
    switch(num)
    {
        case 1:
            alpha = 'a';
            break;
        case 2:
            alpha = 'b';
            break;
        case 3:
            alpha = 'c';
            break;
        case 4:
            alpha = 'd';
            break;
        case 5:
            alpha = 'e';
            break;
        case 6:
            alpha = 'f';
            break;
        case 7:
            alpha = 'g';
            break;
        case 8:
            alpha = 'h';
            break;
        default:
            alpha = 'q';
            break;
    }
    return alpha;
}

