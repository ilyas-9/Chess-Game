# Chess Game 🏆

A comprehensive chess game implementation in C++ featuring a complete chess engine with standard rules, move validation, and an intuitive interface.

![Chess Game](https://img.shields.io/badge/Game-Chess-blue)
![Language](https://img.shields.io/badge/Language-C++-orange)
![License](https://img.shields.io/badge/License-MIT-green)

## 📋 Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Game Rules](#game-rules)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [Future Enhancements](#future-enhancements)
- [License](#license)

## ✨ Features

### Current Features
- Classic 8x8 chess board representation
- Complete set of chess pieces (King, Queen, Rook, Bishop, Knight, Pawn)
- Valid move generation and validation
- Check and checkmate detection
- Standard chess rules implementation
- Turn-based gameplay for two players

### Planned Features
- [ ] Graphical user interface (GUI)
- [ ] AI opponent with multiple difficulty levels
- [ ] Save and load game functionality
- [ ] Move history and undo functionality
- [ ] Chess notation support (PGN format)
- [ ] Online multiplayer support
- [ ] Time controls and chess clocks
- [ ] Opening book and endgame tablebase

## 🔧 Prerequisites

Before you begin, ensure you have the following installed on your system:

- **C++ Compiler**: GCC 7.0+ or Clang 5.0+ with C++17 support
- **Build System**: CMake 3.10+ (recommended) or Make
- **Operating System**: Linux, macOS, or Windows

### Dependencies
- Standard C++ Library (STL)
- (Optional) SFML 2.5+ for graphical interface
- (Optional) CMake for cross-platform building

## 🚀 Installation

### Option 1: Using CMake (Recommended)
```bash
# Clone the repository
git clone https://github.com/ilyas-9/Chess-Game.git
cd Chess-Game

# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
make

# Run the game
./chess_game
```

### Option 2: Direct Compilation
```bash
# Clone the repository
git clone https://github.com/ilyas-9/Chess-Game.git
cd Chess-Game

# Compile directly
g++ -std=c++17 -o chess_game src/*.cpp

# Run the game
./chess_game
```

## 🎮 Usage

### Starting a Game
1. Run the executable: `./chess_game`
2. The game will display an 8x8 chess board
3. White moves first (standard chess rules)
4. Enter moves using algebraic notation (e.g., "e2e4" or "Nf3")

### Making Moves
- **Standard Move**: `e2e4` (moves piece from e2 to e4)
- **Castling**: `O-O` (kingside) or `O-O-O` (queenside)
- **Pawn Promotion**: `e7e8Q` (promotes pawn to Queen)
- **Quit Game**: Type `quit` or `exit`

### Game Controls
```
Commands:
  help     - Display available commands
  board    - Show current board position
  history  - Show move history
  undo     - Undo last move (if available)
  quit     - Exit the game
```

## ♟️ Game Rules

This chess implementation follows standard FIDE (World Chess Federation) rules:

### Basic Rules
- **Objective**: Checkmate the opponent's king
- **Turns**: Players alternate turns, White moves first
- **Piece Movement**: Each piece has specific movement patterns
- **Capture**: Land on opponent's piece to capture it

### Special Rules
- **Castling**: King and rook special move (conditions apply)
- **En Passant**: Special pawn capture rule
- **Pawn Promotion**: Pawn reaching the opposite end promotes to any piece
- **Check**: King under attack must be resolved immediately
- **Checkmate**: King under attack with no legal moves to escape
- **Stalemate**: No legal moves available but king not in check (draw)

### Piece Values (Reference)
| Piece | Symbol | Value |
|-------|--------|-------|
| Pawn  | ♟      | 1     |
| Knight| ♞      | 3     |
| Bishop| ♝      | 3     |
| Rook  | ♜      | 5     |
| Queen | ♛      | 9     |
| King  | ♚      | ∞     |

## 📁 Project Structure

```
Chess-Game/
├── src/                    # Source files
│   ├── main.cpp           # Main application entry point
│   ├── board.cpp          # Chess board implementation
│   ├── piece.cpp          # Chess piece classes
│   ├── game.cpp           # Game logic and state management
│   ├── move.cpp           # Move generation and validation
│   └── utils.cpp          # Utility functions
├── include/               # Header files
│   ├── board.h           # Board class definition
│   ├── piece.h           # Piece class definitions
│   ├── game.h            # Game class definition
│   ├── move.h            # Move related definitions
│   └── utils.h           # Utility function declarations
├── tests/                # Unit tests
│   ├── test_board.cpp    # Board testing
│   ├── test_pieces.cpp   # Piece movement tests
│   └── test_game.cpp     # Game logic tests
├── docs/                 # Documentation
│   ├── design.md         # Design decisions and architecture
│   └── api.md           # API documentation
├── assets/              # Game assets (if GUI implemented)
│   └── pieces/          # Piece sprites/images
├── CMakeLists.txt       # CMake build configuration
├── Makefile            # Make build configuration
└── README.md           # This file
```

## 🤝 Contributing

We welcome contributions to improve the Chess Game! Here's how you can help:

### Getting Started
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Make your changes and commit: `git commit -m 'Add amazing feature'`
4. Push to the branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

### Contribution Guidelines
- Follow C++ best practices and coding standards
- Write clear, documented code with meaningful variable names
- Add unit tests for new functionality
- Update documentation as needed
- Ensure all existing tests pass

### Areas for Contribution
- Bug fixes and optimizations
- New game features
- GUI development
- AI implementation
- Code documentation
- Performance improvements

## 🚀 Future Enhancements

### Short Term
- [ ] Complete console-based interface
- [ ] Comprehensive unit testing
- [ ] Move validation optimization
- [ ] Basic AI opponent

### Medium Term
- [ ] Graphical user interface using SFML
- [ ] Advanced AI with minimax algorithm
- [ ] Save/load game functionality
- [ ] Chess notation support

### Long Term
- [ ] Online multiplayer support
- [ ] Tournament mode
- [ ] Chess analysis engine
- [ ] Mobile app version

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2024 Chess-Game

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🙏 Acknowledgments

- Chess piece Unicode symbols from the Unicode Standard
- Inspiration from classic chess implementations
- FIDE rules and regulations for standard chess gameplay

---

**Happy Chess Playing!** ♟️

For questions, suggestions, or bug reports, please [open an issue](https://github.com/ilyas-9/Chess-Game/issues).