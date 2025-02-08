

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

// Adjacency matrix representation of the graph
int graph[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int pushOrder[MAX_VERTICES];
int popOrder[MAX_VERTICES];
int pushIndex = 0, popIndex = 0;

// Function to perform DFS
void DFS(int vertex, int n) {
    // Mark the current vertex as visited
    visited[vertex] = 1;
    pushOrder[pushIndex++] = vertex;

    // Explore all adjacent vertices
    for (int i = 0; i < n; i++) {
        if (graph[vertex][i] == 1 && !visited[i]) {
            DFS(i, n);
        }
    }

    // Add to pop order when finished exploring the vertex
    popOrder[popIndex++] = vertex;
}

int main() {
    int n, edges;
    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    printf("Enter the number of edges: ");
    scanf("%d", &edges);

    // Initialize the adjacency matrix and visited array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
        visited[i] = 0;
    }

    printf("Enter the edges (u v) (0-based indexing):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        graph[u][v] = 1;
        graph[v][u] = 1; // Comment this line for a directed graph
    }

    // Perform DFS starting from vertex 0
    printf("Starting DFS...\n");
    DFS(0, n);

    // Print the push and pop order
    printf("Push order: ");
    for (int i = 0; i < pushIndex; i++) {
        printf("%d ", pushOrder[i]);
    }
    printf("\n");

    printf("Pop order: ");
    for (int i = 0; i < popIndex; i++) {
        printf("%d ", popOrder[i]);
    }
    printf("\n");

    return 0;
}

