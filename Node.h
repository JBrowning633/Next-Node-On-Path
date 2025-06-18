//
// Created by James Browning on 4/12/24.
//
//This header file contains all the get and set declarations and implementations for the node class

//Aside from the regular variables and their functions, there are also variables to track the depth
//of the node in the tree, the position of the node in a pre order traversal, and the first and last 
//positions in which the node appears in a euler tour of the tree.


#ifndef ADSIMPLEMENTATION_NODE_H
#define ADSIMPLEMENTATION_NODE_H

class TreeNode {
public:
    TreeNode(int v):right(nullptr),left(nullptr),parent(nullptr){
        value = v;
    }

    TreeNode * getParent() const{
        return parent;}
    TreeNode * getLeft() const{
        return left;}
    TreeNode * getRight() const{
        return right;}
    int getValue() const{
        return value;}
    int getDepth() const{
        return depth;
    }
    int getpreOrder() const{
        return preOrder;
    }
    int getEulerFirst() const{
        return eulerFirst;
    }
    int getEulerLast() const{
        return eulerLast;
    }
    void setValue(int x){
        value = x;
    }
    void setDepth(int x){
        depth = x;
    }
    void setEulerFirst(int x){
        eulerFirst = x;
    }
    void setEulerLast(int x){
        eulerLast = x;
    }
    void setParent(TreeNode *x){
        parent = x;
    }
    void setLeft(TreeNode *x){
        left = x;
    }
    void setRight(TreeNode *x){
        right = x;
    }
    void setPreOrder(int x){
        preOrder = x;
    }

private:
    int value;
    int depth;
    int preOrder;
    int eulerFirst = -1;
    int eulerLast;
    TreeNode * parent;
    TreeNode * left;
    TreeNode * right;
};

#endif //ADSIMPLEMENTATION_NODE_H
