static void
choose_quot(char *str, int l, char *lquot, char *rquot)
{
   char *s;

   int ch, n = 0;

   do
   {
      int lq = *lquot;

      int rq = *rquot;

      for (s = str; (ch = *s); ++s)
	 if (ch == lq || ch == rq)
	    break;
      if (!ch)
	 return;
      switch (*lquot)
      {
      case '"':
	 *lquot = '\'';
	 *rquot = '\'';
	 break;
      case '\'':
      case '[':
	 *lquot = '"';
	 *rquot = '"';
	 break;
      }
   }
   while (n++ < 3);
}
