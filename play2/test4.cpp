#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include "freq.hpp"
using namespace std;

const char* alpha = "abcdefghijklmnopqrstuvwxyz";

void shift(string &s, char k)
{
  int d = 0;
  if ('a' <= k && k <= 'z') d = k - 'a';
  else if ('A' <= k && k <= 'Z') d = k - 'A';
  else
    d = 0; // do I want to throw an error?
  const int N = s.size(), delta = d;
  for(int start = 0, count = N; count > 0; start++)
  {
    int from = start, to = start + delta;
    char buff = s[start];
    while(to != start) {
      swap(buff,s[to]);
      from = to;
      to = (to + delta)%N;
      count--;
    }
    s[to] = buff;
    count--;
  }
}

void unshift(string &s, char k)
{
  int d = 0;
  if ('a' <= k && k <= 'z') d = k - 'a';
  else if ('A' <= k && k <= 'Z') d = k - 'A';
  else return;
  shift(s,'a' + (26 - d)%26);  
}


static const char smix[27] = "dveuftgshriqjpkolnmzaybxcw";
static const char sunmix[27] = "uwyacegikmoqsrpnljhfdbzxvt";

void mix(string &s)
{
  static string tmp;
  const int N = s.size();
  if (tmp.size() != N) { tmp.resize(N); }
  for(int i = 0; i < N; i++)
    tmp[i] = s[i];
  // for(int i = 0; i < N/2; i++)
  // {
  //   s[2*i] = tmp[i];
  //   s[2*i+1] = tmp[N/2+i];
  // }
  for(int i = 0; i < N; i++)
    s[i] = tmp[smix[i] - 'a'];
}

void unmix(string &s)
{
  static string tmp;
  const int N = s.size();
  if (tmp.size() != N) { tmp.resize(N); }
  for(int i = 0; i < N; i++)
    tmp[i] = s[i];
  // for(int i = 0; i < N/2; i++)
  // {
  //   s[i] = tmp[2*i];
  //   s[N/2+i] = tmp[2*i+1];
  // }
  for(int i = 0; i < N; i++)
    s[i] = tmp[sunmix[i] - 'a'];
}


void f(string &s, char k1, char k2, char k3)
{
  shift(s,k1);
  mix(s);
  shift(s,k2);
  mix(s);
  shift(s,k3);
}
void f(string &s, const string& key)
{
  const int N = key.size();
  if (N == 0) return;
  shift(s,key[0]);
  for(int i = 1; i < N; i++) { mix(s); shift(s,key[i]); }
}


string shiftmap(const string& key)
{
  string s = alpha;
  f(s,key);
  return s;
}

//-- n is the keylength, require n > 0 
void getAllKeys(int n, const string& constraints, vector<string> &fmaps)
{
  string s = alpha;
  string key(n,'a');
  int i=n-1;
  while(true)
  {
    // generate fwd map from key
    s = alpha;
    f(s,key);

    // add fwd map to fmaps if it satisfies constraints
    bool pass = true;
    for(int i = 0; pass && i < constraints.size(); i += 2)
      pass = (s[constraints[i] - 'a'] == constraints[i+1]);
    if (pass) { fmaps.push_back(s); }
    
    // increment key
    for(i = n-1; i >= 0 && key[i] == 'z'; i--)
      key[i] = 'a';
    if (i >= 0) { key[i]++; }
    else break;
  }
}

string composedMapMOD(string key)
{
  int n = key.size();
  vector<string> stepmaps(2*n-1);
  unshift((stepmaps[0] = alpha),key[0]);
  for(int i = 1; i < n; i++)
  {
    mix(stepmaps[2*i-1] = alpha);
    unshift((stepmaps[2*i] = alpha),key[i]);
  }
  string res = alpha;
  for(int i = 0; i < 26; i++)
  {
    char c = alpha[i];
    for(int j = 0; j < stepmaps.size(); j++) {
      c = stepmaps[j][c-'a'];
    }
    res[i] = c;
  }
  return res;
}

void getAllMapsMOD(int n, const string& constraints, vector<string> &fmaps)
{
  string s = alpha;
  string key(n,'a');
  int i=n-1;
  while(true)
  {
    // generate fwd map from key
    s =  composedMapMOD(key);

    // add fwd map to fmaps if it satisfies constraints
    bool pass = true;
    for(int i = 0; pass && i < constraints.size(); i += 2)
      pass = (s[constraints[i] - 'a'] == constraints[i+1]);
    if (pass) { fmaps.push_back(s); }
    
    // increment key
    for(i = n-1; i >= 0 && key[i] == 'z'; i--)
      key[i] = 'a';
    if (i >= 0) { key[i]++; }
    else break;
  }
}


void printAllKeys(int n, const string& constraints)
{
  vector<string> fmaps;
  getAllKeys(n,constraints,fmaps);
  for(int i = 0; i < fmaps.size(); i++)
    cout << fmaps[i] << endl;
}

