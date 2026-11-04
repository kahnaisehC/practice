#include<bits/stdc++.h>
using namespace std;
//________tener rapido la factorizacion en primos de muchos numeros hasta 2e5____________________
const int MAXN = 1e6+1;//este lo podes cambiar
int min_div[MAXN];
void div(){
  min_div[1] = 1;
  for(int i = 2; i <MAXN; i++){
    if(min_div[i]==0){
      for(int j = i; j < MAXN; j+=i)min_div[j] = i;
    }
  }
}

void factorizar(int x, vector<int> &factores){
//x es el numero a factorizar y factores es el array donde los guarda.
//para usarlo NO OLVIDES INICIALIZAR DIV()
  if(x == 1){
    factores.push_back(1);
    return;
  }
  int i = min_div[x];
  while(x%i == 0){
    factores.push_back(i);
    x/= i;
  }
  factorizar(x, factores);
}
//____________numero de divisores de muchos numeros hasta 2e5
const int MAXN = 1e6+1;//este lo podes cambiar
int min_div[MAXN];

void div(){
  min_div[1] = 1;
  for(int i = 2; i <MAXN; i++){
    if(min_div[i]==0){
      for(int j = i; j < MAXN; j+=i)min_div[j] = i;
    }
  }
}
void count_divs(int x, int &count){
//cout << x << endl;
int r = 0;
if(x == 1){
return;
}
int i = min_div[x];
while(x%i == 0){
r++;
x/= i;
}
count*=(r+1);
count_divs(x, count);
}
//__________MCM de un array
long long mcm_array(vector<int> v, int MOD){
//MCM de muchos numeros (cada numero menor a 2e5).
//MOD es un modulo pues mcm puede ser muy grande. Usa: factorizar
long long ans = 1;
22
map<int,int> m;
vector<int> factores(0);
for(auto k: v){
factores.resize(0);
factorizar(k, factores);
int j = factores[0];
int r = 1;
for(int i = 1; i<int(factores.size()); i++){
if(factores[i] == factores[i-1])r++;
else m[factores[i-1]] = max(m[factores[i-1]], r), r = 1;
}
int s = int(factores.size())-1;
m[factores[s]] = max(m[factores[s]], r);
}
ans = 1;
for(auto k:m){
long long g = pow(k.first,k.second);
ans = (ans*g)%MOD;
}
return ans;
}
//________________
//________FUNCION PHI. PHI(N)= CANTIDA DE COPRIMOS ANTES QUE N
//SIRVE PARA HALLAR INVERSO MODULAR POR EJEMPLO,
//PUES SI A, M COPRIMOS(UNICO CASO EN QUE EXISTE EL INVERSO)
//(Aˆ[PHI(M)-1]) = Aˆ(-1)(MOD M)
//OJO QUE SI M PRIMO ALCANZA CON Aˆ(M-1) PUES PHI(PRIMO)=PRIMO-1
long long phi_to_n(long long n) {
//da la funcion phi de un numero en o(sqrt(n)) sin buscar su factorizacion
long long result = n;
for (int i = 2; i * i <= n; i++) {
if (n % i == 0) {
while (n % i == 0)
n /= i;
result -= result / i;
}
}
if (n > 1)
result -= result / n;
return result;
}
void phi_1_to_n(int n, vector<int> &ans){//phi de {1,...,n} en nlogn
vector<int> phi(n + 1);
phi[0] = 0;
phi[1] = 1;
for (int i = 2; i <= n; i++)phi[i] = i - 1;
for (int i = 2; i <= n; i++){
for (int j = 2 * i; j <= n; j += i)phi[j] -= phi[i];
}
ans = phi;
}
//_______ALGORTIMO DE EUCLUIDES PARA ENCONTRAR SOLUCONES DE A*X+B*Y = gcd(A,B)
//(TARDA LO MISMO QUE GCD)
long long extenden_euclidean(long long a, long long b, long long& x, long long& y) {
if (b == 0) {
x = 1;
23
y = 0;
return a;
}
long long x1, y1;
long long d = extenden_euclidean(b, a % b, x1, y1);
x = y1;
y = x1 - y1 * (a / b);
return d;
}
//________usa el algoritmo de euclides anterior para decidir si existe solucion
//(y hallarla en x, y) de ax +by = c
bool find_any_solution(
long long a, long long b, long long c,
long long &x0, long long &y0, long long &g
) {
//x, y no necesitanm tener valores inicales, en ellos se guardará la solucion.
g = extenden_euclidean(abs(a), abs(b), x0, y0);
if (c % g) {
return false;
}
x0 *= c / g;
y0 *= c / g;
if (a < 0) x0 = -x0;
if (b < 0) y0 = -y0;
return true;
}
//____________INVERSO MODULAR CON ALGORITMO DE EUCLIDES
//(NO NECESITA PHI NI FACTORIZACION).
//IGUAL DE RAPIDO QUE GCD (ojo que despues hay uno para calcular
//el inverso de un array de tamaño n en o(n)
//DADOS A, M COPRIMOS, NOS DA INVERSO DE A MOD M.
int inverso_modm(long long a, long long m){
//devuelve inverso de a(positivo) si son coprimos,
//-1 en otro caso(si no son coprimos no existe el inverso de a)
a = a%m;
long long x, y;//sin valores
int g = extenden_euclidean(a, m, x, y);//este me da ax+my=gcd(a,m) = 1
if(g!= 1) return -1;
return ((x%m+m)%m);
}
//dado m primo devuelve el inverso de [1,..., m-1] en o(m)
void inverso_to_1_m(long long m, vector<long long> & inv){
//el problema aca es si m es muy grande. para eso sirve el algortimo que sigue a este
inv.resize(m+1);
inv[1] = 1;
for(int a = 2; a < m; ++a) inv[a] = m - (long long)(m/a) * inv[m%a] % m;
}
//dado un array de tamaño n nos da todos sus inversos modulo m
//(no necesariamente primo, posiblemente grande) en o(n)
void inverso_to_array(vector<long long> v, long long m, vector<long long> & ans){
//v es el array original. en ans se guardan sus inversos.
//se asume que todos tienen inversos(que son coprimos con m)
long long n = v.size();
24
long long producto = 1;
vector<long long> prefix(n);
vector<long long> sufix(n);
for(auto &i : v) producto = (producto*i)%m;
long long x, y;
long long d = extenden_euclidean(producto, m, x, y);
long long inv = (x%m+m)%m;//este es el inversol producto de todos
prefix[0] = 1;
sufix[n-1] = 1;
for(int i = 1; i < n; i++){
prefix[i] = (prefix[i-1]*v[i-1])%m;
sufix[n-i-1] = (sufix[n-i]*v[n-i])%m;
}
for(int i = 0; i < n; i++){
ans[i] = (((prefix[i]*sufix[i])%m)*inv)%m;
}
}
// pow(a, b) % MOD.
// fomra mas facil de hallar eñ inverso modular a respecto de un primo p es mpow(a,p-2,MOD)
long long mpow(long long a, long long b, long long MOD) {
long long x = 1;
while (b > 0) {
if (b & 1) { /* fast way of checking if b is odd */
x = (x * a) % MOD;
}
a = (a * a) % MOD; /* compute the next power of 2 */
b >>= 1; /* this is equivalent to b = floor(b / 2) */
}
return x;
}
int main(){
int n; cin >> n;
cout << phi_to_n(n);
}
