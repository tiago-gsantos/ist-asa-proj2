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
  unsigned int f; // Precisamos disto?? Ja vamos ter a orderedFinishTime
  int color;
  int sccID;
};


std::stack<vertice*> dfs(std::list<int> *graph, std::vector<vertice> &vertices, std::stack<vertice*> &stack, unsigned int V){
  std::stack<vertice*> orderedByFinishTime;

  unsigned int time = 0;
  
  while(!stack.empty()){
    struct vertice *v = stack.top();
    
    if(v->color == WHITE){
      v->color = GREY;
      time++;

      for(unsigned int adjValue : graph[v->value]){
        if(vertices[adjValue].color == WHITE)
          stack.push(&vertices[adjValue]);
      }
    }
    else if(v->color == GREY){
      stack.pop();
      
      v->color = BLACK;
      v->f = ++time;
      
      orderedByFinishTime.push(v);
    }
    else{
      stack.pop();
    }
  }

  return orderedByFinishTime;
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

  // Do first DFS
  std::stack<vertice*> orderedByFinishTime = dfs(graph, vertices, stack, V);

  while(!orderedByFinishTime.empty()){
    std::cout << orderedByFinishTime.top()->value << '\n';
    orderedByFinishTime.pop();
  }

  return 0;
}

