#ifndef __BVHNODE__
#define __BVHNODE__

#include "Object.hpp"

class BVHNode {
    BVHNode * leftChild;
    BVHNode * rightChild;
    Object * obj; // Will be valid if node is a leaf
    public:
        BVHNode();
        BVHNode(BVHNode * leftChild, BVHNode * rightChild, Object * obj);
        BVHNode * getLeftChild() const;
        BVHNode * getRightChild() const;
        Object * getObject() const;
        void setLeftChild(BVHNode * n);
        void setRightChild(BVHNode * n);
        void setObject(Object * o);
};

#endif
