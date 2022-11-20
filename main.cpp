#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
FILE *outFile;
// For the items of each tree Name
class itemNode {
public:
  char name[50]{};
  int count;
  itemNode *left, *right;
  itemNode() {
    name[0] = '\0';
    count = 0;
    left = NULL;
    right = NULL;
  }
  itemNode(char itemName[], int population) {
    strcpy(name, itemName);
    count = population;
    left = NULL;
    right = NULL;
  }
};
// For Tree Names
class treeNameNode {
public:
  char treeName[50]{};
  treeNameNode *left, *right;
  itemNode *theTree;
  treeNameNode() {
    treeName[0] = '\0';
    theTree = NULL;
    left = NULL;
    right = NULL;
  }
  treeNameNode(char name[]) {
    strcpy(treeName, name);
    theTree = NULL;
    left = NULL;
    right = NULL;
  }
};

// Insert Name Nodes
treeNameNode *insertNameNode(treeNameNode *root, treeNameNode *newNode) {
  if (root == NULL) { // base case
    return newNode;
  } else {
    if (strcmp(root->treeName, newNode->treeName) >
        0) { // compare root treename with treename new node
      if (root->left != NULL) {
        root->left = insertNameNode(root->left, newNode);
      } else {
        root->left = newNode; // root is newnode
      }
    } else {
      if (root->right != NULL) {
        root->right = insertNameNode(root->right, newNode);
      } else {
        root->right = newNode; // root is newnode
      }
    }
  }
  return root;
}
// Insert Item Nodes
itemNode *insertItemNode(itemNode *treeRoot, itemNode *newNode) {
  if (treeRoot == NULL) { // base case
    return newNode;
  } else {
    if (strcmp(treeRoot->name, newNode->name) >
        0) { // compare tree root name to new node name
      if (treeRoot->left != NULL) {
        treeRoot->left = insertItemNode(treeRoot->left, newNode);
      } else {
        treeRoot->left = newNode;
      }
    } else {
      if (treeRoot->right != NULL) {
        treeRoot->right = insertItemNode(treeRoot->right, newNode);
      } else {
        treeRoot->right = newNode;
      }
    }
  }
  return treeRoot;
}
// Insert Other tree
itemNode *insertOtherTree(itemNode *treeRoot, itemNode *newNode) {
  if (treeRoot == NULL) {
    return newNode;
  } else {
    if (strcmp(treeRoot->name, newNode->name) > 0) {
      if (treeRoot->left != NULL) {
        treeRoot->left = insertItemNode(treeRoot->left, newNode);
      } else {
        treeRoot->left = newNode;
      }
    } else {
      if (treeRoot->right != NULL) {
        treeRoot->right = insertItemNode(treeRoot->right, newNode);
      } else {
        treeRoot->right = newNode;
      }
    }
  }
  return treeRoot;
}
// Name Searching
treeNameNode *searchNameNode(treeNameNode *root, char treeName[50]) {
  if (root == NULL) { // base case
    return NULL;
  }
  treeNameNode *nameNode = NULL;
  if (strcmp(root->treeName, treeName) ==
      0) { // compare root tree name to treename
    return root;
  } else if (strcmp(root->treeName, treeName) >
             0) { // if root tree name is less go left
    nameNode = searchNameNode(root->left, treeName);
  } else {
    nameNode = searchNameNode(root->right,
                              treeName); // if root tree name is less go right
  }
  return nameNode;
}

// Search Item Node
itemNode *searchItemNode(itemNode *root, char *itemName) {
  if (!root) {
    return NULL;
  }
  int cmp = strcmp(root->name, itemName);
  if (cmp > 0) {
    return searchItemNode(root->left, itemName);
  } else if (cmp < 0) {
    return searchItemNode(root->right, itemName);
  }
  return root;
}

