//
// Created by x11y45 on 6/26/25.
//

#include "Chess.h"

namespace Chess {
	///////////////////////////////////////////////////////////////
	// Constructors and Destructors
	// Initializes the Chess game and its window.
	// Cleans up resources when the game is destroyed.
//////////////////////////////////////////////////////////////////
	Chess::Chess() : window(nullptr) {
		initVariables();
		initTextures();
		initSprites();
		initAudio();
		initWindows();
		initBoard();
		setPosition();
		UpCovSquare();

	}
	Chess::~Chess() {
								// Select a piece if none is selected
		delete this->window;
		std::cout << "Chess game destroyed." << std::endl;
	}




//////////////////////////////////////////////////////////////////
	// Public methods
	void Chess::ProcessEvents() {
		sf::Event event;
			while (window->pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window->close();
				}else if (event.key.code == sf::Keyboard::Space) {
					window->close();
				}else if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						// Handle left mouse button click
						const sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
						const int X = (mousePos.x - 100) / 100; // Calculate column
						const int Y = (mousePos.y - 100) / 100; // Calculate row
						if (X >= 0 && X < 8 && Y >= 0 && Y < 8) {
							// Add logic to handle piece selection or movement here

							if (selPiece == nullptr) {
								if (board[Y][X] != nullptr && board[Y][X]->color != turn) {
									// do nothing if the piece is of the opposite color
								}else if (board[Y][X] != nullptr && board[Y][X]->color == turn) {
									// Select a piece if none is selected
									selPiece = board[Y][X];
									UpCovSquare();
									islegal(selPiece);
									selPiece = board[Y][X];
								}
							}else {
								if (board[Y][X] != nullptr && selPiece->color == board[Y][X]->color) {
									// If the selected piece is of the same color, deselect it
									selPiece = board[Y][X];
									UpCovSquare();
									islegal(selPiece);
									selPiece = board[Y][X];
								}else {
									if (selPiece->CovSquare[Y][X] != 0) {

										MakeMove(selPiece, X, Y);
										turn = !turn; // Switch turns
										isCheck[0] = false; // Reset check status for white
										isCheck[1] = false; // Reset check status for black
										ischeck(turn); // Check if the current player is in check
										if (isCheck[turn]) {
											CheckSound.play();
										}else if (selPiece->CovSquare[Y][X] == 2 || selPiece->CovSquare[Y][X] == 4) {
											CaptureSound.play();
										}else if (selPiece->CovSquare[Y][X] == 3) {
											CastlingSound.play();
										}else if (selPiece->CovSquare[Y][X] == 1) {
											MoveSound.play();
										}else if (selPiece->CovSquare[Y][X] == 9 or selPiece->CovSquare[Y][X] == 8) {
											PromotionSound.play();
										}
										UpCovSquare();

										if (checkPosition(turn)) {
											Gameover();
										}
									}else {
										illegalSound.play();
									}
									selPiece = nullptr; // deselect the piece after moving it
								}

							}
						}
					}
				}else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
					// Handle left arrow key for undoing the last move
					UndoMove();
					UpCovSquare();
				}
			}
	}




	void Chess::update() {
		// Update game logic here
		// Add any other game updates here
	}




	void Chess::Render() {
		this->window->clear(sf::Color::Black);
		// Render game elements here
		this->window->draw(BoardSprite);
		if (selPiece != nullptr) {
			// Draw the selected piece with a highlight
			sf::RectangleShape highlight(sf::Vector2f(100.f, 100.f));
			highlight.setPosition(100.f + selPiece->x * 100.f, 100.f + selPiece->y * 100.f);
			highlight.setFillColor(sf::Color(255, 255, 0, 128)); // Semi-transparent yellow
			this->window->draw(highlight);
			// Draw the covered square
			for (int ny = 0; ny < 8; ny++) {
				for (int nx = 0; nx < 8; nx++) {
					if (selPiece->CovSquare[ny][nx] != 0) {
						if (selPiece->CovSquare[ny][nx] == 2 || selPiece->CovSquare[ny][nx] == 8) {
							highlight.setFillColor(sf::Color(255,25, 0, 128));
							highlight.setPosition(100.f + nx * 100.f, 100.f + ny * 100.f);
							this->window->draw(highlight);
						}else {
							sf::CircleShape highlight1(25.f);
							highlight1.setFillColor(sf::Color(10, 10, 10, 100));
							highlight1.setPosition(125.f + nx * 100.f, 125.f + ny * 100.f);
							this->window->draw(highlight1);
						}
					}
				}
			}
		}
		// Draw white pieces
		for ( int i = 0; i < 8; i++) {
			for ( int j = 0; j < 8; j++) {
				if (board[i][j] == nullptr) {

					continue; // Skip empty squares
				}else {
					int type = board[i][j]->type;
					int color = board[i][j]->color;
					if (color == 1) { // White piece
						WPieceSprite[type - 1].setPosition(100.f + j * 100.f, 100.f + i * 100.f);
						this->window->draw(WPieceSprite[type - 1]);
					} else if (color == 0) { // Black piece
						BPieceSprite[type - 1].setPosition(100.f + j * 100.f, 100.f + i * 100.f);
						this->window->draw(BPieceSprite[type - 1]);
					}

				}
			}
		}
		// Display the contents of the window
		this->window->display();
	}
	///////////////////////////////////////////////////////////////
	// Check if the window is open
	bool Chess::isOpen() const {
		return this->window->isOpen();
	}





