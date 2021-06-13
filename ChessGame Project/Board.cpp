#include <iostream>

#include "Game.h"
#include "Window.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define HIGHLIGHT_COLOR {0,255,0,100}
#define ATTACK_COLOR {255,0,0,100}
#define WIN_COLOR {255,215,0,200}
#define AMOUNT_OF_BOX .8
#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

//#define TEST_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

int Board::boxXWidth;
int Board::boxYHeight;
int Board::boardXBoxes;
int Board::boardYBoxes;

void Board::reset()
{
	for (Piece* p : pieceContainer) {
		delete p;
	}
	pieceContainer.clear();

	for (int x = 0; x < boardXBoxes; ++x) {
		for (int y = 0; y < boardYBoxes; ++y) {

			boardState->getBoard()[x][y] = nullptr;
		}
	}
	draggingPiece = false;
	draggingPieceX = -1;
	draggingPieceY = -1;
	loadBoardFromFen(STARTING_FEN, boardState);

	legalMoves = calculateLegalMoves(boardState);

	highlightKingBox.x = -1;
	highlightKingBox.y = -1;
	winnerKing.x = winnerKing.y = -1;
	gameOver = false;
}

void Board::init()
{
	Piece::init();
	boardXBoxes = 8;
	boardYBoxes = 8;
	boxXWidth = Window::screenWidth / boardXBoxes;
	boxYHeight = Window::screenHeight / boardYBoxes;

	boardColor1 = { 234,233,210,255 };

	boardColor2 = { 75,115,153,255 };

	boardState = new BoardState();

	std::vector<std::vector<Piece*>> board;
	for (int x = 0; x < boardXBoxes; ++x) {
		std::vector<Piece*> column;
		for (int y = 0; y < boardYBoxes; ++y) {
			column.push_back(nullptr);
		}
		board.push_back(column);
	}
	boardState->setBoard(board);

	draggingPiece = false;
	draggingPieceX = -1;
	draggingPieceY = -1;
	loadBoardFromFen(STARTING_FEN, boardState);


	legalMoves = calculateLegalMoves(boardState);
	

	highlightKingBox.x = -1;
	highlightKingBox.y = -1;
	winnerKing.x = winnerKing.y = -1;
	gameOver = false;


}

Board::~Board()
{
	Piece::destroyImages();
	for (Piece* p : pieceContainer) {
		delete p;
	}
	pieceContainer.clear();
	
	delete boardState;
}

void Board::render(BoardState* currentBoardState)
{
	renderBoard();
	if (draggingPiece) {
		renderHighlightMoves();
	}
	if (highlightKingBox.x != -1 || winnerKing.x != -1) {
		renderKingBox();
	}
	renderPieces(currentBoardState);
	if (draggingPiece) {
		renderDraggedPiece();
	}
	if (waitingForPromotionChoice) {
		renderPromotionOptions();
	}

}

void Board::renderPromotionOptions()
{
	SDL_Rect renderRect;
	renderRect.w = boxXWidth * 2;
	renderRect.h = boxYHeight * 2;
	renderRect.x = Game::boardTopLeftX;
	renderRect.y = Game::boardTopLeftY + (getHeight() - renderRect.h) / 2;
	int w, h;
	SDL_Rect fromRect;
	fromRect.x = fromRect.y = 0;
	if (boardState->getCurrentTurn() == 'w') {
		SDL_QueryTexture(Piece::whiteQueenTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::whiteQueenTexture, &fromRect, &renderRect);
		renderRect.x += renderRect.w;

		SDL_QueryTexture(Piece::whiteRookTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer,Piece::whiteRookTexture, &fromRect, &renderRect);
		renderRect.x += renderRect.w;

		SDL_QueryTexture(Piece::whiteBishopTexture , NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::whiteBishopTexture, &fromRect, &renderRect);
		renderRect.x += renderRect.w;

		SDL_QueryTexture(Piece::whiteKnightTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::whiteKnightTexture , &fromRect, &renderRect);


	}
	else {
		SDL_QueryTexture(Piece::blackQueenTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::blackQueenTexture, &fromRect, &renderRect);
		renderRect.x += renderRect.w;

		SDL_QueryTexture(Piece::blackRookTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::blackRookTexture, &fromRect, &renderRect);
		renderRect.x += renderRect.w;

		SDL_QueryTexture(Piece::blackBishopTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::blackBishopTexture, &fromRect, &renderRect);
		renderRect.x += renderRect.w;

		SDL_QueryTexture(Piece::blackKnightTexture, NULL, NULL, &w, &h);
		fromRect.w = w;
		fromRect.h = h;
		SDL_RenderCopy(Window::renderer, Piece::blackKnightTexture, &fromRect, &renderRect);
	}
}


