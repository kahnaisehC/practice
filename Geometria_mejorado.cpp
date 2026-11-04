#include <bits/stdc++.h>

using namespace std;

typedef complex<long double> point;
#define ld long double
#define x real()
#define y imag()
const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

// _________ FUNCIONES AUXILIARES DE PRECISIÓN _________
// Retorna 1 si es positivo, -1 si negativo, 0 si es cercano a cero
int sign(ld a) {
    return (a > EPS) - (a < -EPS);
}

// _________ OPERACIONES FUNDAMENTALES _________
// Producto punto: proyecta un vector sobre otro. (<0)
ld dot(point a, point b) { return (conj(a) * b).x; }

// Producto cruz: área orientada (me dice >0 si a esta a la izquierda del vector (0,b); <0 "a" a la derecha de b; == 0 misma recta )
ld cross(point a, point b) { return (conj(a) * b).y; }

// Posición relativa de 'p' respecto a la recta dirigida a->b
// 1: izquierda, -1: derecha, 0: colineales
int pos_relativa(point p, point a, point b) {
    return sign(cross(b - a, p - a));
}

// ____________________ DISTANCIAS Y PROYECCIONES de un punto a una recta o segmento___________________ _________
// Devuelve la proyección ortogonal del punto p sobre la recta [a, b]
point proyeccion_punto_recta(point p, point a, point b) {
    point dir = b - a;
    return a + dir * (dot(p - a, dir) / norm(dir)); // norm(dir) equivale a |dir|^2
}

// Distancia del punto p a la recta infinita [a, b]
ld dist_punto_recta(point p, point a, point b) {
    return abs(cross(b - a, p - a)) / abs(b - a);
}

// DEVUELVE TRUE SI EL PUNTO P ESTÁ EN EL SEGMENTO [a, b]
bool point_in_segment(point p, point a, point b) {
    // Debe ser colineal Y el producto punto hacia los extremos debe ser <= 0
    return pos_relativa(p, a, b) == 0 && sign(dot(a - p, b - p)) <= 0;
}

// Distancia del punto p al segmento [a, b]
ld dist(point p, point a, point b) {
    if (a == b) return abs(p - a);
    if (sign(dot(b - a, p - a)) < 0) return abs(p - a); // Cae fuera del lado de a
    if (sign(dot(a - b, p - b)) < 0) return abs(p - b); // Cae fuera del lado de b
    return dist_punto_recta(p, a, b);                   // Cae en el medio, devolvemos dist a recta
}

// _____________________ POLÍGONOS ______________________________
// DISTANCIA DE UN PUNTO A UN POLIGONO /O(cant vertices)
//distancia dentre dos poligonos se hace con minkowski y aparece mas abajo
ld dist_point_poligon(point p, const vector<point> &poligono) {
    ld mini = LDBL_MAX;
    int n = poligono.size();
    for(int i = 0; i < n; i++) {
        mini = min(mini, dist(p, poligono[i], poligono[(i + 1) % n]));
    }
    return mini;
}

// Área del polígono. O(cant vertices)
ld area(const vector<point> &poligono) {
    ld ans = 0;
    int n = poligono.size();
    for(int i = 0; i < n; i++) {
        ans += cross(poligono[i], poligono[(i + 1) % n]);
    }
    return abs(ans) / 2.0; 
}

// Posición de un punto respecto a polígono: 0 si está en el borde, 1 adentro, -1 afuera(O(cant vertices))
//OJO QUE SI EL PROBLEMA ES "TE DOY MUCHOS PUNTOS Y DECIME SI CAEN DENTRO" hay una solucion con consultas de logN mas abajo
int in_polygon(point p, const vector<point> &poligono) {
    bool inside = false;
    int n = poligono.size();
    for (int i = 0; i < n; i++) {
        point a = poligono[i], b = poligono[(i + 1) % n];
        if (point_in_segment(p, a, b)) return 0; // Borde
        if ((a.y > p.y) != (b.y > p.y)) {
            ld intersect_x = a.x + (p.y - a.y) * (b.x - a.x) / (b.y - a.y);
            if (p.x < intersect_x) inside = !inside;
        }
    }
    return inside ? 1 : -1;
}

