#include<bits/stdc++.h>

using namespace std;

int n;
int l;
vector<vector<int>>adj;
vector<vector<int>>up;
vector<int>tin;
vector<int>tout;
vector<int>lvl;
int timer = 0;

void dfs(int curr, int p, int lv){
  tin[curr] = timer++;
  lvl[curr] = lv;

  up[curr][0] = p;
  for(int i = 1; i <= l; i++){
    up[curr][i] = up[up[curr][i-1]][i-1];
  }

  for(auto neigh: adj[curr]){
    if(neigh == p)continue;
    dfs(neigh, curr, lv+1);
  }
  tout[curr] = timer++;
}

// v is anc of u
bool is_anc(int v, int u){
  return tin[v] <= tin[u] && tout[v] >= tin[u];
}

int lca(int v, int u){
  if(is_anc(v, u))return v;
  if(is_anc(u, v))return u;

  for(int i = l; i >=0; i--){
    if(!is_anc(up[v][i], u))v = up[v][i];
  }

  return up[v][0];
}

 
int main(){
	int q;
	cin >> n >> q;
	l = ceil(log2(n));
 
	adj.assign(n, vector<int>());
	up.assign(n, vector<int>(l+1));
	lvl.resize(n);
	tin.resize(n);
	tout.resize(n);
 
	for(int i = 1; i < n; i++){
		int boss;
		cin >> boss;
		boss--;
		adj[i].push_back(boss);
		adj[boss].push_back(i);
	}
	dfs(0,0,0);
	while(q--){
		int a, b;
		cin >> a >> b;
		a--;b--;
		cout << lca(a,b)+1 << endl;
	}
 
	return 0;
}

