#include "boardmanager.h"
#include "board.h"
#include "raylib.h"

BoardManager::BoardManager(unsigned int size, bool plays_white) {
    _size = size;
    _inverted = plays_white;
}

void BoardManager::printBoard() {
    int square_size = (int) _size / 8;

    for (int rank = 0; rank < 8; rank++) {
        int pos_y = rank * square_size;

        for (int file = 0; file < 8; file++) {

            int pos_x = file * square_size;
            bool isWhite = ( file + rank ) % 2 == 0;

            DrawRectangle(pos_x, pos_y, square_size, square_size, isWhite ? _white : _black);
        }
    }

    Board board = Board();


}

std::array<int, 16> BoardManager::loadAssets(std::string assetPath) {
    std::string pieceNames[6] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    // Load Pieces
    std::string color = "White_";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            Image image = LoadImageSvg((color + pieceNames[j]).c_str());
        }
        color = "Black_";
    }

    // Load Black Pieces

}