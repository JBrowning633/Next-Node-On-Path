// Created by James Browning on 4/12/24.

#ifndef ADSIMPLEMENTATION_TREE_H
#define ADSIMPLEMENTATION_TREE_H
#include "Node.h"

using namespace std;

extern vector<int> eTour;
extern vector<int> eTourDepth;
extern vector<int> RMQLCA[];
extern vector<int> eulerFirst;
extern vector<int> eulerLast;
extern vector<int> postOrderSeqPreLabels;
extern vector<int> postOrderSeqValLabels;
extern vector<int> indexByValuePostOrder;
extern vector<int> RMQSubtree[20];
extern vector<int> RMQSubtreeindex[20];
extern vector<int> nextLowestPower;
extern int sizeDepth;
extern int treeSize;
extern int preOrderIterator;

class Tree{

public:
    Tree();
    ~Tree();
    void insert(int val);
    TreeNode* find(int val) const;
    void display() const;
    TreeNode * getRoot(){
        return root;
    }
    void eulerTour();
    void windowMinsforLCA();
    int RMQ(int, int, int);
    int LCA(int, int);
    void preOrder(TreeNode *p);
    void postOrder(TreeNode *p);
    void setEulerFirstLast();
    void eulerFirstLastPreprocess(TreeNode * node);
    int nextNodeOnPath(int, int);
    void indexByValueSubtrees();
    void postOrderSubtrees(TreeNode *node, vector<int> *subTree);
    vector<int> RMQblockGeneratorforSubtree(vector<int>, int, int, int);
    int RMQforSubtree(int, int, int);
    void windowMinsforSubtree();
    int yChildLocator(int, int);
    void nextLowestPowerGenerator();

private:
    TreeNode * root;
    bool insertNode(TreeNode * newNode);
    void display(TreeNode *, int level) const;
    void deleteTree(TreeNode * sel);
    void eulerTour(TreeNode *);
    vector<int> RMQblockGeneratorforLCA(vector<int>, int, int, int);
};

#endif //ADSIMPLEMENTATION_TREE_H
