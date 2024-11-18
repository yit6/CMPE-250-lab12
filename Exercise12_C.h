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
	White,
	Black,
} Color;

typedef struct piece {
	PieceType type : 4;
	Color color : 1;
} Piece;

// board[rank][file],
// A file is 0, 1st rank is 0
typedef struct board {
	Piece board[8][8];
} Board;

typedef struct move {
	unsigned int soure_rank: 3;
	unsigned int soure_file: 3;
	unsigned int destination_rank: 3;
	unsigned int destination_file: 3;
} Move;

Board new_board();

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
