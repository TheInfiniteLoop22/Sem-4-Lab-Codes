#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

// Graph structure
typedef struct Graph {
    int numVertices;
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
    int inDegree[MAX_VERTICES]; // Array to store in-degrees
} Graph;

// Function to initialize the graph
void initGraph(Graph *graph, int vertices) {
    graph->numVertices = vertices;

    // Initialize adjacency matrix and in-degree array
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->adjMatrix[i][j] = 0;
        }
        graph->inDegree[i] = 0;
    }
}

// Function to add an edge to the graph
void addEdge(Graph *graph, int start, int end) {
    graph->adjMatrix[start][end] = 1; // Directed edge from start to end
    graph->inDegree[end]++;           // Increment the in-degree of the destination vertex
}

// Function to perform topological sort using Source Removal method
void topologicalSort(Graph *graph) {
    int opCount = 0;
    int visited[MAX_VERTICES] = {0};
    int topologicalOrder[MAX_VERTICES];
    int top = 0;

    // Repeat until all vertices are visited
    while (top < graph->numVertices) {
        int foundSource = 0;

        // Look for a source vertex (in-degree of 0) that hasn't been visited
        for (int i = 0; i < graph->numVertices; i++) {
            opCount++; // Increment operation count for the iteration

            if (graph->inDegree[i] == 0 && !visited[i]) {
                // Add this source vertex to the topological order
                topologicalOrder[top++] = i;
                visited[i] = 1; // Mark the vertex as visited

                // Remove this vertex from the graph (decrease in-degrees of its neighbors)
                for (int j = 0; j < graph->numVertices; j++) {
                    if (graph->adjMatrix[i][j] == 1) {
                        graph->inDegree[j]--; // Decrement the in-degree of the destination vertex
                    }
                }

                foundSource = 1;
                break;
            }
        }

        // If no source vertex is found, the graph has a cycle
        if (!foundSource) {
            printf("The graph has a cycle. Topological sort is not possible.\n");
            return;
        }
    }

    // Print the topological sort
    printf("Topological Sort: ");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d ", topologicalOrder[i]);
    }
    printf("\n");

    // Print the operation count
    printf("Number of major operations (iterations): %d\n", opCount);
}

int main() {
    Graph graph;
    int vertices, edges;

    // Take user input for the number of vertices and edges
    printf("Enter number of vertices: ");
    scanf("%d", &vertices);
    initGraph(&graph, vertices);

    printf("Enter number of edges: ");
    scanf("%d", &edges);

    // Take user input for the edges
    printf("Enter the edges (start vertex and end vertex):\n");
    for (int i = 0; i < edges; i++) {
        int start, end;
        scanf("%d %d", &start, &end);
        addEdge(&graph, start, end);
    }

    // Perform topological sorting using Source Removal
    topologicalSort(&graph);

    return 0;
}
