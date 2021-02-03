#include <iostream>
using namespace std;

const char* alpha = "abcdefghijklmnopqrstuvwxyz";

string shift(const string &s, char k)
{
  string res = s;
  const int N = s.size(), d = k - 'a';
  for(int i = 0; i < N; i++)
    res[(i + d)%N] = s[i];
  return res;
}

string mix(string &s)
{
  string res = s;
  const int N = s.size();
  for(int i = 0; i < N/2; i++)
  {
    res[2*i] = s[i];
    res[2*i+1] = s[N/2+i];
  }
  return res;  
}


string f(const string &s, char k1, char k2)
{
  string res1 = shift(s,k1);
  // cout << "res1 = " << res1 << endl;
  string res2 = mix(res1);
  // cout << "res2 = " << res2 << endl;
  string res3 = shift(res2,k2);
  return res3;
}

int main()
{
  char k1 = 'p', k2 = 'j';
  for(k1 = 'a'; k1 <= 'z'; k1++)
    for(k2 = 'a'; k2 <= 'z'; k2++)
      cout << f(alpha,k1,k2) << endl;
  
  return 0;
}
