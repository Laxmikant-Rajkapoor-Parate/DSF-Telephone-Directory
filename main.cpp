#ifndef __LCC__
#define __LCC__
#endif

#include <mysql.h>
#include<windows.h>
#include <bits/stdc++.h>
#include<cstring>
using namespace std;



char hostname[20]="localhost";

//To implement database on your machine
//change the following attributes accordingly
char username[20]="root";
char password[20]="Siddhartha751";
char database[20]="testdb";
//----------------------------------------




// to be used for nodes for coloring
enum Color {red, black};

// class to store info of a single person
class node {
  public :
    // for persons info
    string name, email;
    string mob_number;

    // for node info
    node *parent, *left, *right;
    bool color;

    // constructor...
    node (string R_name, string R_mob_number, string R_email);
    node* sibling();
    bool isOnLeft();
    bool hasRedChild();
};


node :: node (string R_name, string R_mob_number, string R_email) {
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
    // to delete a record
    void Delete (node *v);
    void fixDoubleBlack (node* v);
    // to print all records
    void printDirectory(node *root);

void SQL_push(node* root);
void finish_with_error(MYSQL *con);
void database_push_SQL();
void database_pull_SQL();
};

void RedBlackTree::database_pull_SQL()
{

MYSQL *conn=mysql_init(0);

if (mysql_real_connect(conn,hostname,username,password,database, 0, NULL, 0))
{cout<<"Connecting To Database....\nGetting Data from SQL\n\n";}

if (mysql_query(conn, "SELECT * FROM phonebook"))
  {}

MYSQL_RES *result = mysql_store_result(conn);
if(result==NULL)
{
cout<<"SQL Database is Empty\n\n\n";
return;
}
int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
char index[1][num_fields];
  while ((row = mysql_fetch_row(result)))
  {
node *dummy = new node(row[0],row[1],row[2]);
Insert (dummy);//Inserts Data from SQL table into Tree
}
}












void RedBlackTree::database_push_SQL()
{
MYSQL *conn=mysql_init(0);
if (mysql_real_connect(conn,hostname,username,password,database, 0, NULL, 0))
{cout<<"Connecting To Database....\n";}
if(root!=NULL){
if (mysql_query(conn, "DROP TABLE IF EXISTS phonebook")) { //This deletes the table
finish_with_error(conn);                               //So the new data can be entered
  }}
mysql_close(conn);
SQL_push(root);
}




void RedBlackTree::finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(0);
}

void RedBlackTree :: SQL_push(node *r) {
    if (r) {

MYSQL *conn=mysql_init(0);

 if (mysql_real_connect(conn, "localhost", "root", "Siddhartha751","testdb", 0, NULL, 0))

{}

if (mysql_query(conn, "create table phonebook(name varchar(100),mob varchar(100),email varchar(100));"))
  {}




//-----------------------------------------------
string SQL1="insert into phonebook values('";
string SQL2(r->name);
string SQL3("','");
string SQL4(r->mob_number);
string SQL5(r->email);
string SQL6("');");

SQL1.append(SQL2);
SQL1.append(SQL3);
SQL1.append(SQL4);
SQL1.append(SQL3);
SQL1.append(SQL5);
SQL1.append(SQL6);
int numSQL=SQL1.size();
char SQLary[numSQL+1];


for (int i = 0; i < numSQL; i++)
{SQLary[i] = SQL1[i];} //Converts String to Char Array

//-----------------------------------------------



SQL_push(r->left);

//mysql_
if(mysql_query(conn,SQLary))
{finish_with_error(conn);}
SQL_push(r->right);


mysql_close(conn);
}
}


















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
    string n;  // to store new number
    string e;
    // a dummy node to store search results
    node *dummy = Search (root, key);
    if ( !dummy ) {
        cout << "\n  Alert!!!"
                "\n  No Element found." << endl;
    }
    else {
        cout << "\n  Record found!!!";
        cout << "\n  Enter the new number : ";
        cin >> n;
        dummy->mob_number = n;
        cout << "\n  Enter the new email : ";
        cin >> e;
        dummy->email = e;
        cout << "\n  After modifying : ";
        cout << "\n  Name : " << dummy->name;
        cout << "\n  Number : " << dummy->mob_number;
        cout << "\n  Email : " << dummy->email << endl;
    }
}

bool node :: hasRedChild() {
    return (left != NULL and left->color == red) || (right != NULL and right->color == red);
}

bool node :: isOnLeft() {
    return this == parent->left;
}

node* node :: sibling() {
    // sibling null if no parent
    if (parent == NULL)
      return NULL;

    if (isOnLeft())
      return parent->right;

    return parent->left;
  }

node *successor(node *x) {
    node *temp = x;

    while (temp->left != NULL)
      temp = temp->left;

    return temp;
  }

