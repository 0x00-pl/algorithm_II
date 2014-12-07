#pragma once
#include <vector>
#include <algorithm>
using namespace std;


bool my_next_permutation(vector<int>& dig){
  for(size_t i=9; i>0; i--){
    if(dig[i-1]<dig[i]){
      size_t nl=10;
      while(dig[i-1]>=dig[--nl]){}
      int t= dig[i-1];
      dig[i-1]= dig[nl];
      dig[nl]= t;
      for(size_t n=0; (i+(n<<1))<9 ;n++){
	t= dig[i+n];
	dig[i+n]= dig[9-n];
	dig[9-n]= t;
      }
      return true;
    }
  }
  return false;
}

uint64_t f43(){
  uint64_t sum=0;
  vector<int> dig(10);
  dig[0]=0;
  dig[1]=1;
  dig[2]=2;
  dig[3]=3;
  dig[4]=4;
  dig[5]=5;
  dig[6]=6;
  dig[7]=7;
  dig[8]=8;
  dig[9]=9;
  do{
    if(dig[3]%2==0
      && (dig[2]*100+dig[3]*10+dig[4]) %3==0
      && dig[5] %5==0
      && (dig[4]*100+dig[5]*10+dig[6]) %7==0
      && (dig[5]*100+dig[6]*10+dig[7]) %11==0
      && (dig[6]*100+dig[7]*10+dig[8]) %13==0
      && (dig[7]*100+dig[8]*10+dig[9]) %17==0
    ){
      sum+=1;
    }
  }while(my_next_permutation(dig));
  //}while(next_permutation(dig.begin(),dig.end()));
  return sum;
}