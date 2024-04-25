#include "ChessPiece.h"

// Getters
unsigned int ChessPiece::getId() const {
    return _id;
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

