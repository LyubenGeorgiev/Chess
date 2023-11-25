#pragma once


/**
 * \brief Public struct with everything about a move
 * \author L. Georgiev
 */
struct Move {
	int fromX;
	int fromY;
	int toX;
	int toY;
	bool kingSideCastle;
	bool queenSideCastle;
	bool enPassant;
	bool isPromotion;
	char promotionType;
};