#include<bits/stdc++.h>

using namespace std;

using ll = long long ;


struct MaxFlow {
  static const ll INF = 1e18;

  struct Edge {
    int from;
    int to;
    ll w;
  };


  int n, source ,sink;
  vector<vector<int>>g;
  vector<Edge> edges;
  vector<int> level;
  vector<int>ptr;
  ll flow = 0;


  MaxFlow(int n, int source, int sink): n(n), source(source), sink(sink), g(n), level(n), ptr(n) {}

  int add_edge(int from, int to, ll forward, ll backward = 0){
    const int id = (int)edges.size();
    g[from].emplace_back(id);
    edges.push_back({from, to, forward});
    g[to].emplace_back(id + 1);
    edges.push_back({to, from, backward});
    return id;
  }
  bool bfs(){
    level.assign(n, -1);
    queue<int>q;
    q.push(source);
    level[source] = 0;
    while(!q.empty()){
      int s = q.front();
      q.pop();
      for(int i : g[s]){
        auto &e = edges[i];
        if(level[e.to] == -1 && e.w > 0){
          level[e.to] = level[s]+1;
          q.push(e.to);
        }
      }
    }
    return level[sink] != -1;
  }
  ll dfs(int node, ll flow){

    if(node == sink)return flow;
    for(int &it  = ptr[node]; it < g[node].size(); ++it){
      int i = g[node][it];
      auto &e = edges[i];
      auto &back = edges[i ^ 1];
      if(e.w > 0  && level[e.to] == level[node] + 1){
        ll f = dfs(e.to, min(flow, e.w));
        if(f > 0){
          e.w -= f;
          back.w += f;
          return f;
        }
      }
    }
    return 0;
  }
  ll max_flow(){
    while(bfs()){
      ptr.assign(n, 0);
      ll f;
      while((f = dfs(source, INF)) > 0)flow += f;
    }
    return flow;
  }
};


int main(){

    int n, m;
    cin >> n >> m;

    MaxFlow flow(n, 0, n - 1);
    for (int i = 0; i < m; ++i) {
        int a, b, w;
        cin >> a >> b >> w;
        a--;
        b--;
        flow.add_edge(a, b, w, 0);
    }

    cout << flow.max_flow() << '\n';


}
