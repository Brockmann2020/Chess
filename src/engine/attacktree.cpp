#include "attacktree.h"

AttackMapVector::AttackMapVector(int x, int y) : _x(x), _y(y) { }

std::shared_ptr<AttackMapRoot> AttackMap::addAttackTree(int index, const AttackMapRoot& root) {
    std::shared_ptr<AttackMapRoot> treeRoot = std::make_shared<AttackMapRoot>(root);
    _attackTrees[index] = treeRoot;
    return treeRoot;
}

std::shared_ptr<AttackMapRoot> AttackMap::getAttackTreeForPiece(int index) {
    return _attackTrees[index];
}