// _________ _______________INTERSECCIONES de rectas y segmentos___________________________
// Intersección entre dos rectas [a,b] y [p,q]
point intersection(point a, point b, point p, point q) {
    ld c1 = cross(p - a, b - a);
    ld c2 = cross(q - a, b - a);
    return (c1 * q - c2 * p) / (c1 - c2); // Nota: si son paralelas, c1 == c2 y explota
}

// ¿Dos segmentos se cortan? (Corregido para detectar uniones en forma de T y colineales)
bool interseccion_segmentos(point a, point b, point p, point q) {
    int oa = pos_relativa(a, p, q), ob = pos_relativa(b, p, q);
    int op = pos_relativa(p, a, b), oq = pos_relativa(q, a, b);
    
    // Se cruzan en un único punto estrictamente
    if (oa != ob && op != oq) return true; 
    
    // Casos borde: colineales superpuestos o un extremo toca al otro segmento
    if (point_in_segment(a, p, q) || point_in_segment(b, p, q) ||
        point_in_segment(p, a, b) || point_in_segment(q, a, b)) return true;
        
    return false;
}

//interseccion de dos poligonos esta en poligonos tryhard

// ___________________ INTERSECCIONES DE CIRCUNFERENCIAS____________________

// Intersección entre circunferencia C(c, r) y la recta infinita que pasa por a y b
vector<point> intersection_circle_line(point c, ld r, point a, point b) {
    point p = proyeccion_punto_recta(c, a, b);
    ld d = abs(c - p);
    
    if (d > r + EPS) return {}; // No se cruzan (0 puntos)
    if (abs(r - d) <= EPS) return {p}; // Tangente (1 punto)
    
    ld h = sqrtl(r * r - d * d);
    point dir = (b - a) / abs(b - a);
    return {p - dir * h, p + dir * h}; // Secante (2 puntos)
}

// Intersección de dos circunferencias C1(c1, r1) y C2(c2, r2)
vector<point> intersection_circle_circle(point c1, ld r1, point c2, ld r2) {
    ld d = abs(c2 - c1);
    
    // Disjuntos, uno dentro del otro, o circunferencias idénticas/concéntricas
    if (d > r1 + r2 + EPS || d < abs(r1 - r2) - EPS || d < EPS) return {};
    ld a = (r1 * r1 - r2 * r2 + d * d) / (2.0L * d);
    ld h = sqrtl(max(0.0L, r1 * r1 - a * a)); 
    point p = c1 + (c2 - c1) * (a / d);
    if (h <= EPS) return {p}; // Tangentes (1 punto)
    point offset = (c2 - c1) * point(0, 1) * (h / d); 
    return {p + offset, p - offset}; // Secantes (2 puntos)
}


// ___________________________________ ÁNGULOS _______________________________________________________
ld angle(point a, point b) { // Ángulo del vector a->b
    return arg(b - a);
}

// Ángulo entre a, b y c. Devuelve el ángulo en (-pi, pi]
ld angle_ABC(point a, point b, point c) {
    ld g = arg(a - b) - arg(c - b); 
    if (g <= -PI) g += 2.0L * PI;
    if (g > PI) g -= 2.0L * PI;
    return g;
}

//____________________________________poligonos tryhard________________________________________________

// Función auxiliar para ordenar puntos (Izquierda a Derecha, Abajo a Arriba)
bool compP(const point& a, const point& b) {
    if (abs(a.x - b.x) > EPS) return a.x < b.x;
    return a.y < b.y;
}

vector<point> convex_hull(vector<point>& pts) {//dado un conjunto de puntos, cual es el poligono mas pequeño que los encierra(ejemplo para encontrar los dos mas lejanos)
    int n = pts.size(), k = 0;
    if (n <= 2) return pts; 
    vector<point> h(2 * n);
    sort(pts.begin(), pts.end(), compP);
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(h[k-1] - h[k-2], pts[i] - h[k-2]) <= 0) k--;
        h[k++] = pts[i];
    }

    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(h[k-1] - h[k-2], pts[i] - h[k-2]) <= 0) k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1);
    return h;
}

