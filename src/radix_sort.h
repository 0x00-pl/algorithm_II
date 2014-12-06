#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <functional>
using namespace std;

template<typename T>
class key_indexed_counting{
public:
  vector<T> sorted;
  vector<size_t> count;
  key_indexed_counting(vector<T>& src, size_t key_range, function<size_t(T&)> fhash)
  :sorted(src.size())
  ,count(key_range,0){
    for(T& i : src){
      count[fhash(i)%key_range]++;
    }
    size_t n=0;
    for(size_t i=0; i<key_range; i++){
      size_t t=count[i];
      count[i]=n;
      n+=t;
    }
    for(size_t i=0; i<src.size(); i++){
      sorted[count[fhash(src[i])%key_range]++]=src[i];
    }
  }
};
void three_way_string_sort(vector<string>& sp, size_t b, size_t e, size_t dep){
  if(b>=e) return;
  char cur= sp[b][dep];
  size_t mb=b;
  size_t me=e;
  for(size_t i=b+1; i<me;){
    if(sp[i][dep] < cur){
      sp[mb].swap(sp[i]);
      mb++; i++;
    }
    else if(cur < sp[i][dep]){
      sp[me-1].swap(sp[i]);
      me--;
    }
    else{ i++; }
  }
  three_way_string_sort(sp,b,mb,dep);
  if(cur!='\0') three_way_string_sort(sp,mb,me,dep+1);
  three_way_string_sort(sp,me,e,dep);
}
void three_way_string_sort(vector<string>& sp){
  three_way_string_sort(sp,0,sp.size(),0);
}

class suffix_sort{
public:
  vector<size_t> sorted;
  vector<size_t> sorted_refback;
  class three_way_sort_task{
  public:
    size_t beg;
    size_t end;
    size_t dep;
  };
  suffix_sort(string& str)
  :sorted(str.size())
  ,sorted_refback(str.size()){
    queue<three_way_sort_task> task;
    key_indexed_sort_1(str,task);
    while(!task.empty()){
      three_way_sort_task cur_tsk= task.front(); task.pop();
      //cout<<"---"<<cur_tsk.beg<<" "<<cur_tsk.end<<" "<<cur_tsk.dep<<" "<<endl;//dbg
      //show_all(str,90);//dbg
      three_way_string_sort(cur_tsk.beg, cur_tsk.end, cur_tsk.dep, task);
    }
  }
  void key_indexed_sort_1(string& str, queue<three_way_sort_task>& task){
    vector<size_t> bkt(1<<8);
    for(size_t i=0; i<str.size(); i++){
      bkt[str[i]]++;
    }
    size_t sum=0;
    for(size_t i=0; i<1<<8; i++){
      size_t t= bkt[i];
      bkt[i]=sum;
      sum+=t;
    }
    vector<size_t> bkt_f(bkt.begin(),bkt.end());
    for(size_t i=0; i<str.size(); i++){
      sorted_refback[i]= bkt_f[str[i]];
      sorted[bkt[str[i]]]=i;
      bkt[str[i]]++;
    }
    size_t last=0;
    for(size_t i=0; i<bkt_f.size(); i++){
      //to run three_way_string_sort(last, bkt_f[i], 1, task)
      if(last < bkt_f[i]) task.push({last, bkt_f[i], 1});
      last= bkt_f[i];
    }
  }
  void swap_o(size_t i1, size_t i2){
    if(sorted_refback[sorted[i1]]!=sorted_refback[sorted[i2]]){
      cout<<"unknow error at swap_o("<<i1<<", "<<i2<<");"<<endl;
    }
    size_t t= sorted[i1];
    sorted[i1]= sorted[i2];
    sorted[i2]= t;
  }
  bool lees_o(size_t s1, size_t s2){
    return sorted_refback[s1]<sorted_refback[s2];
  }
  void three_way_string_sort(size_t b, size_t e, size_t dep, queue<three_way_sort_task>& task){
    if(b>=e){
      return;
    }
    size_t mb=b;
    size_t me=e;
    for(size_t i=b+1; i<me;){
      if(lees_o(sorted[i]+dep, sorted[mb]+dep)){
	swap_o(i,mb); mb++; i++;
      }
      else if(lees_o(sorted[mb]+dep, sorted[i]+dep)){
	swap_o(i,me-1); me--;
      }
      else{i++;}
    }
    for(size_t i=mb; i<me; i++){ sorted_refback[sorted[i]]=mb; }
    for(size_t i=me; i<e; i++){ sorted_refback[sorted[i]]=me; }
    three_way_string_sort(b,mb,dep,task);
    three_way_string_sort(me,e,dep,task);
    if((sorted[mb]+dep*2)<sorted.size()){
      //to run three_way_string_sort(mb,me,dep*2)
      task.push({mb,me,dep*2});
    }
  }
  void show_all(string& ostr, size_t maxlen){
    for(size_t i=0; i<sorted.size(); i++){
      cout<<sorted_refback[sorted[i]]<<":::"<<ostr.substr(sorted[i],maxlen)<<endl;
    }
  }
};
