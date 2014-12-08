#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include "graph.h"
using namespace std;


class nfs_regex{
public:
  bool check(string s){
    if(s[0]!='(') return false;
    if(s[s.size()-1]!='0') return false;
    int count_p=0;
    for(size_t i=1; i<s.length()-1; i++){
      if(s[i]=='(') count_p++;
      else if(s[i]==')') count_p--;
      else if(s[i]=='*' && s[i-1]=='(') return false;
      else if(s[i]=='*' && s[i-1]=='|') return false;
      else if(s[i]=='*' && s[i-1]=='*') return false;
    }
    if(count_p!=0) return false;
    return true;
  }
  //need ilgal check for re
  nfs_regex(string _re)
  :nfs(_re.size()+1)
  ,re(_re){
    build_nfs();
  }
  digraph nfs;
  string re;
  void build_nfs(){
    vector<size_t> last_unit_stack;
    size_t last_unit=0;
    for(size_t i=0; i<re.size(); i++){
      if(re[i]=='('){
	nfs.add_edge(i,i+1);
	last_unit_stack.push_back(i);
      }else if(re[i]==')'){
	nfs.add_edge(i,i+1);
	while(!last_unit_stack.empty()){
	  size_t top= last_unit_stack.back();
	  last_unit_stack.pop_back();
	  if(re[top]=='|'){
	    nfs.add_edge(top,i);
	  }else if(re[top]=='('){
	    last_unit= top;
	    break;
	  }else{
	    //unknow
	  }
	}
      }else if(re[i]=='|'){
	for(size_t j=0; j<last_unit_stack.size(); j++){
	  if(re[last_unit_stack[last_unit_stack.size()-j-1]]=='('){
	    nfs.add_edge(last_unit_stack[last_unit_stack.size()-j-1],i+1);
	    break;
	  }
	}
	last_unit_stack.push_back(i);
      }else if(re[i]=='*'){
	nfs.add_edge(i,i+1);
	nfs.add_edge(i,last_unit);
	nfs.add_edge(last_unit,i);
      }else{
	last_unit=i;
      }
    }
    nfs.add_edge(re.size()-1,re.size());
  }
  bool match(string text){
    vector<bool> marked(nfs.sum_v());
    queue<size_t> q;
    q.push(0);
    for(size_t p=0; p<text.size(); p++){
      if(q.empty()) return false;
      fill(marked.begin(),marked.end(),false);
      mark_(marked,q);
      
      for(size_t i=0; i<marked.size(); i++){
	if(marked[i]){
	  if(re[i]==text[p] || re[i]=='.'){
	    q.push(i+1);
	  }
	}
      }
    }
    fill(marked.begin(),marked.end(),false);
    mark_(marked,q);
    return marked[marked.size()-1];
  }
  void mark_(vector<bool>& marked, queue<size_t>& q){
    while(!q.empty()){
      size_t cur= q.front();
      q.pop();
      if(!marked[cur]){
	marked[cur]=true;
	for(size_t i : nfs.adj(cur)){
	  if(!marked[i]) q.push(i);
	}
      }
    }
  }
};

bool nfs_regex_test(){
  cout<<"nfs_regex_test()"<<endl;
  nfs_regex nr("((A*B|AC)*D)");
  bool r= nr.match("ACD");
  cout<<"match result:"<<r<<endl;
  cout<<endl;
  return r==true;
}