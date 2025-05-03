#ifndef AVL_H
#define AVL_H

#include <algorithm> 
#include <iostream>
#include <vector>

struct AVLNode {
    int key;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode *root;

    int getHeight(AVLNode *node);
    int getBalanceFactor(AVLNode *node);
    void updateHeight(AVLNode *node);
    AVLNode* rotateRight(AVLNode *y);
    AVLNode* rotateLeft(AVLNode *x);
    AVLNode* balance(AVLNode *node);
    AVLNode* insertRecursive(AVLNode *node, int key);
    AVLNode* findMin(AVLNode *node);
    AVLNode* findMax(AVLNode *node);
    AVLNode* searchRecursive(AVLNode *node, int key) const;
    AVLNode* deleteRecursive(AVLNode *node, int key);
    void destroyRecursive(AVLNode *node);
    AVLNode* floorRecursive(AVLNode* node, int key) const;
    AVLNode* ceilingRecursive(AVLNode* node, int key) const;
    void rangeQueryRecursive(AVLNode* node, int x, int y, std::vector<int>& result) const;
    void inOrderTraversal(AVLNode* node, std::vector<AVLNode*>& nodes) const;
    AVLNode* buildBalancedTree(const std::vector<AVLNode*>& nodes, int start, int end);

public:
    AVLTree();
    ~AVLTree();

    void insert(int key); // O(log n)
    void remove(int key); // O(log n)
    bool search(int key) const; // O(log n)
    bool isEmpty() const; // O(1)
    AVLTree join(const AVLTree& other); // O(n + m)
    int floor(int key) const; // O(log n)
    int ceiling(int key) const; // O(log n)
    std::vector<int> rangeQuery(int x, int y) const; // O(k + log n) - k is the number of elements in the range
    void printRange(int x, int y) const;
};

#endif
