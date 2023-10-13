#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc >= 3) {
    Arg_Parse(argv, argc);
  } else {
    Print_Error();
  }
  return 0;
}

_Bool Arg_Parse(char *argv[], int argc) {
  _Bool run = false;
  int flag;             // for flags
  int count_files = 0;  // count files
  char pattern[size_massive] = {0};
  int pat_count = 0;
  char cflags = REG_EXTENDED | REG_NEWLINE;
  GrepInfo info = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static struct option long_options[] = {{"flag_e", no_argument, 0, 'e'},
                                         {"flag_i", no_argument, 0, 'i'},
                                         {"flag_v", no_argument, 0, 'v'},
                                         {"flag_c", no_argument, 0, 'c'},
                                         {"flag_l", no_argument, 0, 'l'},
                                         {"flag_n", no_argument, 0, 'n'},
                                         {"flag_h", no_argument, 0, 'h'},
                                         {"flag_s", no_argument, 0, 's'},
                                         {"flag_o", no_argument, 0, 'o'},
                                         {"flag_f", no_argument, 0, 'f'},
                                         {0, 0, 0, 0}};

  while ((flag = getopt_long(argc, argv, "e:ivclnhsf:o", long_options, NULL)) !=
         -1) {
    switch (flag) {
      case 'e':
        info.flag_e = 1;
        Pattern_Write(pattern, optarg, &pat_count);
        break;
      case 'i':
        info.flag_i = 1;
        cflags |= REG_ICASE;
        break;
      case 'v':
        info.flag_v = 1;
        break;
      case 'c':
        info.flag_c = 1;
        break;
      case 'l':
        info.flag_l = 1;
        break;
      case 'n':
        info.flag_n = 1;
        break;
      case 'h':
        info.flag_h = 1;
        break;
      case 's':
        info.flag_s = 1;
        break;
      case 'o':
        info.flag_o = 1;
        break;
      case 'f':
        info.flag_f = 1;
        FILE *fd;
        fd = fopen(optarg, "r");
        if (fd != NULL) {
          while (!feof(fd)) {
            char string[size_massive];
            fgets(string, 1024, fd);
            if (string[0] != '\n') {
              for (int i = 0; i < (int)strlen(string); i++) {
                if (string[i] == '\n') string[i] = '\0';
              }
              Pattern_Write(pattern, string, &pat_count);
            } else if (string[0] == '\n') {
              string[1] = '\0';
              Pattern_Write(pattern, string, &pat_count);
            } else if ((strlen(string) == 1) && string[0] == '\0') {
              strcat(pattern, ".");
            }
          }
          fclose(fd);
        } else {
          fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
        }
        break;
      default:
        fprintf(stderr, "grep: illegal option\n");
    }
  }
  if (info.flag_v == 1) {
    info.flag_o = 0;
  }
  if (info.flag_e == 0 && info.flag_f == 0) {
    strcat(pattern, argv[optind++]);
  }
  count_files = argc - optind;
  Base_Function(pattern, optind, count_files, cflags, argc, argv, &info);
  run = true;
  return run;
}

void Base_Function(char *pattern, int optind, int count_files, char cflags,
                   int argc, char *argv[], GrepInfo *info) {
  char tmp_string[size_massive];
  regmatch_t pmatch[size_massive];
  regex_t re;
  FILE *fd;
  int reg;
  int rege;
  while (optind < argc) {
    fd = fopen(argv[optind], "r");
    if (fd != NULL) {
      int count_matched_strings = 0;
      int count_all_strings = 0;
      while ((fgets(tmp_string, 1024, fd)) != 0) {
        count_all_strings++;
        if ((reg = regcomp(&re, pattern, cflags)) == 0) {
          if ((rege = regexec(&re, tmp_string, 0, NULL, 0)) == 0) {
            if (!info->flag_v) {
              count_matched_strings++;
              if (!info->flag_c && !info->flag_l) {
                if (!info->flag_h && count_files > 1)
                  printf("%s:", argv[optind]);
                if (info->flag_n) {
                  printf("%d:", count_all_strings);
                }
                if (info->flag_o) {
                  int n;
                  int eflag = 0;
                  char *massiv_for_o = tmp_string;
                  while ((n = (regexec(&re, massiv_for_o, 1, pmatch, eflag))) ==
                         0) {
                    int length = pmatch[0].rm_eo - pmatch[0].rm_so;
                    printf("%.*s\n", length, massiv_for_o + pmatch[0].rm_so);
                    massiv_for_o += pmatch[0].rm_eo;
                    eflag = REG_NOTBOL;
                  }
                  massiv_for_o = 0;
                } else if (!info->flag_v) {
                  char *ach;
                  if ((ach = strchr(tmp_string, '\n')) != NULL) {
                    printf("%s", tmp_string);
                  } else if (ach == NULL) {
                    printf("%s\n", tmp_string);
                  }
                }
              }
            }
          } else if ((info->flag_v) && (rege != 0)) {
            count_matched_strings++;
            if (!info->flag_c && !info->flag_l) {
              if (!info->flag_h && count_files > 1) printf("%s:", argv[optind]);
              if (info->flag_n) {
                printf("%d:", count_all_strings);
              }
              char *ach;
              if ((ach = strchr(tmp_string, '\n')) != NULL) {
                printf("%s", tmp_string);
              } else if (ach == NULL) {
                printf("%s\n", tmp_string);
              }
            }
          }
        } else {
          regfree(&re);
          exit(1);
        }
        regfree(&re);
      }
      if (info->flag_c && !info->flag_h && count_files > 1)
        printf("%s:", argv[optind]);
      if (info->flag_c)
        printf("%d\n", info->flag_l ? count_matched_strings > 0
                                    : count_matched_strings);
      if (info->flag_l && count_matched_strings) {
        printf("%s\n", argv[optind]);
      }
      tmp_string[size_massive] = 0;
      fclose(fd);
    } else {
      if (info->flag_s == 0) {
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
      }
    }
    optind++;
  }
}

void Pattern_Write(char *pattern, char *optarg, int *pat_count) {
  if (*pat_count == 0) {
    strcat(pattern, optarg);
  } else if (*pat_count >= 1) {
    strcat(pattern, "|");
    strcat(pattern, optarg);
  }
  (*pat_count)++;
}

void Print_Error() {
  fprintf(stderr, "grep: invalid option\n");
  fprintf(stderr,
          "grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] [-C[num]]\n");
  fprintf(stderr,
          "[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
  fprintf(
      stderr,
      "[--context[=num]] [--directories=action] [--label] [--line-buffered]\n");
  fprintf(stderr, "[--null] [pattern] [file ...]\n");
}
