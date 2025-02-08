//Q1
//Ritesh

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE 100  // Queue size for source removal

int queue[MAX_QUEUE], front = 0, rear = 0;

void topologicalSort(int n) {
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    popIndex = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS(i, n);
        }
    }
    printf("Topological Sort (DFS-based): ");
    for (int i = popIndex - 1; i >= 0; i--) {
        printf("%d ", popOrder[i]);
    }
    printf("\n");
}
void topologicalSort_Kahn(int n) {
    int inDegree[MAX_VERTICES] = {0};  // Array to store in-degree of nodes
    int topoOrder[MAX_VERTICES];       // Store topological order
    int topoIndex = 0;                 // Index for topological order
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == 1) {
                inDegree[j]++;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }
    while (front < rear) {
        int vertex = queue[front++];
        topoOrder[topoIndex++] = vertex;

        // Reduce in-degree of neighbors
        for (int i = 0; i < n; i++) {
            if (graph[vertex][i] == 1) {
                inDegree[i]--;
                if (inDegree[i] == 0) {
                    queue[rear++] = i;
                }
            }
        }
    }
    if (topoIndex == n) {
        printf("Topological Sort (Kahn’s Algorithm): ");
        for (int i = 0; i < n; i++) {
            printf("%d ", topoOrder[i]);
        }
        printf("\n");
    } else {
        printf("Graph has a cycle! Topological sorting not possible.\n");
    }
}

int main() {
    int n, edges;
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    printf("Enter the number of edges: ");
    scanf("%d", &edges);

    // Initialize adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
        visited[i] = 0;
    }

    printf("Enter the edges (u v) (0-based indexing, directed graph):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        graph[u][v] = 1;  // Directed graph
    }

    printf("Performing Topological Sort using DFS...\n");
    topologicalSort(n);  // DFS-based topological sorting

    printf("Performing Topological Sort using Source Removal (Kahn’s Algorithm)...\n");
    topologicalSort_Kahn(n);  // Kahn's Algorithm
    return 0;
}
