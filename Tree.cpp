/*James Browning
Advanced Data Structures
Implementation Project Spring 2024
Professor: Dr. Brian C. Dean
Please refer to the included document for information on runtimes.  There is code below which will allow the user to
replicate these tests.
*/

#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <queue>
#include <algorithm>
#include "Tree.h"
#include "Node.h"

vector<int> eTour;
vector<int> eTourDepth;
vector<int> RMQLCA[20];
vector<int> RMQLCAindex[20];
vector<int> eulerFirst;
vector<int> eulerLast;
vector<int> postOrderSeqPreLabels;
vector<int> postOrderSeqValLabels;
vector<int> indexByValuePostOrder;
vector<int> indexByValuePostOrderSubtreeLeft;
vector<int> indexByValuePostOrderSubtreeRight;
vector<int> RMQSubtree[20];
vector<int> RMQSubtreeindex[20];
vector<int> parentVector;
vector<int> nextLowestPower;
int sizeDepth;
int sizePostOrder;
int treeSize;
int preOrderIterator = 1;
int postOrderIterator = 1;

Tree::Tree():root(nullptr){
}
Tree::~Tree(){
    deleteTree(root);
}

void Tree::insert(int val) {
    // make a new node, fill in the value portion
    TreeNode *newNode;
    newNode = new TreeNode(val);
    treeSize++;
    // call insertNode to place it into the tree
    insertNode(newNode);
}

TreeNode*  Tree::find(int val) const{
    // declare a cursor and set it to point to the root node
    // declare a bool and set it to false (for use in the while loop)
    TreeNode * cursor;
    cursor = root;
    bool found = false;
    // return false if either the root is nullptr or the cursor is nullptr
    if (root == nullptr || cursor == nullptr){
        return nullptr;
    }
    else {
        // while the node either matching the passed in key or directly above where it should be inserted has not been
        // found the while loop will continue to iterate
        // the while loop will move down the list so long as it doesn't reach a nullptr or a node with the same key, at
        // which point it will return a pointer to the proper node
        while (!found) {
            if (val < cursor->getValue()) {
                if (cursor->getLeft() != nullptr) {
                    cursor = cursor->getLeft();
                    continue;
                }
                else{
                    return cursor;
                }
            }
            if (val == cursor->getValue()) {
                return cursor;
            }
            if (val > cursor->getValue()) {
                if(cursor->getRight() != nullptr) {
                    cursor = cursor->getRight();
                    continue;
                }
                else{
                    return cursor;
                }
            }
            found = true;
        }
        return cursor;
    }
}

bool Tree::insertNode(TreeNode * newNode){
    // make a cursor pointer and if the tree is empty insert the node at the root and return true
    TreeNode * cursor;
    if (root == nullptr){
        root = newNode;
        newNode->setDepth(0);
        return true;
    }
    // properly insert the new node as a left or right child (based on the key) after detecting for duplicates
    else{
        cursor = find(newNode->getValue());
        if(cursor->getValue() == newNode->getValue()){
            cout << newNode->getValue() << " is a duplicate" << endl;
            return false;
        }
        if(cursor->getValue() > newNode->getValue()){
            cursor->setLeft(newNode);
            newNode->setParent(cursor);
            newNode->setDepth(cursor->getDepth()+1);
        }
        if(cursor->getValue() < newNode->getValue()){
            cursor->setRight(newNode);
            newNode->setParent(cursor);
            newNode->setDepth(cursor->getDepth()+1);
        }
        return true;
    }
}

void Tree::deleteTree(TreeNode *sel) {
    // If the pointed passed in is null, the function returns
    if (sel == nullptr) {
        return;
    }
    // recursively delete the left and then the right child trees
    deleteTree(sel->getLeft());
    deleteTree(sel->getRight());
    if(sel != root){
        delete sel;
        return;
    }
    // delete the root node and set the root pointer to nullptr after the left and right child trees have been deleted
    if(sel == root && root->getRight() == nullptr && root->getLeft() == nullptr){
        root = nullptr;
        delete sel;
        return;

    }
}

