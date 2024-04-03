
#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#endif //CHESS_PIECE_H

#include "raylib.h"

class Piece {
public:
    static const int None = 0;
    static const int Pawn = 1;
    static const int Knight = 2;
    static const int Bishop = 3;
    static const int Rook = 4;
    static const int Queen = 5;
    static const int King = 6;

    static const int White = 8;
    static const int Black = 16;

    static Texture2D loadTexture(int Piece);
};