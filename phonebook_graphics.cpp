#include <bits/stdc++.h>
#include<windows.h> //for graphics
#include<graphics.h> //for graphics
using namespace std;

// to be used for nodes for coloring
enum Color {red, black};

// class to store info of a single person
class node {
  public :
    // for persons info
    string name, email;
    long long mob_number;

    // for node info
    node *parent, *left, *right;
    bool color;

    // constructor...
    node (string R_name, long long R_mob_number, string R_email);
};

node :: node (string R_name, long long R_mob_number, string R_email) {
    // initialize the basic info
    name = R_name;
    mob_number = R_mob_number;
    email = R_email;
    // all pointers are initialized to NULL
    // color by default is set to red; can change in future
    parent = left = right = NULL;
    color = red;
}

// class to store a tree
class RedBlackTree {
  // these protected functions will only be accessed my the member functions
  protected :
    // removes violations of Red Black tree
    void validateTree (node *&root, node *&newNode);
    // rotates nodes towards left
    void rotateLeft (node *&root, node *&newNode);
    // rotates nodes towards right
    void rotateRight (node *&root, node *&newNode);

  public :
    node *root;
    // constructor...
    RedBlackTree ();
    // to insert the nodes into tree
    void Insert (node *&newNode);
    // after insertion validate function is called
    // to search the number
    node* Search (node *root, string key);
    // to modify the records
    void Modify (string key);
    // to print all records
    void printDirectory(node *root);
    void ClearConsoleToColors(int,int);
    void welcome();
    void thankyou();
    int menu();
};

// constructor...
RedBlackTree :: RedBlackTree () {
    root = NULL;
}

// inserting the elements into RBTree like BST
node* insertLikeBST (node *root, node *newNode) {
    // if root is a null node
    if ( !root ) {
        // so the new node will be root node
        return newNode;
    }
    // if number is greater than root's number
    if (newNode->name > root->name) {
        // move towards right side
        root->right = insertLikeBST (root->right, newNode);
        root->right->parent = root;
    }
    else {
        // else move to left side
        root->left = insertLikeBST (root->left, newNode);
        root->left->parent = root;
    }
    // return root after insertion
    return root;
}

// insertion of node into RBTree
void RedBlackTree :: Insert (node *&newNode) {
    // first normal BSTlike insertion
    root = insertLikeBST (root, newNode);
    // then we fix violations of red black tree
    validateTree (root, newNode);
}

// fighting violations caused by insertion
void RedBlackTree :: validateTree (node *&root, node *&newNode) {
    // these two are dummy nodes
    node *parent_D = NULL;
    node *grandparent_D = NULL;

    while ((newNode != root) && (newNode->color != black) && (newNode->parent->color == red)) {
        // assign dummy nodes to actual parent and grandparent
        parent_D = newNode->parent;
        grandparent_D = newNode->parent->parent;
        /* Case 1 :
           parent of newNode is left child
           of grandparent of newNode     */
        if (parent_D == grandparent_D->left) {
            /* initialize a dummy uncle of newNode
               to right child of grandparent    */
            node *uncle_D = grandparent_D->right;
            /* case 1.1 :
               uncle is red
               so only coloring is required    */
            if ((uncle_D != NULL) && (uncle_D->color == red)) {
                grandparent_D->color = red;
                parent_D->color = black;
                uncle_D->color = black;
                newNode = grandparent_D;
            }
            else {
                /* case 1.2 :
                   newNode is right child of parent
                   so left rotation is required    */
                if (newNode == parent_D->right) {
                    rotateLeft (root, parent_D);
                    newNode = parent_D;
                    parent_D = newNode->parent;
                }
                /* case 1.3 :
                   newNode id left child of its parent
                   so right rotation is required    */
                rotateRight (root, grandparent_D);
                swap (parent_D->color, grandparent_D->color);
                newNode = parent_D;
            }
        }
        /* Case 2 :
           parent of newNode is right child
           of grandparent of newNode    */
        else {
            /* initialize a dummy uncle of newNode
               to left child of grandparent    */
            node *uncle_D = grandparent_D->left;
            /* case 2.1 :
               uncle is red
               so only coloring is required    */
            if ((uncle_D != NULL) && (uncle_D->color == red)) {
                grandparent_D->color = red;
                parent_D->color = black;
                uncle_D->color = black;
                newNode = grandparent_D;
            }
            else {
                /* case 2.2 :
                   newNode is left child of parent
                   so only right rotation is required    */
                if (newNode == parent_D->left) {
                    rotateRight (root, parent_D);
                    newNode = parent_D;
                    parent_D = newNode->parent;
                }
                /* case 2.3 :
                   newNode is right child of parent
                   so left rotation is required    */
                rotateLeft (root, grandparent_D);
                swap (parent_D->color, grandparent_D->color);
                newNode = parent_D;
            }
        }
    }
    // since root is always black, so we......
    root->color = black;
}