//__________ distancia máxima al cuadrado entre dos puntos del conjunto en NlogN_____
ld max_distance(vector<point>& pts) {
    vector<point> h = convex_hull(pts);
    int n = h.size();
    
    if (n <= 1) return 0;
    if (n == 2) return norm(h[0] - h[1]);
    ld max_dist2 = 0;
    int j = 1; 
    for (int i = 0; i < n; i++) {
        int next_i = (i + 1) % n;
        while (abs(cross(h[next_i] - h[i], h[(j + 1) % n] - h[i])) > 
               abs(cross(h[next_i] - h[i], h[j] - h[i]))) {
            j = (j + 1) % n;
        }
        max_dist2 = max({max_dist2, norm(h[i] - h[j]), norm(h[next_i] - h[j])});
    }
    return max_dist2; // O usa sqrtl(max_dist2) si te piden la distancia lineal real
}



// __________O(log N) punto está dentro de un polígono CONVEXO________________
// Retorna: 1 (adentro), 0 (borde), -1 (afuera)
//PRIMERO va esto para preparar el poligono. VA EN TU MAIN DESPUES DE LEER EL POLIGONO(poly)
//copiar de aca
if (pos_relativa(poly[2], poly[0], poly[1]) < 0) {
    reverse(poly.begin(), poly.end());
}
auto it = min_element(poly.begin(), poly.end(), compP);
rotate(poly.begin(), it, poly.end());
//hasta aca y ahora si la funcion:

int in_convex_polygon_log(point p, const vector<point>& poly) {//usando lo que dice aca arriba, 
    int n = poly.size();
    if (n < 3) {
        if (n == 1) return (abs(p - poly[0]) <= EPS) ? 0 : -1;
        if (n == 2) return point_in_segment(p, poly[0], poly[1]) ? 0 : -1;
        return -1;
    }
    point p0 = poly[0];
    int pos1 = pos_relativa(p, p0, poly[1]);
    int posn = pos_relativa(p, p0, poly[n - 1]);
    if (pos1 < 0 || posn > 0) return -1; 
    
    if (point_in_segment(p, p0, poly[1])) return 0;
    if (point_in_segment(p, p0, poly[n - 1])) return 0;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = l + (r - l) / 2;
        if (pos_relativa(p, p0, poly[mid]) >= 0) {
            l = mid; // El punto está a la izquierda (o sobre) la diagonal p0 -> poly[mid]
        } else {
            r = mid; // El punto está a la derecha
        }
    }
    int final_pos = pos_relativa(p, poly[l], poly[r]);
    
    if (final_pos > 0) return 1; // Estrictamente adentro
    if (final_pos == 0 && point_in_segment(p, poly[l], poly[r])) return 0; // Sobre la arista exterior de la tapa
    
    return -1; // Afuera
}

//_______________SUMA DE MINKOWSKI en O(|P|+|Q|): __________________

//dados dos poligonos convexos, sumar los poligonos(que tambien da convexo)
//sirve para: 1) si deducis que un punto debe ser comb lineal de los puntos de varios poligonos, haes la suma y verificas si cae dentro.
//2) ver si dos poligonos se intersectan en O(|P|+|Q|) (chequeas si (0,0) esta en P-Q)
//primero esto fuera del main
void reorder_polygon(vector<point>& P) {
    auto it = min_element(P.begin(), P.end(), compP);
    rotate(P.begin(), it, P.end());
}
//va todo esto en el main antes de llamar a la suma
if (pos_relativa(P[2], P[0], P[1]) < 0) reverse(P.begin(), P.end());

if (pos_relativa(Q[2], Q[0], Q[1]) < 0) reverse(Q.begin(), Q.end());

//algoritmo:
// Suma de Minkowski en O(N + M)
vector<point> minkowski(vector<point> P, vector<point> Q) {
    reorder_polygon(P);
    reorder_polygon(Q);
    P.push_back(P[0]); P.push_back(P[1]);
    Q.push_back(Q[0]); Q.push_back(Q[1]);
    
    vector<point> result;
    size_t i = 0, j = 0;

    while (i < P.size() - 2 || j < Q.size() - 2) {
        result.push_back(P[i] + Q[j]);
        point edgeP = P[i + 1] - P[i];
        point edgeQ = Q[j + 1] - Q[j];

        int c = sign(cross(edgeP, edgeQ));
        
        if (c >= 0 && i < P.size() - 2) ++i;
        if (c <= 0 && j < Q.size() - 2) ++j;
    }
    
    return result; // El polígono resultante ya sale convexo y ordenado
}



int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout << setprecision(20);
    return 0;
}
