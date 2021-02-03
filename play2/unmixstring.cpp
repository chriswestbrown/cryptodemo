#include <iostream>
using namespace std;

int main()
{
  const char* alpha = "abcdefghijklmnopqrstuvwxyz";
  static const char smix[27] = "dveuftgshriqjpkolnmzaybxcw";
  char sunmix[27] = {'\0'};
  for(int i = 0; i < 26; i++)
    sunmix[smix[i] - 'a'] = 'a' + i;
  cout << alpha << endl;
  cout << smix << endl;
  cout << sunmix << endl;
  return 0;
}
// sunmix is uwyacegikmoqsrpnljhfdbzxvt
