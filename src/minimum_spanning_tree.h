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
  bool lees(weight_edge& o){
    return weight<o.weight;
  }
};


class weight_edge_graph{
public:
  vector<vector<size_t> > edge_data;
  vector<weight_edge> pool_of_edge;
  weight_edge_graph(size_t v):edge_data(v){}
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
      ss<<".add_edge("<<pool_of_edge[i].v1<<", "<<pool_of_edge[i].v2<<", "<<pool_of_edge[i].weight<<");";
    }
    return ss.str();
  }
};

weight_edge_graph kruskal_minimum_spanning_tree(weight_edge_graph& src, weight_edge_graph& _out){
  vector<weight_edge> edges(src.pool_of_edge.begin(),src.pool_of_edge.end());
  sort(edges.begin(),edges.end(),[](weight_edge& e1,weight_edge& e2){return e1.lees(e2);});
  union_set us(src.sum_v());
  for(size_t i=0; i<src.sum_v(); i++){
    if(us.find(edges[i].v1)!=us.find(edges[i].v2)){
      us.union_node(edges[i].v1, edges[i].v2);
      _out.add_edge(edges[i].v1,edges[i].v2,edges[i].weight);
    }
  }
  return _out;
}

