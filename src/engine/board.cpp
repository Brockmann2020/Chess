#include "board.h"
#include "piece.h"
#include "memory"

Board::Board() {
    // Clear the board
    for (int i = 0; i < 64; ++i) {
        _squares[i] = Piece::None;
    }

    // Set up pawns
    for (int i = 8; i < 16; ++i) {
        _squares[i] = Piece::Black | Piece::Pawn;
        _squares[i + 40] = Piece::White | Piece::Pawn;
    }

    // Piece order for Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook
    int pieceOrder[8] = {Piece::Rook, Piece::Knight, Piece::Bishop, Piece::Queen, Piece::King, Piece::Bishop, Piece::Knight, Piece::Rook};

    // Set up other pieces
    for (int i = 0; i < 8; ++i) {
        _squares[i] = Piece::Black | pieceOrder[i];
        _squares[i + 56] = Piece::White | pieceOrder[i];
    }
}

const std::array<int, 64>& Board::getSquares() const {
    return _squares;
}