void Tree::display() const{
    // either display empty tree if the root points to nullptr or call the other display function and pass in the root
    // for the pointer and an integer 1 for the level
    if (root == nullptr){
        cout << "Empty Tree" << endl;
        return;
    }
    else{
        display(root, 1);
        return;
    }
}

void Tree::display(TreeNode *ptr, int level) const {
    // declare an integer for use in the while loop and check for nullptr
    // If nullptr is detected it will skip to the end of the function and simply output end-line
    int i = 0;
    if (ptr != nullptr) {
        // Uses recursion to make sure the leaf at the topmost edge if the tree (if looking at the tree rotated 90
        // degrees counter-clockwise) is printed first and the rest are printed in the correct place
        display(ptr->getRight(), level + 1);
        cout << endl;
        // detects for the root and inserts the required text if it is present
        if (ptr == root) {
            cout << "Root-->: ";
        }
        // This while loop provides the proper amount of spacing based on the level at which a node exists
        while (i < level && ptr != root) {
            cout << "        ";
            i++;
        }
        // This line is what actually outputs the key
        // notice that the recursive implementation of display on the left node is inserted after the outputting line
        // this ensures that the right subtree will be printed first
        cout << ptr->getValue();
        display(ptr->getLeft(), level + 1);
    }
    else {
        cout << endl;
    }
}

void Tree::eulerTour() {
    eulerTour(root);
}

void Tree::eulerTour(TreeNode *node) {
    eTour.push_back(node->getValue());
    eTourDepth.push_back(node->getDepth());
    if(node->getLeft() != nullptr){
        eulerTour(node->getLeft());
        eTour.push_back(node->getValue());
        eTourDepth.push_back(node->getDepth());
    }
    if(node->getRight() != nullptr){
        eulerTour(node->getRight());
        eTour.push_back(node->getValue());
        eTourDepth.push_back(node->getDepth());
    }
}

void Tree::windowMinsforLCA(){
    vector<int> temp = eTourDepth;
    sizeDepth = eTourDepth.size();
    int iter = 2;
    while (iter < sizeDepth){
        iter = iter*2;
    }
    iter = log2(iter) - 1;
    vector<int> temp2;
    int x = 1;
    int tail;
    int width;
    RMQLCAindex[0] = eTour;
    RMQLCA[0] = eTourDepth;
    while (iter != 0){
        tail = pow(2, x) - 1;
        width = pow(2, x) - (pow(2, x-1) -1);
        temp2 = RMQblockGeneratorforLCA(temp, tail, width, x);
        RMQLCA[x] = temp2;
        temp = temp2;
        x++;
        iter--;
    }
}

vector<int> Tree::RMQblockGeneratorforLCA(vector<int> n, int tail, int width, int index){
    int y;
    int iterator = 0;
    int min;
    int z;
    int minIndex;
    vector<int> returnVector;
    sizeDepth = eTourDepth.size();
    while (iterator < sizeDepth - tail){
        y = 0;
        z = iterator;
        while(y < width){
            if (z > n.size()){
                y++;
                z++;
                continue;
            }
            if (n.at(z) < min){
                min = n.at(z);
                minIndex = RMQLCAindex[index-1].at(z);
            }
            z++;
            y++;
        }
        iterator++;
        returnVector.push_back(min);
        RMQLCAindex[index].push_back(minIndex);
        min = std::numeric_limits<int>::max();
    }
    return returnVector;
}

int Tree::RMQ(int L, int R, int type) {
    int range = abs(R - L) + 1;
    int nextLwstpwr = nextLowestPower[range];
    if (type == 0) {
        int min = RMQLCA[nextLwstpwr].at(L);
        int index;
        if (RMQLCA[nextLwstpwr].at(R - (pow(2, nextLwstpwr) - 1)) < min) {
            index = R - (pow(2, nextLwstpwr) - 1);
        } else {
            index = L;
        }
        return RMQLCAindex[nextLwstpwr].at(index);
    }
    if (type == 1){
        int min = RMQSubtree[nextLwstpwr].at(L);
        int index;
        if (RMQSubtree[nextLwstpwr].at(R - (pow(2, nextLwstpwr) - 1)) < min) {
            index = R - (pow(2, nextLwstpwr) - 1);
        } else {
            index = L;
        }
        return RMQSubtreeindex[nextLwstpwr].at(index);
    }
}

