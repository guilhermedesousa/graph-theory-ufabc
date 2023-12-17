/*
 * Desvendando a Magia das Conexões na Grafolândia
 * Nome:  Guilherme de Sousa Santos
 * RA:    11201921175
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Node {
    int vertex;
    struct Node* next;
};

struct Graph {
    int V;
    struct Node** adjList;
};

struct Node* createNode(int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = vertices;

    graph->adjList = (struct Node**)malloc(vertices * sizeof(struct Node*));

    for (int i = 0; i < vertices; ++i)
        graph->adjList[i] = NULL;

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

void DFS(struct Graph* graph, int v, bool visited[], int component[], int count) {
    component[v] = count;
    visited[v] = true;

    struct Node* temp = graph->adjList[v];
    while (temp != NULL) {
        int adjVertex = temp->vertex;
        if (!visited[adjVertex])
            DFS(graph, adjVertex, visited, component, count);
        temp = temp->next;
    }
}

void printDFS(int count, int V, int component[]) {
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < V; j++) {
            if (component[j] == i) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
}

struct Graph* getTranspose(struct Graph* graph) {
    struct Graph* transposedGraph = createGraph(graph->V);

    for (int v = 0; v < graph->V; v++) {
        struct Node* temp = graph->adjList[v];
        while (temp != NULL) {
            addEdge(transposedGraph, temp->vertex, v);
            temp = temp->next;
        }
    }

    return transposedGraph;
}

void fillOrder(struct Graph* graph, int v, bool visited[], int* stack, int* index) {
    visited[v] = true;

    struct Node* temp = graph->adjList[v];
    while (temp != NULL) {
        int adjVertex = temp->vertex;
        if (!visited[adjVertex])
            fillOrder(graph, adjVertex, visited, stack, index);
        temp = temp->next;
    }

    stack[(*index)++] = v;
}

void printSCCs(struct Graph* graph) {
    int V = graph->V;
    bool visited[V];
    int component[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    int stack[V];
    int index = 0;

    for (int i = 0; i < V; i++) {
        if (!visited[i])
            fillOrder(graph, i, visited, stack, &index);
    }

    struct Graph* transposedGraph = getTranspose(graph);

    for (int i = 0; i < V; i++)
        visited[i] = false;

    int count = 0;
    char* output = (char*)malloc(V * sizeof(char));

    while (index > 0) {
        int v = stack[--index];
        if (!visited[v]) {
            DFS(transposedGraph, v, visited, component, count);
            count++;
        }
    }

    if (count == 1) {
        printf("Ha %d regiao altamente conectada:\n", count);
    } else {
        printf("Ha %d regioes altamente conectadas:\n", count);
    }
    
    printDFS(count, V, component);
}

void resetCurrentDFS(int V, bool currentDFS[]) {
    for (int i = 0; i < V; i++)
        currentDFS[i] = false;
}

int main() {
    int V, E;
    scanf("%d %d", &V, &E);

    struct Graph* graph = createGraph(V);

    for (int i = 0; i < E; ++i) {
        int src, dest;
        scanf("%d %d", &src, &dest);
        addEdge(graph, src, dest);
    }

    printSCCs(graph);

    return 0;
}
