#include <iostream>
#include <vector>
using namespace std;

int d(const string& a, const string &b)
{
  int k = 0;
  for(int i = 0; i < 26; i++)
    k += (a[i] == b[i]);
  return k;
}

int main()
{
  vector<string> V;
  string s;
  while(cin >> s)
    V.push_back(s);

  for(int i = 0; i < V.size(); i++)
    for(int j = i + 1; j < V.size(); j++)
      cout << d(V[i],V[j]) << endl;
  
  return 0;
}