int Tree::LCA(int i, int j){
    int L = eulerFirst[i];
    int R = eulerLast[j];
    if (L > R){
        L = eulerFirst[j];
        R = eulerLast[i];
    }
    int LCA = RMQ(L, R, 0);
    return LCA;
}

void Tree::windowMinsforSubtree(){
    vector<int> temp = postOrderSeqPreLabels; // 2, 3, 5, 9, 17
    sizePostOrder = postOrderSeqPreLabels.size();
    int iter = 2;
    while (iter < sizePostOrder){
        iter = iter*2;
    }
    iter = log2(iter) - 1;
    vector<int> temp2;
    int x = 1;
    int tail;
    int width;
    RMQSubtreeindex[0] = postOrderSeqValLabels;
    RMQSubtree[0] = postOrderSeqPreLabels;
    while (iter != 0){
        tail = pow(2, x) - 1;
        width = pow(2, x) - (pow(2, x-1) -1);
        temp2 = RMQblockGeneratorforSubtree(temp, tail, width, x);
        RMQSubtree[x] = temp2;
        temp = temp2;
        x++;
        iter--;
    }
}

vector<int> Tree::RMQblockGeneratorforSubtree(vector<int> n, int tail, int width, int index) {
    int y;
    int iterator = 0;
    int min;
    int z;
    int minIndex;
    vector<int> returnVector;
    sizePostOrder = postOrderSeqPreLabels.size();
    while (iterator < sizePostOrder - tail) {
        y = 0;
        z = iterator;
        while (y < width) {
            if (z > n.size()) {
                y++;
                z++;
                continue;
            }
            if (n.at(z) < min) {
                min = n.at(z);
                minIndex = RMQSubtreeindex[index - 1].at(z);
            }
            z++;
            y++;
        }
        iterator++;
        returnVector.push_back(min);
        RMQSubtreeindex[index].push_back(minIndex);
        min = std::numeric_limits<int>::max();
    }
    return returnVector;
}

int Tree::yChildLocator(int i, int j){
    int L = indexByValuePostOrder[j];
    int R = indexByValuePostOrderSubtreeRight[i];
    int yChild = RMQ(L, R, 1);
    return yChild;
}

void Tree::preOrder(TreeNode *node){
    if (node == nullptr){
        return;
    }
    node->setPreOrder(preOrderIterator);
    preOrderIterator++;
    preOrder(node->getLeft());
    preOrder(node->getRight());
}

void Tree::postOrder(TreeNode *node){
    if (node == nullptr){
        return;
    }
    postOrder(node->getLeft());
    postOrder(node->getRight());
    indexByValuePostOrder[node->getValue()] = postOrderIterator;
    if(node->getParent()!= nullptr) {
        parentVector[node->getValue()] = node->getParent()->getValue();
    }
    postOrderIterator++;
    postOrderSeqPreLabels.push_back(node->getpreOrder());
    postOrderSeqValLabels.push_back(node->getValue());
}

void Tree::indexByValueSubtrees(){
    int x = 0;
    TreeNode * node;
    vector<int> subTree;
    while (x < treeSize){
        subTree.clear();
        if (indexByValuePostOrder[x] != 0){
            node = find(x);
            postOrderSubtrees(node, &subTree);
            subTree.pop_back();
            indexByValuePostOrderSubtreeLeft[x] = (*min_element(subTree.begin(), subTree.end())-1);
            indexByValuePostOrderSubtreeRight[x] = (*max_element(subTree.begin(), subTree.end())-1);
        }
        x++;
    }
}

