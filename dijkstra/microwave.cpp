#include<bits/stdc++.h>


using namespace std;

int main(){
  ios::sync_with_stdio(NULL);
  cin.tie(0);

  int n, m, k;
  cin >> n >> m >> k;


  vector<vector<pair<int, int>>>adj(n);
  vector<vector<pair<int, int>>>adjw(n);

  for(int i =0 ;i < m; i++){
    int u, v, f, w;
    cin >> u >> v >> f >> w;
    u--;v--;
    adj[u].push_back({v, f});
    adj[v].push_back({u, f});
    if(w != -1){
      adjw[u].push_back({v, w});
      adjw[v].push_back({u, w});
    }
  }

  priority_queue<
    pair<long long, int>,
    vector<pair<long long,int>>,
    greater<pair<long long, int>>
    >pq;

  long long mn = LLONG_MAX;
  vector<long long>dist(11*n+10, -1);
  pq.push({0, 0});
  while(!pq.empty()){
    int curr = pq.top().second;
    long long dst= pq.top().first;
    pq.pop();

    if(dist[curr] != -1)continue;
    dist[curr] = dst;
    int curr_node = curr%n;
    int w_used = curr/n;
    if(curr_node == n-1){
      mn = dst;
      break;
    }

    for(auto [neigh, d]: adj[curr_node]){
      if(dist[neigh + w_used*n] != -1)continue;
      pq.push({
          dst+d,
          neigh+w_used*n
          });
    }
    if(w_used < k){
      for(auto [neigh, d]: adjw[curr_node]){
        if(dist[neigh + w_used*n+n] != -1)continue;
        pq.push({
            dst+d,
            neigh+w_used*n+n
          });
      }
    }
  }
  cout << mn << endl;




}

