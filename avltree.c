//Implementation of AVL Trees

#include <stdio.h>
#include <stdlib.h>



// Create a Node
 struct Node {
       int key;
    struct Node* left;
    struct Node* right;
    int height;
} ;



// Function to get the height of a node
int height(struct Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}



// Function to get the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}



//Crteate a new node
struct Node *newnode(int key)
{
	struct Node *node=(struct Node*)malloc(sizeof(struct Node));
	node->key=key;
	node->left=NULL;
	node->right=NULL;
	node->height=1;
	return (node);
	
}
// Get Balance Factor


int getBalance (struct Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
//To find minimum value


struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}



// left rotate

struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
  struct  Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

//Right Rotate

struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
   struct Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

//Insert Node


struct Node* insertnode(struct Node* node, int key) {
//Find the correct position to insert the node and insert it
    if (node == NULL) {
       struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        temp->key = key;
        temp->left = NULL;
        temp->right = NULL;
        temp->height = 1;
        return temp;
    }

    if (key < node->key)
        node->left = insertnode(node->left, key);
    else if (key > node->key)
        node->right = insertnode(node->right, key);
    else
        return node;
//Update the balance factor of each node and balance the tree
    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);
//Rotations

    if (balance > 1 && key < node->left->key)//RR Rotation
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)//LL Rotation
        return leftRotate(node);

    if (balance > 1 && key > node->left->key)//LR Rotation
 {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) //RL Rotation
{
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Delete Function


struct Node* deleteNode(struct Node* root, int key) {
//Find the node and delete it
    if (root == NULL)//If node not found
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else 
{
        if ((root->left == NULL) || (root->right == NULL)) 
{
            struct Node* temp = root->left ? root->left : root->right;
           if (temp == NULL) //node to be deleted is leaf node it has zero children
{
                temp = root;
                root = NULL;
            } 
else//Node to be deleted has one child
                *root = *temp;
            free(temp);
        } 

else //Node to be deleted has two children 
{
            struct Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)//value not found
        return root;
//Update the balance factor of each node and balance the tree

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);
//Rotate after deletion
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
// In-Order Traversal
void inorderTraversal(struct Node* root, FILE* file) {
    if (root != NULL) {
        inorderTraversal(root->left, file);
        fprintf(file, "%d\n", root->key);
        inorderTraversal(root->right, file);
    }
}

// Function to Read Elements from a File and Construct AVL Tree
struct Node* constructTreeFromFile(const char* filename) {
    struct Node* root = NULL;
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s for reading\n", filename);
        return NULL;
    }

    int key;
    while (fscanf(file, "%d", &key) != EOF) {
        root = insertnode(root, key);
    }
    fclose(file);
    return root;
}

// Function to Write In-Order Traversal to a File
void writeInOrderToFile(struct Node* root, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Could not open file %s for writing\n", filename);
        return;
    }

    inorderTraversal(root, file);
    fclose(file);
}

// Main Function
int main() {
    const char* inputFile = "elements.txt";
    const char* outputFile = "inorder_traversal.txt";

    // Construct AVL tree from file
   struct Node* root = constructTreeFromFile(inputFile);

    // Insert an element
    root = insertnode(root, 15);

    // Delete an element
    root = deleteNode(root, 10);

    // Write in-order traversal to file
    writeInOrderToFile(root, outputFile);

    return 0;
}
