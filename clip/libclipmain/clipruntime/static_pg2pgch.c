static unsigned char
pg2pgch(unsigned char ch)
{
   switch (ch)
   {
   case PG_HLINE:
      return PGCH_HLINE;
   case PG_LLCORNER:
      return PGCH_LLCORNER;
   case PG_LRCORNER:
      return PGCH_LRCORNER;
   case PG_TTEE:
      return PGCH_TTEE;
   case PG_RTEE:
      return PGCH_RTEE;
   case PG_LTEE:
      return PGCH_LTEE;
   case PG_BTEE:
      return PGCH_BTEE;
   case PG_ULCORNER:
      return PGCH_ULCORNER;
   case PG_URCORNER:
      return PGCH_URCORNER;
   case PG_VLINE:
      return PGCH_VLINE;

   case PG_HLINE2:
      return PGCH_HLINE2;
   case PG_LLCORNER2:
      return PGCH_LLCORNER2;
   case PG_LRCORNER2:
      return PGCH_LRCORNER2;
   case PG_TTEE2:
      return PGCH_TTEE2;
   case PG_RTEE2:
      return PGCH_RTEE2;
   case PG_LTEE2:
      return PGCH_LTEE2;
   case PG_BTEE2:
      return PGCH_BTEE2;
   case PG_ULCORNER2:
      return PGCH_ULCORNER2;
   case PG_URCORNER2:
      return PGCH_URCORNER2;
   case PG_VLINE2:
      return PGCH_VLINE2;

   case PG_PLUS:
      return PGCH_PLUS;
   case PG_PLUS2:
      return PGCH_PLUS2;
   case PG_BLOCK:
      return PGCH_BLOCK;

   case PG_LARROW:
      return PGCH_LARROW;
   case PG_RARROW:
      return PGCH_RARROW;

   }
   return 0;
}
