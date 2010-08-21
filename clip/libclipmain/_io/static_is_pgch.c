static int
is_pgch(int ch)
{
   switch (ch)
   {
   case PGCH_HLINE:
   case PGCH_LLCORNER:
   case PGCH_LRCORNER:
   case PGCH_TTEE:
   case PGCH_RTEE:
   case PGCH_LTEE:
   case PGCH_BTEE:
   case PGCH_ULCORNER:
   case PGCH_URCORNER:
   case PGCH_VLINE:

   case PGCH_HLINE2:
   case PGCH_LLCORNER2:
   case PGCH_LRCORNER2:
   case PGCH_TTEE2:
   case PGCH_RTEE2:
   case PGCH_LTEE2:
   case PGCH_BTEE2:
   case PGCH_ULCORNER2:
   case PGCH_URCORNER2:
   case PGCH_VLINE2:

   case PGCH_PLUS:
   case PGCH_PLUS2:
   case PGCH_BLOCK:

   case PGCH_RARROW:
   case PGCH_LARROW:
      return 1;
   }

   return 0;
}
