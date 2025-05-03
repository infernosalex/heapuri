#include "avl.h"
#include "scapegoat.h"
#include <iostream>

void testAVLTree() {
    std::cout << "\n=== AVL Tree ===\n" << std::endl;
    
    AVLTree tree;

    // insert some keys
    std::cout << "Inserting keys: 10, 20, 30, 40, 50, 25" << std::endl;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    // search for a key
    int searchKey = 30;
    std::cout << "Searching for key " << searchKey << ": ";
    if (tree.search(searchKey)) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }

    // remove a key
    int removeKey = 20;
    std::cout << "Removing key " << removeKey << std::endl;
    tree.remove(removeKey);

    // search again after removal
    std::cout << "Searching for key " << removeKey << " after removal: ";
    if (tree.search(removeKey)) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }
    
    // test floor operation
    try {
        int floorKey = 24;
        std::cout << "Floor of " << floorKey << ": " << tree.floor(floorKey) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Floor error: " << e.what() << std::endl;
    }
    
    // test ceiling operation
    try {
        int ceilingKey = 24;
        std::cout << "Ceiling of " << ceilingKey << ": " << tree.ceiling(ceilingKey) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Ceiling error: " << e.what() << std::endl;
    }
    
    // test range query
    int x = 25, y = 45;
    std::cout << "Range query [" << x << ", " << y << "]:" << std::endl;
    tree.printRange(x, y);
    
    // test join operation
    std::cout << "\nTesting join operation:" << std::endl;
    AVLTree tree2;
    tree2.insert(5);
    tree2.insert(15);
    tree2.insert(55);
    tree2.insert(60);
    
    std::cout << "Tree2 contains: ";
    tree2.printRange(0, 100);
    
    AVLTree joinedTree = tree.join(tree2);
    std::cout << "Joined tree contains: ";
    joinedTree.printRange(0, 100);
}

void testScapegoatTree() {
    std::cout << "\n=== Scapegoat Tree ===\n" << std::endl;
    
    ScapegoatTree tree;

    // insert some keys
    std::cout << "Inserting keys: 10, 20, 30, 40, 50, 25, 5, 15, 35, 45" << std::endl;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);
    tree.insert(5);
    tree.insert(15);
    tree.insert(35);
    tree.insert(45);

    // search for a key
    int searchKey = 30;
    std::cout << "Searching for key " << searchKey << ": ";
    if (tree.search(searchKey)) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }

    // remove a key
    int removeKey = 20;
    std::cout << "Removing key " << removeKey << std::endl;
    tree.remove(removeKey);

    // search again after removal
    std::cout << "Searching for key " << removeKey << " after removal: ";
    if (tree.search(removeKey)) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }
    
    // search for another key
    searchKey = 15;
    std::cout << "Searching for key " << searchKey << ": ";
    if (tree.search(searchKey)) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }
    
    // test floor operation
    try {
        int floorKey = 26;
        std::cout << "Floor of " << floorKey << ": " << tree.floor(floorKey) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Floor error: " << e.what() << std::endl;
    }
    
    // test ceiling operation
    try {
        int ceilingKey = 26;
        std::cout << "Ceiling of " << ceilingKey << ": " << tree.ceiling(ceilingKey) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Ceiling error: " << e.what() << std::endl;
    }
    
    // test range query
    int x = 25, y = 45;
    std::cout << "Range query [" << x << ", " << y << "]:" << std::endl;
    tree.printRange(x, y);
    
    // test join operation
    std::cout << "\nTesting join operation:" << std::endl;
    ScapegoatTree tree2;
    tree2.insert(2);
    tree2.insert(60);
    tree2.insert(70);
    tree2.insert(12);
    
    std::cout << "Tree2 contains: ";
    tree2.printRange(0, 100);
    
    ScapegoatTree joinedTree = tree.join(tree2);
    std::cout << "Joined tree contains: ";
    joinedTree.printRange(0, 100);
}

int main() {
    testAVLTree();
    testScapegoatTree();
    
    return 0;
}
