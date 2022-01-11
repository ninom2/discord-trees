#include "utree.h"
#include <random>
#include <string>

#define NUMACCTS 20
#define RANDDISC (distAcct(rng))

std::mt19937 rng(10);
std::uniform_int_distribution<> distAcct(0, 9999);

class Tester {
public:
    bool testBasicDTreeInsert(DTree& dtree);
    bool dtreeAssignmentOperator(DNode * node, DNode * nodeCopy);
    bool dtreeAssignmentInsert(const DTree &rhs, string);
    bool dtreeRemove(DTree& dtree);
    DNode* getRoot(DTree &dtree);
    bool dtreeInsertRemoveRebalance(DTree& dtree);
    void dtreeInsertPerformance(int numTrials, int N);
    bool dtreeInsertRetrieve(DTree &dtree);
    bool rebalanceTest();
    bool dtreeGetNumUsers();

    // notes:
    // test for a username used twice?
    // check for an empty assignment operator
    // test get num users
    // test rebalance?? not sure how this would be tested honestly

    bool testBasicUTreeInsert(UTree& utree);
    bool utreeInsert(UTree& utree);
    bool utreeEmptyRemove();
    bool utreeRemoveUser(UTree & tree, string username, int disc);
    void utreeInsertPerformance(int numTrials, int N);
    
    
};



bool Tester::utreeEmptyRemove(){
    UTree utree;
    DNode * removed = nullptr;
    if (!utree.removeUser("username", RANDDISC, removed)) return true;
    return false;
}

bool Tester::dtreeGetNumUsers(){
    DTree dtree;
    int accounts = 5;
    for (int i = 0; i < accounts; i++){
        dtree.insert(Account("nino", 1000 + i, true, "", ""));
    }
    if (dtree.getNumUsers() == accounts) return true;
    return false;
}

bool Tester::rebalanceTest(){
    DTree dtree;
    int accounts = 5;
    for (int i = 0; i < accounts; i++){
        dtree.insert(Account("nino", 1000 + i, true, "", ""));
    }
    if (dtree.checkImbalance(dtree._root)) return false; // this checks if the tree was actually rebalanced or not
    else return true;
    return false;
}

bool Tester::dtreeInsertRetrieve(DTree &dtree){
    int disc = RANDDISC;
    dtree.insert(Account("nino", disc, true, "", ""));
    if (dtree.retrieve(disc)->getDiscriminator() == disc) return true;
    return false;
    
}