// Build Name Tree
treeNameNode *buildNameTree(FILE *inFile, int N) { // insert name into name tree
  treeNameNode *topTree = NULL;
  char treeName[50];
  for (int i = 0; i < N; i++) {
    fscanf(inFile, "%s", treeName);
    treeNameNode *temp = new treeNameNode(treeName);
    topTree = insertNameNode(topTree, temp);
  }
  return topTree;
}
// Building the Other Trees
void buildOtherTrees(FILE *infile, treeNameNode *theTree, int I) {
  char treeName[50], itemName[50];
  treeNameNode *foundTopTree;
  int pop;
  for (int i = 0; i < I; i++) {
    fscanf(infile, "%s %s %d", treeName, itemName, &pop);
    itemNode *newItemNode = new itemNode(itemName, pop);
    treeNameNode *foundTopTree = searchNameNode(theTree, treeName);
    foundTopTree->theTree = insertItemNode(foundTopTree->theTree, newItemNode);
  }
}
// Traversing Trees and Items
void inOrder_Item(itemNode *root) { // display item in order
  if (root != NULL) {
    inOrder_Item(root->left); // recusively traverse
    printf("%s ", root->name);
    fprintf(outFile, "%s ", root->name);
    inOrder_Item(root->right);
  }
}
void traverse_in_traverse(
    treeNameNode *root) { // print traversed items and trees
  if (root != NULL) {
    traverse_in_traverse(root->left); // recusively traverse
    printf("\n***%s***\n", root->treeName);
    fprintf(outFile, "\n***%s***\n", root->treeName);
    inOrder_Item(root->theTree); // calls to display item in order
    traverse_in_traverse(root->right);
  }
}
void inOrder_Tree(treeNameNode *root) { // display tree name in order
  if (root != NULL) {
    inOrder_Tree(root->left); // recusively traverse
    printf("%s ", root->treeName);
    fprintf(outFile, "%s ", root->treeName);
    inOrder_Tree(root->right);
  }
}
// Queries
// Search
// seprating different cases based on existance of Namenode and ItemNode
int search(treeNameNode *root, char *treeName, char *itemName, int *res) {
  treeNameNode *topTree = searchNameNode(root, treeName);
  if (topTree == NULL) {
    cout << "nameNode does not exist\n";
    return -1;
  }
  itemNode *itemNode = searchItemNode(topTree->theTree, itemName);
  if (!itemNode) {
    return -2;
  }
  *res = itemNode->count;
  return 0;
}
// Count
int countTree(treeNameNode *root, char treeName[50]) {
  int co = 0;
  if (root == NULL) {
    return 0;
  } else {
    searchNameNode(root, treeName);
    co += countTree(root->left, treeName);
    co += countTree(root->right, treeName);
    return co;
  }
}
// height_balance comand
int getHeight(itemNode *root) {
  if (root == NULL) {
    return 0;
  }
  int leftHeight = getHeight(root->left);
  int rightHeight = getHeight(root->right);

  return leftHeight && rightHeight;
}
int isBalanced(treeNameNode *root, char *treeName, int *leftHeight,
               int *rightHeight) {
  if (root == NULL) {
    return 0;
  } else {
    searchNameNode(root, treeName);
    if (abs(leftHeight - rightHeight) > 1) {
      cout << "not balanced";
      return 0;
    } else {
      cout << "balanced";
      return 0;
    }
  }
}
int countItem(itemNode *root, char ItemName[50]) {
  int co = 0;
  if (root == NULL) {
    return 0;
  } else {
    searchItemNode(root, ItemName);
    co += countItem(root->left, ItemName);
    co += countItem(root->right, ItemName);
    return co;
  }
}
// Item Before
int item_before(treeNameNode *root, char treeName[50], char ItemName[50]) {}
// Perform Queries
void performQueries(FILE *inFile, treeNameNode *root, int Q) {
  char queryType[50], paramA[50], paramB[50];
  char treeName[50];
  int paramC;
  int leftHeight, rightHeight;
  for (int i = 0; i < Q; ++i) {
    fscanf(inFile, "%s", queryType);                   // read in query type
    if (strcmp(queryType, "search") == 0) {            // if search read in like
      fscanf(inFile, "%s %s\n", paramA, paramB);       // read in parameters
      int res = search(root, paramA, paramB, &paramC); // call search function
      if (res == 0) {                                  // if match
        printf("%d %s found in %s\n", paramC, paramB, paramA);
        fprintf(outFile, "%d %s found in %s\n", paramC, paramB, paramA);
      } else if (res == 1) {
        printf("%s does not exist\n", paramA);
        fprintf(outFile, "%s does not exist\n", paramA);
      } else {
        printf("%s is not found in %s\n", paramB, paramA);
        fprintf(outFile, "%s is not found in %s\n", paramB, paramA);
      }
    }
    if (strcmp(queryType, "count") == 0) { // if count
      fscanf(inFile, " %s", treeName);     // read in file
      countTree(root, treeName);
      printf("%s count %d\n", treeName, countTree(root, treeName)); // print
      fprintf(outFile, "%s count %d\n", treeName, countTree(root, treeName));
    }
  }
}

int main() {
  FILE *inFile = fopen("in.txt", "r");
  outFile = fopen("out.txt", "w");
  if (inFile == NULL) { // base case
    return 0;
  }
  treeNameNode *topTree = NULL; // creating name tree node

  int N, I, Q;
  fscanf(inFile, "%d %d %d", &N, &I, &Q); // n=4 I=28 q=17

  topTree =
      buildNameTree(inFile, N); // top tree node that goes into buildName Tree
  buildOtherTrees(inFile, topTree, I); //

  inOrder_Tree(topTree); // display tree name in order
  printf("\n");
  fprintf(outFile, "\n");
  traverse_in_traverse(topTree); //-
  performQueries(inFile, topTree, Q);
  fclose(inFile);
  fclose(outFile);

  return 0;
}
