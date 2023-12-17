/*
 * Desvendando a Magia das Conexões na Grafolândia
 * Nome:  Guilherme de Sousa Santos
 * RA:    11201921175
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// define alguns tipos de dados
typedef int Vertex;
typedef struct edge Edge;
typedef struct graph *Graph;
typedef struct node *link;
typedef struct queue Queue;

// define as estruturas dos dados
struct edge {
  Vertex src;
  Vertex dest;
};

struct graph {
  int V;
  int E;
  link *adj;
};

struct node {
  Vertex w;
  link next;
};

struct queue {
  int size;
  int capacity;
  Vertex *array;
};

// define a interface das funcoes utilizadas no programa
Edge edge(Vertex, Vertex);
Graph graph(int);
int graph_has_edge(Graph, Edge);
link list_insert(link, Vertex);
void graph_insert_edge(Graph, Edge);

void fill_order(Graph, Vertex, bool[], Vertex*, int*);
Graph get_transpose(Graph);
void DFS(Graph, Vertex, bool[], int[], int);
void Kosajaru(Graph);
void printDFS(int, int, int[]);

int main(void) {
  int n_towns, n_roads;
  Graph G;

  scanf("%d %d", &n_towns, &n_roads);

  G = graph(n_towns);

  for (int i = 0; i < n_roads; i++) {
    int x, y;
    scanf("%d %d", &x, &y);

    Edge e = edge(x, y);
    graph_insert_edge(G, e);
  }

  Kosajaru(G);

  return 0;
}

Graph graph(int V) {
  Graph G = malloc(sizeof(*G));

  G->V = V;
  G->E = 0;
  G->adj = malloc(V * sizeof(link));

  for (Vertex u = 0; u < V; u++) {
    G->adj[u] = NULL;
  }

  return G;
}

Edge edge(Vertex x, Vertex y) {
  Edge e;
  e.src = x;
  e.dest = y;
  return e;
}

int graph_has_edge(Graph G, Edge e) {
  assert(G);
  assert(e.src >= 0 && e.src < G->V);
  assert(e.dest >= 0 && e.dest < G->V);

  for (link p = G->adj[e.src]; p != NULL; p = p->next) {
    if (p->w == e.dest) {
      return 1;
    }
  }

  return 0;
}

link list_insert(link head, Vertex w) {
  link p = malloc(sizeof(*p));
  p->w = w;
  p->next = head;
  return p;
}

void graph_insert_edge(Graph G, Edge e) {
  assert(G);
  assert(e.src >= 0 && e.src < G->V);
  assert(e.dest >= 0 && e.dest < G->V);

  if (!graph_has_edge(G, e)) {
    G->adj[e.src] = list_insert(G->adj[e.src], e.dest);
    G->E += 1;
  }
}

void fill_order(Graph G, Vertex v, bool visited[], Vertex* stack, int* index) {
    visited[v] = true;

    link temp = G->adj[v];
    while (temp != NULL) {
        int adjVertex = temp->w;
        if (!visited[adjVertex]) {
          fill_order(G, adjVertex, visited, stack, index);
        }
        temp = temp->next;
    }

    stack[(*index)++] = v;
}

Graph get_transpose(Graph G) {
    Graph transposedGraph = graph(G->V);

    for (int v = 0; v < G->V; v++) {
        link temp = G->adj[v];
        while (temp != NULL) {
            Edge e = edge(temp->w, v);
            graph_insert_edge(transposedGraph, e);
            temp = temp->next;
        }
    }

    return transposedGraph;
}

void DFS(Graph G, Vertex v, bool visited[], int component[], int count) {
    component[v] = count;
    visited[v] = true;

    link temp = G->adj[v];
    while (temp != NULL) {
        int adjVertex = temp->w;
        if (!visited[adjVertex])
            DFS(G, adjVertex, visited, component, count);
        temp = temp->next;
    }
}

// algoritmo de Kosajaru para encontrar componentes fortemente conexas no digrafo
void Kosajaru(Graph G) {
    int V = G->V;
    bool visited[V];
    int component[V];

    for (int i = 0; i < V; i++) {
      visited[i] = false;
    }

    Vertex stack[V];
    int index = 0;

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
          fill_order(G, i, visited, stack, &index);
        }
    }

    Graph transposedGraph = get_transpose(G);

    for (int i = 0; i < V; i++) {
      visited[i] = false;
    }

    int count = 0;
    char* output = (char*)malloc(V * sizeof(char));

    while (index > 0) {
        Vertex v = stack[--index];
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