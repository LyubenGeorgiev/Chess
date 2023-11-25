#pragma once

#include "SDL.h"
#include "BoardState.h"
#include "Box.h"

/**
 * \brief Class containing most of the game logic for chess
 * \author L. Georgiev
 */
class Board
{
private:
	SDL_Color boardColor1;
	SDL_Color boardColor2;

	BoardState* boardState;

	//! Contains the pieces so in order to free the memory at the end.
	std::vector<Piece*> pieceContainer;


	// Moving piece stuff.
	bool draggingPiece;
	int draggingPieceX;
	int draggingPieceY;
	SDL_Texture* draggingPieceTexture;

	//! Has all moves that are following just the rules for moving a piece.
	std::vector<Move> pseudoLegalMoves;
	//! Has all actually legal moves in chess.
	std::vector<Move> legalMoves;

	//! Has the moves a piece you clicked on can make.
	std::vector<Move> highlightMoves;

	//! If a king is in check coordinates.
	Box highlightKingBox;
	//! Winner king coordinates.
	Box winnerKing;

	Move promotionMove;
	bool gameOver;

	bool waitingForPromotionChoice;

	char winner;

public:
	static int boxXWidth;
	static int boxYHeight;
	static int boardXBoxes;
	static int boardYBoxes;

	~Board();
	void render(BoardState*);
	void resize();
	void init();
	void renderBoard();
	void renderDraggedPiece();

	void handleMouseButtonDown(SDL_MouseButtonEvent&, BoardState*);
	void attemptPickupPiece(int x, int y, BoardState*);
	void attemptPlacePiece(int x, int y, BoardState*);
	void stopDraggingPiece();

	int getWidth();
	int getHeight();
	BoardState* getBoardState();

	//! Used this for debugging.
	void printBoardState(BoardState*);

	void renderPieces(BoardState*);
	void renderPieceTexture(SDL_Texture*, int x, int y);

	//! Loads a board form a given Forsyth–Edwards Notation.
	void loadBoardFromFen(const char* fen, BoardState*);

	void switchTurns(BoardState*);

	//! Updates the en passant squares.
	void updateEnPassant(int, int, int, int, BoardState*);
	bool isEnPassant(int fromX, int fromY, int toX, int toY, BoardState*);

	//! If a king moves he loses the right to castle.
	void updateCastling(int fromX, int fromY, int toX, int toY, BoardState*);

	
	void clearMoves();
	//! Calculates all moves that are following just the rules for moving a piece.
	std::vector<Move> calculatePseudoLegalMoves(BoardState*);
	//! Calculates all actually legal moves.
	std::vector<Move> calculateLegalMoves(BoardState*);
	//! Calculates all pseudo legal moves at given position.
	void calculateMovesAt(int x, int y, BoardState*, std::vector<Move>&);


	void nextTurn(BoardState* boardState);
	bool inLegalMoves(struct Move&);
	bool inPseudoMoves(struct Move&);

	void makeMove(struct Move, BoardState*);

	//what moves should we highlight?
	void renderHighlightMoves();
	void createHighlightMoves(int x, int y);

	//check and checkmate stuff
	bool kingInCheck(BoardState* currentBoardState);
	void findKingLocation(int*, int*, BoardState*);
	bool squareAttacked(int x, int y, BoardState* currentBoardState);

	void updateHighlightKingBox();
	void renderKingBox();


	int isGameOver(BoardState* currentBoardState);

	void reset();

	//! Usefull for testing.
	int totalPossibleFutureBoardPositions(BoardState*, int depth);
	void calculateBoardStates();

	void renderPromotionOptions();
	void tryPickingPromotionPiece(int, int, BoardState*);

	void promoteQueen(BoardState*);
	void promoteRook(BoardState*);
	void promoteBishop(BoardState*);
	void promoteKnight(BoardState*);

	void makeRandomMove(BoardState*);

};