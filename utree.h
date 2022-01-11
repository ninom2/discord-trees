/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * UserTree.h
 * An interface for the UTree class.
 */

#pragma once

#include "dtree.h"
#include <fstream>
#include <sstream>

#define DEFAULT_HEIGHT 0

class Grader;   /* For grading purposes */
class Tester;   /* Forward declaration for testing class */

class UNode {
    friend class Grader;
    friend class Tester;
    friend class UTree;
public:
    UNode() {
        _dtree = new DTree();
        _height = DEFAULT_HEIGHT;
        _left = nullptr;
        _right = nullptr;
    }

    ~UNode() {
        delete _dtree;
        _dtree = nullptr;
    }

    /* Getters */
    DTree*& getDTree() {return _dtree;}
    int getHeight() const {return _height;}
    string getUsername() const {return _dtree->getUsername();}

private:
    DTree* _dtree;
    int _height;
    UNode* _left;
    UNode* _right;

    /* IMPLEMENT (optional): Additional helper functions */

};

class UTree {
    friend class Grader;
    friend class Tester;

public:
    UTree():_root(nullptr){}

    /* IMPLEMENT: destructor */
    ~UTree();

    /* IMPLEMENT: Basic operations */

    void loadData(string infile, bool append = true);
    bool insert(Account newAcct);
    bool removeUser(string username, int disc, DNode*& removed);
    UNode* retrieve(string username);
    DNode* retrieveUser(string username, int disc);
    int numUsers(string username);
    void clear();
    void printUsers() const;
    void dump() const {dump(_root);}
    void dump(UNode* node) const;


    /* IMPLEMENT: "Helper" functions */
    
    void updateHeight(UNode* node);
    int checkImbalance(UNode* node);
    //----------------
    //void rebalance(UNode*& node);
    // -- OR --
    UNode* rebalance(UNode* node);
    //----------------

private:
    UNode* _root;

    /* IMPLEMENT (optional): any additional helper functions here! */
    bool numUsers(UNode * node);
    int max(int a, int b);
    void insertHelper(Account newAcct, UNode *& node);
    void removeHelper(string username, int disc, UNode * node, DNode *& removed);
    void updateLeftHeights(UNode * node);
    UNode * findLowestParent(UNode * node); // finds the second lowest node from the selected subtree
    bool isLeaf(UNode * node); // checks if node is a leaf or not
    int oneSubtree(UNode * node); // checks if node only has one subtree
    UNode * rootSearch(UNode * node); // searches for the new root of the delete nodes subtree
    UNode * retrieveHelper(string username, UNode * node);
    DNode * retrieveUserHelper(string username, int disc, UNode* node);
    UNode * left(UNode * node);
    UNode * right(UNode * node);
    void clearTraverse(UNode * node);


    //UNode * leftRightHelper(UNode * node);
    //UNode * rightLeftHelper(UNode * node);

};
