#pragma once

#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "Move.h"


/// Enum for the type of a given piece
enum class PieceType
{
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};


/**
 * \brief Parent class inherited by the actual pieces from chess
 * \author L. Georgiev
 */
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


/**
 * \brief A pawn with it's pseudo legal move rules
 * \author L. Georgiev
 */
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


/**
 * \brief A rook with it's pseudo legal move rules
 * \author L. Georgiev
 */
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


/**
 * \brief A knight with it's pseudo legal move rules
 * \author L. Georgiev
 */
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


/**
 * \brief A bishop with it's pseudo legal move rules
 * \author L. Georgiev
 */
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


/**
 * \brief A queen with it's pseudo legal move rules
 * \author L. Georgiev
 */
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


/**
 * \brief A king with it's pseudo legal move rules
 * \author L. Georgiev
 */
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