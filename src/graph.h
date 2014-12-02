#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <algorithm>
using namespace std;


class digraph{
public:
  vector<vector<size_t> > graph_e;
  size_t sum_of_edge;
  digraph(size_t v_size):graph_e(v_size,vector<size_t>()),sum_of_edge(0){}
  void add_edge(size_t v, size_t v2){
    graph_e[v].push_back(v2);
    sum_of_edge+=2;
  }
  const vector<size_t>& adj(size_t v){
    return graph_e[v];
  }
  size_t sum_v(){return graph_e.size();}
  size_t sum_e(){return sum_of_edge/2;}
  string expr(string name){
    stringstream ret;
    ret<<"digraph "<<name<<"("<<sum_v()<<");\n";
    for(size_t v=0; v<graph_e.size(); ++v){
      for(auto v2 : graph_e[v]){
	ret<<name<<".add_edge("<<v<<","<<v2<<");\n";
      }
    }
    return ret.str();
  }
  digraph& reverse(digraph& _out){
    for(size_t v=0; v<graph_e.size(); ++v){
      for(auto v2 : graph_e[v]){
	_out.add_edge(v2,v);
      }
    }
    return _out;
  }
};

class graph: public digraph{
public:
  graph(size_t v_size):digraph(v_size){}
  void add_edge(size_t v, size_t v2){
    graph_e[v].push_back(v2);
    graph_e[v2].push_back(v);
    sum_of_edge++;
  }
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
				  vector<bool>& node_marked){
  fbeg(cur);
  node_marked[cur]= true;
  
  for(auto i : g.adj(cur)){
    if(!node_marked[i]){
      graph_deep_first_search_iter(g,i,fbeg,fend,node_marked);
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
    graph_deep_first_search_iter(g,cur,[&](size_t cur){fbeg(cur);unmarked_node.erase(cur);},fend,node_marked);
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


vector<size_t> toplogical_sort(digraph& dg){
  graph& g= static_cast<graph&>(dg);
  vector<size_t> ret;
  graph_deep_first_search(g,[](size_t){}, [&](size_t cur){ret.push_back(cur);}, [](size_t){});
  std::reverse(ret.begin(),ret.end());
  return ret;
}


vector<size_t> strong_components(digraph& dg){
  digraph rg(dg.sum_v());
  dg.reverse(rg);
  auto reverse_toplogical= toplogical_sort(rg);
  
  vector<size_t> ret(dg.sum_v());
  vector<bool> node_marked(dg.sum_v(),false);
  size_t min_unmarked_index=0;
  size_t group_index=0;
  graph& g= static_cast<graph&>(dg);
  
  while(min_unmarked_index<g.sum_v()){
    size_t cur= min_unmarked_index;
    graph_deep_first_search_iter(g,reverse_toplogical[cur],[](size_t){},[&](size_t c){ret[c]=group_index;min_unmarked_index++;},node_marked);
    group_index++;
  }
  return ret;
}

