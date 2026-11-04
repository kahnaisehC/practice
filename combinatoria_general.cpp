#include<bits/stdc++.h>
using namespace std;
//Propiedades Basicas de numeros combinatorios
C(n,k) = C(n-1,k-1) + C(n-1,k)
C(n,k) = C(n,n-k)
//________calcular cualquier combinatorio C(a,b)
//con 0<=b<=a<=1e6 en o(1) con precomputo de O(n+logMOD) creo
//usa mpow que está en teoria de numeros
//NO OLVIDES INICIALIZAR PRECOMPUTO()
const long long MOD = 1e9+7;//ojo este
long long MAXN = 1e6+100;


vector<long long> factorial(MAXN+1);
vector<long long> inversos(MAXN+1);
void precomputo(){

  factorial[0] = 1;
  for(int i =1; i <= MAXN; i++){
    factorial[i] = (factorial[i-1]*i)%MOD;
  }

  inversos[MAXN] = mpow(factorial[MAXN], MOD-2, MOD);
  for(int i = MAXN-1; i >=0; i--){
    inversos[i] = (inversos[i+1]*(i+1))%MOD;
  } 
}
long long combinatorio(long long a, long long b){
  return (
      (factorial[a]*inversos[b]%MOD)*
      inversos[a-b])%MOD;
}

