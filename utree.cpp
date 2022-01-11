/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
    this->clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */ 
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
    insertHelper(newAcct, this->_root);

    if (retrieve(newAcct.getUsername())->getUsername() == newAcct.getUsername()) return true;
    return false;
}

void UTree::insertHelper(Account newAcct, UNode *& node){
    if (!this->_root){ // create a URoot if there isnt one there
        UNode * bruh = new UNode();
        this->_root = bruh;
        bruh->getDTree()->insert(newAcct);
        updateHeight(node);
    }

    if (node){

        if (newAcct.getUsername() < node->getUsername()){ // traverse based on the string
            if (node->_left){   
                insertHelper(newAcct, node->_left);
                updateHeight(node);
                // if (node->_left) node->_left = rebalance(node->_left);
                // if (node->_right) node->_right = rebalance(node->_right);
                node = rebalance(node);
                updateHeight(node);

            }else{
                UNode * bruh = new UNode();
                node->_left = bruh;
                bruh->getDTree()->insert(newAcct);
                updateHeight(node);

        
            }
            
        }
        if (newAcct.getUsername() > node->getUsername()){ // traverse based on the string
            if (node->_right){
                insertHelper(newAcct, node->_right);
                updateHeight(node);
                node = rebalance(node);
                updateHeight(node);

            }else{
                UNode * bruh = new UNode();
                node->_right= bruh;
                bruh->getDTree()->insert(newAcct);
                updateHeight(node);


                
            }
        }

        if (newAcct.getUsername() == node->getUsername()){
            node->getDTree()->insert(newAcct);
            updateHeight(node);
        }
        

    }

    
    updateHeight(node);

    if (node && node == _root) _root = rebalance(_root);
    
    

}

UNode * UTree::left(UNode * a){ // rotates the subtree to the right
    UNode * b = a->_right; // x, y and c are variables that were used in the project doc AVL example
    UNode * c = b->_left;
    b->_left = a;
    a->_right = c;


    updateHeight(b);
    updateHeight(a);
    
    return b;
}

UNode * UTree::right(UNode * a){ // rotates the subtree to the right
    UNode * b = a->_left; // x, y and c are variables that were used in the project doc AVL example
    UNode * c = b->_right;
    
    b->_right = a;
    a->_left = c;

    updateHeight(b);
    updateHeight(a);
    
    

    return b;
}


/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode*& removed) {
    

    bool populated; // this boolean will change how we evaluate whether the DNode or UNode was removed
    if (!retrieve(username)) {// node has to be in the tree to be removed

        return false; 

    }

    if (!retrieveUser(username, disc)) {

        return false;

    }

    if (numUsers(retrieve(username))) populated = true;
    else populated = false;
    


    if (username == this->_root->getUsername()){ // case to check root
        if(numUsers(this->_root)){
            this->_root->getDTree()->remove(disc, removed);
        }else{
            if (isLeaf(_root)){
                delete _root;
                _root = nullptr;
            }
            else{
                cout << "yes" << endl;
                UNode * x = rootSearch(_root); // save the node that will replace the old node
                cout << x->getUsername() << endl;
                UNode * y = _root->_left; // save the left and right of the node to be deleted
                UNode * z = _root->_right;

                x->_left = y; // set the left and right of the node that will replace it
                x->_right = z;
                delete _root; // deallocate old node
                _root = x; // set the current nodes left to the new replacement
                updateHeight(_root);
                updateLeftHeights(_root->_left);
                cout << "no" << endl;
            }            
        }

    }else{
        removeHelper(username, disc, this->_root, removed); // if the root isnt the one we are deleting, then we go on an adventure through the tree to find it
    }

    if (populated){
        if (!retrieveUser(username, disc) || retrieveUser(username, disc)->isVacant()){ //THESE WORK, WHAT WE DONT HAVE IS A VACANCY CHECK IN DTREE
            return true;
        }else{
            return false;
        }
    }else{
        if (retrieve(username)) {
            return false;
        }
    }
    return true;
}

