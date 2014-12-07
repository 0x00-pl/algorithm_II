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
      // all 2 if is necessary.
      if(text[i]!=patt[pb]){
	pb= fail_back(pb,text[i]);
      }
      if(text[i]==patt[pb]){ 
	pb++; 
      }
      if(pb>=patt.size()) 
	return i+1-patt.size();
    }
    return text.size();
  }
};

class boyer_moore{
public:
  vector<size_t> _last_ch_at;
  string patt;
  boyer_moore(string& str)
  :_last_ch_at(256,str.size())
  ,patt(str){
    bulid_last_ch_at();
  }
  void bulid_last_ch_at(){
    for(size_t i=0; i<patt.size(); i++){
      _last_ch_at[patt[i]]= i;
    }
  }
  size_t search(string& text){
    size_t p=0;
    while(p<text.size()){
      size_t delta;
      for(size_t i=0; i<patt.size(); i++){
	size_t rfp= patt.size()-1-i; //reverse finding ptr
	if(patt[rfp]!=text[p+rfp]){
	    if(_last_ch_at[text[p+rfp]] > rfp){
	      delta=1;
	    }else{
	      delta= patt.size()-_last_ch_at[text[p+rfp]]-1;
	    }
	    break;
	}
	if(rfp==0) return p;
      }
      p+=delta;
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
bool boyer_moore_search_test(){
  cout<<"boyer_moore_search()"<<endl;
  string patt("nana");
  string text("0123456789banana");
  boyer_moore bms(patt);
  size_t r= bms.search(text);
  cout<<"12=="<<r<<endl;
  cout<<endl;
  return 12==r;
}