void Board::renderDraggedPiece()
{
	int w, h, mouseX, mouseY;
	SDL_QueryTexture(draggingPieceTexture, NULL, NULL, &w, &h);
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Rect fromRect, toRect;
	fromRect.w = w;
	fromRect.h = h;
	fromRect.x = fromRect.y = 0;

	toRect.w = boxXWidth * AMOUNT_OF_BOX;
	toRect.h = boxYHeight * AMOUNT_OF_BOX;



	toRect.x = mouseX - (toRect.w / 2);
	toRect.y = mouseY - (toRect.h / 2);


	SDL_RenderCopy(Window::renderer, draggingPieceTexture, &fromRect, &toRect);

}

BoardState* Board::getBoardState()
{
	return boardState;
}

void Board::renderBoard()
{
	SDL_Rect drawRect;
	drawRect.w = boxXWidth;
	drawRect.h = boxYHeight;
	for (int x = 0; x < boardXBoxes; ++x) {
		for (int y = 0; y < boardYBoxes; ++y) {

			SDL_Color currentColor = (x + y) % 2 == 0 ? boardColor1 : boardColor2;
			drawRect.x = Game::boardTopLeftX + x * boxXWidth;
			drawRect.y = Game::boardTopLeftY + y * boxYHeight;
			SDL_SetRenderDrawColor(Window::renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
			SDL_RenderFillRect(Window::renderer, &drawRect);
		}
	}
}


void Board::renderPieces(BoardState* currentBoardState)
{
	std::vector<std::vector<Piece*>>& board = currentBoardState->getBoard();

	for (int x = 0; x < boardXBoxes; ++x) {
		for (int y = 0; y < boardYBoxes; ++y) {
			if (board[x][y] != nullptr) {
				if (x != draggingPieceX || y != draggingPieceY) {
					renderPieceTexture(board[x][y]->getPieceTexture(), x, y);
				}
			}
		}
	}
}


void Board::renderPieceTexture(SDL_Texture* texture, int x, int y)
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect fromRect, toRect;
	fromRect.w = w;
	fromRect.h = h;
	fromRect.x = fromRect.y = 0;

	toRect.w = boxXWidth * AMOUNT_OF_BOX;
	toRect.h = boxYHeight * AMOUNT_OF_BOX;


	toRect.x = Game::boardTopLeftX + x * boxXWidth + (boxXWidth - toRect.w) / 2;
	toRect.y = Game::boardTopLeftY + y * boxYHeight + (boxYHeight - toRect.h) / 2;


	SDL_RenderCopy(Window::renderer, texture, &fromRect, &toRect);
}


void Board::resize()
{
	boxXWidth = Window::screenWidth / boardXBoxes;
	boxYHeight = Window::screenHeight / boardYBoxes;

	if (boxXWidth > boxYHeight) {
		boxXWidth = boxYHeight;
	}
	else {
		boxYHeight = boxXWidth;
	}
}


int Board::getHeight()
{
	return boardYBoxes * boxYHeight;
}

int Board::getWidth()
{
	return boardXBoxes * boxXWidth;
}

void Board::printBoardState(BoardState* currentBoardState)
{
	if (currentBoardState->getWhiteCanKingsideCastle()) {
		std::cout << "White can kingside castle." << std::endl;
	}
	else {
		std::cout << "White cannot kingside castle." << std::endl;
	}
	if (currentBoardState->getWhiteCanQueensideCastle()) {
		std::cout << "White can queenside castle." << std::endl;
	}
	else {
		std::cout << "White cannot queenside castle." << std::endl;
	}
	if (currentBoardState->getBlackCanKingsideCastle()) {
		std::cout << "Black can kingside castle." << std::endl;
	}
	else {
		std::cout << "Black cannot kingside castle." << std::endl;
	}
	if (currentBoardState->getBlackCanQueensideCastle()) {
		std::cout << "Black can Queenside castle." << std::endl;
	}
	else {
		std::cout << "Black cannot Queenside castle." << std::endl;
	}
	if (currentBoardState->getEnPassantX() != -1) {
		std::cout << "En passant opportunity at x: " << boardState->getEnPassantX() << " and y: " << boardState->getEnPassantY() << std::endl;
	}
	else {
		std::cout << " no en passant :( holy hell" << std::endl;
	}
}

