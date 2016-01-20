#include "BVHNode.hpp"

BVHNode::BVHNode() : leftChild(nullptr), rightChild(nullptr), obj(nullptr) {}

BVHNode::BVHNode(BVHNode * leftChild, BVHNode * rightChild, Object * obj)
    : leftChild(leftChild), rightChild(rightChild), obj(obj) {}

BVHNode * BVHNode::getLeftChild() const {
    return leftChild;
}

BVHNode * BVHNode::getRightChild() const {
    return rightChild;
}

Object * BVHNode::getObject() const {
    return obj;
}

void BVHNode::setLeftChild(BVHNode * n) {
    leftChild = n;
}

void BVHNode::setRightChild(BVHNode * n) {
    rightChild = n;
}

void BVHNode::setObject(Object * o) {
    obj = o;
}
