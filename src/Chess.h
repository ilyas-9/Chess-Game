//
// Created by x11y45 on 6/26/25.
//
#pragma once

#ifndef CHESS_H
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <vector>
#define CHESS_H

namespace Chess {

class Chess {
private:
	// Private members for the Chess class can be defined here
	sf::RenderWindow *window = nullptr; // Pointer to the SFML window
	sf:: View *view = nullptr;
	sf::Event event;
	// Private members for the Chess class
	sf::Texture BoardTexture; // Texture for the chessboard
	sf::Sprite BoardSprite; // Sprite for the chessboard
	sf::Texture WPieceTexture[6]; // Array for white pieces textures
	sf::Texture BPieceTexture[6]; // Array for black pieces textures
	sf::Sprite WPieceSprite[6];
	sf::Sprite BPieceSprite[6];

	// sound effects for moves, captures, checks, castling, illegal moves, and promotion
	sf::SoundBuffer MoveBuffer;
	sf::Sound MoveSound;
	sf::SoundBuffer CaptureBuffer;
	sf::Sound CaptureSound;
	sf::SoundBuffer CheckBuffer;
	sf::Sound CheckSound;
	sf::SoundBuffer CastlingBuffer;
	sf::Sound CastlingSound;
	sf::SoundBuffer illegalBuffer;
	sf::Sound illegalSound;
	sf::SoundBuffer PromotionBuffer;
	sf::Sound PromotionSound;

	std::string PiecePosition; // FEN string for initial piece positions
	// a struct to represent a chess piece
	struct Piece {
		int color;
		int type; // 1 for rook, 2 for knight, 3 for bishop, 4 for queen, 5 for king, 6 for pawn
		// position on the board
		int x;
		int y;
		int temp[5]; // temporary variables , temp[0] is for if the piece has moved or not, temp[1] is for en passant, the rest are unused.
		int CovSquare[8][8];// 2D array to store covered squares, it holds the move of the piece.
		// 0 = not covered
		// 1 = can move
		// 2 = can capture
		// 3 = castling , only for king
		// 4 = en passant , only for pawn
		// 5 = check , only for pieces attacking the king
		// 8 = capture with promotion , only for pawn
		// 9 = move with promotion , only for pawn
	};

	// a struct to represent a move , so you can save the move and undo it later
	struct Move {
		// the piece in the starting square
		Piece starPiece;
		int startX;
		int startY;
		// the piece in the destination square
		Piece destPiece;
		int destX;
		int destY;
		int turn; // 1 for white, 0 for black
		// Add any additional information needed for the move
		bool isCheck[2];
		bool isCheckMate;
		bool isCapture; // true if the move is a capture
		bool isCastling; // true if the move is castling
		bool isEnPassant;
	};

	Piece WPiece[6];
	Piece BPiece[6];
	std::map<char, int> PieceMap;
	Piece *board[8][8];
	Piece *selPiece = nullptr;
	int turn = 1; // 1 for white, 0 for black
	bool isCheck[2] = {false, false}; // Check status for both players
	bool isCheckMate = false;
	bool islegalProcessing = false; // this boolean variable is to avoid to call the promotion function during the legal move processing
	std::vector<Move> Moves; // Vector to store moves made in the game


public:
	Chess();
	~Chess();

// Public methods for the Chess class
	void ProcessEvents();
	void Render();
	// Check if the window is open
	bool isOpen() const;

private:
	// Private methods for initialization
	void initWindows();
	void initVariables();
	void initTextures();
	void initSprites();
	void initAudio();
	int promoWindow();

	void initBoard();
	void CoverSquare(Piece *piece);
	void ClrCovSquare(Piece *piece);
	void UpCovSquare();

	void setPosition();

	void MakeMove(Piece*, int, int);
	void UndoMove();
	void islegal(Piece*);
	void ischeck(int);
	bool checkPosition(int);
	void saveMove(Piece *, int, int);
	void Gameover();
};

} // Chess

#endif //CHESS_H