void Board::loadBoardFromFen(const char* fenNotation, BoardState* currentBoardState)
{
	int index = 0;
	int column;

	std::vector<std::vector<Piece*>>& board = currentBoardState->getBoard();
	for (int rank = 0; rank < boardYBoxes; rank++) {
		column = 0;
		while (fenNotation[index] != '/' && fenNotation[index] != ' ') {
			if (isdigit(fenNotation[index])) {
			

				column += (fenNotation[index] - '0');
				index++;
			}
			else {
				if (board[column][rank] != nullptr) {
					delete board[column][rank];
				}

				switch (fenNotation[index]) {
				case 'P':
					board[column][rank] = new Pawn(true);
					break;
				case 'p':
					board[column][rank] = new Pawn(false);
					break;
				case 'R':
					board[column][rank] = new Rook(true);
					break;
				case 'r':
					board[column][rank] = new Rook(false);
					break;
				case 'N':
					board[column][rank] = new Knight(true);
					break;
				case 'n':
					board[column][rank] = new Knight(false);
					break;
				case 'B':
					board[column][rank] = new Bishop(true);
					break;
				case 'b':
					board[column][rank] = new Bishop(false);
					break;
				case 'Q':
					board[column][rank] = new Queen(true);
					break;
				case 'q':
					board[column][rank] = new Queen(false);
					break;
				case 'K':
					board[column][rank] = new King(true);
					break;
				case 'k':
					board[column][rank] = new King(false);
					break;
				}

				pieceContainer.push_back(board[column][rank]);

				++index;
				++column;
			}
		}
		++index;
	}
	//currentBoardState->setBoard(board);

	//now we are out of the long /'s, and are onto the current players turn.
	currentBoardState->setCurrentTurn(fenNotation[index]);
	index += 2;
	currentBoardState->setWhiteCanKingsideCastle(false);
	currentBoardState->setBlackCanKingsideCastle(false);
	currentBoardState->setWhiteCanQueensideCastle(false);
	currentBoardState->setBlackCanQueensideCastle(false);

	//now we are onto the castling.
	if (fenNotation[index] == '-') {

		index += 2;
	}
	else {
		while (fenNotation[index] != ' ') {
			switch (fenNotation[index]) {
			case 'K':
				currentBoardState->setWhiteCanKingsideCastle(true);
				break;
			case 'k':
				currentBoardState->setBlackCanKingsideCastle(true);
				break;
			case 'Q':
				currentBoardState->setWhiteCanQueensideCastle(true);
				break;
			case 'q':
				currentBoardState->setBlackCanQueensideCastle(true);
				break;
			}
			++index;
		}
		++index;
	}

	//now we are onto the en-passant option.

	if (fenNotation[index] == '-') {
		index += 2;
		currentBoardState->setEnPassantX(-1);
		currentBoardState->setEnPassantY(-1);
	}
	else {
		currentBoardState->setEnPassantX(fenNotation[index] - 'a');
		++index;
		currentBoardState->setEnPassantY(boardYBoxes - (fenNotation[index] - '0'));
		++index;
	}
}



void Board::handleMouseButtonDown(SDL_MouseButtonEvent& b, BoardState* currentBoardState)
{
	int x, y, boardX, boardY;
	if (b.button == SDL_BUTTON_LEFT) {
		SDL_GetMouseState(&x, &y);
		if (x < Game::boardTopLeftX || x > Game::boardTopLeftX + getWidth()) {

		}
		else if (y < Game::boardTopLeftY || y > Game::boardTopLeftY + getHeight()) {

		}
		else {
			boardX = (x - Game::boardTopLeftX) / boxXWidth;
			boardY = (y - Game::boardTopLeftY) / boxYHeight;
			if (waitingForPromotionChoice) {
				tryPickingPromotionPiece(boardX, boardY,currentBoardState);
			}
			else {
				if (!draggingPiece) {
					attemptPickupPiece(boardX, boardY, currentBoardState);
				}
				else {
					attemptPlacePiece(boardX, boardY, currentBoardState);
				}
			}
		}

	}
	else if (b.button == SDL_BUTTON_RIGHT) {
		stopDraggingPiece();
	}
}


