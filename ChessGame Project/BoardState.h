#pragma once

#include <stdint.h>
#include "Piece.h"


/**
 * \brief Contains everything you need to know about the state of a game
 * \author L. Georgiev
 */
class BoardState
{
public:
	std::vector<std::vector<Piece*>>& getBoard();
	void setBoard(std::vector<std::vector<Piece*>>);


	void setCurrentTurn(char);
	char getCurrentTurn();

	bool getWhiteCanKingsideCastle();
	void setWhiteCanKingsideCastle(bool);

	bool getWhiteCanQueensideCastle();
	void setWhiteCanQueensideCastle(bool);

	bool getBlackCanKingsideCastle();
	void setBlackCanKingsideCastle(bool);

	bool getBlackCanQueensideCastle();
	void setBlackCanQueensideCastle(bool);


	int getDepth();
	int getEnPassantX();
	void setEnPassantX(int);

	int getEnPassantY();
	void setEnPassantY(int);

	int getHalfMoveClock();
	void setHalfMoveClock(int);

	int getFullMoveClock();
	void setFullMoveCLock(int);

	bool pieceIsCurrentPlayerPiece(int x, int y);

	static BoardState* copyBoardState(BoardState*);

private:
	std::vector<std::vector<Piece*>> board;
	char currentTurn;
	bool whiteCanKingsideCastle;
	bool whiteCanQueensideCastle;
	bool blackCanKingsideCastle;
	bool blackCanQueensideCastle;
	int enPassantX;
	int enPassantY;
	int halfMoveClock;
	int fullMoveClock;

	int depth;	//0 for main state, add 1 for every move after it.
};