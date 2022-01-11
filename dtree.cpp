/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */

#include "dtree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
DTree::~DTree() {
    clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree& DTree::operator=(const DTree& rhs) {
    if (this != &rhs) assignmentRoot(rhs._root);
    return *this;
    
}

/**
 * Dynamically allocates a new DNode in the tree. 
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct) {

    // edge case to see if the disc is already used
    if (retrieveTraverse(newAcct.getDiscriminator(), this->_root) != nullptr) return false; // if it is in the lis


    // testing to see if the newly inserted node's discriminator matches the discriminator passed in 
    insertTraverse(newAcct, this->_root);
    updateSize(this->_root);
    if (checkImbalance(this->_root)) this->_root = rebalance(_root); // rebalance the root if its imbalanced after the inserts

    if (retrieveTraverse(newAcct.getDiscriminator(), this->_root) != nullptr) return true;

    return false;
}



/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode* DTree::retrieve(int disc) {
    return retrieveTraverse(disc, this->_root);
}

// /**
//  * Helper for the destructor to clear dynamic memory.
//  */
void DTree::clear() {
    clearTraverse(this->_root);
}

// /**
//  * Prints all accounts' details within the DTree.
//  */
void DTree::printAccounts() const {
    printTraverse(_root);
}

/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const {
    return validTraverse(_root); 
}



/**
 * Updates the size of a node based on the imedaite children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode* node) {


    if (!node) return;
    node->_size = 1 + countTraverse(node);
    updateNumVacant(node); 
    updateSize(node->_left);
    updateSize(node->_right);
}


// /**
//  * Updates the number of vacant nodes in a node's subtree based on the immediate children
//  * @param node DNode object in which the number of vacant nodes in the subtree will be updated
//  */
void DTree::updateNumVacant(DNode* node) {
    if (!node) return;
    
    
    else if (node->isVacant()) node->_numVacant++;
}

// /**
//  * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
//  * @param checkImbalance DNode object to inspect for an imbalance
//  * @return (can change) returns true if an imbalance occured, false otherwise
//  */
bool DTree::checkImbalance(DNode* node) {
    if (node && node->_left && node->_right){
        
        // For a "Discord" BST, we can define an imbalance to occur when one child's size is 
        // 50% larger than the other child and at least one child's size is 4 or greater.
        //cout << node->_left->_size << " " << node->_right->_size * 2 << endl;
        if((node->_left->_size >= node->_right->_size * 2 && node->_left->_size > 3)
            || (node->_right->_size >= node->_left->_size * 2 && node->_right->_size > 3)){
            return true;
        }else{
            return false;

        }
    }else{
        return false;
    }
}


/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
DNode* DTree::rebalance(DNode* node) { // balances tree based on the rules of a "Discord Tree"

    DNode ** nodes = new DNode * [node->_size];

    int * index = new int(0);
    sortTraverse(nodes, node, index);



    node = rebalanceTraverse(nodes, *index);
    updateSize(node);


    delete index;
    delete [] nodes;
    return node;



}


/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream& operator<<(ostream& sout, const Account& acct) {
    sout << "Account name: " << acct.getUsername() << 
            "\n\tDiscriminator: " << acct.getDiscriminator() <<
            "\n\tNitro: " << acct.hasNitro() << 
            "\n\tBadge: " << acct.getBadge() << 
            "\n\tStatus: " << acct.getStatus();
    return sout;
}

/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool DTree::remove(int disc, DNode*& removed) {
    
    delete removed;
    removed = nullptr;
    
    removeTraverse(disc, this->_root, removed);
    if(removed){
        removed->_vacant = true;
        //updateNumVacant(this->_root);
        delete removed;
        removed = nullptr;
        return true;
    }else{
        return false;
    }
    

}


void DTree::removeTraverse(int disc, DNode* node, DNode*& removed){ // returns a boolean based on whether the specified node was remove or not
    if (node == nullptr) return; // this may not work check back once u run

    if (node->getAccount().getDiscriminator() == disc){
        removed = new DNode(node->_account);
        *removed = *node;

        return;
    }

    if (removed){
        updateNumVacant(node);
    }
    if (disc < node->getDiscriminator())  removeTraverse(disc, node->_left, removed);
    if (disc > node->getDiscriminator())  removeTraverse(disc, node->_right, removed);

    return;

}


void DTree::assignmentRoot(DNode* rhsRoot){ // assigns the root of the tree, before calling the recursive calls on the rest of the tree
    if (rhsRoot == nullptr) return;

    _root = new DNode(rhsRoot->_account);

    _root->_size = rhsRoot->_size;
    _root->_numVacant = rhsRoot->_numVacant;
    assignmentTraverse(_root, _root->_left, rhsRoot->_left, true);
    assignmentTraverse(_root, _root->_right, rhsRoot->_right, false);
}

void DTree::assignmentTraverse(DNode* prev, DNode* node, DNode* rhsNode, bool leftRight){ // assigns the rest of the tree
    if (rhsNode == nullptr) return;

    node = new DNode(rhsNode->_account);
    node->_size = rhsNode->_size;
    node->_numVacant = rhsNode->_numVacant;

    if (leftRight) prev->_left = node;
    else prev->_right = node;


    assignmentTraverse(node, node->_left, rhsNode->_left, true);
    assignmentTraverse(node, node->_right, rhsNode->_right, false);
    
}

int DTree::validTraverse(DNode* node) const{// returns the amount of non vacant nodes

    
    if (!node) {
        return 0;


    }else{
        if (node->isVacant()){
            return validTraverse(node->_left) + validTraverse(node->_right);
        }else{
            int count = 1;
            count += validTraverse(node->_left);
            count += validTraverse(node->_right);
            return count;
        }
    }





}

DNode* DTree::retrieveTraverse(int disc, DNode* node){ //returns a specific node

    if (node){
        if (node->getDiscriminator() == disc){
            return node;
        }else{
            if (disc < node->getDiscriminator()) return retrieveTraverse(disc, node->_left);
            else return retrieveTraverse(disc, node->_right);
        }
    }else{
        return nullptr;
    }
    



}

void DTree::printTraverse(DNode* node) const{ // prints the accounts of the entire tree
    if (!node) return;
    cout << node->_account;
    printTraverse(node->_left);
    printTraverse(node->_right);
}

void DTree::clearTraverse(DNode* node){ // traversal for destructor
    if(node){
        clearTraverse(node->_left);
        clearTraverse(node->_right);
        delete node;
    }
}

int DTree::countTraverse(DNode* node){
    if (!node) return 0;

    int count = 0;
    if (node->_left) count++;
    if (node->_right) count++;

    return count + countTraverse(node->_left) + countTraverse(node->_right);
}

void DTree::insertTraverse(Account newAcct, DNode* node){
    
    //if list is empty we have to make a new one
    if (!this->_root){
        this->_root = new DNode(newAcct);
        return;
    } 

    if (node){
        // theres no need to check is disc == node->_disc because that is checked
        // by the retrieve function call in insert()
        if (newAcct._disc < node->getDiscriminator()){

            if (node->_left != nullptr){
                insertTraverse(newAcct, node->_left); // 2

            }else{
                node->_left = new DNode(newAcct);
                if (node->_left){
                    if (checkImbalance(node->_left)){
                        node->_left = rebalance(node->_left);
                    }
            }

                
                 
            }
        } // NEEDS VACANT CHECK VACANCY CHECK VACANCY CHECK VACANCY CHECK VACANCY CHECK VACANCY CHECK VACANCY CHECK 
        if (newAcct._disc > node->getDiscriminator()){

            if (node->_right != nullptr){
                insertTraverse(newAcct, node->_right); // 1

            }else{
                node->_right = new DNode(newAcct); // 3
                if (node->_right){
                    if (checkImbalance(node->_right)){
                        node->_right = rebalance(node->_right);
                    }
                }
                
            }

        }
                
    }

    




    
}

void DTree::sortTraverse(DNode ** array, DNode* node, int * count){
    if (!node) return;

    //inorder traversal for creating sorted array
    sortTraverse(array, node->_left, count);
    
    if(node->isVacant()){
        sortTraverse(array, node->_right, count);
        delete node;
    }else{
        array[*count] = node; 
        *count += 1;
        sortTraverse(array, node->_right, count);
    }


    

}


DNode* DTree::rebalanceTraverse(DNode ** nodes, int end){
    // this should never run, but its here just in case
    if (end == 0){
        return nullptr;
    }

    DNode* newRoot;

    // define new leaf nodes of the rebalanced tree
    if (end == 1){
        newRoot = nodes[0];
        newRoot->_left = nullptr;
        newRoot->_right = nullptr;

        return newRoot;

    }


    if (end == 2){
        newRoot = nodes[0];
        newRoot->_left = nullptr;
        DNode ** right = new DNode * [1];
        right[0] = nodes[1];
        newRoot->_right = rebalanceTraverse(right, 1);
        delete [] right;
        return newRoot;
    }

    int middle;
    
    if (end % 2 == 0){
         middle = end/2 - 1; // even arrays should favor the left middle node over the right middle node
        
     }else{
         middle = end / 2; // say the length of the array is 11, 11/2 will be 5.5 rounded down to 5 b/c it is an integer
    
    }

    newRoot = nodes[middle]; // middle node of array



    DNode ** left = new DNode * [middle]; // left bisected array
    for (int i = 0; i < middle; i++){// populate left bisected array
        left[i] = nodes[i];
    
    }
    
    int rightEnd = end - middle - 1; // the -1 is to move the length down one to account for the middle node being ignored
    DNode ** right = new DNode * [rightEnd]; // right bisected array
    for (int i = middle + 1; i < end; i++){
        right[i - middle - 1] = nodes[i]; // -1 is here for the same reason as it is in rightEnd
    
    }

    newRoot->_left = rebalanceTraverse(left, middle);
    newRoot->_right = rebalanceTraverse(right, rightEnd);
    delete [] left;
    delete [] right;
    return newRoot;


}
