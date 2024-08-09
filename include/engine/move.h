#ifndef CHESSBOT_MOVE_H
#define CHESSBOT_MOVE_H

#endif //CHESSBOT_MOVE_H

struct Move {
    int origin{};
    int target{};
    int piece{};
    int MoveType{0};
};

class MoveType {
public:
    static const int None = 0;
    static const int Regular = 1;
    static const int EnPassant = 2;
    static const int ShortCastle = 4;
    static const int LongCastle = 8;
    static const int Capture = 16;
    static const int Check = 32;
    static const int Promote = 64;
};
