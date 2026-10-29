#include<bits/stdc++.h>

using namespace std;

int main(){
  ios::sync_with_stdio(NULL);
  cin.tie(0);


  int n,m;
  cin >> n >>m;
  int p, g;
  cin >> p >> g;
  p--;g--;


  vector<vector<pair<int, int>>>adj(n);
  for(int i = 0; i < m;i++){
    int u, v, d;
    cin >> u >> v >> d;
    u--;v--;
    adj[u].push_back({v, d});
    adj[v].push_back({u, d});
  }

  vector<long long>dist(n, -1);
  priority_queue<
    pair<long long, int>,
    vector<pair<long long, int>>,
    greater<pair<long long,int>>
    >pq;
  pq.push({0, p});

  while(!pq.empty()){
    int curr = pq.top().second;
    long long dst = pq.top().first;
    pq.pop();

    if(dist[curr] != -1)continue;
    dist[curr] = dst;

    for(auto [neigh, d]: adj[curr]){
      if(dist[neigh] != -1)continue;
      pq.push({d+dst, neigh});
    }
  }

  vector<long long>dist_o(n, -1);
  pq.push({0, p});

  while(!pq.empty()){
    int curr = pq.top().second;
    long long dst = pq.top().first;
    pq.pop();

    if(curr == g)continue;
    if(dist_o[curr] != -1)continue;
    dist_o[curr] = dst;

    for(auto [neigh, d]: adj[curr]){
      if(curr == g)continue;
      if(dist_o[neigh] != -1)continue;
      pq.push({d+dst, neigh});
    }
  }
  vector<int>ans;
  for(int i = 0; i < n;i++){
    if(dist[i] != dist_o[i] && dist[i] == dist[g]*2)ans.push_back(i+1);
  }
  if(ans.size() == 0){
    cout << "*" <<endl;
  }else{
    for(auto v: ans){
      cout << v << " ";
    }
    cout << endl;

  }

  

}