void Board::tryPickingPromotionPiece(int x, int y, BoardState* currentBoardState)
{
	if (y == 3 || y == 4) {
		switch (x / 2) {
		case 0:
			promoteQueen(currentBoardState);
			break;
		case 1:
			promoteRook(currentBoardState);
			break;
		case 2:
			promoteBishop(currentBoardState);
			break;
	
		case 3:
			promoteKnight(currentBoardState);
			break;
		}
		waitingForPromotionChoice = false;
		nextTurn(currentBoardState);
	}
}


void Board::promoteQueen(BoardState* currentBoardState)
{
	makeMove({promotionMove.fromX,promotionMove.fromY,
		promotionMove.toX,promotionMove.toY,
		false,false,false,true,'q'}, currentBoardState);
}

void Board::promoteRook(BoardState* currentBoardState)
{
	makeMove({ promotionMove.fromX,promotionMove.fromY,
		promotionMove.toX,promotionMove.toY,
		false,false,false,true,'r' }, currentBoardState);
}

void Board::promoteKnight(BoardState* currentBoardState)
{
	makeMove({ promotionMove.fromX,promotionMove.fromY,
		promotionMove.toX,promotionMove.toY,
		false,false,false,true,'n' }, currentBoardState);
}

void Board::promoteBishop(BoardState* currentBoardState)
{
	makeMove({ promotionMove.fromX,promotionMove.fromY,
		promotionMove.toX,promotionMove.toY,
		false,false,false,true,'b' }, currentBoardState);
}


void Board::attemptPickupPiece(int x, int y, BoardState* currentBoardState)
{
	//if there's a piece on the board space we're clicking.
	if (currentBoardState->getBoard()[x][y] != nullptr) {
		//if it's the piece for the curernt players turn.

		if (currentBoardState->pieceIsCurrentPlayerPiece(x, y)) {
			draggingPiece = true;
			draggingPieceX = x;
			draggingPieceY = y;
			draggingPieceTexture = currentBoardState->getBoard()[x][y]->getPieceTexture();
			createHighlightMoves(x, y);
		}
	}
}


void Board::attemptPlacePiece(int x, int y, BoardState* currentBoardState)
{
	Move newMove = { draggingPieceX,draggingPieceY,x,y };
	int enPassantX = currentBoardState->getEnPassantX();
	int enPassantY = currentBoardState->getEnPassantY();

	//inLegalMoves passes in a reference so we update the castling and sturff here.
	if (inLegalMoves(newMove)) {
		if (newMove.isPromotion) {
			promotionMove = {
				newMove.fromX,
				newMove.fromY,
				newMove.toX,
				newMove.toY,
				false,
				false,
				false,
				true,
				newMove.promotionType
			};
			
			waitingForPromotionChoice = true;
		}
		else {
			
			makeMove(newMove, currentBoardState);
			nextTurn(currentBoardState);
		}
		stopDraggingPiece();
		
	}
	else {
		stopDraggingPiece();
	}
}


void Board::nextTurn(BoardState* currentBoardState)
{
	switchTurns(currentBoardState);

	legalMoves = calculateLegalMoves(currentBoardState);

	if (isGameOver(currentBoardState) == 1) {
		gameOver = true;
	}

	updateHighlightKingBox();
}


void Board::switchTurns(BoardState* currentBoardState)
{
	currentBoardState->setCurrentTurn((currentBoardState->getCurrentTurn() == 'w') ? 'b' : 'w');
}


bool Board::inLegalMoves(struct Move& newMove)
{
	for (int i = 0; i < legalMoves.size(); i++) {
		if (newMove.fromX == legalMoves.at(i).fromX &&
			newMove.toX == legalMoves.at(i).toX &&
			newMove.fromY == legalMoves.at(i).fromY &&
			newMove.toY == legalMoves.at(i).toY) {

			newMove.kingSideCastle = legalMoves.at(i).kingSideCastle;
			newMove.queenSideCastle = legalMoves.at(i).queenSideCastle;
			newMove.enPassant = legalMoves.at(i).enPassant;
			newMove.isPromotion = legalMoves.at(i).isPromotion;
			newMove.promotionType = legalMoves.at(i).promotionType;
			
			return true;
		}
	}
	return false;
}


