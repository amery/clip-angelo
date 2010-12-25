Screen *
new_Screen(ScreenBase * base)
{
   int i;

   Screen *scr;

   char *mem;

   int Lines = base->Lines;

   int Columns = base->Columns;

   scr = (Screen *) calloc(1, sizeof(Screen));
   scr->base = base;
   scr->mem = mem = (char *) calloc(Lines * Columns * 3 + Lines * 3 * sizeof(char *) + Lines * 2 * sizeof(int), 1);

   scr->chars = (unsigned char **) (mem + Lines * Columns * 3);
   scr->colors = (unsigned char **) (mem + Lines * Columns * 3 + Lines * sizeof(char *));
   scr->attrs = (unsigned char **) (mem + Lines * Columns * 3 + Lines * sizeof(char *) * 2);
   scr->touched = (int *) (mem + Lines * Columns * 3 + Lines * sizeof(char *) * 3);
   scr->lnums = (int *) (mem + Lines * Columns * 3 + Lines * sizeof(char *) * 3 + Lines * sizeof(int));

   for (i = 0; i < Lines; i++)
   {
      scr->chars[i] = (unsigned char *) (mem + Columns * i);
      scr->colors[i] = (unsigned char *) (mem + Lines * Columns + Columns * i);
      scr->attrs[i] = (unsigned char *) (mem + Lines * Columns * 2 + Columns * i);
      scr->lnums[i] = i;
      scr->touched[i] = 0;
   }

   memset(mem, ' ', Lines * Columns);
   memset(mem + Lines * Columns, COLOR_WHITE | COLOR_BACK_BLACK, Lines * Columns);
   memset(mem + Lines * Columns * 2, 0, Lines * Columns);

   scr->y = scr->x = 0;
   scr->beeps = 0;

   return scr;
}
