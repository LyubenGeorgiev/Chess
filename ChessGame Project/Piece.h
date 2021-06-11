#pragma once

#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "Move.h"


enum class PieceType
{
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

class Piece
{
protected:
	PieceType pieceType;
	SDL_Texture* pieceTexture;
	bool white;

public:
	Piece(bool colorIsWhite);

	static void init();
	static void loadImages();
	static void destroyImages();

	static SDL_Texture* blackKingTexture;
	static SDL_Texture* whiteKingTexture;
	static SDL_Texture* blackKnightTexture;
	static SDL_Texture* whiteKnightTexture;
	static SDL_Texture* blackRookTexture;
	static SDL_Texture* whiteRookTexture;
	static SDL_Texture* blackBishopTexture;
	static SDL_Texture* whiteBishopTexture;
	static SDL_Texture* whiteQueenTexture;
	static SDL_Texture* blackQueenTexture;
	static SDL_Texture* whitePawnTexture;
	static SDL_Texture* blackPawnTexture;

	bool isWhite();
	PieceType getPieceType();
	SDL_Texture* getPieceTexture();
	virtual void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn, 
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle, 
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo);


};


class Pawn : public Piece
{
private:

public:
	Pawn(bool colorIsWhite);
	void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn,
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo) override;

};


class Rook : public Piece
{
private:

public:
	Rook(bool colorIsWhite);
	void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn,
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo) override;

};


class Knight : public Piece
{
private:

public:
	Knight(bool colorIsWhite);
	void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn,
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo) override;

};


class Bishop : public Piece
{
private:

public:
	Bishop(bool colorIsWhite);
	void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn,
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo) override;

};


class Queen : public Piece
{
private:

public:
	Queen(bool colorIsWhite);
	void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn,
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo) override;

};


class King : public Piece
{
private:

public:
	King(bool colorIsWhite);
	void getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
		int boardXBoxes, int boardYBoxes, char currentTurn,
		int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
		bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
		bool blackCanQueensideCastle, std::vector<Move>& currentPseudo) override;

};