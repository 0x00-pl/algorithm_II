#pragma once
#include <sstream>
#include <vector>
#include <queue>
#include "shortest_paths.h"

using namespace std;



class flow_edge{
public:
  size_t src;
  size_t dst;
  double maxflow;
  double curflow;
  
  flow_edge(){}
  flow_edge(size_t _src, size_t _dst, double _maxflow)
  :src(_src),dst(_dst),maxflow(_maxflow),curflow(0){}
  flow_edge(weight_dircted_edge& e)
  :src(e.src),dst(e.dst),maxflow(e.weight),curflow(0){}
  
  size_t other(size_t v){
    if(v==src) return dst;
    if(v==dst) return src;
    cout<<"value error: flow_edge::other(v) v is not src nor dst."<<endl;
  }
  
  double residual_capacity_to(size_t v){
    if(v==dst) return maxflow-curflow;
    if(v==src) return curflow;
    cout<<"value error: flow_edge::residual_capacity_to(v) v is not src nor dst."<<endl;
  }
  void add_flow_to(size_t v, double delta){
    if(v==dst) curflow+=delta; 
    else if(v==src) curflow-=delta;
    else cout<<"value error: flow_edge::add_flow_to(v) v is not src nor dst."<<endl;
  }
};

class maximum_flow_graph{
public:
  typedef size_t i_edge;
  vector<vector<i_edge> > v_adj;
  vector<flow_edge> edges;
  maximum_flow_graph(weight_digraph& g)
  :v_adj(g.sum_v())
  ,edges(g.sum_e())
  {
    for(size_t i=0; i<g.sum_e(); i++){
      edges[i]= flow_edge(g.edges()[i]);
      v_adj[edges[i].src].push_back(i);
      v_adj[edges[i].dst].push_back(i);
    }
  }
  size_t sum_v(){return v_adj.size();}
  size_t sum_e(){return edges.size();}
  double ford_fulkerson(size_t vbegin, size_t vend){
    double max_flow=0;
    vector<i_edge> path_to(sum_v());
    while(find_path_bfs(vbegin, vend, path_to)){
      double path_maxmim_flow=0;
      for(size_t i=vend; i!=vbegin; i=edges[path_to[i]].other(i)){
	path_maxmim_flow= max(path_maxmim_flow, edges[path_to[i]].residual_capacity_to(i));
      }
      for(size_t i=vend; i!=vbegin; i=edges[path_to[i]].other(i)){
	edges[path_to[i]].add_flow_to(i,path_maxmim_flow);
      }
      max_flow+= path_maxmim_flow;
    }
    return max_flow;
  }
  
  bool find_path_bfs(size_t vbegin, size_t vend, vector<i_edge>& path_to){
    vector<bool> marked(sum_v(), false);
    queue<size_t> q;
    q.push(vbegin);
    marked[vbegin]=true;
    
    while(!q.empty()){
      if(marked[vend]) return true;
      
      size_t cur_v= q.front(); q.pop();
      marked[cur_v]=true;
      
      vector<i_edge>& cur_adj= v_adj[cur_v];
      for(i_edge cur_i_edge : cur_adj){
	size_t other_v= edges[cur_i_edge].other(cur_v);
	if(edges[cur_i_edge].residual_capacity_to(other_v)>0 && !marked[other_v]){
	    path_to[other_v]= cur_i_edge;
	    q.push(other_v);
	}
      }
    }
    return false;
  }
};