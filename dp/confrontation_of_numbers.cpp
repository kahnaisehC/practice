#include<bits/stdc++.h>

using namespace std;

int main(){
  int n;
  cin >> n;
  vector<int>a(n);
  for(auto& a: a)cin >> a;
  vector<int>b(n);
  for(auto& a: b)cin >> a;


  

  int prev_a = 0;


  for(int i = 0; i < n; i++){
    prev_a = min(prev_a, b[i]);
    prev_b = max(prev_b, a[i]);
  }




}
