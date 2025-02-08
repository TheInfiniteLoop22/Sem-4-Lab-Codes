
// 1).Write a program to determine the Topological sort of a given graph using
// i. Depth-First technique





// Algorithm for Topological Sorting using DFS:

// Here’s a step-by-step algorithm for topological sorting using Depth First Search (DFS):

//     Create a graph with n vertices and m-directed edges.
//     Initialize a stack and a visited array of size n.
//     For each unvisited vertex in the graph, do the following:
//         Call the DFS function with the vertex as the parameter.
//         In the DFS function, mark the vertex as visited and recursively call the DFS function for all unvisited neighbors of the vertex.
//         Once all the neighbors have been visited, push the vertex onto the stack.
//     After all, vertices have been visited, pop elements from the stack and append them to the output list until the stack is empty.
//     The resulting list is the topologically sorted order of the graph.





#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

// Graph structure
typedef struct Graph {
    int numVertices;
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
} Graph;

// Function to initialize the graph
void initGraph(Graph *graph, int vertices) {
    graph->numVertices = vertices;

    // Initialize adjacency matrix with 0 (no edges)
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

// Function to add an edge to the graph
void addEdge(Graph *graph, int start, int end) {
    graph->adjMatrix[start][end] = 1; // Directed edge from start to end
}

// Helper function for DFS
void DFS(int vertex, Graph *graph, int *visited, int *stack, int *top, int *opCount) {
    visited[vertex] = 1;
    (*opCount)++;  // Increment operation count for this DFS call

    // Visit all the adjacent vertices of the current vertex
    for (int i = 0; i < graph->numVertices; i++) {
        (*opCount)++;  // Increment operation count for this iteration
        if (graph->adjMatrix[vertex][i] == 1 && !visited[i]) {
            DFS(i, graph, visited, stack, top, opCount);
        }
    }

    // After visiting all the neighbors, add the vertex to the stack
    stack[*top] = vertex;
    (*top)++;
}

// Function to perform topological sort
void topologicalSort(Graph *graph) {
    int visited[MAX_VERTICES] = {0};
    int stack[MAX_VERTICES];
    int top = 0;
    int opCount = 0;

    // Call DFS for each unvisited vertex
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            DFS(i, graph, visited, stack, &top, &opCount);
        }
    }

    // Print the vertices in topologically sorted order
    printf("Topological Sort: ");
    for (int i = top - 1; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    printf("\n");

    // Print the operation count
    printf("Number of major operations (DFS iterations): %d\n", opCount);
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

    // Perform topological sorting
    topologicalSort(&graph);

    return 0;
}
