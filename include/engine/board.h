#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#endif //CHESS_BOARD_H

#include <vector>
#include <memory>
#include "string"
#include "array"
#include "move.h"
#include "attacktree.h"

/*struct Move {
    int origin;
    int target;
    int piece;
    bool enPassant = false;

};*/

class Board {
private:
    bool _whiteToMove = true;
    std::array<int, 64> _squares;
    std::vector<Move> _moves;
    AttackMap _attackTree;

    bool _blackLongCastleLegal = true;
    bool _blackShortCastleLegal = true;
    bool _whiteLongCastleLegal = true;
    bool _whiteShortCastleLegal = true;

    int _moveCount;
    int _engineCallCount;
    int _movesGenerated;

    std::vector<Move> generatePawnMoves(int index, bool isWhite);
    std::vector<Move> generateSimpleMoves(int index, bool isWhite, std::vector<std::array<int, 2>> movePatterns, bool limit);
    std::vector<Move> generateKingMoves(int index, bool isWhite);
    void handleCastling(int index, const std::shared_ptr<std::vector<Move>>& legalMoves, bool shortCastleLegal, bool longCastleLegal);
    std::shared_ptr<AttackMapRoot> populateAttackTree(int index, std::vector<std::array<int, 2>> movePatterns);
public:
    Board();
    std::string generatePGN();
    void logGameStatistics();
    bool movePiece(Move move);
    const std::array<int, 64>& getSquares() const;

    std::vector<Move> generateMoves(int depth);
    std::vector<Move> generateMovesForPiece(int index);
    Move getRandomMove();
};