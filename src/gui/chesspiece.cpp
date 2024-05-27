#include "ChessBoard.h"


void ChessPiece::movePiece(Vector2 mousePosition) {
    float targetX = (float) ((int) (mousePosition.x / ChessBoard::getSquareSize())) * ChessBoard::getSquareSize();
    float targetY = (float) ((int) (mousePosition.y / ChessBoard::getSquareSize())) * ChessBoard::getSquareSize();
    _pos = {targetX, targetY};
    _collisionRect = {targetX, targetY, _collisionRect.width, _collisionRect.height};
    _boardIndex = ((int) (mousePosition.x / ChessBoard::getSquareSize())) + 8 * ((int) (mousePosition.y / ChessBoard::getSquareSize()));
}


// Getters
unsigned int ChessPiece::getBoardIndex() const {
    return _boardIndex;
}

unsigned int ChessPiece::getCode() const {
    return _code;
}

Vector2 ChessPiece::getPos() const {
    return _pos;
}

std::shared_ptr <Texture> ChessPiece::getTexture() const {
    return _texture;
}

Rectangle ChessPiece::getCollisionRect() const {
    return _collisionRect;
}

void ChessPiece::setCollisionRect(const Rectangle& newRect) {
    _collisionRect = newRect;
}

// Setters
void ChessPiece::setPos(const Vector2& newPos) {
    _pos = newPos;
}

void ChessPiece::setFallbackPosition(const Vector2 &fallbackPosition) {
    _fallbackPosition = fallbackPosition;
}

