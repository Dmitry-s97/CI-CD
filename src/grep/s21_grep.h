#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size_massive 1024

typedef struct {
  bool flag_e;  //-e
  bool flag_i;  //-i
  bool flag_v;  //-v
  bool flag_c;  //-c
  bool flag_l;  //-l
  bool flag_n;  //-n
  bool flag_h;  //-h
  bool flag_s;  //-s
  bool flag_o;  //-o
  bool flag_f;  //-f
} GrepInfo;

void Print_Error();
_Bool Arg_Parse(char *argv[], int argc);
void Pattern_Write(char *pattern, char *optarg, int *pat_count);
void Base_Function(char *pattern, int optind, int count_files, char cflags,
                   int argc, char *argv[], GrepInfo *info);
