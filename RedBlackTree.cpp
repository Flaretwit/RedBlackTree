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

int is_red(struct node* root);

struct node {
  int red;
  int data;
  struct node *link[2];
};

struct tree {
  struct node* root;
};

int is_red(struct node* root)
{
  return root != NULL && root->red == 1;
}

//conducts a single rotation of the RBT, 1 is right, 0 is left
//returns the new root or top of the 3 nodes
struct node* single(struct node* root, int dir)
{
  struct node* save = root->link[!dir];
  root->link[!dir] = save->link[dir];
  save->link[dir] = root;

  root->red = 1;
  save->red = 0;

  return save;
}
//rotates twice
struct node* double(struct node* root, int dir)
{
    root->link[!dir] = single(root->link[!dir], dir);

    return single(root, dir);
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

    // Consecutive red links
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