bool Board::inPseudoMoves(struct Move& newMove)
{
	for (int i = 0; i < pseudoLegalMoves.size(); i++) {
		
		if (newMove.fromX == pseudoLegalMoves.at(i).fromX &&
			newMove.toX == pseudoLegalMoves.at(i).toX &&
			newMove.fromY == pseudoLegalMoves.at(i).fromY &&
			newMove.toY == pseudoLegalMoves.at(i).toY) {

			newMove.kingSideCastle = pseudoLegalMoves.at(i).kingSideCastle;
			newMove.queenSideCastle = pseudoLegalMoves.at(i).queenSideCastle;
			newMove.enPassant = pseudoLegalMoves.at(i).enPassant;
			newMove.isPromotion = pseudoLegalMoves.at(i).isPromotion;
			newMove.promotionType = pseudoLegalMoves.at(i).promotionType;
			
			return true;
		}
	}
	return false;
}


void Board::stopDraggingPiece()
{
	draggingPiece = false;
	draggingPieceX = draggingPieceY = -1;
}


void Board::clearMoves()
{
	pseudoLegalMoves.clear();
	legalMoves.clear();
}


std::vector<Move> Board::calculatePseudoLegalMoves(BoardState* currentBoardState)
{
	std::vector<Move> returnVec;
	for (int x = 0; x < boardXBoxes; ++x) {
		for (int y = 0; y < boardYBoxes; ++y) {
			if (currentBoardState->getBoard()[x][y] != nullptr) {
				//if the piece is a piece that can be moved this turn.
				if (currentBoardState->pieceIsCurrentPlayerPiece(x, y)) {
					calculateMovesAt(x, y, currentBoardState, returnVec);
				}
			}
		}
	}

	return returnVec;
}


std::vector<Move> Board::calculateLegalMoves(BoardState* currentBoardState)
{
	std::vector<Move> currentLegal;
	//std::cout << "Calling this once" << std::endl;
	
	BoardState* newBoardState;
	pseudoLegalMoves = calculatePseudoLegalMoves(currentBoardState);
	for (int i = 0; i < pseudoLegalMoves.size(); i++) {
		//the king can't castle if it's in check
		if (kingInCheck(currentBoardState)) {
			if (pseudoLegalMoves.at(i).kingSideCastle) {
				continue;
			}
			else if (pseudoLegalMoves.at(i).queenSideCastle) {
				continue;
			}
		}
		//the king can't castle through check.
		if (pseudoLegalMoves.at(i).kingSideCastle || pseudoLegalMoves.at(i).queenSideCastle) {
			if (squareAttacked((pseudoLegalMoves.at(i).fromX + pseudoLegalMoves.at(i).toX) / 2,
				pseudoLegalMoves.at(i).fromY,currentBoardState)) {
				continue;
			}
		}
		//there must be clear spaces between the rook and king to be able to castle.
		bool inValid = false;
		if (pseudoLegalMoves.at(i).kingSideCastle) {
			for (int x = pseudoLegalMoves.at(i).fromX + 1; x < boardXBoxes - 1; x++) {
				if (currentBoardState->getBoard()[x][pseudoLegalMoves.at(i).fromY] != nullptr) {
					inValid = true;
					break;
				}
			}
		}
		else if (pseudoLegalMoves.at(i).queenSideCastle) {
			for (int x =1; x < pseudoLegalMoves.at(i).fromX; x++) {
				if (currentBoardState->getBoard()[x][pseudoLegalMoves.at(i).fromY] != nullptr) {
					inValid = true;
					break;
				}
			}
		}
		if (inValid) {
			continue;
		}
		newBoardState = BoardState::copyBoardState(currentBoardState);

		makeMove(pseudoLegalMoves.at(i), newBoardState);
			
		if (!kingInCheck(newBoardState)) {
			//because kingInCheck changed the vector... but it's not working
			currentLegal.push_back({ pseudoLegalMoves.at(i).fromX,
				pseudoLegalMoves.at(i).fromY,
				pseudoLegalMoves.at(i).toX,
				pseudoLegalMoves.at(i).toY,
				pseudoLegalMoves.at(i).kingSideCastle,
				pseudoLegalMoves.at(i).queenSideCastle,
				pseudoLegalMoves.at(i).enPassant,
				pseudoLegalMoves.at(i).isPromotion,
				pseudoLegalMoves.at(i).promotionType}
				);
		}

		delete newBoardState;
	}
	return currentLegal;
}


