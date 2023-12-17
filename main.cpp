#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

#define WHITE (0)
#define GREY (1)
#define BLACK (2)

struct vertice {
  unsigned int value;
  int color;
  int sccID;
};


std::stack<vertice*> DFS(std::list<int> *graph, std::vector<vertice> &vertices, std::stack<vertice*> &stack){
  std::stack<vertice*> orderedByFinishTime;
  
  while(!stack.empty()){
    struct vertice *v = stack.top();
    
    if(v->color == WHITE){
      v->color = GREY;

      for(unsigned int adjValue : graph[v->value]){
        if(vertices[adjValue].color == WHITE)
          stack.push(&vertices[adjValue]);
      }
    }
    else if(v->color == GREY){
      stack.pop();
      
      v->color = BLACK;
      
      orderedByFinishTime.push(v);
    }
    else{
      stack.pop();
    }
  }

  return orderedByFinishTime;
}


void calculateSCCs(std::list<int> *graph, std::vector<vertice> &vertices, std::stack<vertice*> &stack){
  int scc = 1;
  int verticesInSCC = 0;

  while(!stack.empty()){
    struct vertice *v = stack.top();
    
    if(v->color == WHITE){
      v->color = GREY;

      v->sccID = scc;
      verticesInSCC++;

      for(unsigned int adjValue : graph[v->value]){
        if(vertices[adjValue].color == WHITE)
          stack.push(&vertices[adjValue]);
      }
    }
    else if(v->color == GREY){
      stack.pop();
      
      v->color = BLACK;

      if(--verticesInSCC == 0)
        scc++;
    }
    else{
      stack.pop();
    }
  }
}


int main(){
  unsigned int V, E;

  // Read Vertices and Edges
  std::cin >> V;
  std::cin >> E;

  // Create Graph and Trnspost Graph
  std::list<int> *graph = new std::list<int>[V + 1];
  std::list<int> *transpostGraph = new std::list<int>[V + 1];

  // Initialize vector with all vertices
  std::vector<vertice> vertices(V+1);
  for(unsigned int i = 1; i <= V; i++){
    vertices[i] = {.value = i, .color = WHITE};
  }
  
  // Initialize Graphes
  for(unsigned int i = 0; i < E; i++){
    unsigned int u, v;
    
    std::cin >> u >> v;
    
    graph[u].push_back(v);
    transpostGraph[v].push_back(u);
  }

  // Create stack with order for first DFS
  std::stack<vertice*> stack;
  for(int i = V; i >= 1; i--){
    stack.push(&vertices[i]);
  }

  // First DFS
  std::stack<vertice*> orderedByFinishTime = DFS(graph, vertices, stack);

  // Reset vertices color to white
  for(unsigned i = 1; i <= V; i++) 
    vertices[i].color = WHITE;
  
  // Second DFS
  calculateSCCs(transpostGraph, vertices, orderedByFinishTime);

  for(vertice v : vertices){
    std::cout << v.value << ' ' << v.sccID << '\n';
  }

  return 0;
}

