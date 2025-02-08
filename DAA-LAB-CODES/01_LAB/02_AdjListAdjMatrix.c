#include <stdio.h>
#include <stdlib.h>

// Define types for simplicity
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node** adjLists;
} Graph;

// Function to create a new adjacency list node
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with the given number of vertices
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjLists = (Node**)malloc(numVertices * sizeof(Node*));

    for (int i = 0; i < numVertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

// Function to add an edge to the adjacency list representation
void addEdgeList(Graph* graph, int src, int dest) {
    // Add edge from src to dest
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Add edge from dest to src (for undirected graph)
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Function to print the adjacency list representation
void printAdjList(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Node* temp = graph->adjLists[i];
        printf("Vertex %d:\n", i);
        while (temp) {
            printf(" -> %d", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Function to create an adjacency matrix
int** createAdjMatrix(int numVertices) {
    int** matrix = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        matrix[i] = (int*)malloc(numVertices * sizeof(int));
        for (int j = 0; j < numVertices; j++) {
            matrix[i][j] = 0; // Initialize all entries to 0
        }
    }
    return matrix;
}

// Function to add an edge to the adjacency matrix
void addEdgeMatrix(int** matrix, int src, int dest) {
    matrix[src][dest] = 1;
    matrix[dest][src] = 1; // For undirected graph
}

// Function to print the adjacency matrix
void printAdjMatrix(int** matrix, int numVertices) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int numVertices;
    int** adjMatrix = NULL;
    Graph* graph = NULL;
    int choice;

    // Input number of vertices
    printf("Enter number of vertices: ");
    scanf("%d", &numVertices);

    graph = createGraph(numVertices); // Create adjacency list
    adjMatrix = createAdjMatrix(numVertices); // Create adjacency matrix

    while (1) {
        // Menu for user interaction
        printf("\nMenu:\n");
        printf("1. Add Edge\n");
        printf("2. Display Adjacency List\n");
        printf("3. Display Adjacency Matrix\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Adding an edge
                int src, dest;
                printf("Enter source and destination vertices: ");
                scanf("%d %d", &src, &dest);

                if (src >= numVertices || dest >= numVertices || src < 0 || dest < 0) {
                    printf("Invalid vertices!\n");
                } else {
                    addEdgeList(graph, src, dest); // Add to adjacency list
                    addEdgeMatrix(adjMatrix, src, dest); // Add to adjacency matrix
                    printf("Edge added between %d and %d.\n", src, dest);
                }
                break;
            }

            case 2:
                // Display adjacency list
                printf("\nAdjacency List Representation:\n");
                printAdjList(graph);
                break;

            case 3:
                // Display adjacency matrix
                printf("\nAdjacency Matrix Representation:\n");
                printAdjMatrix(adjMatrix, numVertices);
                break;

            case 4:
                // Exit program
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
