#pragma once
#include <string>
#include <map>
#include <iostream>
using namespace std;


class r_way_tries{
public:
  typedef int value_type;
  class node{
  public:
    value_type* val;
    map<char,node*> sub;
    node():val(0){}
    ~node(){
      delete val;
      for(auto i : sub){
	delete i.second;
	i.second=0;
      }
    }
  };
  node root;
  void _detach_all(node* n){
    n->val= nullptr;
    for(auto i : n->sub){
      _detach_all(i.second);
    }
  }
  void detach_all(){
    _detach_all(&root);
  }
  node* _set(string& key, value_type* val, node* n,size_t dep=0){
    if(n==nullptr) n=new node;
    if(key.size()==dep){
      n->val=val;
      return n;
    }
    char ck= key[dep];
    n->sub[ck]= _set(key,val,n->sub[ck],dep+1);
    return n;
  }
  void set(string& key, value_type* val){
    node* proot= &root;
    _set(key,val,proot);
  }
  value_type* _get(string& key, node* n, size_t dep=0){
    if(key.size()==dep) return n->val;
    char ck= key[dep];
    auto i= n->sub.find(ck);
    if(i==n->sub.end()) return nullptr;
    return _get(key, i->second, dep+1);
  }
  value_type* get(string& key){
    return _get(key,&root);
  }
  value_type* _pop(string& key, node* n, size_t dep=0){
    if(key.size()==dep){
      value_type* ret= n->val;
      n->val=nullptr;
      return ret;
    }
    char ck= key[dep];
    auto i= n->sub.find(ck);
    if(i==n->sub.end()) return nullptr;
    value_type* ret= _pop(key, i->second, dep+1);
    if(i->second->val==nullptr && i->second->sub.size()==0){
      delete i->second;
      n->sub.erase(i);
    }
    return ret;
  }
  value_type* pop(string& key){
    return _pop(key,&root);
  }
};


bool r_way_tries_test(){
  cout<<"r_way_tries_test()"<<endl;
  string spool[]={
    "123",
    "132",
    "231"
  };
  size_t spool_size= sizeof(spool)/sizeof(spool[0]);
  r_way_tries rwt;
  for(size_t i=0; i<spool_size; i++){
    rwt.set(spool[i], new int(i));
  }
  for(size_t i=0; i<spool_size; i++){
    int* c= rwt.get(spool[i]);
    *c= *c + 10;
  }
  for(size_t i=0; i<spool_size; i++){
    int* c= rwt.pop(spool[i]);
    cout<<*c<<" ";
    delete c;
  }
  cout<<endl<<endl;
  return true;
}