void Tester::dtreeInsertPerformance(int numTrials, int n){
    double T = 0.0;
    const int a = 2; // factor of increase every trial
    clock_t start, stop;
    DTree tree;
    for (int k = 0; k < numTrials; k++){
        start = clock();
        for (int i = 0; i < n; i++){//the algorithm to be analyzed for efficiency
            tree.insert(Account("nino", RANDDISC, true, "", ""));
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Inserting " << n << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        n = n * a;
    }
}



bool Tester::dtreeAssignmentInsert(const DTree &rhs, string username){
    // this function checks to make sure that inserting into the copy tree will not also insert into the copied tree
    DTree copy;
    copy = rhs;
    int disc = RANDDISC;

    Account person = Account(username, disc, true, "", "");
    copy.insert(person);

    if (copy._root->getSize() == rhs._root->getSize()) return false;
    return true;
}

bool Tester::testBasicDTreeInsert(DTree& dtree) {
    bool allInserted = true;
    for(int i = 0; i < NUMACCTS; i++) {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if(!dtree.insert(newAcct)) {
            cout << "Insertion on node " << disc << " did not return true" << endl;
            allInserted = false;
        }
    }
    return allInserted;
}

bool Tester::dtreeRemove(DTree& dtree){
    dtree.insert(Account("nino", 1234, true, "",""));
    DNode* bruh = nullptr;
    if (dtree.remove(1234, bruh)) return true;
    return false;
}

bool Tester::dtreeInsertRemoveRebalance(DTree& dtree){
        int disc = RANDDISC;
        string username = "nino";
        for (int i = 0; i < 10; i++){
            dtree.insert(Account(username, RANDDISC, false, "", "")); // populate tree
        }
        dtree.insert(Account(username, disc, false, "", "")); //this is the node were gonna work with

        DNode * removed = nullptr;
        dtree.remove(disc, removed);

        for (int i = 0; i < 10; i++){
            dtree.insert(Account(username, 9000 + i, false, "", "")); // populate tree
        }
 

        if(!dtree.retrieve(disc)) return true;
        return false;
        
}


DNode* Tester::getRoot(DTree & dtree){
    return dtree._root;
}

bool Tester::dtreeAssignmentOperator(DNode* node, DNode* nodeCopy){
    if (!node && !nodeCopy) return true;
    if (!node || !nodeCopy) return false;

    // checking for shallow copies
    if (node == nodeCopy) return false;
    
    if (node->getDiscriminator() != nodeCopy->getDiscriminator()) return false;
    if (node->isVacant() != nodeCopy->isVacant()) return false;


    if (!dtreeAssignmentOperator(node->_left, nodeCopy->_left)) return false;

    if (!dtreeAssignmentOperator(node->_left, nodeCopy->_left)) return false;


    return true;
}


bool Tester::utreeInsert(UTree & tree){
    Account temp = Account("bruh", RANDDISC, false, "","");
    if (tree.insert(temp)) return true;
    return false;

}

bool Tester::utreeRemoveUser(UTree & tree, string username, int disc){
    DNode * useless = nullptr;
    
    if (tree.removeUser(username, disc, useless)) return true;
    else return false;
}

bool Tester::testBasicUTreeInsert(UTree& utree) {
    string dataFile = "accounts.csv";
    try {
        utree.loadData(dataFile);

    } catch(std::invalid_argument &e) {
        std::cerr << e.what() << endl;
        return false;
    }
    return true;
}


void Tester::utreeInsertPerformance(int numTrials, int n){
    double T = 0.0;
    const int a = 2; // factor of increase every trial
    clock_t start, stop;
    UTree tree;
    for (int k = 0; k < numTrials; k++){
        start = clock();
        for (int i = 0; i < n; i++){//the algorithm to be analyzed for efficiency
            tree.insert(Account(std::to_string(RANDDISC), RANDDISC, true, "", ""));
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Inserting " << n << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        n = n * a;
    }
}

int main() {
    Tester tester;

    {
        UTree utree;
        cout << "\nUTree: Testing Basic UTree Insert\n";
        if (tester.testBasicUTreeInsert(utree)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";

    }
    {
        cout << "\nUTree: Testing Removal from Empty Utree\n";
        if (tester.utreeEmptyRemove()) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
    }
    {
        UTree utree;
        cout << "\nUTree: Testing Insert and Retrieve User\n";
        if (tester.utreeInsert(utree)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
    }
    {
        UTree utree;
        utree.insert(Account("bbbb", RANDDISC, false,"", ""));
        utree.insert(Account("aaa", 1234, false,"", ""));
        utree.insert(Account("ccc", RANDDISC, false,"", ""));
        utree.insert(Account("cc", RANDDISC, false,"", ""));
        utree.insert(Account("a", RANDDISC, false, "", ""));
        utree.insert(Account("cccc", RANDDISC, false, "", ""));
        utree.insert(Account("aaaa", RANDDISC, false,"", ""));
        utree.insert(Account("1", RANDDISC, false,"", ""));
        utree.insert(Account("ba", 1222, false,"", ""));


        cout << "\nUTree: Testing Remove User\n";
        if (tester.utreeRemoveUser(utree, "ba", 1222)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
        
        cout << "\nUTree: Testing Remove User\n";
        if (tester.utreeRemoveUser(utree, "aaa", 1234)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";

    }
    {

    }
    {
        //Measuring the efficiency of insertion functionality
        cout << "\nUTree: Measuring the efficiency of insertion functionality:\n" << endl;
        int M = 5;//number of trials
        int N = 10;//original input size
        tester.utreeInsertPerformance(M, N);
        cout << "\n";

    }



    {
        DTree tree;
        string username = "nino";
        for (int i = 0; i < 10; i++){
            tree.insert(Account(username, RANDDISC, true, "", ""));
        }
        DTree copy;
        copy = tree;
        cout << "\nDTree: Testing Assignment Operator\n";
        if (tester.dtreeAssignmentOperator(tester.getRoot(tree), tester.getRoot(copy))) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n"; 

    }
    {
        DTree tree;
        cout << "\nDTree: Testing Basic Insert\n";
        if(tester.testBasicDTreeInsert(tree)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
    }
    {
        DTree tree;
        string username = "nino";
        for (int i = 0; i < 10; i++){
            tree.insert(Account(username, RANDDISC, false, "", ""));
        }
        cout << "\nDTree: Testing Insert with Assignment Operator\n";
        if (tester.dtreeAssignmentInsert(tree, username)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
        
    }
    {
        DTree tree;
        cout << "\nDTree: Testing Basic Remove" << endl;
        if (tester.dtreeRemove(tree)) cout << "\tTest Passed" << endl;
        else cout << "\tTest Failed\n";
        
    }
    {
        DTree tree;
        cout << "\nDTree: Testing Insert, Removal, Then Rebalancing to Test that the Vacant Node is Deleted\n";
;
        if (tester.dtreeInsertRemoveRebalance(tree)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";

    }
    {
        DTree tree;
        cout << "\nDTree: Testing Insert and Retrieve functionality" << endl;
        if (tester.dtreeInsertRetrieve(tree)) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
    }
    {
        cout << "\nDTree: Testing Rebalance Functionality\n";
        if (tester.rebalanceTest()) cout << "\tTest Passed\n";
        else cout << "\tTest Failed\n";
    }
    {
        cout << "\nDTree: Testing GetNumUsers\n";
        if (tester.dtreeGetNumUsers()) cout << "\tTest Passed\n" << endl;
        else cout << "\tTest Failed\n" << endl;
    }
    {
        //Measuring the efficiency of insertion functionality
        cout << "\nDTree: Measuring the efficiency of insertion functionality:\n" << endl;
        int M = 5;//number of trials
        int N = 10;//original input size
        tester.dtreeInsertPerformance(M, N);
        cout << "\n";

    }


}
