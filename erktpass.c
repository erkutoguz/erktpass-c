#include "erktpass.h"

int main(int argc, char *argv[]) {
  char *text = "midnight";
  char *key = "IEOFIT21";

  char b_text[64];
  char ip_text[64];

  char l[32];
  char r[32];
  char li_1[32];
  char ri_1[32];

  char b_key[64];
  char key_56[56];
  char round_key[48];

  char c[28];
  char d[28];

  convert_str_2_binary(text, b_text);
  convert_str_2_binary(key, b_key);

  initial_perm(b_text, ip_text);
  split_text(ip_text, li_1, ri_1);
  initial_key(b_key, key_56);

  int rd;
  rd = 1;
  for (; rd < 17; ++rd) {
    split_key(key_56, c, d);
    if (rd == 1 || rd == 2 || rd == 9 || rd == 16) {
      l_shift_key(c, 1);
      l_shift_key(d, 1);
    } else {
      l_shift_key(c, 2);
      l_shift_key(d, 2);
    }
    combine_key(key_56, c, d);
    generate_subkey(key_56, round_key);

    _round(li_1, ri_1, l, r, round_key, rd);
  }

  char enc[8];

  combine_encrpyted(b_text, l, r);

  convert_binary_2_string(b_text, enc);

  printf("%X %X %X %X %X %X %X %X", enc[0] & 0xff, enc[1] & 0xff, enc[2] & 0xff,
         enc[3] & 0xff, enc[4] & 0xff, enc[5] & 0xff, enc[6] & 0xff, enc[7]);
  return 0;
}

void _round(char li_1[32], char ri_1[32], char li[32], char ri[32],
            char round_key[48], int rd) {

  char b_48[48];
  char s_32[32];
  char perm_s_32[32];
  memcpy(li, ri_1, 32);

  expand32_48(ri_1, b_48);
  _xor(b_48, b_48, round_key, 48);

  substitution(b_48, s_32, rd);

  perm(perm_s_32, s_32);
  _xor(ri, li_1, perm_s_32, 32);
  memcpy(li_1, li, 32);
  memcpy(ri_1, ri, 32);
}

void combine_encrpyted(char res[64], char l[32], char r[32]) {
  int i;
  for (i = 0; i < 32; ++i) {
    res[i] = l[i];
    res[i + 32] = r[i + 32];
  }
}

void convert_binary_2_string(char bin[64], char str[8]) {
  int i;
  unsigned char c;

  c = 0;
  for (i = 0; i < 8; ++i) {
    c += bin[i * 8] * 128;
    c += bin[i * 8 + 1] * 64;
    c += bin[i * 8 + 2] * 32;
    c += bin[i * 8 + 3] * 16;
    c += bin[i * 8 + 4] * 8;
    c += bin[i * 8 + 5] * 4;
    c += bin[i * 8 + 6] * 2;
    c += bin[i * 8 + 7] * 1;

    str[i] = c;
    c = 0;
  }
}

void substitution(char s_48[48], char s_32[32], int rd) {
  char s_box[4][16];
  int i, r, c, v;
  char rb[2];
  char cb[4];
  char b[4];

  i = rd - 1;

  memcpy(s_box[0], S_BOXES[i][0], 16);
  memcpy(s_box[1], S_BOXES[i][1], 16);
  memcpy(s_box[2], S_BOXES[i][2], 16);
  memcpy(s_box[3], S_BOXES[i][3], 16);

  for (int m = 0; m < 8; ++m) {
    rb[0] = s_48[m * 6];
    rb[1] = s_48[m * 6 + 5];
    r = rb[0] * 2 + rb[1] * 1;

    cb[0] = s_48[m * 6 + 1];
    cb[1] = s_48[m * 6 + 2];
    cb[2] = s_48[m * 6 + 3];
    cb[3] = s_48[m * 6 + 4];
    c = cb[0] * 8 + cb[1] * 4 + cb[2] * 2 + cb[3] * 1;

    v = s_box[r][c];
    int4_to_binary(b, v);

    for (int n = 0; n < 4; ++n) {
      s_32[m * 4] = b[0];
      s_32[m * 4 + 1] = b[1];
      s_32[m * 4 + 2] = b[2];
      s_32[m * 4 + 3] = b[3];
    }
  }
}

void int4_to_binary(char b[4], int v) {
  int n, s;
  n = v;
  s = 3;
  while (n > 0) {
    b[s--] = n % 2;
    n /= 2;
  }
}

void perm(char res[32], char s_32[32]) {
  int i, p;

  for (i = 0; i < 32; ++i) {
    p = P_TABLE[i] - 1;
    res[i] = s_32[p];
  }
}

void initial_perm(char b_message[64], char ip_message[64]) {
  int i, p;
  for (i = 0; i < 64; ++i) {
    p = IP_TABLE[i] - 1;
    ip_message[i] = b_message[p];
  }
}

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

void split_key(char round_key[56], char c[28], char d[28]) {
  int i;
  i = 0;
  while (i < 28) {
    c[i] = round_key[i];
    d[i + 28] = round_key[i + 28];
    ++i;
  }
}

void combine_key(char round_key[56], char c[28], char d[28]) {
  int i;
  i = 0;
  while (i < 28) {
    round_key[i] = c[i];
    round_key[i + 28] = d[i + 28];
    ++i;
  }
}

void initial_key(char key[64], char out[56]) {
  int i, p;

  for (i = 0; i < 56; ++i) {
    p = PC1_TABLE[i] - 1;
    out[i] = key[p];
  }
}

void expand32_48(char half[32], char to[48]) {
  int i, p;

  for (i = 0; i < 48; ++i) {
    p = E_TABLE[i] - 1;
    to[i] = half[p];
  }
}

void generate_subkey(char key[56], char subkey[48]) {
  int i, p;
  for (i = 0; i < 48; ++i) {
    p = PC2_TABLE[i] - 1;
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
