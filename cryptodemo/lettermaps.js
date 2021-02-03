var alpha = "abcdefghijklmnopqrstuvwxyz";
/*
Shift string s left by amount key
@param {str} permutation of lower-case alphabet as string
@param {key} character encoding shift amount (string lenth 1)
*/
var shift = function(str, key) {
  let d = Number(key);
  if (isNaN(d)) {
    let x = key.charCodeAt(0);
    if (97 <= k && k <= 122) d = key - 'a';
    else if (65 <= k && k <= 90) d = key - 'A';
    else
      throw 'shift amount key must be letter a-z, received ' + key;
  }
  let N = str.length(), delta = d;
  let s = Array.from(str);
  for (let start = 0, count = N; count > 0; start++) {
    let from = start, to = start + delta;
    let buff = s[start];
    while (to != start) {
      { let tmp = buff; buff = s[to]; s[to] = tmp; } //swap(buff, s[to]);
      from = to;
      to = (to + delta) % N;
      count--;
    }
    s[to] = buff;
    count--;
  }
}
