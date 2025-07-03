#include "erktpass.h"

int main(int argc, char *argv[]) {
  char *text = "midnight";
  char *key = "IEOFIT#1";

  char b_text[64];
  char ip_text[64];

  char l[32];
  char r[32];

  char b_key[64];
  char key_56[56];

  char c[28];
  char d[28];

  convert_str_2_binary(text, b_text);
  convert_str_2_binary(key, b_key);

  initial_perm(b_text, ip_text);
  split_text(ip_text, l, r);
  initial_key(b_key, key_56);

  int rd;
  rd = 1;
  for (; rd < 17; ++rd) {
  }
  return 0;
}

void round(char li_1[32], char ri_1[32], char li[32], char ri[32],
           char round_key[32]) {
  char b_48[48];
  memcpy(li, ri_1, 32);

  expand32_48(ri_1, b_48);
  _xor(b_48, b_48, round_key, 48);
}

void initial_perm(char b_message[64], char ip_message[64]) {
  int i, p;
  for (i = 0; i < 64; ++i) {
    p = IP_TABLE[i] - 1;
    ip_message[i] = b_message[p];
  }
}

// 00010011 00110100 11011010 01010110 10001010 00101010 00011010 10100101
// 0001001  0011010  1101101  0101011  1000101  0010101  0001101  1010010
// 00010010 01101011 01101010 10111000 10100101 01000110 11010010

void convert_str_2_binary(char *str, char *bin) {
  int i, j, len;
  len = strlen(str);
  i = 0;
  j = 0;

  while (i < len) {
    bin[j] = (str[i] >> (7 - (j % 8))) & 1;
    ++j;
    if (j % 8 == 0) {
      ++i;
    }
  }
}

void split_text(char ip_text[64], char l0[32], char r0[32]) {
  int p;
  for (p = 0; p < 32; ++p) {
    l0[p] = ip_text[p];
    r0[p] = ip_text[p + 32];
  }
}

void initial_key(char key[64], char out[56]) {
  int i, p;

  for (i = 0; i < 56; ++i) {
    p = PC1_TABLE[i];
    out[i] = key[p];
  }
}

void expand32_48(char half[32], char to[48]) {
  int i, p;

  for (i = 0; i < 48; ++i) {
    p = E_TABLE[i];
    to[i] = half[p];
  }
}

void generate_subkey(char key[56], char subkey[48]) {
  int i, p;
  for (i = 0; i < 48; ++i) {
    p = PC2_TABLE[i];
    subkey[i] = key[p];
  }
}

void l_shift_key(char key[28], int sh) {
  int i, end, t1, t2;

  t1 = key[0];
  if (sh == 2) {
    t2 = key[1];
  }

  i = sh;
  end = 28;

  for (; i < end; ++i) {
    key[i - 1] = key[i];
  }

  if (sh == 2) {
    i = sh;
    for (; i < end; ++i) {
      key[i - 1] = key[i];
    }
  }

  if (sh == 1) {
    key[27] = t1;
  } else {
    key[26] = t1;
    key[27] = t2;
  }
}

void _xor(char res[], char f[], char s[], int len) {
  int i;

  for (i = 0; i < len; ++i) {
    res[i] = f[i] ^ s[i];
  }
}