void Tree::postOrderSubtrees(TreeNode *node, vector<int> *subTree){
    if (node == nullptr){
        return;
    }
    postOrderSubtrees(node->getLeft(), subTree);
    postOrderSubtrees(node->getRight(), subTree);
    int value = node->getValue();
    subTree->push_back(indexByValuePostOrder[value]);
}

void Tree::setEulerFirstLast(){
    int x = 0;
    int value;
    TreeNode * node;
    eulerFirst.resize(treeSize*8);
    eulerLast.resize(treeSize*8);
    while (x < eTour.size()){
        value = eTour[x];
        node = find(value);
        if (node->getEulerFirst() == -1){
            node->setEulerFirst(x);
        }
        node->setEulerLast(x);
        x++;
    }
}

void Tree::eulerFirstLastPreprocess(TreeNode * node){
    if (node == nullptr){
        return;
    }
    eulerFirstLastPreprocess(node->getLeft());
    eulerFirst.insert(eulerFirst.begin() + node->getValue(), node->getEulerFirst());
    eulerLast.insert(eulerLast.begin() + node->getValue(), node->getEulerLast());
    eulerFirstLastPreprocess(node->getRight());
}

int Tree::nextNodeOnPath(int i, int j) {
    int varLCA = LCA(i, j);
    if (varLCA != i){
        return parentVector[i];
    }
    else{
        return yChildLocator(i, j);
    }

}

void Tree::nextLowestPowerGenerator() {
    int nextLwstpwr;
    int maxSize = 0;
    while (maxSize < eTour.size()) {
        nextLwstpwr = 2;
        while (nextLwstpwr < maxSize) {
            nextLwstpwr = nextLwstpwr * 2;
        }
        nextLwstpwr = log2(nextLwstpwr) - 1;
        nextLowestPower.push_back(nextLwstpwr);
        maxSize++;
    }
}

//This program runs the n = 10 test by default.  To run the n = 1000 test, comment lines 473-483 and line 521.  Then,
//uncomment lines 484-496 and line 520.  There is also a display function available at the bottom for reference.
//However, it's pretty ugly and difficult to read (especially with a large tree).
int main() {
    Tree *myTree = new Tree();
    myTree->insert(5);
    myTree->insert(3);
    myTree->insert(4);
    myTree->insert(6);
    myTree->insert(7);
    myTree->insert(9);
    myTree->insert(12);
    myTree->insert(11);
    myTree->insert(1);
    myTree->insert(2);
    /*vector<int> randomSequence;
    int x = 1;
    while (x <= 1000) {
        randomSequence.push_back(x);
        x++;
    }
    random_device rand;
    mt19937 f(rand());
    shuffle(randomSequence.begin(), randomSequence.end(), f);
    while (!randomSequence.empty()){
        myTree->insert(randomSequence.back());
        randomSequence.pop_back();
    }*/

    //Preprocessing for nextNodeOnPath
    myTree->eulerTour();
    myTree->windowMinsforLCA();
    myTree->setEulerFirstLast();
    myTree->eulerFirstLastPreprocess(myTree->getRoot());
    indexByValuePostOrder.resize(treeSize*8);
    indexByValuePostOrderSubtreeLeft.resize(treeSize*8);
    indexByValuePostOrderSubtreeRight.resize(treeSize*8);
    parentVector.resize(treeSize*8);
    myTree->preOrder(myTree->getRoot());
    myTree->postOrder(myTree->getRoot());
    myTree->indexByValueSubtrees();
    myTree->windowMinsforSubtree();
    myTree->nextLowestPowerGenerator();


    //nextNodeOnPath function call
    //The timer was copied from stackoverflow
    //https://stackoverflow.com/questions/25483620/how-to-measure-running-time-of-specific-function-in-c-very-accurate
    //It is only used for evaluation of the program
    typedef std::chrono::high_resolution_clock Clock;
    auto t1 = Clock::now();
    //cout << myTree->nextNodeOnPath(254, 599) << "\n";
    cout << myTree->nextNodeOnPath(3, 2) << "\n";
    auto t2 = Clock::now();
    std::cout <<std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << '\n';
    myTree->display();
    return 0;
}

