#include "piece.h"
#include "memory"
#include "vector"
#include "map"

#ifndef CHESSBOT_ATTACKTREE_H
#define CHESSBOT_ATTACKTREE_H


struct AttackMapVector {
    AttackMapVector(int x, int y);

    int _x{};
    int _y{};

    int dist_till_blocked{0};
    bool looks_at_eob{false};
    bool looks_at_check{false};
    bool looks_at_friendly_piece{false};
    bool looks_at_enemy_piece{false};

    std::shared_ptr<AttackMapVector> child{};
};

struct AttackMapRoot {
    int piece;
    bool isSlidingPiece;
    std::vector<std::shared_ptr<AttackMapVector>> mapData;
};

class AttackMap {
private:
    std::map<int, std::shared_ptr<AttackMapRoot>> _attackTrees;
public:
    std::shared_ptr<AttackMapRoot> addAttackTree(int index, const AttackMapRoot& root);
    std::shared_ptr<AttackMapRoot> getAttackTreeForPiece(int index);
};


#endif //CHESSBOT_ATTACKTREE_H
