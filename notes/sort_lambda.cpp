#include<bits/stdc++.h>


using namespace std;
int main()
{
    array<int, 10> arr = { 1,2,3,4,5,6,7,8,9 };

    sort(begin(arr), 
         end(arr), 
         [](int a, int b) {return a%2 > b%2; });

    for (auto item : arr)
      cout << item << " ";

    return 0;
}
