//Red and Black Trees by Ke Shen
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

#define ADD 1
#define READ 2
#define PRINT 3
#define EXIT 4
#define SEARCH 5
#define DELETE 6

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
int parseCommand(char *input);
void split(string str, char delim, vector<int>*& result);
int search(node* root, int data);

int main() {
  tree* t = new tree();

  char input[80];
  bool done = false;
  while(!done) {
    cout << "ADD, READ, OR PRINT, SEARCH, DELETE OR EXIT?" << endl;
    cin >> input;
    switch(parseCommand(input)) {
      case ADD: {
        char temp[80];
        cout << "Enter an integer: " << flush;
        cin >> temp;
        insert(t, atoi(temp));
        break;
      }
      case PRINT:
        printTree(t->root, t->root);
        break;
      case READ: {
        char* contents = new char [1000];
        char filename[80];
        vector<int> *result = new vector<int>;
        cout << "Enter in the filename (with suffix): " << endl;
        cin >> filename;
        ifstream infile(filename);
        //if unable to open the file
        if(!infile) {
            cout << "Unable to open the file ... exiting" << flush;
            return -1;
        }
        infile.getline(contents, 1000, '\n');
        split(contents, ',', result);
        for(int i = 0; i < result->size(); i++) {
          insert(t, result->at(i));
        }
        break;
      }
      case SEARCH:
        char temp[80];
        cout << "What number do you want to search for? " << flush;
        cin >> temp;
        if(!search(t->root, atoi(temp))) {
          cout << "Number not found." << endl;
        }
        else {
          cout << "Number found." << endl;
        }
        break;
      case EXIT:
        done = true;
        break;
    }
  }
  return 0;
}

node* delete_r(node* root, int data, int *done)
{
    if (root == NULL)
    {
      *done = 1;
    }
    else
    {
      int dir;

      if(root->data == data)
      {
        if(root->link[0] == NULL || root->link[1] == NULL)
        {
          node* save = root->link[root->link[0] == NULL];

          /* Case 0 */
          if(is_red(root))
          {
            *done = 1;
          }
          else if(is_red(save))
          {
            save->red = 0;
            *done = 1;
          }

          free(root);
          return save;
        }
        else
        {
          node* heir = root->link[0];
          while(heir->link[1] != NULL)
          {
            heir = heir->link[1];
          }

          root->data = heir->data;
          data = heir->data;
        }
      }

      dir = root->data < data;
      root->link[dir] = remove_r(root->link[dir], data, done);

      if(!*done)
      {
        root = remove_balance(root, dir, done);
      }
    }

    return root;
}


node* remove_balance(node* root, int dir, int *done)
{
    node* p = root;
    node* s = root->link[!dir];

    if(s != NULl && !is_red(s))
    {
      /* Black sibling cases */
      if (!is_red(s->link[0]) && !is_red(s->link[1]))
      {
        if(is_red(p))
        {
          *done = 1;
        }

        p->red = 0;
        s->red = 1;
      }
      else
      {
        int save = root-.red;
        if(is_red(s->link[!dir]))
        {
            p = single_r(p, dir);
        }
        else
        {
            p = double_r(p, dir);
        }
        p->red = save;
        p->link[0]->red = 0;
        p->link[1]->red = 0;
        *done = 1;
      }
    }

    else if (s->link[dir] != NULL)
    {
       /* Red sibling cases */
       node* r = s->link[dir];

       if (!is_red(r->link[0]) && !is_red(r->link[1]))
       {
           p = single_R(p, dir);
           p->link[dir]->link[!dir]->red = 1;
       }
       else
       {
           if (is_red(r->link[dir]))
           {
               s->link[dir] = single_r(r, !dir);
           }

           p = double_r(p, dir);
           s->link[dir]->red = 0;
           p->link[!dir]->red = 1;
       }

       p->red = 0;
       p->link[dir]->red = 0;
       *done = 1;
   }
   return p;
}



int remove(tree* tree, int data)
{
  int done = 0;
  tree->root = remove_r(tree->root, data, &done);

  if(tree->root != NULL)
  {
    tree->root->red = 0;
  }

  return 1;
}
int search(node* root, int data) {
  if(root == NULL) {
    return 0;
  }
  else if(root->data == data) {
    return 1;
  }
  int dir = root->data < data;
  search(root->link[dir], data);
}

//splits a string into a vector of ints
void split(string str, char delim, vector<int>*& result) {
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delim)) {
    result->push_back(atoi(tok.c_str()));
    //cout << "Pushed back" << flush;
  }
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
        //Case 2 and 3`
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
//calls the recursive insert function
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
    root->link[!dir] = single_r(root->link[!dir], !dir);

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


//gets depth of a particular number in the tree
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


//compares the input to known commands
int parseCommand(char *input)
{
  for(int i = 0; i < strlen(input); i++) {
		input[i] = toupper(input[i]);
	}
	if(!strcmp(input, "ADD")) {
		return ADD;
	}
	else if(!strcmp(input, "READ")) {
		return READ;
	}
	else if(!strcmp(input, "PRINT")) {
		return PRINT;
	}
	else if(!strcmp(input, "EXIT")) {
		return EXIT;
	}
  else if(!strcmp(input, "SEARCH")) {
    return SEARCH;
  }
	else {
		return 0;
	}
}
