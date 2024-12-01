#define puts(str) PutStringSB(str, 255)

#define pst_value(p, rank, file) (piece_square[p.type][file][rank ^ ((p.color << 3) - p.color)] ^ (0 - p.color)) + p.color
#define pos_pst_value(b, rank, file) pst_value(b->board[rank][file], rank, file)

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
	short pst_eval;
};

#define HIST_AMT 200

// board[rank][file],
// A file is 0, 1st rank is 0
typedef struct board {
	Piece board[8][8];
	CastlingRights castling_rights;
	
	Color current_turn;
	char en_pas_file; // -1 for no en pas
	
	int ply;
	struct game_history hist[HIST_AMT];
	
	short pst_eval;
} Board;

extern const short piece_square[7][8][8];

#define GAMEOVER_MASK 0x1
#define CHECK_MASK 0x2

void new_board(Board *b);
void from_fen(Board *b, char *fen);
char is_attacked(Board *n, Color color, char rank, char file);
char is_check(Board *b, Color c);
char is_legal(Board *b, Move *m);
void make_move(Board *b, Move *m);
void make_unmove(Board *b);
void for_each_legal(Board *b, void f(int i, Move m));
short evaluate(Board *b);
char is_gameover(Board *b);
char get_mate_state(Board *b);

Move random_move(Board *b);
unsigned int random(void);

Move parse_move(char *uci);
void print_move(Move *m);
void print_move_san(Board *b, Move *m);
void print_board(Board *b);
void print_fen(Board *b);
void print_pgn(Board *b);

unsigned long long perft(Board *b, int depth);
void debug_game(void);

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
void PutNumU (UInt32);
void PutNumUB (UInt8);
void PutStringSB (char String[], int StringBufferCapacity);

//IO stuff
#define RED_MASK 0xFF0000
#define GREEN_MASK 0x00FF00
#define BLUE_MASK 0x0000FF

void init_TPM(void);
void init_PIT(void);
void set_RGB(UInt32 rgb); //0 through 9
void rainbow_ISR(void);
