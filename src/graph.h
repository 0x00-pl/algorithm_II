#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <functional>
using namespace std;


class graph{
public:
  vector<vector<size_t> > graph_e;
  size_t sum_of_edge;
  graph(size_t v_size):graph_e(v_size,vector<size_t>()),sum_of_edge(0){}
  void add_edge(size_t v, size_t v2){
    graph_e[v].push_back(v2);
    graph_e[v2].push_back(v);
    sum_of_edge++;
  }
  const vector<size_t>& adj(size_t v){
    return graph_e[v];
  }
  size_t sum_v(){return graph_e.size();}
  size_t sum_e(){return sum_of_edge;}
  string expr(string name){
    stringstream ret;
    ret<<"graph "<<name<<"("<<sum_v()<<");\n";
    for(size_t v=0; v<graph_e.size(); ++v){
      for(auto v2 : graph_e[v]){
	if(v2<=v) ret<<name<<".add_edge("<<v<<","<<v2<<");\n";
      }
    }
    return ret.str();
  }
};

void graph_deep_first_search_iter(graph& g, size_t cur, function<void(size_t)> fbeg, function<void(size_t)> fend,
				  vector<bool>& node_marked, set<size_t>& unmarked_node){
  fbeg(cur);
  node_marked[cur]= true;
  unmarked_node.erase(cur);
  
  for(auto i : g.adj(cur)){
    if(!node_marked[i]){
      graph_deep_first_search_iter(g,i,fbeg,fend,node_marked,unmarked_node);
    }
  }
  fend(cur);
}
void graph_deep_first_search(graph& g, function<void(size_t)> fbeg,
			     function<void(size_t)> fend, function<void(size_t)> fnextgroup){
  vector<bool> node_marked(g.sum_v(),false);
  set<size_t> unmarked_node;
  for(size_t i=0; i<g.sum_v(); i++){
    unmarked_node.insert(i);
  }
  
  while(!unmarked_node.empty()){
    size_t cur= *unmarked_node.rbegin(); //back()
    graph_deep_first_search_iter(g,cur,fbeg,fend,node_marked,unmarked_node);
    fnextgroup(cur);
  }
}
void graph_deep_first_search(graph& g){
  graph_deep_first_search(g,[](size_t){},[](size_t){},[](size_t){});
}

vector<size_t> connect_components(graph& g){
  vector<size_t> ret(g.sum_v());
  size_t group_index=0;
  graph_deep_first_search(g,[&](size_t cur){ret[cur]=group_index;},[](size_t){},[&](size_t){group_index++;});
  return ret;
}