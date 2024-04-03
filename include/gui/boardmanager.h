#ifndef RAYLIB_TEMPLATE_BOARD_H
#define RAYLIB_TEMPLATE_BOARD_H

#endif //RAYLIB_TEMPLATE_BOARD_H

#include "raylib.h"
#include "array"
#include "string"

class BoardManager {
private:
    unsigned int _size;
    bool _inverted = false;
    std::array<int, 12> _assets;
    Color _black = Color{126,149,94,255};
    Color _white = Color{237,238,213,255};
public:
    explicit BoardManager(unsigned int size, bool plays_white);
    void printBoard();
    void updateBoard();
    std::array<int, 16> loadAssets(std::string assetPath);
};