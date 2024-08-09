#include "board.h"
#include "piece.h"
#include "memory"
#include <algorithm>
#include <iostream>
#include <random>

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

    if (move.MoveType == MoveType::ShortCastle || move.MoveType == MoveType::LongCastle) {
        // Handle Castling
        int king = _squares[move.origin];
        int rook = _squares[move.MoveType == MoveType::ShortCastle ? move.origin + 3 : move.origin - 4];
        _squares[move.MoveType == MoveType::ShortCastle ? move.origin + 1 : move.origin - 1] = rook;
        _squares[move.target] = king;
        _squares[move.origin] = Piece::None;
    } else {
        // Regular Move
        int draggedPiece = _squares[move.origin];
        _squares[move.origin] = Piece::None;
        _squares[move.target] = draggedPiece;
    }

    // Handle Castling Rights
    if (_whiteToMove) {
        if ((move.piece & 7) == Piece::Rook && move.origin == 63) {
            _whiteShortCastleLegal = false;
        } else if ((move.piece & 7) == Piece::Rook && move.origin == 56) {
            _whiteLongCastleLegal = false;
        } else if ((move.piece & 7) == Piece::King) {
            _whiteShortCastleLegal = false;
            _whiteLongCastleLegal = false;
        }
    } else {
        if ((move.piece & 7) == Piece::Rook && move.origin == 63) {
            _blackShortCastleLegal = false;
        } else if ((move.piece & 7) == Piece::Rook && move.origin == 56) {
            _blackLongCastleLegal = false;
        } else if ((move.piece & 7) == Piece::King) {
            _blackShortCastleLegal = false;
            _blackLongCastleLegal = false;
        }
    }
    _moveCount++;
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
        moveCount += allMovesForSquare.size();
        moves.insert(moves.end(), allMovesForSquare.begin(), allMovesForSquare.end());
    }
    _engineCallCount++;
    _movesGenerated += moveCount;
    std::cout << moveCount << " moves found!" << std::endl;
    return moves;
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
    std::vector<std::array<int, 2>> queenPatterns = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

    switch (piece & 7) {
        case Piece::Pawn: return generatePawnMoves(index, isWhite);
        case Piece::Knight: return generateSimpleMoves(index, isWhite, knightPatterns, true);
        case Piece::Bishop: return generateSimpleMoves(index, isWhite, bishopPatterns, false);
        case Piece::Rook: return generateSimpleMoves(index, isWhite, rookPatterns, false);
        case Piece::Queen: return generateSimpleMoves(index, isWhite, queenPatterns, false);
        case Piece::King: return generateKingMoves(index, isWhite);
        default: return {};
    }
}

std::vector<Move> Board::generatePawnMoves(int index, bool isWhite) {
    std::vector<Move> moves;
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;
    int enPassantRow = isWhite ? 3 : 4;

    // First Move
    if (index / 8 == startRow) {
        if ((_squares[index + 16 * direction] & 7) == Piece::None && (_squares[index + 8 * direction] & 7) == Piece::None) {
            moves.push_back({index, index + 16 * direction});
        }
    }

    // Regular Move
    if ((_squares[index + 8 * direction] & 7) == Piece::None) {
        moves.push_back({index, index + 8 * direction, Piece::Pawn, MoveType::Regular});
    }

    // Captures
    if (index % 8 != 0 && (_squares[index + 7 * direction] & 7) != Piece::None && (_squares[index + 7 * direction] & 8) != (_squares[index] & 8)) {
        moves.push_back({index, index + 7 * direction, Piece::Pawn, MoveType::Regular});
    }
    if (index % 8 != 7 && (_squares[index + 9 * direction] & 7) != Piece::None && (_squares[index + 9 * direction] & 8) != (_squares[index] & 8)) {
        moves.push_back({index, index + 9 * direction, Piece::Pawn, MoveType::Regular});
    }

    // En Passant
    if (!_moves.empty()) {
        Move lastMove = _moves.back();
        int lastMovePiece = _squares[lastMove.target];

        if ((lastMovePiece & 7) == Piece::Pawn) {
            if (std::abs(lastMove.origin - lastMove.target) == 16) { // Double-step move
                if (lastMove.target / 8 == enPassantRow) {
                    // Check left capture
                    if (lastMove.target == index + 1 && index % 8 != 0) {
                        moves.push_back({index, index + 7 * direction, Piece::Pawn, MoveType::EnPassant});
                    }
                    // Check right capture
                    if (lastMove.target == index - 1 && index % 8 != 7) {
                        moves.push_back({index, index + 9 * direction, Piece::Pawn, MoveType::EnPassant});
                    }
                }
            }
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
                    moves.push_back({index, newIndex, _squares[newIndex] & 7, MoveType::Regular});
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

std::vector<Move> Board::generateKingMoves(int index, bool isWhite) {
    std::vector<std::array<int, 2>> KingPatterns = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    std::vector<int> possibleTargets;
    std::vector<Move> legalMoves;

    // Handle Castling
    auto sharedLegalMoves = std::make_shared<std::vector<Move>>(legalMoves);
    if (_whiteToMove) {
        handleCastling(index, sharedLegalMoves, _whiteShortCastleLegal, _whiteLongCastleLegal);
    } else {
        handleCastling(index, sharedLegalMoves, _blackShortCastleLegal, _blackLongCastleLegal);
    }
    legalMoves = *sharedLegalMoves;

    int rank = index / 8;
    int file = index % 8;

    for (auto& move : KingPatterns) {
        int newRank = rank + move[0];
        int newFile = file + move[1];

        // Check if the new position is within the board limits
        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
            possibleTargets.push_back(newRank * 8 + newFile);
        }
    }

    for (auto & move : possibleTargets) {
        if ((_squares[move] & 7) == Piece::None || (_squares[move] & 8) != (_squares[index] & 8)) {
            legalMoves.push_back({index, move, Piece::King});
        }
    }
    return legalMoves;
}

void Board::handleCastling(int index, const std::shared_ptr<std::vector<Move>>& legalMoves, bool shortCastleLegal, bool longCastleLegal) {
    if (longCastleLegal && _squares[index - 1] == Piece::None && _squares[index - 2] == Piece::None && _squares[index-3] == Piece::None) {
        legalMoves->push_back({index, index-2, Piece::King, MoveType::LongCastle});
    }
    if (shortCastleLegal && _squares[index + 1] == Piece::None && _squares[index + 2] == Piece::None) {
        legalMoves->push_back({index, index+2, Piece::King, MoveType::ShortCastle});
    }
}

Move Board::getRandomMove() {
    std::vector<Move> moves = generateMoves(1);

    if (moves.empty()) {
        throw std::out_of_range("No moves available");
    }

    // Create a random number generator
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(0, moves.size() - 1); // Define the range

    // Get a random index
    int randomIndex = distr(gen);

    // Return the move at the random index
    return moves[randomIndex];
}

void Board::logGameStatistics() {

}