//////////////////////////////////////////////////////////////////
// Private methods
	void Chess::initAudio() {
		// initialize audio components here if needed
		if (!MoveBuffer.loadFromFile("./chess-items/Sounds/Move.wav")) {
			std::cerr << "Failed to load move sound." << std::endl;
		}if (!CaptureBuffer.loadFromFile("./chess-items/Sounds/capture.wav")) {
			std::cerr << "Failed to load capture sound." << std::endl;
		}if (!CheckBuffer.loadFromFile("./chess-items/Sounds/Check.wav")) {
			std::cerr << "Failed to load check sound." << std::endl;
		}if (!CastlingBuffer.loadFromFile("./chess-items/Sounds/castle.wav")) {
			std::cerr << "Failed to load castling sound." << std::endl;
		}if (!illegalBuffer.loadFromFile("./chess-items/Sounds/illegal.wav")) {
			std::cerr << "Failed to load illegal move sound." << std::endl;
		}if (!PromotionBuffer.loadFromFile("./chess-items/Sounds/promote.wav")) {
			std::cerr << "Failed to load promotion sound." << std::endl;
		}
		PromotionSound.setBuffer(PromotionBuffer);
		illegalSound.setBuffer(illegalBuffer);
		MoveSound.setBuffer(MoveBuffer);
		CaptureSound.setBuffer(CaptureBuffer);
		CheckSound.setBuffer(CheckBuffer);
		CastlingSound.setBuffer(CastlingBuffer);
	}
	// Initializes the game window with a specific size and title.
	void Chess::initWindows() {
		this->window = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Chess Game",sf::Style::Titlebar | sf::Style::Close);
		std::cout << "Chess game initialized." << std::endl;
		this->window->setFramerateLimit(60);
		// Set window position to center on the screen (optional, or set to (0,0) if you want top-left)
		// Example: this->window->setPosition(sf::Vector2i(100, 100));
		std::cout << "Window created." << std::endl;
		this->view = new sf::View(sf::FloatRect(0, 0, 1000, 1000));
		std::cout << "View created." << std::endl;
		this->window->setView(*this->view);
	}

	void Chess::initVariables() {
		PiecePosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w"; // Initial position in FEN format
		for (int i = 0; i < 6; i++) {
			WPiece[i].color = 1;
			WPiece[i].type = i + 1;
			BPiece[i].color = 0;
			BPiece[i].type = i + 1;
			WPiece[i].x = 0;
			WPiece[i].y = 0;
			BPiece[i].x = 0;
			BPiece[i].y = 0;
			for (int j = 0; j < 8; j++) {
				for (int k = 0; k < 8; k++) {
					WPiece[i].CovSquare[j][k] = 0;
					BPiece[i].CovSquare[j][k] = 0;
				}
			}
		}
		PieceMap = {
			{'r', 1}, {'n', 2}, {'b', 3}, {'q', 4}, {'k', 5}, {'p', 6},
			{'R', 1}, {'N', 2}, {'B', 3}, {'Q', 4}, {'K', 5}, {'P', 6}
		};
	}

	void Chess::initTextures() {
		if (!BoardTexture.loadFromFile("./chess-items/Board/board.png")) {
			std::cerr << "Failed to load board texture." << std::endl;
		}
		BoardTexture.setSmooth(true);
		BoardTexture.setRepeated(false);
		// Load piece textures
		for (int i = 0; i < 6; ++i) {
			std::string whitePiecePath = "./chess-items/WhitePieces/" + std::to_string((i+1)) + ".png";
			std::string blackPiecePath = "./chess-items/BlackPieces/" + std::to_string((i+1)) + ".png";
			if (!WPieceTexture[i].loadFromFile(whitePiecePath)) {
				std::cerr << "Failed to load white piece texture: " << whitePiecePath << std::endl;
			}
			if (!BPieceTexture[i].loadFromFile(blackPiecePath)) {
				std::cerr << "Failed to load black piece texture: " << blackPiecePath << std::endl;
			}
		}


	}

	void Chess::initSprites() {
		BoardSprite.setTexture(BoardTexture);
		// Center the sprite in the window
		BoardSprite.setPosition(100.f, 100.f);
		// Initialize piece sprites
		for (int i = 0; i < 6; ++i) {
			WPieceSprite[i].setTexture(WPieceTexture[i]);
			BPieceSprite[i].setTexture(BPieceTexture[i]);
		}
	}
	void Chess::initBoard() {
		// Initialize the chess board with pieces
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				board[i][j] = nullptr; // Initialize all squares to nullptr
			}
		}
	}
	void Chess::setPosition() {

		int j = 0;
		for (char i : PiecePosition) {
			if (i == '/') {
				continue; // Skip the '/' character
			}else if (i >= '0' && i <= '9') {
				j += i - '0'; // Skip empty squares
				continue;
			}else if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z')) {
				int pieceType = PieceMap[i];
				if (i >= 'a' and i <= 'z') { // Black pieces
					board[j/8][j%8] = new Piece(0,pieceType,j%8,j/8,{0,0,0,0}); // Create a new piece for the board
				} else { // White pieces
					board[j/8][j%8] = new Piece(1,pieceType,j%8,j/8,{0,0,0,0}); // Create a new piece for the board
				}
				j++;
			}else if (i == ' ') {
				i++;
				if (i == 'w') {
					turn = 1; // White to move
				} else if (i == 'b') {
					turn = 0; // Black to move
				}
				break; // End of position string
			}
		}


		// printing the board for debugging
		std::cout << "Chess board"<< std::endl;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == nullptr) {
					std::cout << "[*]";
				}else {
					std::cout<< "[" << board[i][j]->type << "]";
				}
			}
			std::cout << std::endl;
		}
	}
	void Chess::saveMove(Piece *piece , int x, int y) {
		Move move;
		move.starPiece = *piece; // Copy the piece being moved
		move.startX = piece->x; // Store the starting position
		move.startY = piece->y; // Store the starting position
		move.destX = x; // Store the destination position
		move.destY = y; // Store the destination position
		move.destPiece = (board[y][x] != nullptr) ? *board[y][x] : Piece{0, 0, 0, 0, {0, 0, 0, 0}}; // Copy the piece at the destination if it exists
		move.turn = turn; // Store the current turn
		move.isCheck[1] = isCheck[1],move.isCheck[0] = isCheck[0];// Store check status
		move.isCheckMate = isCheckMate; // Store checkmate status
		move.isCapture = (board[y][x] != nullptr); // Check if the move is a capture
		move.isCastling = (piece->type == 5 && piece->CovSquare[y][x] == 3); // Check if the move is castling
		move.isEnPassant = (piece->type == 6 && piece->CovSquare[y][x] == 4); // Check if the move is en passant
		Moves.push_back(move); // Add the move to the history
	}

	void Chess::MakeMove(Piece *piece, int x, int y) {
		saveMove(piece, x, y);
		if ((piece->CovSquare[y][x] == 2 || piece->CovSquare[y][x] == 8) && board[y][x] != nullptr) {
				// If the square is attackable and contains a piece, remove that piece
				delete board[y][x]; // Delete the captured piece
				board[y][x] = nullptr;
		}if (piece->type == 5 && piece->CovSquare[y][x] == 3) {
				// Castling logic
				if (x == 2) { // Castling left
					if (board[y][0] != nullptr && board[y][0]->type == 1 && board[y][0]->temp[0] == 0) { // Rook on the left
						board[y][3] = new Piece(*board[y][0]); // Move the rook to the right
						delete board[y][0];
						board[y][0] = nullptr;
						board[y][3]->x = 3; // Update rook's x position
						board[y][3]->y = y; // Update rook's y position
					}
				}else if (x == 6) { // Castling right
					if (board[y][7] != nullptr && board[y][7]->type == 1 && board[y][7]->temp[0] == 0) { // Rook on the right
						board[y][5] = new Piece(*board[y][7]); // Move the rook to the left
						delete board[y][7];
						board[y][7] = nullptr;
						board[y][5]->x = 5;
						board[y][5]->y = y; // Update rook's y position
					}
				}
		}if (piece->type == 6 && piece->CovSquare[y][x] == 4) {
				//en passant logic
				if (piece->color == 1) {
					delete board[y+1][x];
					board[y+1][x] = nullptr;
				}else {
					delete board[y-1][x];
					board[y-1][x] = nullptr;
				}
		}if (piece->CovSquare[y][x] == 8 or piece->CovSquare[y][x] == 9) {
			// Pawn promotion logic
			int promoType = 0;
			if (islegalProcessing) {
				promoType = 4;
			}else {
				promoType = promoWindow();
				if (promoType != 0) {
					piece;
					board[y][x] = new Piece(turn, promoType , x, y, {1, 0, 0, 0}); // Promote to the selected piece
					board[piece->y][piece->x] = nullptr; // Clear the old square

				}else {
					UndoMove();
					turn = !turn; // Switch turns back
				}
				return;
			}
		}
		isCheck[0] = false; // Reset check status for white
		isCheck[1] = false; // Reset check status for black
		isCheckMate = false; // Reset checkmate status
		board[y][x] = new Piece(*piece);
		delete piece;
		board[board[y][x]->y][board[y][x]->x] = nullptr; // Clear the old square
		board[y][x]->x = x; // Update piece's x position
		board[y][x]->y = y; // Update piece's y position
		board[y][x]->temp[0] = 1; // Mark as moved
	}
	void Chess::UndoMove() {
		if (Moves.empty()) {
			std::cout << "No moves to undo." << std::endl;
			return; // No moves to undo
		}
		Move lastMove = Moves.back(); // Get the last move
		Moves.pop_back(); // Remove the last move from history

		// Restore the piece's position
		board[lastMove.startY][lastMove.startX] = new Piece(lastMove.starPiece);
		board[lastMove.startY][lastMove.startX]->x = lastMove.startX;
		board[lastMove.startY][lastMove.startX]->y = lastMove.startY;
		board[lastMove.startY][lastMove.startX]->temp[0] = 0; // Mark as not moved

		if (board[lastMove.destY][lastMove.destX] != nullptr) {
			delete board[lastMove.destY][lastMove.destX]; // Delete the captured piece
			board[lastMove.destY][lastMove.destX] = nullptr;
		}
		if (lastMove.isCapture) {
			// Restore the captured piece if it exists
			board[lastMove.destY][lastMove.destX] = new Piece(lastMove.destPiece);
			board[lastMove.destY][lastMove.destX]->x = lastMove.destX;
			board[lastMove.destY][lastMove.destX]->y = lastMove.destY;
			board[lastMove.destY][lastMove.destX]->temp[0] = 0; // Mark as not moved
		}else if (lastMove.isCastling) {
			// Restore the rook if it was castling
			if (lastMove.destX == 2) { // Castling left
				board[lastMove.startY][0] = new Piece(lastMove.turn, 1, 0, lastMove.startY, {0, 0, 0, 0}); // Rook on the left
				board[lastMove.startY][0]->temp[0] = 0; // Mark as not moved
				board[lastMove.startY][3] = nullptr; // Clear the rook's new position
			}else if (lastMove.destX == 6) { // Castling right
				board[lastMove.startY][7] = new Piece(lastMove.turn, 1, 7, lastMove.startY, {0, 0, 0, 0}); // Rook on the right
				board[lastMove.startY][7]->temp[0] = 0; // Mark as not moved
				board[lastMove.startY][5] = nullptr; // Clear the rook's new position
			}
		}else if (lastMove.isEnPassant) {
			// Restore the pawn if it was en passant
			if (lastMove.turn == 1) {
				board[lastMove.destY + 1][lastMove.destX] = new Piece(0, 6, lastMove.destX, lastMove.destY + 1, {1, 0, 0, 0}); // Black pawn
			} else {
				board[lastMove.destY - 1][lastMove.destX] = new Piece(1, 6, lastMove.destX, lastMove.destY - 1, {1, 0, 0, 0}); // White pawn
			}
		}

		selPiece = nullptr; // Deselect the piece after undoing
		turn = lastMove.turn; // Restore the previous turn
		isCheck[0] = lastMove.isCheck[0]; // Restore check status for white
		isCheck[1] = lastMove.isCheck[1]; // Restore check status for black
		isCheckMate = lastMove.isCheckMate; // Restore checkmate status
	}

	void Chess::islegal(Piece* piece) {
		islegalProcessing = true;
	    if (!piece) return;
	    int color = piece->color;
	    int originalX = piece->x;
	    int originalY = piece->y;
	    int orgCovSquare[8][8];

	    // Copy the original CovSquare to orgCovSquare
	    for (int i = 0; i < 8; i++)
	        for (int j = 0; j < 8; j++)
	            orgCovSquare[i][j] = piece->CovSquare[i][j];

	    for (int Y = 0; Y < 8; Y++) {
	        for (int X = 0; X < 8; X++) {
	            if (orgCovSquare[Y][X] == 0) continue;
	            Piece* current = board[originalY][originalX];
	            if (!current) continue;
	            MakeMove(current, X, Y);
	            isCheck[0] = false;
	            isCheck[1] = false;
	            ischeck(color);
	            bool illegal = isCheck[color];
	            UndoMove();
	            // After UndoMove, get the pointer again
	            current = board[originalY][originalX];
	            if (illegal && current)
	                orgCovSquare[Y][X] = 0;
	        }
	    }
	    // Copy the filtered moves back
	    Piece* current = board[originalY][originalX];
	    if (current) {
	        for (int i = 0; i < 8; i++)
	            for (int j = 0; j < 8; j++)
	                current->CovSquare[i][j] = orgCovSquare[i][j];
	    }
		islegalProcessing = false;
	}

	void Chess::ischeck(int color) {
		// Check if the king of the specified color is in check
		int derections[8][2] = {
			{-1, 0}, {1, 0}, {0, -1}, {0, 1}, // Vertical and horizontal
			{-1, -1}, {-1, 1}, {1, -1}, {1, 1} // Diagonal
		};
		int kingX = -1, kingY = -1;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] != nullptr && board[i][j]->type == 5 && board[i][j]->color == color) { // Find the king
					kingX = j;
					kingY = i;
					break;
				}
			}
			if (kingX != -1) break; // Break if the king is found
		}
		if (kingX == -1 || kingY == -1) {
			std::cout << "King not found for color " << color << std::endl;
			return; // King not found
		}
		for (int d = 0; d < 8; d++) {
			const int dx = derections[d][0];
			const int dy = derections[d][1];
			int nx = kingX;
			int ny = kingY;
			while (true) {
				nx += dx;
				ny += dy;
				if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break; // Out of bounds
				if (board[ny][nx] != nullptr) {
					if (board[ny][nx]->color != color) {
						if (d < 4) { // Vertical or horizontal
							if (board[ny][nx]->type == 1 || board[ny][nx]->type == 4) { // Rook or Queen
								isCheck[color] = true; // Set check status
								return;
							}
						} else { // Diagonal
							if (board[ny][nx]->type == 3 || board[ny][nx]->type == 4) { // Bishop or Queen
								isCheck[color] = true; // Set check status
								return;
							}
						}
					}
					break; // Stop if a piece is encountered
				}
			}
		}
		// Check for knight attacks
		int knightMoves[8][2] = {
			{-2, -1}, {-1, -2}, {1, -2}, {2, -1},
			{2, 1}, {1, 2}, {-1, 2}, {-2, 1}
		};
		for (int d = 0; d < 8; d++) {
			const int nx = kingX + knightMoves[d][0];
			const int ny = kingY + knightMoves[d][1];
			if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
				if (board[ny][nx] != nullptr && board[ny][nx]->color != color && board[ny][nx]->type == 2) { // Knight
					isCheck[color] = true; // Set check status
					return;
				}
			}
		}
		// Check for pawns
		if (color == 1) { // White pawns
			if (kingY - 1 >= 0) {
				if (kingX - 1 >= 0 && board[kingY - 1][kingX - 1] != nullptr && board[kingY - 1][kingX - 1]->color == 0 && board[kingY - 1][kingX - 1]->type == 6) {
					isCheck[color] = true; // Set check status
					return;
				}
				if (kingX + 1 < 8 && board[kingY - 1][kingX + 1] != nullptr && board[kingY - 1][kingX + 1]->color == 0 && board[kingY - 1][kingX + 1]->type == 6) {
					isCheck[color] = true; // Set check status
					return;
				}
			}
		} else { // Black pawns
			if (kingY + 1 < 8) {
				if (kingX - 1 >= 0 && board[kingY + 1][kingX - 1] != nullptr && board[kingY + 1][kingX - 1]->color == 1 && board[kingY + 1][kingX - 1]->type == 6) {
					isCheck[color] = true; // Set check status
					return;
				}
				if (kingX + 1 < 8 && board[kingY + 1][kingX + 1] != nullptr && board[kingY + 1][kingX + 1]->color == 1 && board[kingY + 1][kingX + 1]->type == 6) {
					isCheck[color] = true; // Set check status
					return;
				}
			}
		}
	}

	bool Chess::checkPosition(int color) {
		UpCovSquare();
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == nullptr || board[i][j]->color != color) continue;
				islegal(board[i][j]);
				for (int d = 0; d < 8; d++) {
					for (int e = 0; e < 8; e++) {
						if (board[i][j]->CovSquare[d][e] != 0) {
							return false; // Found a legal move, not checkmate
						}
					}
				}
			}
		}
		// No legal moves found
		return true; // No legal moves found, it's checkmate or stalemate
	}

	void Chess::CoverSquare(Piece *piece) {
	// logic for sliding pieces
		ClrCovSquare(piece);
		int directions[8][2] = {
			{-1, 0}, {1, 0}, {0, -1}, {0, 1}, // Vertical and horizontal
			{-1, -1}, {-1, 1}, {1, -1}, {1, 1} // Diagonal
		};
		if (piece->type == 1 || piece->type == 3 || piece->type == 4) {
			int d,f;
			if (piece->type == 1) { // Rook
				d = 0; f = 4; // Only vertical and horizontal
			} else if (piece->type == 3) { // Bishop
				d = 4; f = 8; // Only diagonal moves
			} else{ // Queen
				d = 0; f = 8; // All directions
			}
			for (; d < f; d++) {
				const int dx = directions[d][0];
				const int dy = directions[d][1];
				int nx = piece->x;
				int ny = piece->y;
				while (true) {
					nx += dx;
					ny += dy;
					if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break; // Out of bounds
					if (board[ny][nx] != nullptr) {
						if (board[ny][nx]->color != piece->color) {
							if (board[ny][nx]->type == 5) {
								// the piece is attacking the king, soo its check
								piece->CovSquare[ny][nx] = 5; // Mark as attackable
								isCheck[board[ny][nx]->color ] = true; // Set check status
							}else {
								piece->CovSquare[ny][nx] = 2; // Mark as attackable
							}
						}
						break; // Stop if a piece is encountered
					}
					piece->CovSquare[ny][nx] = 1; // Mark the square as covered
				}
			}
		}else if (piece->type == 5) {						// KING
			// king move one square in any direction
			for (int d = 0; d < 8; d++) {
				const int nx = piece->x + directions[d][0];
				const int ny = piece->y + directions[d][1];
				if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
					if (board[ny][nx] == nullptr) {
						piece->CovSquare[ny][nx] = 1;
					}else if (board[ny][nx]->color != piece->color) {
						piece->CovSquare[ny][nx] = 2; // Mark as attackable
					}
				}
			}
			// Castling logic
			if (piece->temp[0] == 0) { // If the king has not moved
				// Check for castling on the left side
				if (board[piece->y][0] != nullptr && board[piece->y][0]->type == 1 && board[piece->y][0]->temp[0] == 0 && board[piece->y][1] == nullptr && board[piece->y][2] == nullptr && board[piece->y][3] == nullptr) {
					piece->CovSquare[piece->y][2] = 3; // Castling left
				}
				// Check for castling on the right side
				if (board[piece->y][7] != nullptr && board[piece->y][7]->type == 1 && board[piece->y][7]->temp[0] == 0 && board[piece->y][6] == nullptr && board[piece->y][5] == nullptr) {
					piece->CovSquare[piece->y][6] = 3; // Castling right
				}
			}
		}else if (piece->type == 6) {
			// Pawn move
			if (piece->color == 1) {
				// White pawn moves
				if (piece->y-1 >= 0 && piece->x+1 < 8 && board[piece->y - 1][piece->x + 1] != nullptr && board[piece->y - 1][piece->x + 1]->color == 0) {
					if (board[piece->y - 1][piece->x + 1]->type == 5) {
						// the piece is attacking the king, soo its check
						piece->CovSquare[piece->y - 1][piece->x + 1] = 5; // Mark as attackable
					}else {
						piece->CovSquare[piece->y - 1][piece->x + 1] = 2; // Capture right
					}
				}if (piece->y-1 >= 0 && piece->x-1 >= 0 && board[piece->y - 1][piece->x - 1] != nullptr && board[piece->y - 1][piece->x - 1]->color == 0) {
					if (board[piece->y - 1][piece->x - 1]->type == 5) {
						// the piece is attacking the king, soo its check
						piece->CovSquare[piece->y - 1][piece->x - 1] = 5; // Mark as attackable
						isCheck[1] = true; // Set check status for white
					}else {
						piece->CovSquare[piece->y - 1][piece->x - 1] = 2; // Capture left
					}
				}
				if (piece->y == 6) {
					// Can move two squares from the starting position
					if (board[piece->y - 1][piece->x] == nullptr) {
						piece->CovSquare[piece->y - 1][piece->x] = 1; // One square forward
						if (board[piece->y - 2][piece->x] == nullptr) {
							piece->CovSquare[piece->y - 2][piece->x] = 1; // Two squares forward
						}
					}

				} else if (piece->y == 1) {
					// promotion logic
					if (board[piece->y - 1][piece->x] == nullptr) {
						piece->CovSquare[piece->y - 1][piece->x] = 9; // One square forward with promotion
					}if (board[piece->y - 1][piece->x-1] != nullptr and board[piece->y - 1][piece->x-1]->color == 0) {
						piece->CovSquare[piece->y - 1][piece->x-1] = 8; // Capture left with promotion
					}if (board[piece->y - 1][piece->x+1] != nullptr and board[piece->y - 1][piece->x+1]->color == 0) {
						piece->CovSquare[piece->y - 1][piece->x+1] = 8; // Capture right with promotion
					}
				}else {
					if (piece->y - 1 >= 0 && board[piece->y - 1][piece->x] == nullptr) {
						piece->CovSquare[piece->y - 1][piece->x] = 1; // One square forward
					}if (piece->y == 3) {
						// en passant capture
						if (piece->x - 1 >= 0 && board[piece->y][piece->x - 1] != nullptr && board[piece->y][piece->x - 1]->color == 0 && board[piece->y][piece->x - 1]->type == 6) {
							if (Moves.back().starPiece.type == 6 and Moves.back().destX == piece->x - 1 and Moves.back().destY == piece->y and Moves.back().turn == 0 and Moves.back().startY == 1) {
								piece->CovSquare[piece->y - 1][piece->x - 1] = 4; // Capture left
							}
						}if (piece->x + 1 < 8 && board[piece->y][piece->x + 1] != nullptr && board[piece->y][piece->x + 1]->color == 0 && board[piece->y][piece->x + 1]->type == 6) {
							if (Moves.back().starPiece.type == 6 and Moves.back().destX == piece->x + 1 and Moves.back().destY == piece->y and Moves.back().turn == 0 and Moves.back().startY == 1) {
								piece->CovSquare[piece->y - 1][piece->x + 1] = 4; // Capture right
							}
						}
					}
				}
			}else {
				// Black pawn moves
				if (piece->y+1 <= 7 && piece->x+1 < 8 && board[piece->y + 1][piece->x + 1] != nullptr && board[piece->y + 1][piece->x + 1]->color == 1) {
					if (board[piece->y + 1][piece->x + 1]->type == 5) {
						// the piece is attacking the king, soo its check
						piece->CovSquare[piece->y + 1][piece->x + 1] = 5; // Mark as attackable
						isCheck[0] = true; // Set check status for black
					}else {
						piece->CovSquare[piece->y + 1][piece->x + 1] = 2; // Capture right
					}
				}if (piece->y+1 <= 7 && piece->x-1 >= 0 && board[piece->y + 1][piece->x - 1] != nullptr && board[piece->y + 1][piece->x - 1]->color == 1) {
					if (board[piece->y + 1][piece->x - 1]->type == 5) {
						// the piece is attacking the king, soo its check
						piece->CovSquare[piece->y + 1][piece->x - 1] = 5; // Mark as attackable
						isCheck[0] = true; // Set check status for black
					}else {
						piece->CovSquare[piece->y + 1][piece->x - 1] = 2; // Capture left
					}
				}if (piece->y == 1) {
					// Can move two squares from the starting position
					if (board[piece->y + 1][piece->x] == nullptr) {
						piece->CovSquare[piece->y + 1][piece->x] = 1; // One square forward
						if (board[piece->y + 2][piece->x] == nullptr) {
							piece->CovSquare[piece->y + 2][piece->x] = 1; // Two squares forward
						}
					}
				} else if (piece->y == 6) {
					// promotion logic
					if (board[piece->y + 1][piece->x] == nullptr) {
						piece->CovSquare[piece->y + 1][piece->x] = 9; // One square forward with promotion
					}if (board[piece->y + 1][piece->x-1] != nullptr and board[piece->y + 1][piece->x-1]->color == 1) {
						piece->CovSquare[piece->y + 1][piece->x-1] = 8; // Capture left with promotion
					}if (board[piece->y + 1][piece->x+1] != nullptr and board[piece->y + 1][piece->x+1]->color == 1) {
						piece->CovSquare[piece->y + 1][piece->x+1] = 8; // Capture right with promotion
					}
				}else {
					if (piece->y + 1 < 8 && board[piece->y + 1][piece->x] == nullptr) {
						piece->CovSquare[piece->y + 1][piece->x] = 1; // One square forward
					}if (piece->y == 4) {
						// en passant capture
						if (piece->x - 1 >= 0 && board[piece->y][piece->x - 1] != nullptr && board[piece->y][piece->x - 1]->color == 1 && board[piece->y][piece->x - 1]->type == 6) {
							if (Moves.back().starPiece.type == 6 and Moves.back().destX == piece->x - 1 and Moves.back().destY == piece->y and Moves.back().turn == 1 && Moves.back().startY == 6) {
								piece->CovSquare[piece->y + 1][piece->x - 1] = 4; // Capture left
							}
						}if (piece->x + 1 < 8 && board[piece->y][piece->x + 1] != nullptr && board[piece->y][piece->x + 1]->color == 1 && board[piece->y][piece->x + 1]->type == 6) {
							if (Moves.back().starPiece.type == 6 and Moves.back().destX == piece->x + 1 and Moves.back().destY == piece->y and Moves.back().turn == 1 && Moves.back().startY == 6) {
								piece->CovSquare[piece->y + 1][piece->x + 1] = 4; // Capture right
							}
						}
					}
				}
			}
		}else if (piece->type == 2) {
			// Knight move
			int knightMoves[8][2] = {
				{-2, -1}, {-1, -2}, {1, -2}, {2, -1},
				{2, 1}, {1, 2}, {-1, 2}, {-2, 1}
			};
			for (int d = 0; d < 8; d++) {
				int nx = piece->x + knightMoves[d][0];
				int ny = piece->y + knightMoves[d][1];
				if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
					if (board[ny][nx] == nullptr) {
						piece->CovSquare[ny][nx] = 1; // Mark as covered
					} else if (board[ny][nx]->color != piece->color) {
						if (board[ny][nx]->type == 5) {
							// the piece is attacking the king, soo its check
							piece->CovSquare[ny][nx] = 5; // Mark as attackable
							isCheck[board[ny][nx]->color] = true; // Set check status
						}else {
							piece->CovSquare[ny][nx] = 2; // Mark as attackable
						}
					}
				}
			}
		}
	}

	int Chess::promoWindow() {
		float orgX = selPiece->x * 100 + 100 + (this->window->getPosition().x);
		float orgY = selPiece->y * 100 + 100 + (this->window->getPosition().y);
		sf::RenderWindow promoWindow(sf::VideoMode(100, 400), "Promote to");
		promoWindow.setPosition(sf::Vector2i((int)orgX,(int)orgY));
		int promoType = 0;
		while (promoWindow.isOpen()) {
			sf::Event evt;
			while (promoWindow.pollEvent(evt)) {
				if (evt.type == sf::Event::MouseButtonPressed) {
					sf::Vector2 mousePos = sf::Mouse::getPosition(promoWindow);
					int x = (int) mousePos.x;
					int y = (int) mousePos.y;
					// Check if the click is within the bounds of the piece options
					if (x >= 0 && x <= 100 && y >= 0 && y <= 400) {
						int index = y / 100; // Determine which piece was clicked
						if (index >= 0 && index < 4) {
							promoType = index + 1; // Set the promotion type (1-4)
							promoWindow.close();
							return promoType; // Return the selected promotion type
						}
					}
				}else if (evt.type == sf::Event::Closed) {
					promoWindow.close();
					return 0; // Window closed without selection
				}
			}
			promoWindow.clear(sf::Color::White);
			if (turn == 1) {
				// white promotion/home/x11y45/CLionProjects/Chess
				for (int i = 0; i < 4; i++) {
					WPieceSprite[i].setPosition((float)(0),(float)(i * 100));
					promoWindow.draw(WPieceSprite[i]);
				}
			}else{
				// black promotion
				for (int i = 0; i < 4; i++) {
					BPieceSprite[i].setPosition((float)(0),(float)(i * 100));
					promoWindow.draw(BPieceSprite[i]);
				}
			}
			promoWindow.display();
		}
		return promoType;
	}


	void Chess::ClrCovSquare(Piece *piece) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				piece->CovSquare[i][j] = 0;
			}
		}
	}
	void Chess::UpCovSquare() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] != nullptr and (board[i][j]->color == turn)) {
					CoverSquare(board[i][j]); // Update covered squares for each piece
				}
			}
		}
	}

	void Chess::Gameover() {
		// is there a check
		int result;
		int color = turn;
		ischeck(color);
		if (isCheck[color]) {
			result = !color; // there is a check, so its checkmate, so it a win for the opposite color
		}else {
			result = 3; // there is no check, so its a draw
		}
		this->window->close();  // Close the main game window as
			// the current player is in checkmate
		// there is a checkmate, so the game is over
		sf::RenderWindow gameOverWindow(sf::VideoMode(400, 200), "Game Over");

		sf::Texture WhiteTexture; // if white wins
		sf::Texture BlackTexture; // if black wins
		sf::Texture DrawTexture; // if draw
		sf::Sprite drawSprite;
		sf::Sprite WhiteSprite;
		sf::Sprite BlackSprite;
		if (!WhiteTexture.loadFromFile("./chess-items/Game-over-textures/whiteWins.png")) {
			std::cout << "Failed to load whiteWins.png" << std::endl;
		}if (!BlackTexture.loadFromFile("./chess-items/Game-over-textures/blackWins.png")) {
			std::cout << "Failed to load blackWins.png" << std::endl;
		}if (!DrawTexture.loadFromFile("./chess-items/Game-over-textures/draw.png")) {
			std::cout << "Failed to load draw.png" << std::endl;
		}
		drawSprite.setTexture(DrawTexture);
		drawSprite.setPosition(0, 0);
		drawSprite.setScale(400.0f / DrawTexture.getSize().x, 200.0f / DrawTexture.getSize().y);
		WhiteSprite.setTexture(WhiteTexture);
		BlackSprite.setTexture(BlackTexture);
		WhiteSprite.setPosition(0, 0);
		BlackSprite.setPosition(0, 0);
		WhiteSprite.setScale(400.0f / WhiteTexture.getSize().x, 200.0f / WhiteTexture.getSize().y);
		BlackSprite.setScale(400.0f / BlackTexture.getSize().x, 200.0f / BlackTexture.getSize().y);

		while (gameOverWindow.isOpen()) {
			sf::Event evt;
			while (gameOverWindow.pollEvent(evt)) {
				if (evt.type == sf::Event::Closed) {
					gameOverWindow.close();
				}
			}

			gameOverWindow.clear(sf::Color::White);
			if (result == 1) {
				gameOverWindow.draw(WhiteSprite); // White wins
			}else if (result == 0) {
				gameOverWindow.draw(BlackSprite);
			}else {
				gameOverWindow.draw(drawSprite);
			}
			gameOverWindow.display();
		}
	}

}
