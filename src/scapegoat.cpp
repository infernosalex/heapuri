#include "scapegoat.h"
#include <stdexcept>

// PRIVATE METHODS
int ScapegoatTree::sizeOf(SGNode *node) {
    if (!node) return 0;
    return 1 + sizeOf(node->left) + sizeOf(node->right);
}

bool ScapegoatTree::isAlphaWeightBalanced(SGNode *node, double alpha) {
    if (!node) return true;
    
    int leftSize = sizeOf(node->left);
    int rightSize = sizeOf(node->right);
    int nodeSize = leftSize + rightSize + 1;
    
    return leftSize <= alpha * nodeSize && rightSize <= alpha * nodeSize;
}

SGNode* ScapegoatTree::findScapegoat(SGNode *node, int key) {
    if (!node) return nullptr;
    
    int leftSize = sizeOf(node->left);
    int rightSize = sizeOf(node->right);
    int nodeSize = leftSize + rightSize + 1;
    
    if (!isAlphaWeightBalanced(node, alpha)) {
        return node;
    }
    
    if (key < node->key) {
        SGNode* leftScapegoat = findScapegoat(node->left, key);
        return leftScapegoat ? leftScapegoat : node;
    } else {
        SGNode* rightScapegoat = findScapegoat(node->right, key);
        return rightScapegoat ? rightScapegoat : node;
    }
}

void ScapegoatTree::flattenToVector(SGNode *node, std::vector<SGNode*> &nodes) {
    if (!node) return;
    
    flattenToVector(node->left, nodes);
    nodes.push_back(node);
    flattenToVector(node->right, nodes);
}

SGNode* ScapegoatTree::rebuildTree(const std::vector<SGNode*> &nodes, int start, int end) {
    if (start > end) return nullptr;
    
    int mid = (start + end) / 2;
    SGNode *node = nodes[mid];
    
    node->left = rebuildTree(nodes, start, mid - 1);
    node->right = rebuildTree(nodes, mid + 1, end);
    
    return node;
}

SGNode* ScapegoatTree::rebuildSubtree(SGNode *scapegoat) {
    std::vector<SGNode*> nodes;
    flattenToVector(scapegoat, nodes);
    
    for (auto node : nodes) {
        node->left = nullptr;
        node->right = nullptr;
    }
    
    return rebuildTree(nodes, 0, nodes.size() - 1);
}

SGNode* ScapegoatTree::insertRecursive(SGNode *node, int key) {
    if (!node) {
        size++;
        maxSize = std::max(maxSize, size);
        return new SGNode(key);
    }
    
    if (key < node->key) {
        node->left = insertRecursive(node->left, key);
    } else if (key > node->key) {
        node->right = insertRecursive(node->right, key);
    } else {
        // duplicate keys not allowed
        return node;
    }
    
    // check if rebalancing is needed
    if (size > std::log(maxSize) / std::log(1/alpha)) {
        node = rebuildSubtree(node);
        maxSize = size;
    }
    
    return node;
}

SGNode* ScapegoatTree::searchRecursive(SGNode *node, int key) const {
    if (!node || node->key == key) {
        return node;
    }
    
    if (key < node->key) {
        return searchRecursive(node->left, key);
    } else {
        return searchRecursive(node->right, key);
    }
}

SGNode* ScapegoatTree::findMin(SGNode* node) const {
    if (!node) return nullptr;
    
    SGNode* current = node;
    while (current->left) {
        current = current->left;
    }
    return current;
}

SGNode* ScapegoatTree::findMax(SGNode* node) const {
    if (!node) return nullptr;
    
    SGNode* current = node;
    while (current->right) {
        current = current->right;
    }
    return current;
}

SGNode* ScapegoatTree::floorRecursive(SGNode* node, int key) const {
    if (!node) return nullptr;
    
    // if key equals node's key, we found exact floor
    if (node->key == key) return node;
    
    // if key is smaller than node's key, look in left subtree
    if (key < node->key) return floorRecursive(node->left, key);
    
    // if key is greater than node's key, look in right subtree
    // the current node could be the floor, but we might find a closer one in right subtree
    SGNode* rightFloor = floorRecursive(node->right, key);
    if (rightFloor) return rightFloor;
    
    // if nothing found in right subtree, this node is the floor
    return node;
}

SGNode* ScapegoatTree::ceilingRecursive(SGNode* node, int key) const {
    if (!node) return nullptr;
    
    // if key equals node's key, we found exact ceiling
    if (node->key == key) return node;
    
    // if key is greater than node's key, look in right subtree
    if (key > node->key) return ceilingRecursive(node->right, key);
    
    // if key is smaller than node's key, look in left subtree
    // the current node could be the ceiling, but we might find a closer one in left subtree
    SGNode* leftCeiling = ceilingRecursive(node->left, key);
    if (leftCeiling) return leftCeiling;
    
    // if nothing found in left subtree, this node is the ceiling
    return node;
}

