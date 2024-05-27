#include <memory>
#include "raylib.h"

#ifndef CHESSBOT_CHESSPIECE_H
#define CHESSBOT_CHESSPIECE_H

#endif //CHESSBOT_CHESSPIECE_H

class ChessPiece {
private:
    unsigned int _boardIndex;
    unsigned int _code;
    Vector2 _pos;
    Vector2 _fallbackPosition;
    std::shared_ptr <Texture> _texture;
    Rectangle _collisionRect;
public:
    ChessPiece(int boardIndex, int code, Vector2 position, std::shared_ptr <Texture> texture, Rectangle _collisionRect):
            _boardIndex(boardIndex),
            _code(code),
            _pos(position),
            _texture(texture),
            _collisionRect(_collisionRect) {};

    unsigned int getBoardIndex() const;
    unsigned int getCode() const;
    Vector2 getPos() const;
    void setPos(const Vector2& newPos);
    std::shared_ptr <Texture> getTexture() const;
    Rectangle getCollisionRect() const;
    void setCollisionRect(const Rectangle& newRect);
    void setFallbackPosition(const Vector2& fallbackPosition);
    void movePiece(Vector2 mousePosition);
};