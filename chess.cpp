#include <stdlib.h>
#include <math.h>
#include "chess.h"

// White Pawn = 1
// White Rook = 2
// White Knight = 3
// White Bishop = 4
// White Queen = 5
// White King = 6
// Black Pawn = 7 (White + 6)
// Black Rook = 8
// Black Knight = 9
// Black Bishop = 10
// Black Queen = 11
// Black King = 12

//movementPawn 0: white pawn start(1), 1: white pawn(1), 2: white pawn attack(2), 3: white pawn en passant(2), 4: black pawn start(1), 5: black pawn(1), 6: black pawn attack(2), 7: black pawn en passant(2)
//movementSliding 0: rook(4), 1: bishop(4), 2: queen(8)
//movementJumping 0: knight(8) 1: king(8)

Chess::Chess()
{
	board = (int**)malloc(8 * sizeof(int*));
	resetBoard();
}
int** Chess::printBoard()
{
	return board;
}
void Chess::resetBoard()
{
	turn = 0;
	enpassRow = -1;
	enpassColumn = -1;
	for(int i = 0; i < 4; i++)
	{
		castle[i] = 1;
	}
	board = (int**)malloc(8 * sizeof(int*));
	for(int i = 0; i < 8; i++)
	{
		board[i] = (int*)malloc(8 * sizeof(int));
		for(int j = 0; j < 8; j++)
		{
			board[i][j] = 0;
		}
	}
	for(int i = 0; i < 8; i++)
	{
		board[6][i] = 1;
		board[1][i] = 7;
	}
	board[7][0] = 2;
	board[7][7] = 2;
	board[7][1] = 3;
	board[7][6] = 3;
	board[7][2] = 4;
	board[7][5] = 4;
	board[7][3] = 5;
	board[7][4] = 6;
	board[0][0] = 8;
	board[0][7] = 8;
	board[0][1] = 9;
	board[0][6] = 9;
	board[0][2] = 10;
	board[0][5] = 10;
	board[0][3] = 11;
	board[0][4] = 12;
}
void Chess::spawnPiece(int piece, int row, int column)
{
	board[row][column] = piece;
}
int Chess::makeMove(int startRow, int startColumn, int endRow, int endColumn) // 0: no, 1: to empty field, 2: normal replace, 3: en passant, 4: castle queen side, 5: castle king side
{
	int whiteOrBlack[2] = {0, 4};
	if(board[startRow][startColumn] != 0)
	{
		int type;
		int color;
		if(board[startRow][startColumn] <= 6)
		{
			type = board[startRow][startColumn];
			color = 0;
		}
		else
		{
			type = board[startRow][startColumn] - 6;
			color = 1;
		}
		switch(type)
		{
			case 1:
				switch(color)
				{
					case 0:
						if(startRow == 6 && startRow + movementPawn[0 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[0 + whiteOrBlack[color]][0][1] == endColumn && board[endRow][endColumn] == 0)
						{
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 1;
							enpassRow = endRow;
							enpassColumn = endColumn;
							return 1;
						}
						else if(startRow + movementPawn[1 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[1 + whiteOrBlack[color]][0][1] == endColumn && board[endRow][endColumn] == 0)
						{
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 1;
							enpassRow = -1;
							enpassColumn = -1;
							return 1;
						}
						else if(((startRow + movementPawn[2 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[2 + whiteOrBlack[color]][0][1] == endColumn) || (startRow + movementPawn[2 + whiteOrBlack[color]][1][0] == endRow && startColumn + movementPawn[2 + whiteOrBlack[color]][1][1] == endColumn)) && board[endRow][endColumn] > 6)
						{
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 1;
							enpassRow = -1;
							enpassColumn = -1;
							return 2;
						}
						else if(((startRow + movementPawn[3 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[3 + whiteOrBlack[color]][0][1] == endColumn) || (startRow + movementPawn[3 + whiteOrBlack[color]][1][0] == endRow && startColumn + movementPawn[3 + whiteOrBlack[color]][1][1] == endColumn)) && board[endRow + 1][endColumn] > 6 && (endRow + 1 == enpassRow && endColumn == enpassColumn))
						{
							board[startRow][startColumn] = 0;
							board[endRow + 1][endColumn] = 0;
							board[endRow][endColumn] = 1;
							enpassRow = -1;
							enpassColumn = -1;
							return 3;
						}
						return 0;
						break;
					case 1:
						if(startRow == 1 && startRow + movementPawn[0 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[0 + whiteOrBlack[color]][0][1] == endColumn && board[endRow][endColumn] == 0)
						{
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 7;
							enpassRow = endRow;
							enpassColumn = endColumn;
							return 1;
						}
						else if(startRow + movementPawn[1 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[1 + whiteOrBlack[color]][0][1] == endColumn && board[endRow][endColumn] == 0)
						{
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 7;
							enpassRow = -1;
							enpassColumn = -1;
							return 1;
						}
						else if(((startRow + movementPawn[2 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[2 + whiteOrBlack[color]][0][1] == endColumn) || (startRow + movementPawn[2 + whiteOrBlack[color]][1][0] == endRow && startColumn + movementPawn[2 + whiteOrBlack[color]][1][1] == endColumn)) && board[endRow][endColumn] <= 6 && board[endRow][endColumn] != 0)
						{
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 7;
							enpassRow = -1;
							enpassColumn = -1;
							return 2;
						}
						else if(((startRow + movementPawn[3 + whiteOrBlack[color]][0][0] == endRow && startColumn + movementPawn[3 + whiteOrBlack[color]][0][1] == endColumn) || (startRow + movementPawn[3 + whiteOrBlack[color]][1][0] == endRow && startColumn + movementPawn[3 + whiteOrBlack[color]][1][1] == endColumn)) && board[endRow + 1][endColumn] <= 6 && (endRow - 1 == enpassRow && endColumn == enpassColumn))
						{
							board[startRow][startColumn] = 0;
							board[endRow - 1][endColumn] = 0;
							board[endRow][endColumn] = 7;
							enpassRow = -1;
							enpassColumn = -1;
							return 3;
						}
						return 0;
						break;
					default:
						break;
				}
				break;
			case 2:
				switch(color)
				{
					case 0:
						for(int i = 0; i < 4; i++)
						{
							int tempRow = startRow + movementSliding[0][i][0];
							int tempColumn = startColumn + movementSliding[0][i][1];
							while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7)
							{
								if((board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] > 6) && tempRow == endRow && tempColumn == endColumn)
								{
									if(startColumn == 0)
									{
										castle[0] = 0;
									}
									else if(startColumn == 7)
									{
										castle[1] = 0;
									}
									enpassRow = -1;
									enpassColumn = -1;
									if(board[endRow][endColumn] > 6)
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 2;
										return 2;
									}
									else
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 2;
										return 1;
									}
								}
								else if(board[tempRow][tempColumn] != 0)
								{
									break;
								}
								tempRow += movementSliding[0][i][0];
								tempColumn += movementSliding[0][i][1];
							}
						}
						return 0;
						break;
					case 1:
						for(int i = 0; i < 4; i++)
						{
							int tempRow = startRow + movementSliding[0][i][0];
							int tempColumn = startColumn + movementSliding[0][i][1];
							while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7)
							{
								if((board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] <= 6) && tempRow == endRow && tempColumn == endColumn)
								{
									if(startColumn == 0)
									{
										castle[2] = 0;
									}
									else if(startColumn == 7)
									{
										castle[3] = 0;
									}
									enpassRow = -1;
									enpassColumn = -1;
									if(board[endRow][endColumn] <= 6)
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 8;
										return 2;
									}
									else
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 8;
										return 1;
									}
									return true;
								}
								else if(board[tempRow][tempColumn] != 0)
								{
									break;
								}
								tempRow += movementSliding[0][i][0];
								tempColumn += movementSliding[0][i][1];
							}
						}
						return false;
						break;
					default:
						break;
				}
				break;
			case 3:
				switch(color)
				{
					case 0:
						for(int i = 0; i < 8; i++)
						{
							int tempRow = startRow + movementJumping[0][i][0];
							int tempColumn = startColumn + movementJumping[0][i][1];
							if(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7 && tempRow == endRow && tempColumn == endColumn && (board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] > 6))
							{
								enpassRow = -1;
								enpassColumn = -1;
								if(board[endRow][endColumn] > 6)
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 3;
									return 2;
								}
								else
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 3;
									return 1;
								}
							}
						}
						return 0;
						break;
					case 1:
						for(int i = 0; i < 8; i++)
						{
							int tempRow = startRow + movementJumping[0][i][0];
							int tempColumn = startColumn + movementJumping[0][i][1];
							if(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7 && tempRow == endRow && tempColumn == endColumn && (board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] <= 6))
							{
								enpassRow = -1;
								enpassColumn = -1;
								if(board[endRow][endColumn] <= 6)
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 9;
									return 2;
								}
								else
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 9;
									return 1;
								}
							}
						}
						return 0;
						break;
					default:
						break;
				}
				break;
			case 4:
				switch(color)
				{
					case 0:
						for(int i = 0; i < 4; i++)
						{
							int tempRow = startRow + movementSliding[1][i][0];
							int tempColumn = startColumn + movementSliding[1][i][1];
							while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7)
							{
								if((board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] > 6) && tempRow == endRow && tempColumn == endColumn)
								{
									enpassRow = -1;
									enpassColumn = -1;
									if(board[endRow][endColumn] > 6)
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 4;
										return 2;
									}
									else
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 4;
										return 1;
									}
									return true;
								}
								else if(board[tempRow][tempColumn] != 0)
								{
									break;
								}
								tempRow += movementSliding[1][i][0];
								tempColumn += movementSliding[1][i][1];
							}
						}
						return 0;
						break;
					case 1:
						for(int i = 0; i < 4; i++)
						{
							int tempRow = startRow + movementSliding[1][i][0];
							int tempColumn = startColumn + movementSliding[1][i][1];
							while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7)
							{
								if((board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] <= 6) && tempRow == endRow && tempColumn == endColumn)
								{
									enpassRow = -1;
									enpassColumn = -1;
									if(board[endRow][endColumn] <= 6)
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 10;
										return 2;
									}
									else
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 10;
										return 1;
									}
								}
								else if(board[tempRow][tempColumn] != 0)
								{
									break;
								}
								tempRow += movementSliding[1][i][0];
								tempColumn += movementSliding[1][i][1];
							}
						}
						return 0;
						break;
					default:
						break;
				}
				break;
			case 5:
				switch(color)
				{
					case 0:
						for(int i = 0; i < 8; i++)
						{
							int tempRow = startRow + movementSliding[2][i][0];
							int tempColumn = startColumn + movementSliding[2][i][1];
							while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7)
							{
								if((board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] > 6) && tempRow == endRow && tempColumn == endColumn)
								{
									enpassRow = -1;
									enpassColumn = -1;
									if(board[endRow][endColumn] > 6)
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 5;
										return 2;
									}
									else
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 5;
										return 1;
									}
								}
								else if(board[tempRow][tempColumn] != 0)
								{
									break;
								}
								tempRow += movementSliding[2][i][0];
								tempColumn += movementSliding[2][i][1];
							}
						}
						return 0;
						break;
					case 1:
						for(int i = 0; i < 8; i++)
						{
							int tempRow = startRow + movementSliding[2][i][0];
							int tempColumn = startColumn + movementSliding[2][i][1];
							while(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7)
							{
								if((board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] <= 6) && tempRow == endRow && tempColumn == endColumn)
								{
									enpassRow = -1;
									enpassColumn = -1;
									if(board[endRow][endColumn] <= 6)
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 11;
										return 2;
									}
									else
									{
										board[startRow][startColumn] = 0;
										board[endRow][endColumn] = 11;
										return 1;
									}
									return true;
								}
								else if(board[tempRow][tempColumn] != 0)
								{
									break;
								}
								tempRow += movementSliding[2][i][0];
								tempColumn += movementSliding[2][i][1];
							}
						}
						return false;
						break;
					default:
						break;
				}
				break;
			case 6:
				switch(color)
				{
					case 0:
						for(int i = 0; i < 8; i++)
						{
							int tempRow = startRow + movementJumping[1][i][0];
							int tempColumn = startColumn + movementJumping[1][i][1];
							if(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7 && tempRow == endRow && tempColumn == endColumn && (board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] > 6))
							{
								enpassRow = -1;
								enpassColumn = -1;
								castle[0] = 0;
								castle[1] = 0;
								if(board[endRow][endColumn] > 6)
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 6;
									return 2;
								}
								else
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 6;
									return 1;
								}
							}
						}
						if(castle[0] == 1 && startRow == endRow && endColumn == startColumn - 2 && board[startRow][startColumn - 1] == 0 && board[startRow][startColumn - 2] == 0 && board[startRow][startColumn - 3] == 0)
						{
							castle[0] = 0;
							castle[1] = 0;
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 6;
							board[endRow][0] = 0;
							board[endRow][3] = 2;
							return 4;
						}
						else if(castle[1] == 1 && startRow == endRow && endColumn == startColumn + 2 && board[startRow][startColumn + 1] == 0 && board[startRow][startColumn + 2] == 0)
						{
							castle[0] = 0;
							castle[1] = 0;
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 6;
							board[endRow][7] = 0;
							board[endRow][5] = 2;
							return 5;
						}
						return 0;
						break;
					case 1:
						for(int i = 0; i < 8; i++)
						{
							int tempRow = startRow + movementJumping[1][i][0];
							int tempColumn = startColumn + movementJumping[1][i][1];
							if(tempRow >= 0 && tempRow <= 7 && tempColumn >= 0 && tempColumn <= 7 && tempRow == endRow && tempColumn == endColumn && (board[tempRow][tempColumn] == 0 || board[tempRow][tempColumn] <= 6))
							{
								enpassRow = -1;
								enpassColumn = -1;
								castle[2] = 0;
								castle[3] = 0;
								if(board[endRow][endColumn] <= 6)
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 12;
									return 2;
								}
								else
								{
									board[startRow][startColumn] = 0;
									board[endRow][endColumn] = 12;
									return 1;
								}
							}
						}
						if(castle[2] == 1 && startRow == endRow && endColumn == startColumn - 2 && board[startRow][startColumn - 1] == 0 && board[startRow][startColumn - 2] == 0 && board[startRow][startColumn - 3] == 0)
						{
							castle[2] = 0;
							castle[3] = 0;
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 12;
							board[endRow][0] = 0;
							board[endRow][3] = 8;
							return 4;
						}
						else if(castle[3] == 1 && startRow == endRow && endColumn == startColumn + 2 && board[startRow][startColumn + 1] == 0 && board[startRow][startColumn + 2] == 0)
						{
							castle[2] = 0;
							castle[3] = 0;
							board[startRow][startColumn] = 0;
							board[endRow][endColumn] = 12;
							board[endRow][7] = 0;
							board[endRow][5] = 8;
							return 5;
						}
						return 0;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	return false;
}