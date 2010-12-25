typedef struct Screen
{
   char *mem;
   unsigned char **chars;
   unsigned char **colors;
   unsigned char **attrs;
   int *touched;
   int *lnums;			/* scroll info */
   int x, y;			/* cursor position */
   int beeps;
   int cursor;			/* 0 - normal; 1 - invisible; 2 - fullblock */
   struct ScreenBase *base;
} Screen;
