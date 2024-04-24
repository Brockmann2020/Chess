#include "piece.h"

bool Piece::isWhite(int id) {
    return (id & 8) != 0;
}

std::string Piece::getPieceName(int id) {
    switch (id & 7) {
        case None:   return "None";
        case Pawn:   return "Pawn";
        case Knight: return "Knight";
        case Bishop: return "Bishop";
        case Rook:   return "Rook";
        case Queen:  return "Queen";
        case King:   return "King";
        default:     return "Unknown";
    }
}

int Piece::getPieceValue(std::string name) {
    if (name == "Pawn") {
        return Pawn;
    } else if (name == "Knight") {
        return Knight;
    } else if (name == "Bishop") {
        return Bishop;
    } else if (name == "Rook") {
        return Rook;
    } else if (name == "Queen") {
        return Queen;
    } else if (name == "King") {
        return King;
    } else {
        return None; // Default or error case
    }
}
