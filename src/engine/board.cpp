#include "board.h"
#include "piece.h"
#include "memory"
#include <algorithm>
#include <iostream>

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

bool Board::movePiece(Move move) {
    int draggedPiece = _squares[move.origin];
    _squares[move.origin] = Piece::None;
    _squares[move.target] = draggedPiece;
    _whiteToMove = !_whiteToMove;
    _moves.push_back(move);
    return true;
}

const std::array<int, 64>& Board::getSquares() const {
    return _squares;
}

std::vector<Move> Board::generateMoves(int depth) {
    std::vector<Move> moves;
    int moveCount = 0;
    for (int i = 0; i < _squares.size(); i++) {
        int piece = _squares[i];
        if ((piece & 7) == Piece::None) {
            continue;
        }
        if ((piece & 8) != _whiteToMove) {
            continue;
        }
        std::vector<Move> allMovesForSquare = generateMovesForPiece(i);
        moves.insert(moves.end(), allMovesForSquare.begin(), allMovesForSquare.end());
    }
    std::cout << moveCount << " moves found!" << std::endl;
    return {};
}

std::vector<Move> Board::generateMovesForPiece(int index) {
    int piece = _squares[index];
    bool isWhite = (piece & 8) != 0;

    if (isWhite != _whiteToMove) {
        return {};
    }

    std::vector<std::array<int, 2>> knightPatterns = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    std::vector<std::array<int, 2>> bishopPatterns = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    std::vector<std::array<int, 2>> rookPatterns = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    std::vector<std::array<int, 2>> queenKingPatterns = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};


    switch (piece & 7) {
        case Piece::Pawn: return generatePawnMoves(index, isWhite);
        case Piece::Knight: return generateSimpleMoves(index, isWhite, knightPatterns, true);
        case Piece::Bishop: return generateSimpleMoves(index, isWhite, bishopPatterns, false);
        case Piece::Rook: return generateSimpleMoves(index, isWhite, rookPatterns, false);
        case Piece::Queen: return generateSimpleMoves(index, isWhite, queenKingPatterns, false);
        case Piece::King: return generateSimpleMoves(index, isWhite, queenKingPatterns, true);
        default: return {};
    }
}

std::vector<Move> generatePawnMoves() {
    return{};
}

std::vector<Move> Board::generatePawnMoves(int index, bool isWhite) {
    std::vector<Move> moves;
    if (!isWhite) {
        // First Move
        if (index >= 8 && index <=15) {
            if ((_squares[index + 16] & 7) == Piece::None) {
                moves.push_back({index, index + 16});
            }
        }
        // Regular Moves
        if ((_squares[index + 8] & 7) == Piece::None) {
            moves.push_back({index, index + 8});
        }
        if (index % 8 != 0 && (_squares[index + 7] & 7) != Piece::None && (_squares[index + 7] & 8) != (_squares[index] & 8)) {
            moves.push_back({index, index + 7});
        }
        if (index % 8 != 7 && (_squares[index + 9] & 7) != Piece::None && (_squares[index + 9] & 8) != (_squares[index] & 8)) {
            moves.push_back({index, index + 9});
        }
        // En Passant
        if (!_moves.empty()) {
            Move lastMove = _moves[_moves.size() - 1];
            if ((lastMove.piece & 7) == Piece::Pawn && lastMove.origin == index + 15 && lastMove.target == index - 1) {
                moves.push_back({index, index + 7, Piece::Pawn, true});
            }
            if ((lastMove.piece & 7) == Piece::Pawn && lastMove.origin == index + 17 && lastMove.target == index + 1) {
                moves.push_back({index, index + 9, Piece::Pawn, true});
            }
        }
    } else {
        if (index >= 48 && index <= 55) {
            if ((_squares[index -16] & 7) == Piece::None) {
                moves.push_back({index, index - 16});
            }
        }
        if ((_squares[index -8] & 7) == Piece::None) {
            moves.push_back({index, index - 8});
        }
        if (index % 8 != 7 && (_squares[index - 7] & 7) != Piece::None && (_squares[index - 7] & 8) != (_squares[index] & 8)) {
            moves.push_back({index, index -7});
        }
        if (index % 8 != 0 && (_squares[index - 9] & 7) != Piece::None&& (_squares[index - 9] & 8) != (_squares[index] & 8)) {
            moves.push_back({index, index - 9});
        }
        // En Passant
        if (!_moves.empty()) {
            Move lastMove = _moves[_moves.size() - 1];
            if ((lastMove.piece & 7) == Piece::Pawn && lastMove.origin == index - 15 && lastMove.target == index + 1 ) {
                moves.push_back({index, index - 7, Piece::Pawn, true});
            }
            if ((lastMove.piece & 7) == Piece::Pawn && lastMove.origin == index - 17 && lastMove.target == index - 1 ) {
                moves.push_back({index, index - 9, Piece::Pawn, true});
            }
        }
    }
    return moves;
}

std::vector<Move> Board::generateKnightMoves(int index, bool isWhite) {
    std::vector<int> knightOffset;
    // Possible moves of a knight from a given position
    int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    int rank = index / 8;
    int file = index % 8;

    for (auto& move : knightMoves) {
        int newRank = rank + move[0];
        int newFile = file + move[1];

        // Check if the new position is within the board limits
        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
            knightOffset.push_back(newRank * 8 + newFile);
        }
    }

    std::vector<Move> moves;
    for (auto & move : knightOffset) {
        if ((_squares[move] & 7) == Piece::None || (_squares[move] & 8) != (_squares[index] & 8)) {
            moves.push_back({index, move});
        }
    }
    return moves;
}

std::vector<Move> Board::generateSimpleMoves(int index, bool isWhite, std::vector<std::array<int, 2>> movePatterns, bool limit) {
    std::vector<Move> moves;

    int rank = index / 8;
    int file = index % 8;

    for (auto& move : movePatterns) {
        int step = 1;
        while (true) {
            int newRank = rank + move[0] * step;
            int newFile = file + move[1] * step;

            // Check if the new position is within the board limits
            if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                int newIndex = newRank * 8 + newFile;
                if ((_squares[newIndex] & 7) == Piece::None || (_squares[newIndex] & 8) != (_squares[index] & 8)) {
                    moves.push_back({index, newIndex});
                    // Stop if it's a capture
                    if ((_squares[newIndex] & 7) != Piece::None) {
                        break;
                    }
                } else {
                    break; // Stop if there's a piece blocking the path
                }
            } else {
                break; // Out of board bounds
            }

            if (limit) {
                break; // Stop after one move for non-sliding pieces
            }
            step++;
        }
    }

    return moves;
}

int Board::signum(int value) {
    return (value > 0) - (value < 0);
}