void Board::makeMove(struct Move move, BoardState* currentBoardState)
{
	std::vector<std::vector<Piece*>>& board = currentBoardState->getBoard();
	int enPassantX = currentBoardState->getEnPassantX();
	int enPassantY = currentBoardState->getEnPassantY();
	if (isEnPassant(move.fromX, move.fromY, move.toX, move.toY, currentBoardState)) {
		//the piece removed depends on the turn.
		if (currentBoardState->getCurrentTurn() == 'w') {
			board[enPassantX][enPassantY + 1] = nullptr;
		}
		else {
			board[enPassantX][enPassantY - 1] = nullptr;
		}
	}
	
	if (move.kingSideCastle) {
		board[move.toX - 1][move.toY] = board[move.toX + 1][move.toY];
		board[move.toX + 1][move.toY] = nullptr;
	}
	else if (move.queenSideCastle) {
		board[move.toX + 1][move.toY] = board[move.toX - 2][move.toY];
		board[move.toX - 2][move.toY] = nullptr;
	}

	updateEnPassant(move.fromX, move.fromY, move.toX, move.toY, currentBoardState);
	updateCastling(move.fromX, move.fromY, move.toX, move.toY, currentBoardState);
	char turn = currentBoardState->getCurrentTurn();
	if (move.isPromotion) {
		switch (move.promotionType) {
		case 'q':
			if (turn == 'w') {
				board[move.toX][move.toY] = new Queen(true);
			}
			else {
				board[move.toX][move.toY] = new Queen(false);
			}
			break;
		case 'r':
			if (turn == 'w') {
				board[move.toX][move.toY] = new Rook(true);
			}
			else {
				board[move.toX][move.toY] = new Rook(false);
			}
			break;
		case 'n':
			if (turn == 'w') {
				board[move.toX][move.toY] = new Knight(true);
			}
			else {
				board[move.toX][move.toY] = new Knight(false);
			}
			break;
		case 'b':
			if (turn == 'w') {
				board[move.toX][move.toY] = new Bishop(true);
			}
			else {
				board[move.toX][move.toY] = new Bishop(false);
			}
			break;
		}

		pieceContainer.push_back(board[move.toX][move.toY]);
	}
	else {
		board[move.toX][move.toY] = board[move.fromX][move.fromY];
	}
	
	board[move.fromX][move.fromY] = nullptr;
	//currentBoardState->setBoard(board);
}


void Board::calculateMovesAt(int x, int y, BoardState* currentBoardState, std::vector<Move>& currentPseudo) 
{
	currentBoardState->getBoard()[x][y]->getPseudoLegalMoves(currentBoardState->getBoard(), 
		x, y, boardXBoxes, boardYBoxes, currentBoardState->getCurrentTurn(), 
		currentBoardState->getEnPassantX(), currentBoardState->getEnPassantY(), 
		currentBoardState->getWhiteCanKingsideCastle(), currentBoardState->getWhiteCanQueensideCastle(), 
		currentBoardState->getBlackCanKingsideCastle(), currentBoardState->getBlackCanQueensideCastle(), 
		currentPseudo);
}



