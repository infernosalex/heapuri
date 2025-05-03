#include "avl.h" 
#include <stdexcept> 

// PRIVATE
int AVLTree::getHeight(AVLNode *node) {
    return node ? node->height : 0;
}

void AVLTree::updateHeight(AVLNode *node) {
    if (node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
}

int AVLTree::getBalanceFactor(AVLNode *node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode* AVLTree::rotateRight(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // perform rotation
    x->right = y;
    y->left = T2;

    // update heights
    updateHeight(y);
    updateHeight(x);

    // return new root
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // perform rotation
    y->left = x;
    x->right = T2;

    // update heights
    updateHeight(x);
    updateHeight(y);

    // return new root
    return y;
}


// https://www.geeksforgeeks.org/introduction-to-avl-tree/
AVLNode* AVLTree::balance(AVLNode *node) {
    updateHeight(node);
    int balanceFactor = getBalanceFactor(node);

    // left heeavy
    if (balanceFactor > 1) {
        // left-right Case
        if (getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        // left-left Case
        return rotateRight(node);
    }

    // right heavy
    if (balanceFactor < -1) {
        // right-keft Case
        if (getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        // right-right Case
        return rotateLeft(node);
    }

    // balanced tree
    return node;
}


AVLNode* AVLTree::insertRecursive(AVLNode *node, int key) {
    // insert
    if (!node) {
        return new AVLNode(key);
    }

    if (key < node->key) {
        node->left = insertRecursive(node->left, key);
    } else if (key > node->key) {
        node->right = insertRecursive(node->right, key);
    } else {
        // duplicate keys are not allowed, just return the node
        return node;
    }

    // update height + balance the current node
    return balance(node);
}

AVLNode* AVLTree::findMin(AVLNode *node) {
    AVLNode* current = node;
    // find the leftmost leaf
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

AVLNode* AVLTree::findMax(AVLNode *node) {
    AVLNode* current = node;
    // find the rightmost leaf
    while (current && current->right != nullptr) {
        current = current->right;
    }
    return current;
}

AVLNode* AVLTree::searchRecursive(AVLNode *node, int key) const {
    if (!node || node->key == key) {
        return node;
    }
    if (key < node->key) {
        return searchRecursive(node->left, key);
    } else {
        return searchRecursive(node->right, key);
    }
}

AVLNode* AVLTree::floorRecursive(AVLNode* node, int key) const {
    if (!node) return nullptr;
    
    // if key equals node's key, we found exact floor
    if (node->key == key) return node;
    
    // if key is smaller than node's key, look in left subtree
    if (key < node->key) return floorRecursive(node->left, key);
    
    // if key is greater than node's key, look in right subtree
    // the current node could be the floor, but we might find a closer one in right subtree
    AVLNode* rightFloor = floorRecursive(node->right, key);
    if (rightFloor) return rightFloor;
    
    // if nothing found in right subtree, this node is the floor
    return node;
}

AVLNode* AVLTree::ceilingRecursive(AVLNode* node, int key) const {
    if (!node) return nullptr;
    
    // if key equals node's key, we found exact ceiling
    if (node->key == key) return node;
    
    // if key is greater than node's key, look in right subtree
    if (key > node->key) return ceilingRecursive(node->right, key);
    
    // if key is smaller than node's key, look in left subtree
    // the current node could be the ceiling, but we might find a closer one in left subtree
    AVLNode* leftCeiling = ceilingRecursive(node->left, key);
    if (leftCeiling) return leftCeiling;
    
    // if nothing found in left subtree, this node is the ceiling
    return node;
}

void AVLTree::rangeQueryRecursive(AVLNode* node, int x, int y, std::vector<int>& result) const {
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

void AVLTree::inOrderTraversal(AVLNode* node, std::vector<AVLNode*>& nodes) const {
    if (!node) return;
    inOrderTraversal(node->left, nodes);
    nodes.push_back(node);
    inOrderTraversal(node->right, nodes);
}

AVLNode* AVLTree::buildBalancedTree(const std::vector<AVLNode*>& nodes, int start, int end) {
    if (start > end) return nullptr;
    
    int mid = (start + end) / 2;
    AVLNode* node = nodes[mid];
    
    // reset pointers
    node->left = nullptr;
    node->right = nullptr;
    
    // recursively build left and right subtrees
    node->left = buildBalancedTree(nodes, start, mid - 1);
    node->right = buildBalancedTree(nodes, mid + 1, end);
    
    // update height
    updateHeight(node);
    
    return node;
}

AVLNode* AVLTree::deleteRecursive(AVLNode *node, int key) {
    // delete
    if (!node) {
        return node; // key not found
    }

    // delete left subtree
    if (key < node->key) {
        node->left = deleteRecursive(node->left, key);
    }
    // delete right subtree
    else if (key > node->key) {
        node->right = deleteRecursive(node->right, key);
    }

    // node with only one child or no child
    else {
        if (!node->left || !node->right) {
            AVLNode *temp = node->left ? node->left : node->right;

            // no child case
            if (!temp) {
                temp = node;
                node = nullptr;
            } else { // one child case
                // copy the contents of the non-empty child
                *node = *temp;
            }
            delete temp;
        } else {
            AVLNode* temp = findMin(node->right);

            // copy the inorder successor's data to this node
            node->key = temp->key;

            // delete the inorder successor
            node->right = deleteRecursive(node->right, temp->key);
        }
    }

    // one node case
    if (!node) {
        return node;
    }

    // update height + balance the current node
    return balance(node);
}

void AVLTree::destroyRecursive(AVLNode *node) {
    if (node) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

// PUBLIC
AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    destroyRecursive(root);
}

void AVLTree::insert(int key) {
    root = insertRecursive(root, key);
}

void AVLTree::remove(int key) {
    root = deleteRecursive(root, key);
}

bool AVLTree::search(int key) const {
    return searchRecursive(root, key) != nullptr;
}

bool AVLTree::isEmpty() const {
    return root == nullptr;
}

AVLTree AVLTree::join(const AVLTree& other) {
    AVLTree result;
    
    // collect nodes from both trees
    std::vector<AVLNode*> thisNodes;
    std::vector<AVLNode*> otherNodes;
    std::vector<AVLNode*> mergedNodes;
    
    inOrderTraversal(root, thisNodes);
    inOrderTraversal(other.root, otherNodes);
    
    // merge the sorted arrays of nodes
    size_t i = 0, j = 0;
    while (i < thisNodes.size() && j < otherNodes.size()) {
        if (thisNodes[i]->key < otherNodes[j]->key) {
            result.insert(thisNodes[i]->key);
            i++;
        } else if (thisNodes[i]->key > otherNodes[j]->key) {
            result.insert(otherNodes[j]->key);
            j++;
        } else {
            // if both have the same key, just insert once
            result.insert(thisNodes[i]->key);
            i++;
            j++;
        }
    }
    
    // add remaining elements
    while (i < thisNodes.size()) {
        result.insert(thisNodes[i]->key);
        i++;
    }
    
    while (j < otherNodes.size()) {
        result.insert(otherNodes[j]->key);
        j++;
    }
    
    return result;
}

int AVLTree::floor(int key) const {
    AVLNode* floorNode = floorRecursive(root, key);
    if (!floorNode) {
        throw std::runtime_error("No floor value exists");
    }
    return floorNode->key;
}

int AVLTree::ceiling(int key) const {
    AVLNode* ceilingNode = ceilingRecursive(root, key);
    if (!ceilingNode) {
        throw std::runtime_error("No ceiling value exists");
    }
    return ceilingNode->key;
}

std::vector<int> AVLTree::rangeQuery(int x, int y) const {
    std::vector<int> result;
    rangeQueryRecursive(root, x, y, result);
    return result;
}

void AVLTree::printRange(int x, int y) const {
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
