#ifndef AVL_H
#define AVL_H

#include <algorithm> 
#include <iostream>  

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
    AVLNode* searchRecursive(AVLNode *node, int key) const;
    AVLNode* deleteRecursive(AVLNode *node, int key);
    void destroyRecursive(AVLNode *node);

public:
    AVLTree();
    ~AVLTree();

    void insert(int key);
    void remove(int key);
    bool search(int key) const;
    bool isEmpty() const;
};

#endif
