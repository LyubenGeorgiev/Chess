#include "Piece.h"
#include "Window.h"


SDL_Texture* Piece::blackKingTexture;
SDL_Texture* Piece::whiteKingTexture;
SDL_Texture* Piece::blackKnightTexture;
SDL_Texture* Piece::whiteKnightTexture;
SDL_Texture* Piece::blackRookTexture;
SDL_Texture* Piece::whiteRookTexture;
SDL_Texture* Piece::blackBishopTexture;
SDL_Texture* Piece::whiteBishopTexture;
SDL_Texture* Piece::whiteQueenTexture;
SDL_Texture* Piece::blackQueenTexture;
SDL_Texture* Piece::whitePawnTexture;
SDL_Texture* Piece::blackPawnTexture;


void Piece::init()
{
	loadImages();
}


void Piece::loadImages()
{
	SDL_Surface* surface;
	surface = IMG_Load("../../assets/images/blackKing.png");
	blackKingTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/whiteKing.png");
	whiteKingTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/blackQueen.png");
	blackQueenTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/whitequeen.png");
	whiteQueenTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/blackBishop.png");
	blackBishopTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/whiteBishop.png");
	whiteBishopTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/blackKnight.png");
	blackKnightTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/whiteKnight.png");
	whiteKnightTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/blackRook.png");
	blackRookTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/whiteRook.png");
	whiteRookTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/blackPawn.png");
	blackPawnTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("../../assets/images/whitePawn.png");
	whitePawnTexture = SDL_CreateTextureFromSurface(Window::renderer, surface);
	SDL_FreeSurface(surface);


}

void Piece::destroyImages()
{
	SDL_DestroyTexture(whitePawnTexture);
	SDL_DestroyTexture(blackPawnTexture);

	SDL_DestroyTexture(whiteRookTexture);
	SDL_DestroyTexture(blackRookTexture);

	SDL_DestroyTexture(whiteKnightTexture);
	SDL_DestroyTexture(blackKnightTexture);

	SDL_DestroyTexture(whiteBishopTexture);
	SDL_DestroyTexture(blackBishopTexture);

	SDL_DestroyTexture(whiteKingTexture);
	SDL_DestroyTexture(blackKingTexture);

	SDL_DestroyTexture(whiteQueenTexture);
	SDL_DestroyTexture(blackQueenTexture);
}


bool pieceIsCurrentPlayerPiece(int x, int y, std::vector<std::vector<Piece*>> board, char currentTurn)
{
	if (board[x][y] && ((currentTurn == 'w' && board[x][y]->isWhite()) || (currentTurn == 'b' && !(board[x][y]->isWhite())))) {
		return true;
	}
	return false;
}


Piece::Piece(bool colorIsWhite)
	: white(colorIsWhite)
{

}


bool Piece::isWhite()
{
	return white;
}


PieceType Piece::getPieceType()
{
	return pieceType;
}


SDL_Texture* Piece::getPieceTexture()
{
	return pieceTexture;
}


void Piece::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{

}


//Pawn code
Pawn::Pawn(bool colorIsWhite)
	: Piece(colorIsWhite)
{
	pieceType = PieceType::PAWN;
	pieceTexture = (white ? whitePawnTexture : blackPawnTexture);
}


