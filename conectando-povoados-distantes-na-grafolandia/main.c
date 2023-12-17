/*
 * Conectando Povoados Distantes na Grafolândia
 * Nome:  Guilherme de Sousa Santos
 * RA:    11201921175
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// define alguns tipos de dados
typedef int Vertex;
typedef struct edge Edge;
typedef struct graph *Graph;
typedef struct node *link;

// define as estruturas dos dados
struct edge {
  Vertex x;
  Vertex y;
  double weight;
};

struct graph {
  int V;
  int E;
  link *adj;
};

struct node {
  Vertex w;
  double weight;
  link next;
};

// define a interface das funcoes
Graph graph(int);
Edge edge(Vertex, Vertex, double);
int graph_has_edge(Graph, Edge);
link list_insert(link, Vertex, double);
void graph_insert_edge(Graph, Edge);

int Find(int[], int);
void Union(int *, int, int);
int compareEdges(const void *, const void *);
void Kruskal(Graph);

int main(void) {
  int n_towns, n_roads;
  Graph G;

  scanf("%d %d", &n_towns, &n_roads);

  G = graph(n_towns);

  for (int i = 0; i < n_roads; i++) {
    int x, y;
    double weight;
    scanf("%d %d %lf", &x, &y, &weight);

    Edge e1 = edge(x, y, weight);
    graph_insert_edge(G, e1);
    Edge e2 = edge(y, x, weight);
    graph_insert_edge(G, e2);
  }

  Kruskal(G);

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

Edge edge(Vertex x, Vertex y, double weight) {
  Edge e;
  e.x = x;
  e.y = y;
  e.weight = weight;
  return e;
}

int graph_has_edge(Graph G, Edge e) {
  assert(G);
  assert(e.x >= 0 && e.x < G->V);
  assert(e.y >= 0 && e.y < G->V);

  for (link p = G->adj[e.x]; p != NULL; p = p->next) {
    if (p->w == e.y) {
      return 1;
    }
  }

  return 0;
}

link list_insert(link head, Vertex w, double weight) {
  link p = malloc(sizeof(*p));
  p->w = w;
  p->weight = weight;
  p->next = head;
  return p;
}

void graph_insert_edge(Graph G, Edge e) {
  assert(G);
  assert(e.x >= 0 && e.x < G->V);
  assert(e.y >= 0 && e.y < G->V);

  if (!graph_has_edge(G, e)) {
    G->adj[e.x] = list_insert(G->adj[e.x], e.y, e.weight);
    G->adj[e.y] = list_insert(G->adj[e.y], e.x, e.weight);
    G->E += 1;
  }
}

int Find(int parent[], int i) {
    if (parent[i] == -1)
        return i;
    return Find(parent, parent[i]);
}

void Union(int *parent, int x, int y) {
    int xset = Find(parent, x);
    int yset = Find(parent, y);
    parent[xset] = yset;
}

// funcao de comparacao para o qsort
int compareEdges(const void *a, const void *b) {
    Edge *edgeA = (Edge *)a;
    Edge *edgeB = (Edge *)b;
    return (edgeA->weight > edgeB->weight) - (edgeA->weight < edgeB->weight);
}

// encontra a arvore geradora mínima (MST) usando o algoritmo de Kruskal
void Kruskal(Graph G) {
    int V = G->V;
    int E = G->E;

    // cria o vetor resultante
    Edge *F = malloc(V * sizeof(Edge));

    int e = 0;
    int i = 0;

    // copia as arestas de G para C com seus respectivos pesos
    Edge *C = malloc(E * sizeof(Edge));
    int edgeIndex = 0;

    for (Vertex v = 0; v < V; v++) {
        for (link p = G->adj[v]; p != NULL; p = p->next) {
            if (v < p->w) {
                C[edgeIndex].x = v;
                C[edgeIndex].y = p->w;
                C[edgeIndex].weight = p->weight;
                edgeIndex++;
            }
        }
    }

    // ordena C de modo não-decrescente de acordo com o peso das arestas
    qsort(C, E, sizeof(Edge), compareEdges);

    int* parent = (int*)malloc(V * sizeof(int));

    for (int v = 0; v < V; v++) {
        parent[v] = -1;
    }

    while (e < V - 1 && i < G->E) {
        Edge next_edge = C[i++];

        int x = Find(parent, next_edge.x);
        int y = Find(parent, next_edge.y);

        if (x != y) {
            F[e++] = next_edge;
            Union(parent, x, y);
        }
    }

    // calcula e imprime o custo minimo da MST
    double minCost = 0;

    for (int i = 0; i < e; i++) {
        minCost += F[i].weight;
    }

    printf("%.2lf\n", minCost);
    
    // imprime as estradas que devem ser construidas
    for (int i = 0; i < e; ++i) {
        printf("%d %d\n", F[i].x, F[i].y);
    }

    free(parent);
    free(C);
    free(F);
}