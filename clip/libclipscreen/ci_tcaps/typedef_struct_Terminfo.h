typedef struct Terminfo
{
   char *name;
   char bools[MAX_BOOL];
   int nums[MAX_NUM];
   short strings[MAX_STR];
   short keys[MAX_KEY];
   char *buf;
   int bufsize;
   int bufpos;
} Terminfo;