void Pawn::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{
	if (currentTurn == 'w') {
		//moving forward one.
		if (y - 1 >= 0) {
			//pawns cant take vertically.
			if (board[x][y - 1] == nullptr) {
				//if it's a promotion
				if (y - 1 == 0) {
					currentPseudo.push_back({ x,y,x,y - 1,false,false,false,true,'q' });
					currentPseudo.push_back({ x,y,x,y - 1,false,false,false,true,'r' });
					currentPseudo.push_back({ x,y,x,y - 1,false,false,false,true,'n' });
					currentPseudo.push_back({ x,y,x,y - 1,false,false,false,true,'b' });
				}
				else {
					currentPseudo.push_back({ x,y,x,y - 1,false,false,false,false,' ' });
				}

				//we only can move forward 2 if the space is open as well.
				if (y == boardYBoxes - 2) {	//if it's in the starting position.
					if (y - 2 >= 0) {	//this shouldn't be necessary except for tiny boards
						if (board[x][y - 2] == nullptr) {
							currentPseudo.push_back({ x,y,x,y - 2, false, false, false,false,' ' });
						}
					}
				}
			}
		}

		//pawns capture diagonally.
		if (y - 1 >= 0) {
			//if we're not at the edge of the board
			if (x - 1 >= 0) {
				if (board[x - 1][y - 1] != nullptr && !pieceIsCurrentPlayerPiece(x - 1, y - 1, board, currentTurn)) {
					if (y - 1 == 0) {
						currentPseudo.push_back({ x,y,x - 1,y - 1, false, false, false,true,'q' });
						currentPseudo.push_back({ x,y,x - 1,y - 1, false, false, false,true,'r' });
						currentPseudo.push_back({ x,y,x - 1,y - 1, false, false, false,true,'n' });
						currentPseudo.push_back({ x,y,x - 1,y - 1, false, false, false,true,'b' });
					}
					else {
						currentPseudo.push_back({ x,y,x - 1,y - 1, false, false, false,false,' ' });
					}

				}
				//en passant
				else if (x - 1 == enPassantX && y - 1 == enPassantY) {
					currentPseudo.push_back({ x,y,x - 1,y - 1,false,false,true });
				}
			}
			if (x + 1 < boardXBoxes) {
				if (board[x + 1][y - 1] != nullptr && !pieceIsCurrentPlayerPiece(x + 1, y - 1, board, currentTurn)) {
					if (y - 1 == 0) {
						currentPseudo.push_back({ x,y,x + 1,y - 1, false, false, false,true,'q' });
						currentPseudo.push_back({ x,y,x + 1,y - 1, false, false, false,true,'r' });
						currentPseudo.push_back({ x,y,x + 1,y - 1, false, false, false,true,'n' });
						currentPseudo.push_back({ x,y,x + 1,y - 1, false, false, false,true,'b' });
					}
					else {
						currentPseudo.push_back({ x,y,x + 1,y - 1, false, false, false,false,' ' });
					}

				}
				//en passant
				else if (x + 1 == enPassantX && y - 1 == enPassantY) {
					currentPseudo.push_back({ x,y,x + 1,y - 1,false,false,true,false,' ' });
				}
			}
		}
	}
	else {
		//moving forward one.
		if (y + 1 < boardYBoxes) {
			//pawns cant take vertically.
			if (board[x][y + 1] == nullptr) {
				if (y + 1 == boardYBoxes - 1) {
					currentPseudo.push_back({ x,y,x,y + 1 ,false,false,false,true,'r' });
					currentPseudo.push_back({ x,y,x,y + 1 ,false,false,false,true,'n' });
					currentPseudo.push_back({ x,y,x,y + 1 ,false,false,false,true,'b' });
					currentPseudo.push_back({ x,y,x,y + 1 ,false,false,false,true,'q' });
				}
				else {
					currentPseudo.push_back({ x,y,x,y + 1 ,false,false,false,false,' ' });
				}

				//we only can move forward 2 if the space is open as well.
				if (y == 1) {	//if it's in the starting position.
					if (y + 2 < boardYBoxes) {	//this shouldn't be necessary except for tiny boards
						if (board[x][y + 2] == nullptr) {
							currentPseudo.push_back({ x,y,x,y + 2, false, false, false });
						}
					}
				}
			}
		}

		//pawns capture diagonally.
		if (y + 1 < boardYBoxes) {
			//if we're not at the edge of the board
			if (x - 1 >= 0) {
				if (board[x - 1][y + 1] != nullptr && !pieceIsCurrentPlayerPiece(x - 1, y + 1, board, currentTurn)) {
					//promotion time
					if (y + 1 == boardYBoxes - 1) {
						currentPseudo.push_back({ x,y,x - 1,y + 1 ,false,false,false,true,'q' });
						currentPseudo.push_back({ x,y,x - 1,y + 1 ,false,false,false,true,'r' });
						currentPseudo.push_back({ x,y,x - 1,y + 1 ,false,false,false,true,'b' });
						currentPseudo.push_back({ x,y,x - 1,y + 1 ,false,false,false,true,'n' });
					}
					else {
						currentPseudo.push_back({ x,y,x - 1,y + 1 ,false,false,false,false,' ' });
					}
				}
				else if (x - 1 == enPassantX && y + 1 == enPassantY) {
					currentPseudo.push_back({ x,y,x - 1,y + 1,false,false,true });
				}
			}
			if (x + 1 < boardXBoxes) {
				if (board[x + 1][y + 1] != nullptr && !pieceIsCurrentPlayerPiece(x + 1, y + 1, board, currentTurn)) {

					if (y + 1 == boardYBoxes - 1) {
						currentPseudo.push_back({ x,y,x + 1,y + 1 ,false,false,false,true,'r' });
						currentPseudo.push_back({ x,y,x + 1,y + 1 ,false,false,false,true,'q' });
						currentPseudo.push_back({ x,y,x + 1,y + 1 ,false,false,false,true,'b' });
						currentPseudo.push_back({ x,y,x + 1,y + 1 ,false,false,false,true,'n' });
					}
					else {
						currentPseudo.push_back({ x,y,x + 1,y + 1 ,false,false,false,false,' ' });
					}
				}
				else if (x + 1 == enPassantX && y + 1 == enPassantY) {
					currentPseudo.push_back({ x,y,x + 1,y + 1,false,false,true });
				}
			}
		}
	}
}


