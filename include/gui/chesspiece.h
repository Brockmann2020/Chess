#include <memory>
#include "raylib.h"

#ifndef CHESSBOT_CHESSPIECE_H
#define CHESSBOT_CHESSPIECE_H

#endif //CHESSBOT_CHESSPIECE_H

class ChessPiece {
private:
    unsigned int _id;
    Vector2 _pos;
    std::shared_ptr <Texture> _texture;
    Rectangle _collisionRect;
public:
    ChessPiece(int id, Vector2 position, std::shared_ptr <Texture> texture, Rectangle _collisionRect):
        _id(id),
        _pos(position),
        _texture(texture),
        _collisionRect(_collisionRect) {};

    unsigned int getId() const;
    Vector2 getPos() const;
    void setPos(const Vector2& newPos);
    std::shared_ptr <Texture> getTexture() const;
    Rectangle getCollisionRect() const;
    void setCollisionRect(const Rectangle& newRect);

};