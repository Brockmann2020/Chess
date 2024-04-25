#include <iostream>
#include "chessboard.h"
#include "raylib.h"
#include "piece.h"

ChessBoard::ChessBoard(unsigned int size, bool plays_white) {
    _size = size;
    _inverted = plays_white;

    // ChessPiece Creation:
    loadAssets();
    int square_size = _size / 8;
    float piece_size = (float) _size / 8 * 0.9;
    for (int rank = 0; rank < 8; rank++) {
        int pos_y = rank * square_size + 0.05 * square_size;
        int coll_pos_y = rank * square_size + 0.05 * square_size;

        for (int file = 0; file < 8; file++) {
            int pos_x = file * square_size + 0.05 * square_size;
            int coll_pos_x = file * square_size + 0.05 * square_size;

            int board_index = file + rank * 8;

            int piece_id = _board.getSquares()[board_index];
            if (piece_id == Piece::None) {
                continue;
            }

            Vector2 piece_pos = {static_cast<float>(pos_x), static_cast<float>(pos_y)};
            Rectangle piece_collRect = {static_cast<float>(coll_pos_x), static_cast<float>(coll_pos_y), piece_size, piece_size};

            ChessPiece piece = ChessPiece(piece_id, piece_pos, _assets[piece_id], piece_collRect);
            addPiece(std::make_shared<ChessPiece>(piece));
        }
    }
}


void ChessBoard::printBoard() {
    int square_size = (int) _size / 8;

    for (int rank = 0; rank < 8; rank++) {
        int pos_y = rank * square_size;

        for (int file = 0; file < 8; file++) {

            int pos_x = file * square_size;
            bool isWhite = ( file + rank ) % 2 == 0;

            DrawRectangle(pos_x, pos_y, square_size, square_size, isWhite ? _white : _black);
        }
    }
}

void ChessBoard::updateBoard() {
    Vector2 mousePosition = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool mouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    // Handle starting the drag
    if (mousePressed) {
        for (const auto& piece : _pieces) {
            if (CheckCollisionPointRec(mousePosition, piece->getCollisionRect())) {
                _draggedPiece = piece;
                _dragOffset.x = mousePosition.x - piece->getCollisionRect().x;
                _dragOffset.y = mousePosition.y - piece->getCollisionRect().y;
                _fallbackPosition = _draggedPiece->getPos();
                break;  // Start dragging the first piece we find under cursor
            }
        }
    }

    // Update piece position if dragging
    if (mouseDown && _draggedPiece != nullptr) {
        _draggedPiece->setPos({mousePosition.x - _dragOffset.x, mousePosition.y - _dragOffset.y});
        Rectangle rect = _draggedPiece->getCollisionRect();
        _draggedPiece->setCollisionRect({mousePosition.x - _dragOffset.x, mousePosition.y - _dragOffset.y, rect.width, rect.height});
    } else if (_draggedPiece != nullptr) {
        _draggedPiece->setPos(_fallbackPosition);
        _draggedPiece->setCollisionRect({_fallbackPosition.x, _fallbackPosition.y, _draggedPiece->getCollisionRect().width, _draggedPiece->getCollisionRect().height});
        _draggedPiece = nullptr;  // Stop dragging
    }

    // Drawing and other updates
    for (const auto& piece : _pieces) {
        DrawTexture(*piece->getTexture(), piece->getPos().x, piece->getPos().y, WHITE);
        #ifdef DEBUG
        DrawRectangleLines(piece->getCollisionRect().x, piece->getCollisionRect().y, piece->getCollisionRect().width, piece->getCollisionRect().height, RED);
        #endif
    }
}

void ChessBoard::loadAssets() {
    std::string pieceNames[6] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};
    int pieceSize = _size / 8 * 0.9;
    // Load Pieces
    std::string color = "White_";
    for (int i = 0; i < 2; i++) {
        for (const auto & pieceName : pieceNames) {
            std::cout << color << pieceName << std::endl;
            // Load Image
            Image image = LoadImageSvg((ASSETS_PATH + color += pieceName + ".svg").c_str(), pieceSize, pieceSize);
            // Create Texture
            std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(LoadTextureFromImage(image));
            // Save Texture
            _assets[(color == "White_" ? Piece::White : Piece::Black) | Piece::getPieceValue(pieceName)] = texture;

            UnloadImage(image);
        }
        color = "Black_";
    }
}

void ChessBoard::addPiece(std::shared_ptr<ChessPiece> piece) {
    _pieces.push_back(piece);
}

void ChessBoard::removePiece(size_t index) {
    if (index < _pieces.size()) {
        std::swap(_pieces[index], _pieces.back());
        _pieces.pop_back();
    }
}