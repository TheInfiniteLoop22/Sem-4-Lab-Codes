#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10


int graph[MAX_VERTICES][MAX_VERTICES];

void bfs(int startVertex, int n) {
    int visited[MAX_VERTICES] = {0}; 
    int queue[MAX_VERTICES];         
    int front = -1, rear = -1;       
    // Enqueue the start vertex
    queue[++rear] = startVertex;
    visited[startVertex] = 1;
    printf("BFS starting from vertex %d:\n", startVertex);

    while (front != rear) {

        front++;
        int currentVertex = queue[front];
        printf("Visited: %d\n", currentVertex);

        
        for (int i = 0; i < n; i++) {
            if (graph[currentVertex][i] == 1 && !visited[i]) {
                queue[++rear] = i;
                visited[i] = 1;     
            }
        }
    }
}

int main() {
    int n, startVertex;

  
    printf("Enter the number of vertices in the graph: ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix (use 1 for edge, 0 for no edge):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }


    printf("Enter the start vertex: ");
    scanf("%d", &startVertex);


    bfs(startVertex, n);

    return 0;
}