void UTree::removeHelper(string username, int disc, UNode * node, DNode *& removed){
    if (node){
        // check if unode is empty here or not
        if (node->_left){ 
                if (username == node->_left->getUsername()){
                    if(numUsers(node->_left)){
                        node->_left->getDTree()->remove(disc, removed);

                        return;
                    }
                    if (isLeaf(node->_left)){
                        delete node->_left;
                        node->_left = nullptr;
                        updateHeight(node);
                        return;

                    }
                    else if(oneSubtree(node->_left) == 1){ // left of the node to be removed exists but right doesnt
                       
                        UNode * next = node->_left->_left;
                            
                        delete node->_left;

                        node->_left = next;
                        updateHeight(node);

                        return;


                    }
                    else if(oneSubtree(node->_left) == 2){ // right of the node to be removed exists but left doesnt
                        UNode * next = node->_left->_right;
                            
                        delete node->_left;

                        node->_left = next;
                        updateHeight(node);


                        return;
                    }
                    else{
                        UNode * x = rootSearch(node->_left); // save the node that will replace the old node
                        UNode * y = node->_left->_left; // save the left and right of the node to be deleted
                        UNode * z = node->_left->_right;


                        x->_left = y; // set the left and right of the node that will replace it
                        x->_right = z;
                        delete node->_left; // deallocate old node
                        node->_left = x; // set the current nodes left to the new replacement
                        updateHeight(node->_left);
                        updateHeight(node);
                        return;

                    }
                        
                }



        } if (node->_right){ // repeat the check but for right node
            if (username == node->_right->getUsername()){
                if(numUsers(node->_right)){
                    node->_right->getDTree()->remove(disc, removed);
                    return;
                }
                if (isLeaf(node->_right)){
                    delete node->_right;
                    node->_right = nullptr;
                    updateHeight(node);
                    return;

                }
                else if(oneSubtree(node->_right) == 1){ // left of the node to be removed exists but right doesnt
                       
                    UNode * next = node->_right->_left;
                            
                    delete node->_right;

                    node->_right = next;
                    updateHeight(node);

                    
                    return;


                }
                else if(oneSubtree(node->_right) == 2){ // right of the node to be removed exists but left doesnt
                    UNode * next = node->_right->_right;
                            
                    delete node->_right;

                    node->_right = next;
                    updateHeight(node);
                    return;
                }
                else{
                    UNode * x = rootSearch(node->_right); // save the node that will replace the old node
                    UNode * y = node->_right->_left; // save the left and right of the node to be deleted
                    UNode * z = node->_right->_right;


                    x->_left = y; // set the left and right of the node that will replace it
                    x->_right = z;
                    delete node->_right; // deallocate old node
                    node->_right = x; // set the current nodes right to the new replacement
                    updateHeight(node->_right);
                    updateHeight(node);
                    node->_right = rebalance(node->_right);
                    return;

                }
                        
            }

            
        }

        

        removeHelper(username, disc, node->_left, removed);
        removeHelper(username, disc, node->_right, removed);
        return;
    }
}

void UTree::updateLeftHeights(UNode* node){
    if (!node) return;
    updateLeftHeights(node->_left);
    updateLeftHeights(node->_right);
    updateHeight(node);
}

UNode * UTree::rootSearch(UNode * node){
    if(oneSubtree(node)){
        if (node->_right) return node->_right;
        else return node->_left;
    }else{
        UNode * replacementDad = findLowestParent(node); // second lowest node in the subtree
        UNode * replacementRoot = replacementDad->_left;
        replacementDad->_left = nullptr; // we have to disconnect the node from its parent so that its parent wont point back to the root
        return replacementRoot;        
    }

}

UNode * UTree::findLowestParent(UNode * node){
    if (node->_left && !node->_left->_left) return node;
    return findLowestParent(node->_left);
}

bool UTree::isLeaf(UNode* node){
    if (!node->_left && !node->_right) return true;
    return false;
}

int UTree::oneSubtree(UNode * node){
    if (node->_right && !node->_left) return 2;
    if (node->_left && !node->_right) return 1;
    return 0;
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
    return retrieveHelper(username, this->_root);
}

UNode * UTree::retrieveHelper(string username, UNode * node){

    if (node){
        if (node->getUsername() == username){
            return node;
        }else{
            if (username < node->getUsername()) return retrieveHelper(username, node->_left);
            else return retrieveHelper(username, node->_right);
        }
    }else{
        return nullptr;
    }
}



/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
    return retrieveUserHelper(username, disc, this->_root);
}

DNode* UTree::retrieveUserHelper(string username, int disc, UNode * node){
    if (node){
        if (node->getUsername() == username){
            return node->getDTree()->retrieve(disc);
        }else{
            if (username < node->getUsername()) return retrieveUserHelper(username, disc, node->_left);
            else return retrieveUserHelper(username, disc, node->_right);
        }
    }else{
        return nullptr;
    }
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
    return 0; // didnt use this
}

bool UTree::numUsers(UNode * node){
    if(node->getDTree()->getNumUsers() > 0) return true;
    return false;
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
    clearTraverse(this->_root);
}

void UTree::clearTraverse(UNode* node){ // traversal for destructor
    if(node){
        clearTraverse(node->_left);
        clearTraverse(node->_right);
        delete node;
    }
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
    return;
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {

    if (node){
        if (!node->_left && !node->_right){
            node->_height = 0;
            return;
        }
        if (node->_left && node->_right){
            node->_height = max(node->_left->_height, node->_right->_height) + 1;
            return;
        } 
        if(node->_right && !node->_left){
            node->_height = node->_right->_height + 1;
            return;
        }
        if(!node->_right && node->_left){
            node->_height = node->_left->_height + 1;
            return;
        }
    }
    

}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
        if (!node->_left && !node->_right){
            return 0;
        }
        else if(node->_left && !node->_right){
            return node->_left->_height + 1; // balance is left height - right height, so left height - (-1) 
        }
        else if(!node->_left && node->_right){
            return -1 - node->_right->_height;
        }
        else{
            return node->_left->_height - node->_right->_height;
        }
        
        return 0;

}


//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
//void UTree::rebalance(UNode*& node) {
//    return;
// -- OR --
//}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
UNode* UTree::rebalance(UNode* node) {
    int balance = checkImbalance(node);


    if (balance < -1){
        if (checkImbalance(node->_right) > 1){
            node->_right = right(node->_right);
            return left(node);
        }else{
            return left(node);
        }
    }
    if (balance > 1){
        if (checkImbalance(node->_left) < -1){
 
            node->_left = left(node->_left);
            return right(node);
        }else{
            
            return right(node);
        }
    }
    
    return node;
}
//----------------

int UTree::max(int a, int b){
    return (a > b) ? a : b;
}