void printAllKeys(int n) { printAllKeys(n,""); }


char appshiftmap(const string &s, char c)
{
  return 'a' <= c && c <= 'z' ? s[c - 'a']
    : ('A' <= c && c <= 'Z' ? s[c - 'A'] : c); 
}

string revmap(const string &s)
{
  string res = s;
  for(int i = 0; i < s.size(); i++)
    res[s[i] - 'a'] = 'a' + i;
  return res;
}

double logliklihood(const string &rmap, const string &ciphertext)
{
  double p = 0.0;
  for(int i = 0; i < ciphertext.size(); i++) {
    char c = ciphertext[i];
    bool tlc = 'a' <= c && c <= 'z';
    bool tuc = 'A' <= c && c <= 'Z';
    if (tlc || tuc) {
      if (tuc) { c = c - 'A' + 'a'; }
      p += std::log(getLetterFreq(appshiftmap(rmap,c))/100.0);
    }
  }
  return p;
}

double logliklihoodBigram(const string &rmap, const string &ciphertext)
{
  double p = 0.0;
  char prev = ciphertext.size() > 0 ? ciphertext[0] : ' ';
  if('A' <= prev && prev <= 'Z') { prev = prev - 'A' + 'a'; }
  bool ptlc = 'a' <= prev && prev <= 'z';
  for(int i = 1; i < ciphertext.size(); i++) {
    char c = ciphertext[i];
    if ('A' <= c && c <= 'Z') { c = c - 'A' + 'a'; }
    bool tlc = 'a' <= c && c <= 'z';    
    if (tlc && ptlc) {
      char ptprev = appshiftmap(rmap,prev);
      char ptc = appshiftmap(rmap,c);
      p += std::log(getBigramFreq(ptprev,ptc));
    }
    prev = c;
    ptlc = tlc;
  }
  return p;
}

bool fmllpaircmp(const pair<string,double> &a, const pair<string,double> &b)
{
  return a.second > b.second;
}
  
int main(int argc, char **argv)
{
  // cout <<  composedMapMOD(argv[1]) << endl;
  // exit(0);
  
  // for(int i = 0; i < 26; i++)
  //   for(int j = 0; j < 26; j++)      
  //   {
  //     //      for(int i = 0; i < 26; i++) { cout << alpha[i] << ' '; } cout << endl;
  //     for(int i = 0; i < 26; i++) { char c = m3[m2[m1[alpha[i]-'a']-'a']-'a']; cout << c << ' '; }
  //     cout << endl;
  //   }
  int n = atoi(argv[1]);
  vector<string> fmaps;
  getAllMapsMOD(n, "",fmaps);
  for(int i = 0; i < fmaps.size(); i++)
    cout << fmaps[i] << endl;
  return 0;
  
    
  // if (argc == c) {
  //   char x = 'z';
  //   string s = alpha;
  //   mix(s);
  //   cout << s << endl;
  //   unmix(s);
  //   cout << s << endl;
  //   return 0;
  // }

  
  if (argc == 2)
  {
    int n = atoi(argv[1]);
    printAllKeys(n);
  }
  else if (argc == 4 && strcmp(argv[3],"-l") == 0) {
    int n = atoi(argv[1]);
    string constraints = argv[2];
    vector<string> fmaps;
    getAllKeys(n,constraints,fmaps);
    string ciphertext;
    char c;
    while((c = cin.get()) != EOF) { ciphertext += c; }
    vector<pair<string,double> > fmllpairs;
    for(int i = 0; i < fmaps.size(); i++)
      fmllpairs.push_back(make_pair(fmaps[i],logliklihood(revmap(fmaps[i]),ciphertext)));
    sort(fmllpairs.begin(),fmllpairs.end(),fmllpaircmp);
    for(int i = 0; i < fmllpairs.size(); i++) {
      cout << fmllpairs[i].first << ' ' << fmllpairs[i].second << ' ';
      cout << logliklihoodBigram(revmap(fmllpairs[i].first),ciphertext) << ' ';
      string rmap = revmap(fmllpairs[i].first);
      string plain;
      for(int k = 0; k < std::min((int)ciphertext.size(),80); ++k)
  	plain += appshiftmap(rmap,ciphertext[k]);
      cout << plain;
      if (plain.back() != '\n') { cout << endl; }
    }
  }
  else if (argc > 2)
  {
    string key = argv[1];
    string fmap = shiftmap(key);
    string rmap = revmap(fmap);
    if (strcmp(argv[2],"-p") == 0) {
      for(int i = 0; i < fmap.size(); i++) { cout << alpha[i] << ' '; }
      cout << endl;
      for(int i = 0; i < fmap.size(); i++) { cout << fmap[i] << ' '; }
      cout << endl;      
    }
    else {
      string &umap = strcmp(argv[2],"-r") == 0 ? rmap : fmap;
      for(char c; (c = cin.get()) != EOF; cout << appshiftmap(umap, c));
    }
  }
  return 0;
}
