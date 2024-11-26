#define puts(str) PutStringSB(str, 255)

typedef enum piece_type {
	None=0,
	Pawn,
	Knight,
	Bishop,
	Rook,
	Queen,
	King,
} PieceType;

typedef enum color {
	White=0,
	Black=1,
} Color;

typedef struct piece {
	PieceType type : 3;
	Color color : 1;
} Piece;

typedef struct castling_rights {
	int white_kingside: 1;
	int white_queenside: 1;
	int black_kingside: 1;
	int black_queenside: 1;
} CastlingRights;

typedef struct move {
	unsigned int soure_rank: 3;
	unsigned int soure_file: 3;
	unsigned int destination_rank: 3;
	unsigned int destination_file: 3;
	PieceType promotion: 3;
} Move;

struct game_history {
	CastlingRights castling_rights;
	Piece captured;
	char en_pas_file;
	Move move;
};

// board[rank][file],
// A file is 0, 1st rank is 0
typedef struct board {
	Piece board[8][8];
	CastlingRights castling_rights;
	
	Color current_turn;
	char en_pas_file; // -1 for no en pas
	
	int ply;
	struct game_history hist[255];
	
} Board;

void new_board(Board *b);
void from_fen(Board *b, char *fen);
void print_board(Board *b);
char is_attacked(Board *n, Color color, char rank, char file);
char is_check(Board *b, Color c);
char is_legal(Board *b, Move *m);
void make_move(Board *b, Move *m);
void make_unmove(Board *b);
void for_each_legal(Board *b, void f(int i, Move m));
short evaluate(Board *b);
char is_gameover(Board *b);

Move random_move(Board *b);
unsigned int random(void);

Move parse_move(char *uci);
void print_move(Move *m);

unsigned long long perft(Board *b, int depth);

Move best_move(Board *b);
short minimax(Board *b, char depth);

#define EXERCISE_12_C (1)

typedef int Int32;
typedef short int Int16;
typedef char Int8;
typedef unsigned int UInt32;
typedef unsigned short int UInt16;
typedef unsigned char UInt8;

/* assembly language ROM table entries */
extern UInt16 DAC0_table_0;
extern UInt16 PWM_duty_table_0;

/* assembly language subroutines */
char GetChar (void);
void GetStringSB (char String[], int StringBufferCapacity);
void Init_UART0_IRQ (void);
void PutChar (char Character);
void PutNumHex (UInt32);
void PutNumUB (UInt8);
void PutStringSB (char String[], int StringBufferCapacity);

//IO stuff
#define RED_MASK 0x4
#define GREEN_MASK 0x2
#define BLUE_MASK 0x1

void init_LED(void);
void init_TPM(void);
void set_RGB(UInt32 rgb); //0 through 9
void set_LED(char rgb); //bit 2 r, 1 g, 0 b