void Board::updateCastling(int fromX, int fromY, int toX, int toY, BoardState* currentBoardState)
{
	//if the king moved.
	int queenSideX = 0;
	int kingSideX = boardXBoxes - 1;
	int whiteY = boardYBoxes - 1;
	int blackY = 0;

	std::vector<std::vector<Piece*>>& board = boardState->getBoard();

	if (currentBoardState->getCurrentTurn() == 'w') {
		if (board[fromX][fromY]->getPieceType() == PieceType::KING) {
			currentBoardState->setWhiteCanKingsideCastle(false);
			currentBoardState->setWhiteCanQueensideCastle(false);
		}

		if (fromX == queenSideX && fromY == whiteY) {
			currentBoardState->setWhiteCanQueensideCastle(false);
		}
		else if (fromX == kingSideX && fromY == whiteY) {
			currentBoardState->setWhiteCanKingsideCastle(false);
		}

		if (toX == kingSideX && toY == blackY) {
			currentBoardState->setBlackCanKingsideCastle(false);
		}
		else if (toX == queenSideX && toY == blackY) {
			currentBoardState->setBlackCanQueensideCastle(false);
		}
	}
	else {
		if (board[fromX][fromY]->getPieceType() == PieceType::KING) {
			currentBoardState->setBlackCanKingsideCastle(false);
			currentBoardState->setBlackCanQueensideCastle(false);
		}

		if (fromX == queenSideX && fromY == blackY) {
			currentBoardState->setBlackCanQueensideCastle(false);
		}
		else if (fromX == kingSideX && fromY == blackY) {
			currentBoardState->setBlackCanKingsideCastle(false);
		}

		//if the move takes a rook.
		if (toX == kingSideX && toY == whiteY) {
			currentBoardState->setWhiteCanKingsideCastle(false);
		}
		else if (toX == queenSideX && toY == whiteY) {
			currentBoardState->setWhiteCanQueensideCastle(false);
		}
	}


}


