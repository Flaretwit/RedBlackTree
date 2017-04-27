//Red and Black Trees by Ke Shen

//Members:
/*Struct node
int red -> 1 if the node is colored red, 0 if the node is black
int data -> value of the node
struct node* link[2] - array of 2 pointers (right and left), greatly simplifies rebalancing code
*/

#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>
using namespace std;

//Definition of the struct: node
struct node {
  int red; //1 if red, 0 if black
  int data;
  struct node *link[2]; //link[0] represents left pointer, link[1] right pointer
};

struct tree {
  struct node* root;
};

int is_red(struct node* root);
node* single_r(node* root, int dir);
node* double_r(node* root, int dir);
int assert(node* root);
node* make_node(int data);
int insert(tree* tree, int data);
node* insert_r(node* root, int data);
int getDepth(int level, int number, node* root);
void printTree(node* n, node* root);



int main() {
  tree* t = new tree();
  insert(t, 6);
  insert(t, 7);
  insert(t, 4);
  insert(t, 2);
  insert(t, 1);
  insert(t, 8);
  insert(t, 9);
  insert(t, 5);

  printTree(t->root, t->root);
  return 0;
}

node* insert_r(node* root, int data)
{
  if(root == NULL) {
    root = make_node(data);
  }
  else if(data != root->data)
  {
    //if data greater than root->data, dir = 1 and goes right
    //if data not greater than root, dir = 0 and goes left :D
    int dir = root->data < data;
    root->link[dir] = insert_r(root->link[dir], data);


    /* REBALANCE TREE */

    if(is_red(root->link[dir]))
    {
      if(is_red(root->link[!dir]))
      {
        // Case 1
        root->red = 1;
        root->link[0]->red = 0;
        root->link[1]->red = 0;
      }
      else
      {
        //Case 2 and 3
        if(is_red(root->link[dir]->link[dir]))
        {
          root = single_r(root, !dir);
        }
        else if (is_red(root->link[dir]->link[!dir]))
        {
          root = double_r(root, !dir);
        }
      }
    }

  }
  return root;
}

int insert(struct tree* tree, int data)
{
  tree->root = insert_r(tree->root, data);
  tree->root->red = 0;
  //sets the root to black to avoid a red violation at the root
  return 1;
}

//checks if the node has red color;
int is_red(struct node* root)
{
  return root != NULL && root->red == 1;
}

//conducts a single rotation of the RBT, 1 is right, 0 is left
//returns the new root or top of the 3 nodes
node* single_r(node* root, int dir)
{
  struct node* save = root->link[!dir];
  root->link[!dir] = save->link[dir];
  save->link[dir] = root;

  root->red = 1;
  save->red = 0;

  return save;
}
//rotates twice
node* double_r(node* root, int dir)
{
    root->link[!dir] = single_r(root->link[!dir], dir);

    return single_r(root, dir);
}

//checks for violations in the tree by returning 0
//otherwise, returns the black height of the tree
int assert(struct node* root)
{
  int lh, rh;
  if(root == NULL)
  {
    return 1;
  }
  else
  {
    struct node* ln = root->link[0];
    struct node* rn = root->link[1];

    // Consecutive red links means a bad tree
    if(is_red(root))
    {
      if(is_red(ln) || is_red(rn))
      {
        cout << "Red Violation!" << endl;
        return 0;
      }
    }

    lh = assert(ln);
    rh = assert(rn);

    //checks for invalid Binary Search tree
    if((ln != NULL && ln->data >= root->data) || (rn != NULL && rn->data <= root->data))
    {
      cout << "Binary Search Tree Violation!" << endl;
      return 0;
    }

    //Black height mismatch
    if(lh != 0 && rh != 0 && lh != rh)
    {
      cout << "Black violation!" << endl;
    }

    //Only count black links
    if(lh != 0 && rh != 0)
    {
      return is_red(root) ? lh : lh + 1;
    }
    else
    {
      return 0;
    }
  }
}
//returns a node
//defaults to red node because red violations are easier to fix
node* make_node(int data)
{
  node* rn = new node();

  if(rn != NULL)
  {
    rn->data = data;
    rn->red = 1; /* 1 is red, 0 is black*/
    rn->link[0] = NULL;
    rn->link[1] = NULL;
  }
  return rn;
}


//gets depth of a particular number in the RBT
int getDepth(int level, int number, node* root)
{
  if(root == NULL) {
    return 0;
  }
  else if(root->data == number) {
    return level;
  }
  int downLevel = getDepth(level + 1, number, root->link[0]);
  if(downLevel != 0) {
    return downLevel;
  }
  downLevel = getDepth(level + 1, number, root->link[1]);
  return downLevel;
}
//prints the red and black tree
void printTree(node* n, node* root)
{
     if (n == NULL)
          return;
     /* first recur on right child */
     printTree(n->link[1], root);

     int num = getDepth(0, n->data, root);
     for(int i = 0; i < num; i++) {
       cout << "\t" << flush;
     }
     /* then print the data of the node */
     cout << " " << n->data << "," << flush;
     if(n->red)
     {
       cout << "R" << endl;
     }
     else {
       cout << "B" << endl;
     }

     /* now recur on left child */
     printTree(n->link[0], root);
}
