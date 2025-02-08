#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

int opCount = 0;

struct Node* findCCA(struct Node* root, int n1, int n2) {
    opCount++;

    if (root == NULL || root->data == n1 || root->data == n2) {
        return root;
    }

    struct Node* left = findCCA(root->left, n1, n2); 
    struct Node* right = findCCA(root->right, n1, n2);

    if (left != NULL && right != NULL) {
        return root;
    }

    return (left != NULL) ? left : right;
}

struct Node* insertNode(struct Node* root, int data) {
    if (root == NULL) {
        return newNode(data);
    }

    if (data < root->data) {
        root->left = insertNode(root->left, data);
    } else if (data > root->data) {
        root->right = insertNode(root->right, data);
    }

    return root;
}

int main() {
    int n, n1, n2;
    struct Node* root = NULL;

    printf("Enter the number of nodes in the binary tree: ");
    scanf("%d", &n);

    printf("Enter the nodes of the binary tree (values separated by spaces): ");
    for (int i = 0; i < n; i++) {
        int value;
        scanf("%d", &value);
        root = insertNode(root, value);
    }

    printf("Enter the two nodes to find CCA: ");
    scanf("%d %d", &n1, &n2);

    // Find the CCA
    struct Node* cca = findCCA(root, n1, n2);

    // Output the CCA
    if (cca != NULL) {
        printf("Closest Common Ancestor of %d and %d is: %d\n", n1, n2, cca->data);
    } else {
        printf("No Common Ancestor found.\n");
    }

    printf("Number of operations (iterations): %d\n", opCount);

    return 0;
}
