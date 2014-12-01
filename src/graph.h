#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
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