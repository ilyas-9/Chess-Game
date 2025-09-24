
#include "Chess.h"

int main() {
	Chess::Chess chessGame;

	// Main game loop
	while (chessGame.isOpen()) {
		chessGame.ProcessEvents();
		chessGame.Render();
	}

	std::cout << "Exiting the game." << std::endl;
	return 0;
}
