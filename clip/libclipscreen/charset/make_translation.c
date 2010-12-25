int
make_translation(cons_CharsetEntry * cp1, int len1, cons_CharsetEntry * cp2, int len2, unsigned char buf[256])
{
   int i;

   for (i = 0; i < 256; i++)
      buf[i] = i;

   for (i = 0; i < len1; i++)
   {
      int ch;

      unsigned long unich;

      cons_CharsetEntry *cp;

      int j;

      cp = cp1 + i;

		ch = cp->ch_of_cons_CharsetEntry;
		unich = cp->unich_of_cons_CharsetEntry;

      if (ch >= 256)
	 continue;

      for (j = 0; j < len2; j++)
      {
	 cons_CharsetEntry *cpo = cp2 + j;

	 if (cpo->unich_of_cons_CharsetEntry == unich && cpo->ch_of_cons_CharsetEntry < 256)
	 {
		 buf[ch] = cpo->ch_of_cons_CharsetEntry;
	    break;
	 }
      }

   }

   for (i = 0; i < 32; i++)
      buf[i] = i;

   return 0;
}
