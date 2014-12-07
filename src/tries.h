#pragma once
#include <string>
#include <map>
using namespace std;


class r_way_tries{
public:
  typedef int value_type;
  class node{
  public:
    value_type* val;
    map<char,node*> sub;
    node():val(0){}
  };
  node root;
  
  void _set(string& key, value_type* val, node* n,size_t dep=0){
    if(n==nullptr)
    if(key.size()==dep) n->val=val;
    char ck= key[dep];
    _set(key,val,n->sub[ck],dep+1);
  }
  value_type* _get(string key, node* n, size_t dep=0){}
  value_type* _pop(string key, node* n, size_t dep=0){}
};