//Rook code
Rook::Rook(bool colorIsWhite)
	: Piece(colorIsWhite)
{
	pieceType = PieceType::ROOK;
	pieceTexture = (white ? whiteRookTexture : blackRookTexture);
}


void Rook::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{
	//going to the right on the board.
	for (int currX = x + 1; currX < boardXBoxes; ++currX) {
		if (board[currX][y] == nullptr) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
		}
		else if (pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
			break;
		}
	}
	//going to the left on the board.
	for (int currX = x - 1; currX >= 0; --currX) {
		if (board[currX][y] == nullptr) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
		}
		else if (pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
			break;

		}
	}
	//going up on the board
	for (int currY = y - 1; currY >= 0; --currY) {
		if (board[x][currY] == nullptr) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });
			break;

		}
	}
	//going down the board
	for (int currY = y + 1; currY < boardYBoxes; ++currY) {
		if (board[x][currY] == nullptr) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });
			break;

		}
	}
}


//Knight code
Knight::Knight(bool colorIsWhite)
	: Piece(colorIsWhite)
{
	pieceType = PieceType::KNIGHT;
	pieceTexture = (white ? whiteKnightTexture : blackKnightTexture);
}
#include <iostream>

void Knight::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{
	//knight moving up.
	int knightXChange[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
	int knightYChange[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	for (int i = 0; i < 8; ++i) {
		int newX = x + knightXChange[i];
		int newY = y + knightYChange[i];

		if (newX >= 0 && newY >= 0 && newX < boardXBoxes && newY < boardYBoxes) {
			if (board[newX][newY] == nullptr || !pieceIsCurrentPlayerPiece(newX, newY, board, currentTurn)) {
				currentPseudo.push_back({ x, y, newX, newY, false, false, false });
			}
		}
	}
}


//Bishop code
Bishop::Bishop(bool colorIsWhite)
	: Piece(colorIsWhite)
{
	pieceType = PieceType::BISHOP;
	pieceTexture = (white ? whiteBishopTexture : blackBishopTexture);
}


void Bishop::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{
	//going down and to the right
	for (int change = 1; x + change < boardXBoxes && y + change < boardYBoxes; ++change) {
		if (board[x + change][y + change] == nullptr) {
			currentPseudo.push_back({ x,y,x + change,y + change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x + change, y + change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x + change, y + change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x + change,y + change, false, false, false });
			break;
		}
	}
	//going down and to the left
	for (int change = 1; x - change >= 0 && y + change < boardYBoxes; ++change) {
		if (board[x - change][y + change] == nullptr) {
			currentPseudo.push_back({ x,y,x - change,y + change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x - change, y + change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x - change, y + change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x - change,y + change, false, false, false });
			break;
		}

	}
	//going up and to the left.
	for (int change = 1; x - change >= 0 && y - change >= 0; ++change) {
		if (board[x - change][y - change] == nullptr) {
			currentPseudo.push_back({ x,y,x - change,y - change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x - change, y - change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x - change, y - change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x - change,y - change, false, false, false });
			break;
		}
	}

	//going up and to the right
	for (int change = 1; x + change < boardXBoxes && y - change >= 0; ++change) {
		if (board[x + change][y - change] == nullptr) {
			currentPseudo.push_back({ x,y,x + change,y - change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x + change, y - change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x + change, y - change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x + change,y - change, false, false, false });
			break;
		}
	}
}


//Queen code
Queen::Queen(bool colorIsWhite)
	: Piece(colorIsWhite)
{
	pieceType = PieceType::QUEEN;
	pieceTexture = (white ? whiteQueenTexture : blackQueenTexture);
}


void Queen::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{
	//Bishop movement part
	//going down and to the right
	for (int change = 1; x + change < boardXBoxes && y + change < boardYBoxes; ++change) {
		if (board[x + change][y + change] == nullptr) {
			currentPseudo.push_back({ x,y,x + change,y + change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x + change, y + change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x + change, y + change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x + change,y + change, false, false, false });
			break;
		}
	}
	//going down and to the left
	for (int change = 1; x - change >= 0 && y + change < boardYBoxes; ++change) {
		if (board[x - change][y + change] == nullptr) {
			currentPseudo.push_back({ x,y,x - change,y + change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x - change, y + change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x - change, y + change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x - change,y + change, false, false, false });
			break;
		}

	}
	//going up and to the left.
	for (int change = 1; x - change >= 0 && y - change >= 0; ++change) {
		if (board[x - change][y - change] == nullptr) {
			currentPseudo.push_back({ x,y,x - change,y - change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x - change, y - change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x - change, y - change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x - change,y - change, false, false, false });
			break;
		}
	}

	//going up and to the right
	for (int change = 1; x + change < boardXBoxes && y - change >= 0; ++change) {
		if (board[x + change][y - change] == nullptr) {
			currentPseudo.push_back({ x,y,x + change,y - change, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x + change, y - change, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x + change, y - change, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x + change,y - change, false, false, false });
			break;
		}
	}


	//Rook movement part
	//going to the right on the board.
	for (int currX = x + 1; currX < boardXBoxes; ++currX) {
		if (board[currX][y] == nullptr) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
		}
		else if (pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
			break;
		}
	}
	//going to the left on the board.
	for (int currX = x - 1; currX >= 0; --currX) {
		if (board[currX][y] == nullptr) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(currX, y, board, currentTurn)) {
			currentPseudo.push_back({ x,y,currX,y, false, false, false });
			break;

		}
	}
	//going up on the board
	for (int currY = y - 1; currY >= 0; --currY) {
		if (board[x][currY] == nullptr) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });

		}
		else if (pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });
			break;
		}
	}
	//going down the board
	for (int currY = y + 1; currY < boardYBoxes; ++currY) {
		if (board[x][currY] == nullptr) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });
		}
		else if (pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			break;
		}
		else if (!pieceIsCurrentPlayerPiece(x, currY, board, currentTurn)) {
			currentPseudo.push_back({ x,y,x,currY, false, false, false });
			break;
		}
	}
}


