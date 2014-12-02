#include <iostream>
#include "src/graph.h"

using namespace std;

int main(int argc, char **argv) {
  graph g(10);
  g.add_edge(5,2);
  g.add_edge(3,2);
  g.add_edge(4,2);
  g.add_edge(5,2);
  g.add_edge(4,3);
  g.add_edge(6,5);
  g.add_edge(9,7);
  g.add_edge(8,6);
  std::cout << "Hello, world! " << g.adj(2).size() << std::endl;
  std::cout<<g.expr("g")<<std::endl;
  
  cout<<"graph_deep_first_search()"<<endl;
  graph_deep_first_search(g,[](size_t i){cout<<i<<endl;},[](size_t i){cout<<"~"<<i<<endl;},[](size_t){cout<<"---"<<endl;});
  cout<<endl;
  
  cout<<"connect_components()"<<endl;
  auto ccp= connect_components(g);
  for(size_t i=0; i<ccp.size(); i++){
    cout<<i<<" ->group"<<ccp[i]<<endl;
  }
  cout<<endl;
  
  
  digraph dg(10);
  /*dg.add_edge(5,2);
  dg.add_edge(3,2);
  dg.add_edge(4,2);*/
  dg.add_edge(2,1);
  dg.add_edge(9,8);
  dg.add_edge(8,7);
  dg.add_edge(0,1);
  dg.add_edge(1,2);
  cout<<"toplogical_sort()"<<endl;
  auto tps= toplogical_sort(dg);
  for(size_t i=0; i<ccp.size(); i++){
    cout<<tps[i]<<" ";
  }
  cout<<endl;
  
  cout<<"strong_components()"<<endl;
  auto stcp= strong_components(dg);
  for(size_t i=0; i<stcp.size(); i++){
    cout<<i<<" ->group"<<stcp[i]<<endl;
  }
  cout<<endl;
  
  return 0;
}
