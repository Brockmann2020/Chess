#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#endif //CHESS_BOARD_H

#include "string"
#include "array"

class Board {
private:
    std::array<int, 64> _squares;
public:
    Board();
    std::string generatePGN();
    bool movePiece(int piece, int square);
    const std::array<int, 64>& getSquares() const;

};
