#pragma once
#include <vector>
#include <string>
#include "heaps.h"
using namespace std;
/* un safe
char* bin2char(vector<bool>& bits){
  return (char*)&bits[0];
}*/

class bit_pool{
public:
  vector<char> pool;
  char last;
  size_t pos;
  bit_pool(){pos=0;}
  // maxlen is 16
  void push(uint32_t bit, int len){
    while(len>0){
      uint32_t t= bit>>pos;
      t= t>>32-8;
      last= (last&(~((1<<(8-pos))-1)))|(t&((1<<(8-pos))-1));
      if(pos+len>=8){
	len= len-(8-pos);
	bit= bit<<(8-pos);
	pos= 0;
	pool.push_back(last);
      }else{
        pos= pos+len;
	len= 0;
      }
    }
  }
  void at_eof(){
    pool.push_back(last);
    pool.push_back('\0');
    pool.push_back('\0');
    pool.push_back('\0');
  }
  //uint32_t pop(int len){}
  size_t size(){
    return pool.size()*8+pos;
  }
  uint32_t get(size_t p, int len){
    size_t b= p/8;
    size_t c= p%8;
    uint32_t r3= pool[b];
    uint32_t r2= pool[b+1];
    uint32_t r1= pool[b+2];
    uint32_t r0= pool[b+3];
    uint32_t ret= ((r3&((1<<8)-1))<<24) | ((r2&((1<<8)-1))<<16) | ((r1&((1<<8)-1))<<8) |(r0&((1<<8)-1));
    ret= ret<<c;
    return ret;
  }
  vector<char>& to_chars(){return pool;}
  string to_string(){
    char ret[pool.size()+1];
    ret[pool.size()]='\0';
    return string(ret);
  }
};


class huffman{
public:
  class node{
  public:
    char c;
    size_t weight;
    node* _0;
    node* _1;
    node* prev;
    node(node* __0, node* __1, size_t _weight, char _c):_0(__0),_1(__1),weight(_weight),c(_c),prev(nullptr){
      if(_0!=nullptr) _0->prev= this;
      if(_1!=nullptr) _1->prev= this;
    }
    bool leaf(){return _1==nullptr;}
    void push_bit_iter(node* root, vector<bool>& _out){
      if(prev!=root) prev->push_bit_iter(root,_out);
      if(prev->_0==this){
	_out.push_back(false);
      }else{
	_out.push_back(true);
      }
    }
  };
  vector<node*> node_pool;
  node* node_root;
  
  huffman(vector<bool>& huffman_tree_data)
  :node_pool(256,nullptr){
    size_t pos=0;
    node_root= bulid_tree_from_bits(huffman_tree_data,pos);
  }
  void vector_add_char(vector<bool>& _v,char c){
    for(size_t i=0; i<8; i++){
      _v.push_back((c>>7)&1);
      c= c<<1;
    }
  }
  char vector_get_char(vector<bool>& _v, size_t& pos){
    char c=0;
    for(size_t i=0; i<8; i++){
      bool bit= _v[pos++];
      c= (c<<1)|bit;
    }
    return c;
  }
  node* bulid_tree_from_bits(vector<bool>& _in, size_t& pos){
    node* ret;
    if(_in[pos++]){      
      char c= vector_get_char(_in,pos);
      ret= new node(nullptr,nullptr,1,c);
      node_pool[static_cast<size_t>(c)&((1<<8)-1)]= ret;
      return ret;
    }else{
      node* _0= bulid_tree_from_bits(_in,pos);
      node* _1= bulid_tree_from_bits(_in,pos);
      ret= new node(_0,_1,1,'\0');
      node_pool.push_back(ret);
      return ret;
    }
  }
  void export_tree_to_bits(node* n, vector<bool>& _out){
    if(n->leaf()){
      _out.push_back(true);
      vector_add_char(_out,n->c);
    }else{
      _out.push_back(false);
      export_tree_to_bits(n->_0, _out);
      export_tree_to_bits(n->_1, _out);
    }
  }
  void export_tree_to_bits(vector<bool>& _out){
    export_tree_to_bits(node_root,_out);
  }
  huffman(string& text)
  :node_pool(256,nullptr)
  ,node_root(nullptr){
    vector<size_t> fenq(256,0);
    bulid_fenq(fenq,text);
    for(size_t i=0; i<256; i++){
      node_pool[i]= new node(nullptr,nullptr,fenq[i],(char)i);
    }
    binary_heap hp([&](size_t a,size_t b){ return node_pool[a]->weight < node_pool[b]->weight; });
    for(size_t i=0; i<256; i++){
      hp.insert(i);
    }
    
    while(hp.size()>1){
      size_t n0= hp.min(); hp.pop();
      size_t n1= hp.min(); hp.pop();
      node_pool.push_back(new node(node_pool[n0], node_pool[n1], node_pool[n0]->weight+node_pool[n1]->weight,'\0'));
      hp.insert(node_pool.size()-1);
    }
    node_root= node_pool[hp.min()];
  }
  void bulid_fenq(vector<size_t>& fenq, string& text){
    for(size_t i=0; i<text.size(); i++){
      fenq[static_cast<size_t>(text[i])&((1<<8)-1)]++;
    }
  }
  ~huffman(){
    for(size_t i=0; i<node_pool.size(); i++){
      delete node_pool[i];
      node_pool[i]=nullptr;
    }
  }
  vector<bool>& zip(string& text, vector<bool>& _out){
    for(size_t i=0; i<text.size(); i++){
      node_pool[text[i]]->push_bit_iter(node_root, _out);
    }
  }
  string unzip(vector<bool>& _in, size_t len){
    string ret;
    size_t pos_of_in= 0;
    for(size_t i=0; i<len; i++){
      char c[]={'\0','\0'};
      c[0]= get_char(_in, pos_of_in);
      ret.append(c);
    }
    return ret;
  }
  char get_char(vector<bool>& _in, size_t& pos){
    node* p=node_root;
    while(!p->leaf()){
      if(!_in[pos]){
	p=p->_0;
      }else{
	p=p->_1;
      }
      pos++;
    }
    return p->c;
  }
};

bool huffman_test(){
  cout<<"huffman"<<endl;
  string text="banana";
  cout<<"zip():"<<text<<endl;
  vector<bool> text_z;
  vector<bool> tree_z;
  {
    huffman hf(text);
    hf.zip(text, text_z);
    hf.export_tree_to_bits(tree_z);
  }
  //------
  {
    huffman hf2(tree_z);
    string text2= hf2.unzip(text_z, text.size());
    cout<<"unzip():"<<text2<<endl<<endl;
  }
  return true;
}