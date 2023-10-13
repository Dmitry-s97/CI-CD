#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "cat_functions.h"

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc < 1)
    ;
  else
    CatArgsFlag(argc, argv);
  return 0;
}

void CatArgsFlag(int argc, char *argv[]) {
  CatInfo info = {0, 0, 0, 0, 0, 0};
  for (int i = 1; i != argc; ++i) {
    if (*argv[i] == '-' && !run) {
      CatParseArg(&info, argv[i]);
    }
    if (*argv[i] != '-') {
      run = true;
      CatFile(argv[i], &info);
    }
    if (*argv[i] == '-' && run) {
      CatFile(argv[i], &info);
    }
  }
}

int CatParseArg(CatInfo *info, char *argv) {
  ++argv;
  if (*argv == '-') {
    ++argv;
    if (strcmp(argv, "number-nonblank") == 0) {
      info->string_empty = true;
    } else if (strcmp(argv, "number") == 0) {
      info->string_full = true;
    } else if (strcmp(argv, "squeeze-blank") == 0) {
      info->squeeze = true;
    } else {
      fprintf(stderr, "cat: illegal option --%s\n", argv);
      return 1;
    }
    return 0;
  }
  for (char *it = argv; *it != '\0'; ++it) {
    switch (*it) {
      case 'v':
        info->show_letters = true;
        break;
      case 'b':
        info->string_empty = true;
        break;
      case 'e':
        info->show_end = true;
        info->show_letters = true;
        break;
      case 'E':
        info->show_end = true;
        break;
      case 'n':
        info->string_full = true;
        break;
      case 's':
        info->squeeze = true;
        break;
      case 't':
        info->tabs = true;
        info->show_letters = true;
        break;
      case 'T':
        info->tabs = true;
        break;
      default:
        fprintf(stderr, "cat: illegal option --%s\n", argv);
        return 1;
    }
  }
  return 0;
}

int CatFile(char *argv, CatInfo *info) {
  char ch;
  char previous;
  char prepre;
  FILE *fp;
  fp = fopen(argv, "r");
  _Bool letter = false;
  _Bool letter_sq = true;
  int string = 1;
  if (fp != NULL) {
    while ((ch = fgetc(fp)) != EOF) {
      if (info->squeeze) {
        if (prepre == '\n' && ch == '\n') {
          prepre = previous;
          previous = ch;
          ch = fgetc(fp);
          if (previous != ch) {
            letter_sq = false;
          }
          if (letter_sq) continue;
        }
      }
      if (info->string_empty) {
        if (ch != '\n') {
          if (!letter) {
            printf("%6d\t", string++);
            letter = true;
          }
        }
        if (ch == '\n') {
          letter = false;
        }
      } else if (info->string_full) {
        if (!letter) {
          printf("%6d\t", string++);
          letter = true;
        }
        if (ch == '\n') {
          letter = false;
        }
      }
      if (info->tabs) {
        if (ch == '\t') {
          printf("^I");
          continue;
        }
      }
      if (info->show_end) {
        if (ch == '\n') {
          printf("$");
        }
      }
      if (info->show_letters) {
        if (ch >= 0 && ch <= 31) {
          if (ch != '\n' && ch != '\t') {
            printf("^%c", ch + 64);
            continue;
          }
        }
      }
      if (info->show_letters) {
        if (ch == 127) {
          printf("^?");
          continue;
        }
      }
      if (!letter_sq) {
        putchar(ch);
        ch = fgetc(fp);
        letter_sq = true;
      }
      prepre = previous;
      putchar(ch);
      previous = ch;
    }
  } else {
    fprintf(stderr, "cat: %s: No such file or directory\n", argv);
    return 1;
  }
  fclose(fp);
  return 0;
}
