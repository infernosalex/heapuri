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
    // root = deleteRecursive(root, key);
}

bool AVLTree::search(int key) const {
    return searchRecursive(root, key) != nullptr;
}

bool AVLTree::isEmpty() const {
    return root == nullptr;
}