node *BSTreplace(node *x) {
    // when node have 2 children
    if (x->left != NULL and x->right != NULL)
      return successor(x->right);

    // when leaf
    if (x->left == NULL and x->right == NULL)
      return NULL;

    // when single child
    if (x->left != NULL)
      return x->left;
    else
      return x->right;
  }

void RedBlackTree :: fixDoubleBlack(node *x) {
    if (x == root)
      // Reached root
      return;

    node *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
      // No sibiling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == red) {
        // Sibling red
        parent->color = red;
        sibling->color = black;
        if (sibling->isOnLeft()) {
          // left case
          rotateRight(root, parent);
        } else {
          // right case
          rotateLeft(root, parent);
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == red) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rotateRight(root, parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rotateRight(root, sibling);
              rotateLeft(root, parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              rotateLeft(root, sibling);
              rotateRight(root, parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              rotateLeft(root, parent);
            }
          }
          parent->color = black;
        } else {
          // 2 black children
          sibling->color = red;
          if (parent->color == black)
            fixDoubleBlack(parent);
          else
            parent->color = black;
        }
      }
    }
  }

// to delete the record
void RedBlackTree :: Delete (node *v) {
    node *u = BSTreplace(v);

    // True when u and v are both black
    bool uvBlack = ((u == NULL or u->color == black) and (v->color == black));
    node *parent = v->parent;

    if (u == NULL) {
      // u is NULL therefore v is leaf
      if (v == root) {
        // v is root, making root null
        root = NULL;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibling() != NULL)
            // sibling is not null, make it red"
            v->sibling()->color = red;
        }

        // delete v from the tree
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }

    if (v->left == NULL or v->right == NULL) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the value of u to v, and delete u
        v->name = u->name;
        v->left = v->right = NULL;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = black;
        }
      }
      return;
    }

    // v has 2 children, swap values with successor and recurse
    swap (u, v);
    Delete(u);
}

// to print whole telephone directory
void RedBlackTree :: printDirectory(node *r) {
    if (r) {
        printDirectory (r->left);
        cout << "\n\t" << r->name << "\t\t" << r->mob_number << "\t\t" << r->email;
        printDirectory (r->right);
    }
}

int menu () {
    int x;
    cout << "\n^________MENU__________^"
            "\n|  1. Insert entries.  |"
            "\n|  2. Search entries.  |"
            "\n|  3. Modify entries.  |"
            "\n|  4. Print directory. |"
            "\n|  5. Delete entries.  |"
            "\n|  6. Exit.            |"
            "\n ~~~~~~~~~~~~~~~~~~~~~~"
            "\n      Choice : ";
    cin >> x;
    return x;
}

int main () {
    // creating an object of class
    RedBlackTree obj;
    // to take entries of names and numbers
    string name, email;
    string number;
    node *dummy, *searchResult, *random;
obj.database_pull_SQL();
    // the loop goes here
    while (1) {
        switch (menu()) {
            case 1 :
                cout << "\n  Enter the Name : ";
                cin >> name;
                cout << "  Enter the Number : ";
                cin >> number;
                cout << "  Enter the Email : ";
                cin >> email;
                // creating a node
                dummy = new node(name, number, email);
                obj.Insert (dummy);
                // node is inserted
                break;
            case 2 :
                cout << "\n  Enter the number to search : ";
                cin >> name;
                // reusing the variables
                searchResult = obj.Search (obj.root, name);
                // search is successful is searchResult is not NULL
                if ( !searchResult ) {
                    cout << "\n  Alert!!!"
                            "\n  No Element found." << endl;
                }
                else {
                    cout << "\n  Record found!!!";
                    cout << "\n  Name : " << searchResult->name;
                    cout << "\n  Number : " << searchResult->mob_number;
                    cout << "\n  Email : " << searchResult->email << endl;
                }
                break;
            case 3 :
                cout << "\n  Enter the name : ";
                cin >> name;
                obj.Modify(name);
                break;
            case 4 :
                if (obj.root) {
                    cout << "\n\tName\t\tNumber\t\tEmail" << endl;
                    obj.printDirectory(obj.root);
                }
                else {
                    cout << "\n  Alert!!"
                            "\n  The directory is empty!!!" << endl;
                }
                break;
            case 5 :
                cout << "\n  Enter the name to delete : ";
                cin >> name;
                random = obj.Search (obj.root, name);
                if ( !random ) {
                    cout << "\n  Alert!!!"
                            "\n  No Element found." << endl;
                }
                else {
                    cout << "\n  Record found!!!";
                    obj.Delete(random);
                    cout << "\n  Entry Deleted!!\n";
                }
                break;
            case 6 :
cout << "\n____THANKS FOR USING____" << endl;
obj.database_push_SQL();

                exit(0);
    default :
                cout << "\n  Bad Choice!!"
                        "\n  Plz Try again!!!" << endl;
        }
    }
    return 0;
}
