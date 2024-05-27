#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#endif //CHESS_BOARD_H

#include <vector>
#include "string"
#include "array"

struct Move {
    int origin;
    int target;
    int piece;
    bool enPassant = false;
};

class Board {
private:
    bool _whiteToMove = true;
    std::array<int, 64> _squares;
    std::vector<Move> _moves;

    std::vector<Move> generatePawnMoves(int index, bool isWhite);
    std::vector<Move> generateKnightMoves(int index, bool isWhite);
    std::vector<Move> generateSimpleMoves(int index, bool isWhite, std::vector<std::array<int, 2>> movePatterns, bool limit);

    int signum(int value);
public:
    Board();
    std::string generatePGN();
    bool movePiece(Move move);
    const std::array<int, 64>& getSquares() const;

    std::vector<Move> generateMoves(int depth);
    std::vector<Move> generateMovesForPiece(int index);
};
