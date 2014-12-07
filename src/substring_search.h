#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;




class kmp_search{
public:
  vector<size_t> fail_back_table;
  string patt;
  kmp_search(string& s)
  :fail_back_table(s.size())
  ,patt(s){
    bulid_fail_back_table();
  }
  size_t fail_back(size_t p, char c){
    //find fail_back of c from p
    while(patt[p]!=c && p!=fail_back_table[p])
      p=fail_back_table[p];
    return p;
  }
  void bulid_fail_back_table(){
    vector<size_t>& fbt= fail_back_table;
    fbt[0]=0;
    size_t pb=0;
    for(size_t i=1; i<fbt.size(); i++){
      if(patt[i]==patt[pb]){
	fbt[i]= fbt[pb];
	pb++;
      }else{
	fbt[i]= pb+1;
	pb= fail_back(pb,patt[i]);
      }
    }
  }
  size_t search(string& text){
    size_t pb=0;
    for(size_t i=0; i<text.size(); i++){
      if(text[i]==patt[pb]){
	pb++;
      }else{
	pb= fail_back(pb,text[i]);
	if(text[i]==patt[pb]) { pb++; }
      }
      if(pb>=patt.size()) return i-patt.size()+1;
    }
    return text.size();
  }
};


bool kmp_search_test(){
  cout<<"kmp_search()"<<endl;
  string patt("aaaab");
  string text("aaaaaaab");
  kmp_search kmps(patt);
  size_t r= kmps.search(text);
  cout<<"3=="<<r<<endl;
  cout<<endl;
  return 3==r;
}