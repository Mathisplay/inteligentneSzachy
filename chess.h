#ifndef CHESS_H
#define CHESS_H
#endif // !CHESS_H

class Chess
{
public:
	int** board;
	bool castle[4] = {1, 1, 1, 1}; //white queen, white king, black queen, black king
	int enpassRow = -1;
	int enpassColumn = -1;
	int turn = 0;
	int movementPawn[8][2][2] = {{{-2, 0}, {0, 0}}, {{-1, 0}, {0, 0}}, {{-1, -1}, {-1, 1}}, {{-1, -1}, {-1, 1}}, {{2, 0}, {0, 0}}, {{1, 0}, {0, 0}}, {{1, -1}, {1, 1}}, {{1, -1}, {1, 1}}}; //0: white pawn start, 1: white pawn, 2: white pawn attack, 3: white pawn en passant, 4: black pawn start, 5: black pawn, 6: black pawn attack, 7: black pawn en passant
	int movementSliding[3][8][2] = {{{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}}; //0: rook, 1: bishop, 2: queen
	int movementJumping[2][8][2] = {{{1, 2}, {-1, 2}, {-1, 2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}}, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}}}; //0: knight 1: king
	
	Chess();
	int** printBoard();
	void resetBoard();
	void spawnPiece(int pieceID, int row, int column);
	int makeMove(int startRow, int startColumn, int endRow, int endColumn);
};
