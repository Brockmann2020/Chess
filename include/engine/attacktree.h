#include "piece.h"
#include "memory"
#include "vector"

#ifndef CHESSBOT_ATTACKTREE_H
#define CHESSBOT_ATTACKTREE_H

#endif //CHESSBOT_ATTACKTREE_H


struct Direction {
    int x{};
    int y{};
};

struct TreeNode {
    Direction direction{};
    bool isEndOfBoard{false};
    bool isEnemyKing{false};
    bool isFriendlyPiece{false};
    bool isEnemyPiece{false};
    std::unique_ptr<TreeNode> child{};
};

class AttackTree {
private:
public:
    int piece;
    std::vector<std::shared_ptr<TreeNode>> children;
    bool isRecursive;
};