// left rotation of nodes
void RedBlackTree :: rotateLeft (node *&root, node *&newNode) {
    // initialize a dummy node right_D
    node *right_D = newNode->right;
    newNode->right = right_D->left;
    // if right child of newNode is not NULL
    if (newNode->right != NULL) {
        newNode->right->parent = newNode;
    }
    right_D->parent = newNode->parent;
    // if parents of newNode is NULL
    if (newNode->parent == NULL) {
        root = right_D;
    }
    else if (newNode == newNode->parent->left) {
        newNode->parent->left = right_D;
    }
    else {
        newNode->parent->right = right_D;
    }
    right_D->left = newNode;
    newNode->parent = right_D;
}

// right rotation of nodes
void RedBlackTree :: rotateRight (node *&root, node *&newNode) {
    // initialize a dummy node left_D
    node *left_D = newNode->left;
    newNode->left = left_D->right;
    // if left child of newNode is not NULL
    if (newNode->left != NULL) {
        newNode->left->parent = newNode;
    }
    left_D->parent = newNode->parent;
    // if parents of newNode is NULL
    if (newNode->parent == NULL) {
        root = left_D;
    }
    else if (newNode == newNode->parent->left) {
        newNode->parent->left = left_D;
    }
    else {
        newNode->parent->right = left_D;
    }
    left_D->right = newNode;
    newNode->parent = left_D;
}

// searches the number and returns the node
node* RedBlackTree :: Search (node *root, string key) {
    // implemented recurcively
    // searching in RBTree is like BST
    if ((root == NULL) || (root->name == key)) {
        return root;
    }
    else if (root->name > key) {
        return (Search(root->left, key));
    }
    else {
        return (Search(root->right, key));
    }
    return NULL;
}

// to modify records
void RedBlackTree :: Modify (string key) {
    long long n;  // to store new number
    string e;
    // a dummy node to store search results
    node *dummy = Search (root, key);
    if ( !dummy ) {
        ClearConsoleToColors(4,0);
        // cout << "\n\t\t\t\t\t\t\t\tAlert!!!"
        cout<<"\n \t\t\t\t\t\t\t\tRecord not Found!!!" << endl;
    }
    else {
        ClearConsoleToColors(2,0);
        cout << "\n\t\t\t\t\t\t\t\tRecord Found!!!"<<endl;
        ClearConsoleToColors(14,0);
        cout << "\n\t\t\t\t\t\t\t\tEnter the new number : ";
        cin >> n;
        dummy->mob_number = n;
        cout << "\t\t\t\t\t\t\t\tEnter the new email : ";
        cin >> e;
        dummy->email = e;
        ClearConsoleToColors(1,0);
        cout << "\n\t\t\t\t\t\t\t\tAfter modifying : ";
        cout << "\n\t\t\t\t\t\t\t\tNumber : " << dummy->mob_number;
        cout << "\n\t\t\t\t\t\t\t\tEmail : " << dummy->email << endl;
    }
}

// to print whole telephone directory
void RedBlackTree :: printDirectory(node *r) {
    if (r) {
        ClearConsoleToColors(3,0);
        printDirectory (r->left);
        cout << "\n\t\t\t\t\t\t\t" << r->name << "\t\t" << r->mob_number << "\t\t" << r->email;
        printDirectory (r->right);
    }
}

void RedBlackTree::welcome(){
    ClearConsoleToColors(0,15);
    string s1 = "WELCOME TO PHONE BOOK APPLICATION SYSTEM   !!";
    cout<<"\n\t\t\t\t\t\t\t!!     ";
    for(int i = 0; i<s1.length();i++){
        cout<<s1.at(i);
        Sleep(200);
    }
}

void RedBlackTree::ClearConsoleToColors(int ForgC, int BackC){
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SetConsoleTextAttribute(hStdOut, wColor);
    return;
}

void RedBlackTree::thankyou(){
    ClearConsoleToColors(0,15);
    string s1 = "THANKS FOR USING PHONE BOOK APPLICATION";
    cout<<"\n\t\t\t\t\t!!      ";
    for(int i = 0; i<s1.length();i++){
        cout<<s1.at(i);
        Sleep(200);
    }
    cout<<"    !!";
    ClearConsoleToColors(7,0);
}

