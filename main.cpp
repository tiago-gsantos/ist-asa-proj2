#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

#define WHITE (0)
#define GREY (1)
#define BLACK (2)

struct vertice {
  unsigned int value;
  int color;
  int sccID;
};

std::stack<vertice*> DFS(std::vector<std::vector<unsigned int>> &graph, std::vector<vertice> &vertices, std::stack<vertice*> &stack){
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


int calculateSCCs(std::vector<std::vector<unsigned int>> &graph, std::vector<vertice> &vertices, std::stack<vertice*> &stack){
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
  return scc-1;
}


int calculateMaxJumps(std::vector<std::vector<unsigned int>> &graph, std::vector<vertice> &vertices, std::stack<vertice*> &stack, int numSCC){
  std::vector<int> sccJumps(numSCC + 1, 0);
  int maxJumps = 0;
  
  while(!stack.empty()){
    struct vertice *v = stack.top();
    stack.pop();

    for(int adjVertice : graph[v->value]){
      if(vertices[adjVertice].sccID == v->sccID)
        sccJumps[v->sccID] = std::max(sccJumps[v->sccID], sccJumps[vertices[adjVertice].sccID]);
      else
        sccJumps[v->sccID] = std::max(sccJumps[v->sccID], sccJumps[vertices[adjVertice].sccID] + 1);
      
      maxJumps = std::max(maxJumps, sccJumps[v->sccID]);
    }
  }
  return maxJumps;
}


int main(){
  std::ios::sync_with_stdio(0); // disable sync with c libs
  std::cin.tie(0); // discard cin buffer after each line of input
  unsigned int V, E;

  // Read Vertices and Edges
  std::cin >> V;
  std::cin >> E;

  // Create Graph and Trnspost Graph
  std::vector<std::vector<unsigned int>> graph(V + 1);
  std::vector<std::vector<unsigned int>> transpostGraph(V + 1);

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
  std::stack<vertice*> orderedByDescendingFinishTime = DFS(graph, vertices, stack);

  std::stack<vertice*> copy = orderedByDescendingFinishTime;
  // Reset vertices color to white
  for(unsigned int i = 1; i <= V; i++) 
    vertices[i].color = WHITE;
  
  // Second DFS
  int numSCC = calculateSCCs(transpostGraph, vertices, orderedByDescendingFinishTime);

  std::stack<vertice*> orderedByIncrislyFinishTime;
  while(!copy.empty()){
    vertice* v = copy.top();
    orderedByIncrislyFinishTime.push(v);
    copy.pop();
  }

  int maxJumps = calculateMaxJumps(graph, vertices, orderedByIncrislyFinishTime, numSCC);

  std::cout << maxJumps << '\n';

  return 0;
}