//King code
King::King(bool colorIsWhite)
	: Piece(colorIsWhite)
{
	pieceType = PieceType::KING;
	pieceTexture = (white ? whiteKingTexture : blackKingTexture);
}


void King::getPseudoLegalMoves(std::vector<std::vector<Piece*>> board, int x, int y,
	int boardXBoxes, int boardYBoxes, char currentTurn,
	int enPassantX, int enPassantY, bool whiteCanKingsideCastle,
	bool whiteCanQueensideCastle, bool blackCanKingsideCastle,
	bool blackCanQueensideCastle, std::vector<Move>& currentPseudo)
{
	int kingXChange[8] = { 1, 1,  1,  0, -1, -1, -1,  0 };
	int kingYChange[8] = { 1, 0, -1, -1, -1,  0,  1,  1 };

	for (int i = 0; i < 8; ++i) {
		int newX = x + kingXChange[i];
		int newY = y + kingYChange[i];

		if (newX >= 0 && newY >= 0 && newX < boardXBoxes && newY < boardYBoxes) {
			if (board[newX][newY] == nullptr || !pieceIsCurrentPlayerPiece(newX, newY, board, currentTurn)) {
				currentPseudo.push_back({ x, y, newX, newY, false, false, false });
			}
		}
	}

	

	if (board[x][y]->isWhite()) {
		if (whiteCanKingsideCastle) {
			if (board[x + 1][y] == nullptr && board[x + 2][y] == nullptr) {
				currentPseudo.push_back({ x,y,x + 2,y,true,false,false });
			}
		}
		if (whiteCanQueensideCastle) {
			if (board[x - 1][y] == nullptr && board[x - 2][y] == nullptr) {
				currentPseudo.push_back({ x,y,x - 2,y,false,true,false });
			}
		}
	}
	else {
		if (blackCanKingsideCastle) {
			if (board[x + 1][y] == nullptr && board[x + 2][y] == nullptr) {
				currentPseudo.push_back({ x,y,x + 2,y,true,false,false });
			}
		}
		if (blackCanQueensideCastle) {
			if (board[x - 1][y] == nullptr && board[x - 2][y] == nullptr) {
				currentPseudo.push_back({ x,y,x - 2,y,false,true,false });
			}
		}
	}
}