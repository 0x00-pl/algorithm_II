#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <iostream>
using namespace std;

class binheap{
public:
  typedef size_t Tvalue;
  typedef size_t Tnode;
  function<bool(Tvalue,Tvalue)> funcl;
  binheap(function<bool(Tvalue,Tvalue)> func_less):funcl(func_less){}
  void insert(Tvalue v){
    data.push_back(v);
    fix(data.size()-1);
  }
  Tvalue min(){
    return data[0];
  }
  void pop(){
    swap_value(0, data.size()-1);
    data.pop_back();
    fix_root();
  }
  void decrease_key(Tnode n, Tvalue v){
    data[n]=v;
    fix(n);
  }
  size_t size(){return data.size();}
private:
  size_t up(size_t cur){return (cur-1)/2;}
  size_t ldown(size_t cur){return cur*2+1;}
  size_t rdown(size_t cur){return cur*2+2;}
  void swap_value(size_t pa, size_t pb){
	Tvalue t= data[pa];
	data[pa]= data[pb];
	data[pb]= t;
  }
  void fix(size_t p){
    while(p>0){
      if(funcl(data[p],data[up(p)])){
	swap_value(p,up(p));
	p= up(p);
      }else{
	break;
      }
    }
  }
  void fix_root(){
    size_t p=0;
    while(p<data.size()){
      if(ldown(p)>=data.size()){break;}
      else if(rdown(p)>=data.size()){
	//l-child only, swap l
	swap_value(p,ldown(p));
	p= ldown(p);
      }else{
	if(funcl(data[ldown(p)],data[rdown(p)])){
	  //swap l
	  swap_value(p,ldown(p));
	  p= ldown(p);
	}else{
	  //swap r
	  swap_value(p,rdown(p));
	  p= rdown(p);
	}
      }
    }
  }
  vector<Tvalue> data;
};


class fib_heap{
public:
  typedef size_t Tvalue;
  class Tnode{
  public:
    Tvalue val;
    Tnode* up,*down,*left,*right;
    size_t size_of_down;
    bool marked;
    Tnode(Tvalue v){
      val=v;
      up=down=left=right=this;
      size_of_down=0;
      marked=false;
    }
    bool is_root(){
      return this==up;
    }
    //have not update up->size_of_down
    void merge(Tnode* n){
      for(Tnode* i=n; i!=n; i=i->right){
	if(up==this) i->up=i;
	else i->up=up;
      }
      Tnode* l=left;
      Tnode* nl=n->left;
      nl->right=this;
      left=nl;
      l->right=n;
      n->left=l;
    }
    void cut(){
      if(up!=this){
	up->size_of_down--;
	if(up->down==this){
	  if(right!=this){
	    up->down=right;
	  }else{
	    up->down=up;
	  }
	}
      }
      Tnode* r=right;
      Tnode* l=left;
      left->right= r;
      right->left= l;
      left=right=up=this;
    }
    ~Tnode(){
      cut();
      while(down!=this){
	delete down;
      }
    }
  };
  function<bool(Tvalue,Tvalue)> funcl;
  fib_heap(function<bool(Tvalue,Tvalue)> func_less):funcl(func_less){
    min_node=nullptr;
    _size=0;
  }
  ~fib_heap(){
    delete min_node;
  }
  Tnode* insert(Tvalue v){
    _size++;
    Tnode* p= new Tnode(v);
    if(min_node==nullptr){
      min_node= p;
      return p;
    }
    min_node->merge(p);
    if(funcl(p->val,min_node->val))
      min_node=p;
    return p;
  }
  Tvalue min(){
    return min_node->val;
  }
  void pop(){
    if(min_node==nullptr) return;
    _size--;
    if(min_node->down!=min_node){
      min_node->merge(min_node->down);
      min_node->down= min_node;
    }
    
    if(min_node->right==min_node){
      delete min_node;
      min_node= nullptr;
    }else{
      Tnode* old_min_node=min_node;
      min_node= min_node->right;
      old_min_node->cut();
      delete old_min_node;
      consolidate();
    }
  }
  void decreased_key(Tnode* n){
    if(n->up==n) return;
    if(funcl(n->up->val,n->val)) return;
    n->cut();
    n->marked=false;
    min_node->merge(n);
    mark(n->up);
  }
  size_t size(){return _size;}
private:
  void mark(Tnode* n){
    if(n->up==n) return;
    if(!n->marked){
      n->marked=true; 
      return;
    }
    n->cut();
    n->marked=false;
    min_node->merge(n);
    mark(n->up);
  }
  void consolidate(){
    if(min_node==nullptr) return;
    vector<Tnode*> pool(32,nullptr);// size of log(n)
    Tnode* new_min_node= min_node;
    for(Tnode* i=min_node; i!=min_node;){
      Tnode* cur=i;
      i=i->right;
      if(pool[cur->size_of_down]==nullptr){
	pool[cur->size_of_down]= cur;
      }else{
	if(funcl(pool[cur->size_of_down]->val, cur->val)){
	  cur->cut();
	  merge_tree(pool[cur->size_of_down],cur);
	}else{
	  pool[cur->size_of_down]->cut();
	  merge_tree(cur, pool[cur->size_of_down]);
	  pool[cur->size_of_down]= cur;
	}
      }
    }
    for(Tnode* i=min_node; i!=min_node;i=i->right){
      if(funcl(i->val, new_min_node->val))
	new_min_node=i;
    }
    min_node=new_min_node;
  }
  //subtree need to be cut()
  void merge_tree(Tnode* dst, Tnode* subtree){
    subtree->up=dst;
    if(dst->down==dst){
      dst->down=subtree;
    }else{
      dst->down->merge(subtree);
      dst->size_of_down++;
    }
  }
  Tnode* min_node;
  size_t _size;
};

