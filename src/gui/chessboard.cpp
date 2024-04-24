#include <iostream>
#include "chessboard.h"
#include "raylib.h"
#include "piece.h"

ChessBoard::ChessBoard(unsigned int size, bool plays_white) {
    _size = size;
    _inverted = plays_white;
}

void ChessBoard::printBoard() {
    int square_size = (int) _size / 8;

    for (int rank = 0; rank < 8; rank++) {
        int pos_y = rank * square_size;

        for (int file = 0; file < 8; file++) {

            int pos_x = file * square_size;
            bool isWhite = ( file + rank ) % 2 == 0;

            DrawRectangle(pos_x, pos_y, square_size, square_size, isWhite ? _white : _black);
        }
    }
}

void ChessBoard::updateBoard() {
    int square_size = (int) _size / 8;

    printBoard();

    for (int rank = 0; rank < 8; rank++) {
        int pos_y = rank * square_size + 0.05 * square_size;

        for (int file = 0; file < 8; file++) {
            int pos_x = file * square_size + 0.05 * square_size;
            int board_index = file + rank * 8;
            //int board_index = ((rank + 1) * (file + 1))-1;

            int piece = _board.getSquares()[board_index];

            if (piece == Piece::None) {
                continue;
            }

            // Check Color
            std::string color = "White_";
            if ((_board.getSquares()[rank * file] & 16) != 0) {
                color = "Black_";
            }

            Texture2D piece_tex = *_chessPieces[_board.getSquares()[board_index]];

            DrawTexture(piece_tex, pos_x, pos_y, WHITE);

        }
    }
}

void ChessBoard::loadAssets() {
    std::string pieceNames[6] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};
    int pieceSize = _size / 8 * 0.9;
    // Load Pieces
    std::string color = "White_";
    for (int i = 0; i < 2; i++) {
        for (const auto & pieceName : pieceNames) {
            std::cout << color << pieceName << std::endl;
            // Load Image
            Image image = LoadImageSvg((ASSETS_PATH + color += pieceName + ".svg").c_str(), pieceSize, pieceSize);
            // Create Texture
            std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(LoadTextureFromImage(image));
            // Save Texture
            _chessPieces[(color == "White_" ? Piece::White : Piece::Black) | Piece::getPieceValue(pieceName)] = texture;
            UnloadImage(image);
        }
        color = "Black_";
    }
}