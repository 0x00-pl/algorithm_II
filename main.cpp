#include <iostream>
#include "src/graph.h"
#include "src/heaps.h"
#include "src/minimum_spanning_tree.h"
#include "src/shortest_paths.h"
#include "src/maximum_flow.h"
#include "src/radix_sort.h"
#include "src/tries.h"
#include "src/substring_search.h"
using namespace std;

//#include "src/temp_pb43.h"

int main(int argc, char **argv) {
  graph g(10);
  g.add_edge(5,2);
  g.add_edge(3,2);
  g.add_edge(4,2);
  g.add_edge(5,2);
  g.add_edge(4,3);
  g.add_edge(6,5);
  g.add_edge(9,7);
  g.add_edge(8,6);
  std::cout << "Hello, world! " << g.adj(2).size() << std::endl;
  std::cout<<g.expr("g")<<std::endl;
  
  cout<<"graph_deep_first_search()"<<endl;
  graph_deep_first_search(g,[](size_t i){cout<<i<<endl;},[](size_t i){cout<<"~"<<i<<endl;},[](size_t){cout<<"---"<<endl;});
  cout<<endl;
  
  cout<<"connect_components()"<<endl;
  auto ccp= connect_components(g);
  for(size_t i=0; i<ccp.size(); i++){
    cout<<i<<" ->group"<<ccp[i]<<endl;
  }
  cout<<endl;
  
  
  digraph dg(10);
  dg.add_edge(2,1);
  dg.add_edge(9,8);
  dg.add_edge(8,7);
  dg.add_edge(0,1);
  dg.add_edge(1,2);
  cout<<"toplogical_sort()"<<endl;
  auto tps= toplogical_sort(dg);
  for(size_t i=0; i<ccp.size(); i++){
    cout<<tps[i]<<" ";
  }
  cout<<endl;
  
  cout<<"strong_components()"<<endl;
  auto stcp= strong_components(dg);
  for(size_t i=0; i<stcp.size(); i++){
    cout<<i<<" ->group"<<stcp[i]<<endl;
  }
  cout<<endl;
  
  cout<<"binheap"<<endl;
  binary_heap bh([](size_t l, size_t r){return l<r;});
  bh.insert(2);
  bh.insert(3);
  bh.insert(1);
  bh.insert(4);
  bh.insert(5);
  while(bh.size()>0){
    cout<<bh.min()<<" ";
    bh.pop();
  }
  cout<<endl;
  
  cout<<"fibheap"<<endl;
  fib_heap fh([](size_t l, size_t r){return l<r;});
  fh.insert(2);
  fh.insert(3);
  fh.insert(1);
  fh.insert(4);
  fh.insert(5);
  while(fh.size()>0){
    cout<<fh.min()<<" ";
    fh.pop();
  }
  cout<<endl;
  
  weight_graph weg(8);
  weg.add_edge(0, 7, 0.16);
  weg.add_edge(2, 3, 0.17);
  weg.add_edge(1, 7, 0.19);
  weg.add_edge(0, 2, 0.26);
  weg.add_edge(5, 7, 0.28);
  weg.add_edge(1, 3, 0.29);
  weg.add_edge(1, 5, 0.32);
  weg.add_edge(2, 7, 0.34);
  weg.add_edge(4, 5, 0.35);
  weg.add_edge(1, 2, 0.36);
  weg.add_edge(4, 7, 0.37);
  weg.add_edge(0, 4, 0.38);
  weg.add_edge(6, 2, 0.40);
  weg.add_edge(3, 6, 0.52);
  weg.add_edge(6, 0, 0.58);
  weg.add_edge(6, 4, 0.93);
  cout<<"kruskal_minimum_spanning_tree()"<<endl;
  weight_graph kru_mst(weg.sum_v());
  kruskal_minimum_spanning_tree(weg, kru_mst);
  cout<<kru_mst.expr();
  cout<<endl;
  
  cout<<"prim_minimum_spanning_tree()"<<endl;
  weight_graph prim_mst(weg.sum_v());
  prim_minimum_spanning_tree(weg, prim_mst);
  cout<<prim_mst.expr();
  cout<<endl;
  cout<<"prim_minimum_spanning_tree2()"<<endl;
  weight_graph prim_mst2(weg.sum_v());
  prim_minimum_spanning_tree(weg, prim_mst2);
  cout<<prim_mst2.expr();
  cout<<endl;
  
  
  weight_digraph wedg(8);
  wedg.add_edge(weight_dircted_edge(0, 1, 5.0 ));
  wedg.add_edge(weight_dircted_edge(0, 4, 9.0 ));
  wedg.add_edge(weight_dircted_edge(0, 7, 8.0 ));
  wedg.add_edge(weight_dircted_edge(1, 2, 12.0));
  wedg.add_edge(weight_dircted_edge(1, 3, 15.0));
  wedg.add_edge(weight_dircted_edge(1, 7, 4.0 ));
  wedg.add_edge(weight_dircted_edge(2, 3, 3.0 ));
  wedg.add_edge(weight_dircted_edge(2, 6, 11.0));
  wedg.add_edge(weight_dircted_edge(3, 6, 9.0 ));
  wedg.add_edge(weight_dircted_edge(4, 5, 4.0 ));
  wedg.add_edge(weight_dircted_edge(4, 6, 20.0));
  wedg.add_edge(weight_dircted_edge(4, 7, 5.0 ));
  wedg.add_edge(weight_dircted_edge(5, 2, 1.0 ));
  wedg.add_edge(weight_dircted_edge(5, 6, 13.0));
  wedg.add_edge(weight_dircted_edge(7, 5, 6.0 ));
  wedg.add_edge(weight_dircted_edge(7, 2, 7.0 ));
  
  cout<<"single_source_path()"<<endl;
  single_source_path ssp(wedg, 0);
  for(size_t i=0; i<ssp.dist.size(); i++){
    cout<<i<<" dist:"<<ssp.dist_to(i)<<" {"<<endl;
    auto paths= ssp.path_to(i);
    for(auto j : paths){
      cout<<j.expr()<<endl;
    }
    cout<<"}"<<endl;
  }
  cout<<endl;
    
  cout<<"maximum_flow_graph()"<<endl;
  maximum_flow_graph mfg(wedg);
  cout<<"max_flow= "<<mfg.ford_fulkerson(0,6)<<endl;
  cout<<endl;
  
  int unsort[]={9,5,6,8,7,3,2,1,4,0};
  vector<int> vunsort(unsort,unsort+(sizeof(unsort)/sizeof(*unsort)));
  cout<<"key_indexed_counting"<<endl;
  key_indexed_counting<int> kic_sorting(vunsort,10
  ,[](int n){return n;});
  cout<<"sorted:";
  for(size_t i=0; i<kic_sorting.sorted.size(); i++){
    cout<<kic_sorting.sorted[i]<<" ";
  }
  cout<<endl<<endl;
  
  
  
  cout<<"three_way_string_sort"<<endl;
  vector<string> vstr;
  vstr.push_back("void three_way_string_sort(vector<string>& sp, size_t b, size_t e, size_t dep){");
  vstr.push_back("  if(b>=e) return;");
  vstr.push_back("  char cur= sp[b][dep];");
  vstr.push_back("  size_t mb=b;");
  vstr.push_back("  size_t me=e;");
  vstr.push_back("  for(size_t i=b+1; i<me;){");
  vstr.push_back("    if(sp[i][dep] < cur){");
  vstr.push_back("      sp[mb].swap(sp[i]);");
  vstr.push_back("      mb++; i++;");
  vstr.push_back("    }");
  vstr.push_back("    else if(cur < sp[i][dep]){");
  vstr.push_back("      sp[me-1].swap(sp[i]);");
  vstr.push_back("      me--;");
  vstr.push_back("    }");
  vstr.push_back("    else{ i++; }");
  vstr.push_back("  }");
  vstr.push_back("  three_way_string_sort(sp,b,mb,dep);");
  vstr.push_back("  if(cur!='\0') three_way_string_sort(sp,mb,me,dep+1);");
  vstr.push_back("  three_way_string_sort(sp,me,e,dep);");
  vstr.push_back("}");
  vstr.push_back("void three_way_string_sort(vector<string>& sp){");
  vstr.push_back("  three_way_string_sort(sp,0,sp.size(),0);");
  vstr.push_back("}");
  
  three_way_string_sort(vstr);
  cout<<"sorted:"<<endl;
  for(size_t i=0; i<vstr.size(); i++){
    cout<<vstr[i]<<endl;
  }
  cout<<endl;
  
  cout<<"suffix_sort"<<endl;
  string suf_sort_str="A string object with a substring of this object.";
  suffix_sort suf_sort(suf_sort_str);
  suf_sort.show_all(suf_sort_str,90);
  cout<<endl;
  
  r_way_tries_test();
  kmp_search_test();
  boyer_moore_search_test();
  boyer_moore_full_search_test();
  
  return 0;
}
