#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned long b8;

const char IP_TABLE[64] = {
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17,  9, 1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 15, 7
};

const char PC1_TABLE[56] = {
  57, 49, 41, 33, 25, 17,  9,
   1, 58, 50, 42, 34, 26, 18,
  10,  2, 59, 51, 43, 35, 27,
  19, 11,  3, 60, 52, 44, 36,
  63, 55, 47, 39, 31, 23, 15,
   7, 62, 54, 46, 38, 30, 22,
  14,  6, 61, 53, 45, 37, 29,
  21, 13,  5, 28, 20, 12,  4  
};

const char E_TABLE[48] = {
  32,  1,  2,  3,  4,  5,
   4,  5,  6,  7,  8,  9,
   8,  9, 10, 11, 12, 13,
  12, 13, 14, 15, 16, 17,
  16, 17, 18, 19, 20, 21,
  20, 21, 22, 23, 24, 25,
  24, 25, 26, 27, 28, 29,
  28, 29, 30, 31, 32,  1
};

const char PC2_TABLE[48] = {
  14, 17, 11, 24,  1,  5,
  3,  28, 15,  6, 21, 10,
  23, 19, 12,  4, 26,  8,
  16, 7,  27, 20, 13,  2,
  41, 52, 31, 37, 47, 55,
  30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53,
  46, 42, 50, 36, 29, 32
};

void convert_str_2_binary(char *str, char *bin);

void initial_perm(char b_text[64], char ip_text[64]);
void initial_key(char key[64], char out[56]);

void split_text(char ip_text[64], char l0[32], char r0[32]);

void expand32_48(char half[32], char to[48]);

void l_shift_key(char key[28], int sh);

void _xor(char res[], char f[], char s[], int len);

void generate_subkey(char key[56], char subkey[48]);

void round(char li_1[32], char ri_1[32], char li[32], char ri[32], char round_key[48]);

void encrypt(char *target, char *plain_text);
void decrypt(char *target, char *encrypted_text);

int main(int argc, char **argv);
