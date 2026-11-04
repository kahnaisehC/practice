#include<bits/stdc++.h>

using namespace std;



void solve(int l, int r, vector<pair<int,int>>&ans){
  int n = r-l+1;
    if(n <= 1){
      return;
    }
    if(n == 2){
      ans.push_back({l, l});
      return;
    }
    if(n == 3){
      ans.push_back({l, l});
      ans.push_back({l+1, l+1});
      return;
    }
    if(n == 4){
      ans.push_back({l, l+1});
      ans.push_back({l+1, l+2});
      return;
    }

    int b = 1;
    while(b*4 < n){
      b*=4;
    }
    solve(l, min(l+b-1, r), ans);
    solve(l+b, min(l+2*b-1, r), ans);
    solve(l+2*b, min(l+3*b-1, r), ans);
    solve(l+3*b, min(l+4*b-1, r), ans);
}


int main(){
  int t;
  cin >> t;
  while(t--){
    int n, m;
    cin >> n >> m;
    vector<pair<int,int>>ans;
    if(n == 1){
      cout << "0" << endl;
      continue;
    }
    int half = n/2 + n%2;
    for(int i = 1; i <= half; i++){
      ans.push_back({i, i+half-1});
    }
    cout << ans.size() << endl;
    for(auto [l, r]: ans){
      cout << l << " " << r << endl;
    }

  }

}
