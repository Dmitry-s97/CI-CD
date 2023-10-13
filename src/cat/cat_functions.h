typedef struct {
  bool string_empty;  //-b
  bool show_end;      //-e
  bool string_full;   //-n
  bool squeeze;       //-s
  bool tabs;          //-t
  bool show_letters;  //-v
} CatInfo;

void CatArgsFlag(int argc, char *argv[]);
int CatParseArg(CatInfo *info, char *argv);
int CatFile(char *argv, CatInfo *info);
_Bool run = false;
