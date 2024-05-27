
#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#endif //CHESS_PIECE_H

#include "raylib.h"
#include <string>

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

    static constexpr int PawnOffset[] = {7,8,9};
    static constexpr int KnightOffset[] = {6,10,15,17, -6,-10,-15,-17};
    static constexpr int BishopOffset[] = {7, 9, -7, -9};
    static constexpr int RookOffset[] = {1, 8, -1, -8};
    static constexpr int QueenOffset[] = {1, 8, 7, 9, -1, -8, -7, -9};
    static constexpr int KingOffset[] = {1, 8, 7, 9, -1, -8, -7, -9};

    static std::string getPieceName(int id);

    static bool isWhite(int id);

    static int getPieceValue(std::string name);


};