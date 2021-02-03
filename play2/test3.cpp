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

void mix(string &s)
{
  static string tmp;
  const int N = s.size();
  if (tmp.size() != N) { tmp.resize(N); }
  for(int i = 0; i < N; i++)
    tmp[i] = s[i];
  for(int i = 0; i < N/2; i++)
  {
    s[2*i] = tmp[i];
    s[2*i+1] = tmp[N/2+i];
  }
}


void f(string &s, char k1, char k2, char k3)
{
  shift(s,k1);
  mix(s);
  shift(s,k2);
  mix(s);
  shift(s,k3);
}

string shiftmap(char k1, char k2, char k3)
{
  string s = alpha;
  f(s,k1,k2,k3);
  return s;
}

void printAllKeys()
{
  string s = alpha;
  char k1 = 'z', k2 = 'j', k3 = 'd';
  for(k1 = 'a'; k1 <= 'z'; k1++)
    for(k2 = 'a'; k2 <= 'z'; k2++)
      for(k3 = 'a'; k3 <= 'z'; k3++)
      {
	s = alpha;
	f(s,k1,k2,k3);
	cout << s << endl;
      }
}

void printAllKeys(const string& constraints)
{
  string s = alpha;
  char k1 = 'z', k2 = 'j', k3 = 'd';
  for(k1 = 'a'; k1 <= 'z'; k1++)
    for(k2 = 'a'; k2 <= 'z'; k2++)
      for(k3 = 'a'; k3 <= 'z'; k3++)
      {
	s = alpha;
	f(s,k1,k2,k3);
	bool pass = true;
	for(int i = 0; pass && i < constraints.size(); i += 2)
	  pass = (s[constraints[i] - 'a'] == constraints[i+1]);
	if (pass) { cout << s << endl; }
      }
}

void getAllKeys(const string& constraints, vector<string> &fmaps)
{
  string s = alpha;
  char k1 = 'z', k2 = 'j', k3 = 'd';
  for(k1 = 'a'; k1 <= 'z'; k1++)
    for(k2 = 'a'; k2 <= 'z'; k2++)
      for(k3 = 'a'; k3 <= 'z'; k3++)
      {
	s = alpha;
	f(s,k1,k2,k3);
	bool pass = true;
	for(int i = 0; pass && i < constraints.size(); i += 2)
	  pass = (s[constraints[i] - 'a'] == constraints[i+1]);
	if (pass) { fmaps.push_back(s); }
      }
}

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
  if (argc == 2) {
    string constraints = argv[1];
    printAllKeys(constraints);
  }
  else if (argc == 3 && strcmp(argv[2],"-l") == 0) {
    string constraints = argv[1];
    vector<string> fmaps;
    getAllKeys(constraints,fmaps);
    //cout << ">>>> fmaps.size() = " << fmaps.size() << endl;
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
  else if (argc > 2 && strlen(argv[1]) == 3)
  {
    if (strcmp(argv[2],"-p") == 0) {
      string fmap = shiftmap(argv[1][0],argv[1][1],argv[1][2]);
      for(int i = 0; i < fmap.size(); i++)
	cout << alpha[i] << ' ';
      cout << endl;
      for(int i = 0; i < fmap.size(); i++)
	cout << fmap[i] << ' ';
      cout << endl;      
    }
    else {
      bool fwd = !(strcmp(argv[2],"-r") == 0);
      string fmap = shiftmap(argv[1][0],argv[1][1],argv[1][2]);
      string rmap = revmap(fmap);
      char c;
      while((c = cin.get()) != EOF) {
	cout << appshiftmap(fwd ? fmap : rmap, c);
      }
    }
  }
  return 0;
}
