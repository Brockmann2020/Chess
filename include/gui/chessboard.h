#ifndef RAYLIB_TEMPLATE_BOARD_H
#define RAYLIB_TEMPLATE_BOARD_H

#endif //RAYLIB_TEMPLATE_BOARD_H

#include "raylib.h"
#include "array"
#include "map"
#include "string"
#include "board.h"
#include "memory"

class ChessBoard {
private:
    Board _board;
    unsigned int _size;
    bool _inverted = false;
    //std::array<Texture2D , 12> _assets;
    //std::array<std::shared_ptr<ChessPiece>, 12> _pieces;
    std::map<int, std::shared_ptr<Texture2D>> _chessPieces;
    Color _black = Color{126,149,94,255};
    Color _white = Color{237,238,213,255};
public:
    explicit ChessBoard(unsigned int size, bool plays_white);
    void printBoard();
    void updateBoard();
    void loadAssets();
};