int RedBlackTree::menu () {
    ClearConsoleToColors(13,0);
    // ClearConsoleToColors(14,8);
    int x;
    cout<<"\n\n";
	cout<<"\t\t\t\t\t\t\t--------------------------------------------"<<endl;
	cout<<"\t\t\t\t\t\t\t--------------------------------------------"<<endl;
	cout<<"\t\t\t\t\t\t\t|          PHONE BOOK APPLICATION          |"<<endl;
	cout<<"\t\t\t\t\t\t\t--------------------------------------------"<<endl;
	cout<<"\t\t\t\t\t\t\t|                                          |"<<endl;
	cout<<"\t\t\t\t\t\t\t|       [1]  Insert entries                |"<<endl;
	cout<<"\t\t\t\t\t\t\t|       [2]  Search by Name                |"<<endl;
	cout<<"\t\t\t\t\t\t\t|       [3]  Update entries                |"<<endl;
	cout<<"\t\t\t\t\t\t\t|       [4]  Print Directory               |"<<endl;
	cout<<"\t\t\t\t\t\t\t|                                          |"<<endl;
	cout<<"\t\t\t\t\t\t\t--------------------------------------------"<<endl;
	cout<<"\t\t\t\t\t\t\t|              [5]  Exit                   |"<<endl;
	cout<<"\t\t\t\t\t\t\t--------------------------------------------"<<endl;
	ClearConsoleToColors(15,0);
	cout<<"\n\t\t\t\t\t\t\tPlease Enter your Choice : ";
    cin >> x;
    return x;
}

int main () {
    // creating an object of class
    RedBlackTree obj;
    obj.welcome();
    // to take entries of names and numbers
    string name, email;
    long long number;
    node *dummy, *searchResult;

    // the loop goes here
    while (1) {
        switch (obj.menu()) {
            case 1 :
                obj.ClearConsoleToColors(3,0);
                cout << "\n\t\t\t\t\t\t\t\tEnter the Name   : ";
                cin>>name;
                cout << "\t\t\t\t\t\t\t\tEnter the Number  : ";
                cin >> number;
                cout << "\t\t\t\t\t\t\t\tEnter the Email   : ";
                cin >> email;
                // creating a node
                dummy = new node(name, number, email);
                obj.Insert (dummy);
                // node is inserted
                break;
            case 2 :
                obj.ClearConsoleToColors(3,0);
                cout << "\n\t\t\t\t\t\t\t\tEnter the name to search : ";
                cin >> name;
                // reusing the variables
                searchResult = obj.Search (obj.root, name);
                // search is successful is searchResult is not NULL
                if ( !searchResult ) {
                    obj.ClearConsoleToColors(4,0);
                    // cout << "\n\t\t\t\t\t\t\t\tAlert!!!"
                            // "\n\t\t\t\t\t\t\t\tNo Record found." << endl;
                    cout<<"\n \t\t\t\t\t\t\t\tRecord not Found!!!" << endl;
                }
                else {
                    obj.ClearConsoleToColors(2,0);
                    cout << "\n\t\t\t\t\t\t\t\tRecord Found!!!"<<endl;
                    obj.ClearConsoleToColors(1,0);
                    cout << "\n\t\t\t\t\t\t\t\tName : " << searchResult->name;
                    cout << "\n\t\t\t\t\t\t\t\tNumber : " << searchResult->mob_number;
                    cout << "\n\t\t\t\t\t\t\t\tEmail : " << searchResult->email << endl;
                }
                break;
            case 3 :
                obj.ClearConsoleToColors(3,0);
                cout << "\n\t\t\t\t\t\t\t\tEnter the name : ";
                cin >> name;
                obj.Modify(name);
                break;
            case 4 :
                if (obj.root) {
                    obj.ClearConsoleToColors(1,0);
                    cout << "\n\t\t\n\t\t\t\t\t\t\tName\t\tNumber\t\tEmail" << endl;
                    obj.printDirectory(obj.root);
                }
                else {
                    obj.ClearConsoleToColors(4,0);
                    cout<<"\n \t\t\t\t\t\t\t\tRecord not Found!!!" << endl;
                    // cout << "\t\t\t\t\t\t\t\t\t\tAlert!!"
                    //         "\n\t\t\t\t\t\t\t\t\t\tThe directory is empty!!!" << endl;
                }
                break;
            case 5 :
                // obj.ClearConsoleToColors(5,7);
                // cout << "\n*******THANKS FOR USING PHONE BOOK APPLICATION*******" << endl;
               obj.ClearConsoleToColors(7,0);
                 obj.thankyou();
                exit(0);
            default :
            obj.ClearConsoleToColors(4,0);
                cout << "\n\t\t\t\t\t\t\t\tBad Choice!!"
                        "\n\t\t\t\t\t\t\t\tPlz Try again!!!" << endl;
        }
    }
    return 0;
}