void ScapegoatTree::rangeQueryRecursive(SGNode* node, int x, int y, std::vector<int>& result) const {
    if (!node) return;
    
    // if node's key is greater than x, explore left subtree
    if (x < node->key) {
        rangeQueryRecursive(node->left, x, y, result);
    }
    
    // add current node's key if it's within range [x, y]
    if (x <= node->key && node->key <= y) {
        result.push_back(node->key);
    }
    
    // if node's key is less than y, explore right subtree
    if (node->key < y) {
        rangeQueryRecursive(node->right, x, y, result);
    }
}

SGNode* ScapegoatTree::deleteRecursive(SGNode *node, int key) {
    if (!node) return nullptr;
    
    if (key < node->key) {
        node->left = deleteRecursive(node->left, key);
    } else if (key > node->key) {
        node->right = deleteRecursive(node->right, key);
    } else {
        // node with the key to be deleted found
        
        // case 1: node has no children or only one child
        if (!node->left) {
            SGNode *temp = node->right;
            delete node;
            size--;
            return temp;
        } else if (!node->right) {
            SGNode *temp = node->left;
            delete node;
            size--;
            return temp;
        }
        
        // case 2: node has two children
        // find the inorder successor (smallest node in right subtree)
        SGNode *current = node->right;
        while (current && current->left) {
            current = current->left;
        }
        
        // copy the successor's key to this node
        node->key = current->key;
        
        // delete the successor
        node->right = deleteRecursive(node->right, current->key);
    }
    
    // check if rebuild is needed after deletion
    if (size < alpha * maxSize) {
        root = rebuildSubtree(root);
        maxSize = size;
    }
    
    return node;
}

void ScapegoatTree::destroyRecursive(SGNode *node) {
    if (node) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

// PUBLIC METHODS
ScapegoatTree::ScapegoatTree(double a) : root(nullptr), size(0), maxSize(0), alpha(a) {
    if (alpha <= 0.5 || alpha >= 1.0) {
        alpha = 0.7; // default to 0.7 if given an invalid alpha
    }
}

ScapegoatTree::~ScapegoatTree() {
    destroyRecursive(root);
}

void ScapegoatTree::insert(int key) {
    root = insertRecursive(root, key);
    
    // check for scapegoat after insertion
    int height = 0;
    SGNode* node = root;
    while (node && node->key != key) {
        height++;
        node = (key < node->key) ? node->left : node->right;
    }
    
    // if height exceeds log_alpha(size), find a scapegoat
    if (height > std::log(size) / std::log(1/alpha)) {
        SGNode* scapegoat = findScapegoat(root, key);
        if (scapegoat) {
            // replace scapegoat with a rebuilt tree
            if (scapegoat == root) {
                root = rebuildSubtree(root);
            } else {
                // find parent of scapegoat
                node = root;
                while (node) {
                    if ((node->left && node->left->key == scapegoat->key) || 
                        (node->right && node->right->key == scapegoat->key)) {
                        break;
                    }
                    node = (key < node->key) ? node->left : node->right;
                }
                
                if (node->left == scapegoat) {
                    node->left = rebuildSubtree(scapegoat);
                } else {
                    node->right = rebuildSubtree(scapegoat);
                }
            }
        }
    }
}

void ScapegoatTree::remove(int key) {
    root = deleteRecursive(root, key);
}

bool ScapegoatTree::search(int key) const {
    return searchRecursive(root, key) != nullptr;
}

bool ScapegoatTree::isEmpty() const {
    return root == nullptr;
}

ScapegoatTree ScapegoatTree::join(const ScapegoatTree& other) {
    ScapegoatTree result(alpha);
    
    // get all nodes from both trees in sorted order
    std::vector<SGNode*> thisNodes;
    std::vector<SGNode*> otherNodes;
    
    flattenToVector(root, thisNodes);
    flattenToVector(other.root, otherNodes);
    
    // insert all keys into the new tree
    // first from this tree
    for (auto node : thisNodes) {
        result.insert(node->key);
    }
    
    // then from the other tree (duplicates will be handled by insert)
    for (auto node : otherNodes) {
        result.insert(node->key);
    }
    
    return result;
}

int ScapegoatTree::floor(int key) const {
    SGNode* floorNode = floorRecursive(root, key);
    if (!floorNode) {
        throw std::runtime_error("No floor value exists");
    }
    return floorNode->key;
}

int ScapegoatTree::ceiling(int key) const {
    SGNode* ceilingNode = ceilingRecursive(root, key);
    if (!ceilingNode) {
        throw std::runtime_error("No ceiling value exists");
    }
    return ceilingNode->key;
}

std::vector<int> ScapegoatTree::rangeQuery(int x, int y) const {
    std::vector<int> result;
    rangeQueryRecursive(root, x, y, result);
    return result;
}

void ScapegoatTree::printRange(int x, int y) const {
    std::vector<int> rangeValues = rangeQuery(x, y);
    
    if (rangeValues.empty()) {
        std::cout << "No values in range [" << x << ", " << y << "]" << std::endl;
        return;
    }
    
    std::cout << "Values in range [" << x << ", " << y << "]: ";
    for (size_t i = 0; i < rangeValues.size(); ++i) {
        std::cout << rangeValues[i];
        if (i < rangeValues.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
} 