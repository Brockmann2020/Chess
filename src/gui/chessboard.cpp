#include <iostream>
#include <dirent.h>
#include "chessboard.h"
#include "raylib.h"
#include "piece.h"
#include "algorithm"

float ChessBoard::_squareSize = 0;

ChessBoard::ChessBoard(unsigned int size, bool plays_white) {
    _size = size;
    _squareSize = size / 8;
    _playsWhite = plays_white;
    _whiteToMove = plays_white;

#ifdef PLAY_AI
    _playComputer = true;
#elif PLAY_AI_DEBUG
    _playComputer = true;
#else
    _playComputer = false;
#endif
    std::cout << "Plays AI: " << _playComputer << "\n";

    // ChessPiece Creation:
    loadAssets();
    loadSounds();
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
            ChessPiece piece = ChessPiece(board_index, piece_id, piece_pos, _assets[piece_id], piece_collRect);
            addPiece(std::make_shared<ChessPiece>(piece));

            PlaySound(*_sounds["start.mp3"]);
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

            if (!_legalMoves.empty() && (_whiteToMove || !_playComputer)) {
                int square = rank * 8 + file;

                for (auto & move : _legalMoves) {
                    if (square == move.target) {
                        DrawRectangle(pos_x, pos_y, square_size, square_size, isWhite ? _lightRed : _darkRed);
                    }
                }
            }
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

                if (_legalMoves.empty()) {
                    _legalMoves = _board.generateMovesForPiece(_draggedPiece->getBoardIndex());
                }
                break;  // Start dragging the first we find at the cursor
            }
        }
    }

    // Update piece position if dragging
    if (mouseDown && _draggedPiece != nullptr) {
        _draggedPiece->setPos({mousePosition.x - _dragOffset.x, mousePosition.y - _dragOffset.y});
        Rectangle rect = _draggedPiece->getCollisionRect();
        _draggedPiece->setCollisionRect({mousePosition.x - _dragOffset.x, mousePosition.y - _dragOffset.y, rect.width, rect.height});
    } else if (_draggedPiece != nullptr ) {

        // Normalise Position
        int targetSquare = ((int) (mousePosition.x / _squareSize)) + 8 * ((int) (mousePosition.y / _squareSize));

        // Move Validation
        bool isLegal = false;
        Move nextMove;
        for (auto & move : _legalMoves) {
            if (move.target == targetSquare) {
                isLegal = true;
                nextMove = move;
                break;
            }
        }

        // Check if Move is legal and if its players turn
        if (isLegal && (_whiteToMove || !_playComputer)) {
            // En Passant
            if (nextMove.MoveType == MoveType::EnPassant) {
                removePiece(_whiteToMove ? targetSquare + 8 : targetSquare - 8);
            }
            // Capture
            else if (_board.getSquares()[targetSquare] != Piece::None && nextMove.MoveType == MoveType::Regular) {
                removePiece(targetSquare);
                nextMove.MoveType = MoveType::Capture;
            }
            // Castling
            else if (nextMove.MoveType == MoveType::ShortCastle || nextMove.MoveType == MoveType::LongCastle) {
                std::shared_ptr<ChessPiece> rook = findPiece(nextMove.MoveType == MoveType::ShortCastle ?
                        nextMove.origin + 3 : nextMove.origin - 4);
                rook->movePiece(calculatePiecePosition(nextMove.MoveType == MoveType::ShortCastle ?
                        nextMove.origin + 1 : nextMove.origin - 1));
            }
            playSound(nextMove);

            // Normalize Position & End Move
            _draggedPiece->movePiece(mousePosition);
            _board.movePiece(nextMove);
            _whiteToMove = !_whiteToMove;
            _legalMoves = {};
        } else {
            // Reset Piece
            _draggedPiece->setPos(_fallbackPosition);
            _draggedPiece->setCollisionRect({_fallbackPosition.x, _fallbackPosition.y, _draggedPiece->getCollisionRect().width, _draggedPiece->getCollisionRect().height});
            _legalMoves = {};
        }
        _draggedPiece = nullptr;  // Stop dragging
    }

    // Computers Turn
    if (_playComputer && !_whiteToMove) {
        Move move = _board.getRandomMove();
        std::shared_ptr<ChessPiece> piece = findPiece(move.origin);

        // Check if it's a capture
        if ((_board.getSquares()[move.target] & 7) != Piece::None) {
            if (move.MoveType == MoveType::EnPassant) {
                removePiece(_whiteToMove ? move.target + 8 : move.target - 8);
            } else if (_board.getSquares()[move.target] != Piece::None) {
                removePiece(move.target);
            }
        }

        piece->movePiece(calculatePiecePosition(move.target));
        _board.movePiece(move);
        _whiteToMove = !_whiteToMove;
    }

    // Drawing and other updates
    for (const auto& piece : _pieces) {
        DrawTexture(*piece->getTexture(), piece->getPos().x, piece->getPos().y, WHITE);
#ifdef DEBUG
        DrawRectangleLines(piece->getCollisionRect().x, piece->getCollisionRect().y, piece->getCollisionRect().width, piece->getCollisionRect().height, RED);
#elif PLAY_AI_DEBUG
        DrawRectangleLines(piece->getCollisionRect().x, piece->getCollisionRect().y, piece->getCollisionRect().width, piece->getCollisionRect().height, RED);
#endif
    }
}

