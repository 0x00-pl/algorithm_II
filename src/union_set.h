#pragma once
#include <vector>
using namespace std;


class union_set{
public:
  vector<size_t> parent;
  union_set(size_t _size):parent(_size){
    for(size_t i=0; i<_size; i++){
      parent[i]=i;
    }
  }
  size_t find(size_t n){
    if(parent[n]!=n){
      parent[n]= find(parent[n]);
    }
    return parent[n];
  }
  void union_node(size_t n, size_t n2){
    parent[find(n2)]=parent[find(n)];
  }
};