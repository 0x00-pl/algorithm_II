#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <limits>

#include "heaps.h"
using namespace std;

class weight_dircted_edge{
public:
  size_t src;
  size_t dst;
  double weight;
  weight_dircted_edge(){}
  weight_dircted_edge(size_t _src, size_t _dst, double _weight){
    src=_src;
    dst=_dst;
    weight=_weight;
  }
  string expr(){
    stringstream ss;
    ss<<"weight_dircted_edge("<<src<<", "<<dst<<", "<<weight<<");";
    return ss.str();
  }
};

class weight_digraph{
public:
  typedef size_t i_edge;
  vector<vector<i_edge> > v_adj;
  vector<weight_dircted_edge> edges_pool;
  weight_digraph(size_t _v):v_adj(_v,vector<i_edge>()){}
  size_t sum_v(){ return v_adj.size(); }
  size_t sum_e(){ return edges_pool.size(); }
  void add_edge(weight_dircted_edge edge){
    edges_pool.push_back(edge);
    v_adj[edge.src].push_back(edges_pool.size()-1);
  }
  vector<i_edge>& adj(size_t v){ return v_adj[v]; }
  vector<weight_dircted_edge>& edges(){ return edges_pool; }
};


class single_source_path{
public:
  vector<bool> marked;
  vector<double> dist;
  vector<weight_dircted_edge> edge_to;
  binary_heap bhpq;
  size_t start_from;
  
  single_source_path(weight_digraph& g, size_t v)
  :dist(g.sum_v(),numeric_limits<double>::infinity())
  ,start_from(v)
  ,edge_to(g.sum_v())
  ,marked(g.sum_v(),false)
  ,bhpq([&](size_t v1, size_t v2){ return dist_to(v1)<dist_to(v2); })
  {
    dist[start_from]=0;
    bhpq.insert(start_from);
    
    while(bhpq.size()>0){
      size_t cur_v=bhpq.min();
      bhpq.pop();
      if(marked[cur_v]) continue;
      marked[cur_v]=true;
      
      for(size_t i : g.adj(cur_v)){
	weight_dircted_edge& cur_edge= g.edges_pool[i];
	if( dist[cur_v] + cur_edge.weight < dist[cur_edge.dst] ){
	  dist[cur_edge.dst]= dist[cur_v] + cur_edge.weight;
	  edge_to[cur_edge.dst]= cur_edge;
	  bhpq.insert(cur_edge.dst);
	}
      }
    }
  }
  bool is_path_to(size_t v){
    return dist[v] != numeric_limits<double>::infinity();
  }
  double dist_to(size_t v){
    return dist[v];
  }
  vector<weight_dircted_edge> path_to(size_t v){
    vector<weight_dircted_edge> ret;
    for(size_t i=v; i!=start_from; i=edge_to[i].src){
      ret.push_back(edge_to[i]);
    }
    return ret;
  }
};