void ChessBoard::playSound(Move move) {
    switch (move.MoveType) {
        case MoveType::EnPassant:
        case MoveType::Capture:
            PlaySound(*_sounds["capture.mp3"]);
            break;
        case MoveType::ShortCastle:
        case MoveType::LongCastle:
            PlaySound(*_sounds["castle.mp3"]);
            break;
        case MoveType::Check:
            PlaySound(*_sounds["check.mp3"]);
            break;
        case MoveType::Promote:
            PlaySound(*_sounds["promote.mp3"]);
        default:
            PlaySound(*_sounds["move.mp3"]);
    }
}

Vector2 ChessBoard::calculatePiecePosition(int index) {
    int row = index / 8;
    int column = index % 8;

    float x = column * _squareSize;
    float y = row * _squareSize;

    return Vector2{x, y};
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

void ChessBoard::loadSounds() {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(ASSETS_PATH)) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (IsFileExtension(ent->d_name, ".mp3")) {
                std::shared_ptr<Sound> sound = std::make_shared<Sound>(LoadSound((std::string(ASSETS_PATH) + ent->d_name).c_str()));
                _sounds[ent->d_name] = sound;
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
    }
}

int ChessBoard::getCurrentColor() const {
    if (_whiteToMove) {
        return Piece::White;
    }
    return Piece::Black;
}

void ChessBoard::addPiece(std::shared_ptr<ChessPiece> piece) {
    _pieces.push_back(piece);
}

std::shared_ptr<ChessPiece> ChessBoard::findPiece(int boardIndex) {
    auto it = std::find_if(_pieces.begin(), _pieces.end(),
                           [boardIndex](const std::shared_ptr<ChessPiece>& piece) {
                               return piece->getBoardIndex() == boardIndex;
                           });

    if (it != _pieces.end()) {
        return *it;  // Dereference iterator to return the shared_ptr
    }
    return nullptr;  // Return nullptr if no piece is found
}

void ChessBoard::removePiece(int boardIndex) {
    auto it = std::remove_if(_pieces.begin(), _pieces.end(),
                             [boardIndex](const std::shared_ptr<ChessPiece>& piece) {
                                 return piece->getBoardIndex() == boardIndex;
                             });

    // Erase the removed elements
    _pieces.erase(it, _pieces.end());
}

int ChessBoard::getSquareSize() {
    return _squareSize;
}
