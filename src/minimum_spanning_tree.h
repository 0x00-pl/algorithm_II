#pragma once
#include <algorithm>
#include <sstream>
#include "heaps.h"
#include "union_set.h"



class weight_edge{
public:
  size_t v1,v2;
  double weight;
  weight_edge()=default;
  weight_edge(size_t _v1, size_t _v2, double _weight){
    v1=_v1;
    v2=_v2;
    weight=_weight;
  }
  size_t other(size_t v){
    if(v==v1)
      return v2;
    return v1;
  }
  bool lees(const weight_edge& o) const {
    return weight<o.weight;
  }
};


class weight_graph{
public:
  vector<vector<size_t> > edge_data;
  vector<weight_edge> pool_of_edge;
  weight_graph(size_t v):edge_data(v){}
  size_t sum_v(){return edge_data.size();}
  size_t sum_e(){return pool_of_edge.size();}
  void add_edge(size_t v1, size_t v2, double weight){
    pool_of_edge.push_back(weight_edge(v1,v2,weight));
    edge_data[v1].push_back(sum_e()-1);
    edge_data[v2].push_back(sum_e()-1);
  }
  vector<size_t>& adj(size_t v){
    return edge_data[v];
  }
  string expr(){
    stringstream ss;
    ss<<"weight_edge_graph("<<sum_v()<<");"<<endl;
    for(size_t i=0; i<sum_e(); i++){
      ss<<".add_edge("<<pool_of_edge[i].v1<<", "<<pool_of_edge[i].v2<<", "<<pool_of_edge[i].weight<<");\n";
    }
    return ss.str();
  }
};

weight_graph kruskal_minimum_spanning_tree(weight_graph& src, weight_graph& _out){
  vector<weight_edge> edges(src.pool_of_edge.begin(), src.pool_of_edge.end());
  sort(edges.begin(),edges.end(),[](const weight_edge& e1, const weight_edge& e2){return e1.lees(e2);});
  union_set us(src.sum_v());
  for(size_t i=0; i<edges.size(); i++){
    if(us.find(edges[i].v1)!=us.find(edges[i].v2)){
      us.union_node(edges[i].v1, edges[i].v2);
      _out.add_edge(edges[i].v1, edges[i].v2, edges[i].weight);
    }
  }
  return _out;
}

void prim_minimum_spanning_tree_mark(weight_graph& src, size_t v, vector<bool>& mark_v, binary_heap& edge_edges){
  mark_v[v]=true;
  auto adjs= src.adj(v);
  for(auto i : adjs){
    if(mark_v[ src.pool_of_edge[i].other(v) ] == false)
      edge_edges.insert(i);
  }
}

weight_graph prim_minimum_spanning_tree(weight_graph& src, weight_graph& _out){
  vector<bool> mark_v(src.sum_v(),false);
  binary_heap edge_edges(
    [&](size_t e1,size_t e2)->bool{
      return src.pool_of_edge[e1].lees(src.pool_of_edge[e2]);
    });
  prim_minimum_spanning_tree_mark(src,0, mark_v, edge_edges);
  while(_out.sum_e() < src.sum_v()-1){
    if(edge_edges.size()==0){
      cout<<"error, graph not connected.";
    }
    
    size_t cur_edge_index= edge_edges.min();
    edge_edges.pop();
    weight_edge& cur_edge= src.pool_of_edge[cur_edge_index];
    
    if(mark_v[cur_edge.v1] && mark_v[cur_edge.v2])
      continue;//ignore looped edge
    
    size_t next_v;
    if( mark_v[cur_edge.v1] ) next_v= cur_edge.v2;
    else next_v= cur_edge.v1;
  
    prim_minimum_spanning_tree_mark(src, next_v, mark_v, edge_edges);
    _out.add_edge(cur_edge.v1, cur_edge.v2, cur_edge.weight);
  }
  return _out;
}


void prim_minimum_spanning_tree2_mark(weight_graph& src, size_t v, vector<bool>& mark_v, fib_heap& edge_edges){
  mark_v[v]=true;
  auto adjs= src.adj(v);
  for(auto i : adjs){
    if(mark_v[ src.pool_of_edge[i].other(v) ] == false)
      edge_edges.insert(i);
  }
}

weight_graph prim_minimum_spanning_tree2(weight_graph& src, weight_graph& _out){
  vector<bool> mark_v(src.sum_v(),false);
  fib_heap edge_edges(
    [&](size_t e1,size_t e2)->bool{
      return src.pool_of_edge[e1].lees(src.pool_of_edge[e2]);
    });
  prim_minimum_spanning_tree2_mark(src,0, mark_v, edge_edges);
  while(_out.sum_e() < src.sum_v()-1){
    if(edge_edges.size()==0){
      cout<<"error, graph not connected.";
    }
    
    size_t cur_edge_index= edge_edges.min();
    edge_edges.pop();
    weight_edge& cur_edge= src.pool_of_edge[cur_edge_index];
    
    if(mark_v[cur_edge.v1] && mark_v[cur_edge.v2])
      continue;//ignore looped edge
    
    size_t next_v;
    if( mark_v[cur_edge.v1] ) next_v= cur_edge.v2;
    else next_v= cur_edge.v1;
  
    prim_minimum_spanning_tree2_mark(src, next_v, mark_v, edge_edges);
    _out.add_edge(cur_edge.v1, cur_edge.v2, cur_edge.weight);
  }
  return _out;
}