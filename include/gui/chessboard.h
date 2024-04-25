#ifndef RAYLIB_TEMPLATE_BOARD_H
#define RAYLIB_TEMPLATE_BOARD_H

#endif //RAYLIB_TEMPLATE_BOARD_H

#include <vector>
#include "raylib.h"
#include "array"
#include "map"
#include "string"
#include "board.h"
#include "memory"
#include "chesspiece.h"

class ChessBoard {
private:
    Board _board;
    unsigned int _size;
    bool _inverted = false;
    std::vector<std::shared_ptr<ChessPiece>> _pieces;
    std::map<int, std::shared_ptr<Texture2D>> _assets;
    Color _black = Color{126,149,94,255};
    Color _white = Color{237,238,213,255};
public:
    explicit ChessBoard(unsigned int size, bool plays_white);
    void printBoard();
    void updateBoard();
    void loadAssets();
    void addPiece(std::shared_ptr<ChessPiece> piece);
    void removePiece(size_t index);
};