void Board::renderHighlightMoves()
{
	SDL_Rect highlightRect;
	highlightRect.w = boxXWidth;
	highlightRect.h = boxYHeight;
	SDL_Color drawColor = HIGHLIGHT_COLOR;
	SDL_SetRenderDrawColor(Window::renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	for (int i = 0; i < highlightMoves.size(); i++) {
		highlightRect.x = Game::boardTopLeftX + highlightMoves.at(i).toX * boxXWidth;
		highlightRect.y = Game::boardTopLeftY + highlightMoves.at(i).toY * boxYHeight;
		SDL_RenderFillRect(Window::renderer, &highlightRect);
	}
}


void Board::createHighlightMoves(int x, int y)
{
	highlightMoves.clear();
	for (int i = 0; i < legalMoves.size(); i++) {
		if (legalMoves.at(i).fromX == x &&
			legalMoves.at(i).fromY == y) {
			highlightMoves.push_back({
				legalMoves.at(i).fromX,
				legalMoves.at(i).fromY,
				legalMoves.at(i).toX,
				legalMoves.at(i).toY
				});
		}
	}
}


void Board::updateEnPassant(int fromX, int fromY, int toX, int toY, BoardState* currentBoardState)
{
	if (currentBoardState->getBoard()[fromX][fromY]->getPieceType() == PieceType::PAWN) {
		if (abs(fromY - toY) == 2) {
			currentBoardState->setEnPassantX(fromX);
			currentBoardState->setEnPassantY((fromY + toY) / 2);
		}
		else {
			currentBoardState->setEnPassantX(-1);
			currentBoardState->setEnPassantY(-1);
		}
	}
	else {
		currentBoardState->setEnPassantX(-1);
		currentBoardState->setEnPassantY(-1);
	}
}


void Board::findKingLocation(int* xPos, int* yPos, BoardState* currentBoardState) 
{
	char currentPlayer = currentBoardState->getCurrentTurn();
	std::vector<std::vector<Piece*>>& board = currentBoardState->getBoard();
	for (int x = 0; x < boardXBoxes; ++x) {
		for (int y = 0; y < boardYBoxes; ++y) {
			Piece* currentSquare = board[x][y];
			if (currentSquare == nullptr) {
				continue;
			}

			if (currentPlayer == 'w') {
				if (currentSquare->isWhite() && currentSquare->getPieceType() == PieceType::KING) {
					*xPos = x;
					*yPos = y;
					return;
				}
			}
			else {
				if (!currentSquare->isWhite() && currentSquare->getPieceType() == PieceType::KING) {
					*xPos = x;
					*yPos = y;
					return;
				}
			}
		}
	}
}


bool Board::isEnPassant(int fromX, int fromY, int toX, int toY, BoardState* currentBoardState)
{
	if (currentBoardState->getBoard()[fromX][fromY]->getPieceType() == PieceType::PAWN) {
		if (toX == currentBoardState->getEnPassantX() && toY == currentBoardState->getEnPassantY()) {
			return true;
		}
	}

	return false;
}


bool Board::squareAttacked(int x, int y, BoardState* currentBoardState)
{
	BoardState* newBoardState = BoardState::copyBoardState(currentBoardState);

	switchTurns(newBoardState);

	std::vector<Move> currentPseudo = calculatePseudoLegalMoves(newBoardState);

	for (int i = 0; i < currentPseudo.size(); i++) {
		if (currentPseudo.at(i).toX == x && currentPseudo.at(i).toY == y) {
			return true;
		}
	}
	
	delete newBoardState;
	return false;
}


bool Board::kingInCheck(BoardState* currentBoardState)
{
	//if the current players king is under attack then the king is in check.
	int kingX, kingY;
	findKingLocation(&kingX, &kingY, currentBoardState);

	bool result = squareAttacked(kingX, kingY, currentBoardState);

	return result;
}


void Board::updateHighlightKingBox()
{
	if (kingInCheck(boardState)) {
		findKingLocation(&highlightKingBox.x, &highlightKingBox.y, boardState);
	}
	else {
		highlightKingBox.x = highlightKingBox.y = -1;
	}
	if (gameOver) {
		BoardState* newBoardState = BoardState::copyBoardState(boardState);
		switchTurns(newBoardState);
		findKingLocation(&winnerKing.x, &winnerKing.y, newBoardState);
		delete newBoardState;
	}
}


void Board::renderKingBox()
{
	SDL_Rect highlightRect;
	highlightRect.w = boxXWidth;
	highlightRect.h = boxYHeight;
	SDL_Color drawColor;
	if (highlightKingBox.x != -1) {
		drawColor = ATTACK_COLOR;
		SDL_SetRenderDrawColor(Window::renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

		highlightRect.x = Game::boardTopLeftX + highlightKingBox.x * boxXWidth;
		highlightRect.y = Game::boardTopLeftY + highlightKingBox.y * boxYHeight;
		SDL_RenderFillRect(Window::renderer, &highlightRect);
	}
	if (winnerKing.x != -1) {
		drawColor = WIN_COLOR;
		SDL_SetRenderDrawColor(Window::renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

		highlightRect.x = Game::boardTopLeftX + winnerKing.x * boxXWidth;
		highlightRect.y = Game::boardTopLeftY + winnerKing.y * boxYHeight;
		SDL_RenderFillRect(Window::renderer, &highlightRect);
	}
}


int Board::isGameOver(BoardState* currentBoardState)
{
	std::vector<Move> currentLegalMoves = calculateLegalMoves(currentBoardState);
	if (currentLegalMoves.size() == 0) {
		if (kingInCheck(currentBoardState)) {
			winner = (currentBoardState->getCurrentTurn() == 'w' ? 'b' : 'w');
		}
		else {
			winner = 'p';
		}
		
		return 1;
	}
	else {
		return 0;
	}
}


void Board::calculateBoardStates()
{
	int initialTime = SDL_GetTicks();
	std::cout << "Total Board states in 1 move: " << totalPossibleFutureBoardPositions(boardState, 1) << std::endl;
	std::cout << "Took : " << SDL_GetTicks() - initialTime << " Milliseconds" << std::endl;
	initialTime = SDL_GetTicks();
	std::cout << "Total Board states in 2 moves: " << totalPossibleFutureBoardPositions(boardState, 2) << std::endl;
	std::cout << "Took : " << SDL_GetTicks() - initialTime << " Milliseconds" << std::endl;
}


int Board::totalPossibleFutureBoardPositions(BoardState* currentBoardState, int depth)
{
	int totalAmount = 0;
	if (depth == 0) {
		return 1;		//if we're at the end then this is a board state
	}
	std::vector<Move> legalMoves = calculateLegalMoves(currentBoardState);
	for (int i = 0; i < legalMoves.size(); i++) {
		
		BoardState* newBoardState = BoardState::copyBoardState(currentBoardState);
		makeMove(legalMoves.at(i), newBoardState);
		switchTurns(newBoardState);
		int amountOfMoves = totalPossibleFutureBoardPositions(newBoardState, depth - 1);
		totalAmount += amountOfMoves;

		delete newBoardState;
	}
	return totalAmount;
}


void Board::makeRandomMove(BoardState* currentBoardState)
{
	legalMoves = calculateLegalMoves(currentBoardState);

	if (legalMoves.size()) {
		int choice = rand() % legalMoves.size();
		makeMove(legalMoves.at(choice), currentBoardState);
		nextTurn(currentBoardState);
	}
}