#ifndef SCAPEGOAT_H
#define SCAPEGOAT_H

#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>

struct SGNode {
    int key;
    SGNode *left;
    SGNode *right;

    SGNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class ScapegoatTree {
private:
    SGNode *root;
    int size;           // current size of the tree
    int maxSize;        // maximum size since last rebuild
    double alpha;       // balance factor (typically between 0.5 and 1)

    // Helper functions
    int sizeOf(SGNode *node);
    bool isAlphaWeightBalanced(SGNode *node, double alpha);
    SGNode* findScapegoat(SGNode *node, int key);
    SGNode* insertRecursive(SGNode *node, int key);
    SGNode* searchRecursive(SGNode *node, int key) const;
    SGNode* deleteRecursive(SGNode *node, int key);
    void flattenToVector(SGNode *node, std::vector<SGNode*> &nodes);
    SGNode* rebuildTree(const std::vector<SGNode*> &nodes, int start, int end);
    SGNode* rebuildSubtree(SGNode *scapegoat);
    void destroyRecursive(SGNode *node);
    SGNode* findMin(SGNode* node) const;
    SGNode* findMax(SGNode* node) const;
    SGNode* floorRecursive(SGNode* node, int key) const;
    SGNode* ceilingRecursive(SGNode* node, int key) const;
    void rangeQueryRecursive(SGNode* node, int x, int y, std::vector<int>& result) const;

public:
    ScapegoatTree(double a = 0.7); // alpha default value is 0.7
    ~ScapegoatTree();

    void insert(int key); // O(log n) amortized
    void remove(int key); // O(log n) amortized
    bool search(int key) const; // O(log n)
    bool isEmpty() const; // O(1)
    ScapegoatTree join(const ScapegoatTree& other); // O(n + m)
    int floor(int key) const; // O(log n)
    int ceiling(int key) const; // O(log n)
    std::vector<int> rangeQuery(int x, int y) const; // O(k + log n) - k is the number of elements in the range
    void printRange(int x, int y) const;
};

#endif 