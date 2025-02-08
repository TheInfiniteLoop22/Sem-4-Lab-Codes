//Ritesh
//Q2

#include <stdio.h>
#include <stdlib.h>

// Define the binary tree node using typedef
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Function to create a new node
Node* newNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

// Function to find the Closest Common Ancestor (CCA) of n1 and n2
Node* findCCA(Node* root, int n1, int n2) {
    // Base case: If the root is NULL or root matches one of the nodes
    if (root == NULL || root->data == n1 || root->data == n2)
        return root;

    // Recursively search for LCA in the left and right subtrees
    Node* leftLCA = findCCA(root->left, n1, n2);
    Node* rightLCA = findCCA(root->right, n1, n2);

    // If both nodes are found in different subtrees, the current node is the CCA
    if (leftLCA && rightLCA)
        return root;

    // Otherwise, return the non-null child (either left or right)
    return (leftLCA != NULL) ? leftLCA : rightLCA;
}

// Function to create the binary tree based on user input
Node* createBinaryTree() {
    int data;

    // Take input once for all node values, '-1' for no node
    printf("Enter node value (-1 for no node): ");
    scanf("%d", &data);

    if (data == -1) {
        return NULL;  // Return NULL if no node
    }

    Node* node = newNode(data);

    printf("Enter left child of %d:\n", data);
    node->left = createBinaryTree();

    printf("Enter right child of %d:\n", data);
    node->right = createBinaryTree();

    return node;
}

// Function to test the Closest Common Ancestor (CCA)
int main() {
    Node* root = NULL;
    int n1, n2;

    // Create binary tree based on user input
    printf("Create the binary tree (enter -1 for no node):\n");
    root = createBinaryTree();

    // Take two nodes as input to find their Closest Common Ancestor
    printf("Enter two nodes to find their Closest Common Ancestor (CCA): ");
    scanf("%d %d", &n1, &n2);

    // Find and print the Closest Common Ancestor
    Node* cca = findCCA(root, n1, n2);
    if (cca != NULL)
        printf("The Closest Common Ancestor (CCA) of %d and %d is: %d\n", n1, n2, cca->data);
    else
        printf("No common ancestor found.\n");

    return 0;
}