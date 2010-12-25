/* Translate_ToUtf8([source codepage], string) */
int
_clip_translate_toutf8(char *p1, unsigned char *str, int len, char **result)
{
   int i, j, clen, total_len, wclen, first, len1 = 0;

   unsigned long *wcs, wc;

   char *res, *bp;

   const unsigned long *wcp;

   cons_CharsetEntry *cs1 = 0;

   if (!p1 || !str)
      return EG_ARG;

   wclen = strlen((const char *) str);
   wcs = calloc(wclen, sizeof(unsigned long));
   if (load_charset_name(p1, &cs1, &len1))
   {
      _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p1, strerror(errno));
   }
   if (cs1)
   {
      for (i = 0; i < wclen; i++)
      {
	 for (j = 0; j < len1; j++)
	 {
	    cons_CharsetEntry *cp;

	    cp = cs1 + j;
		 if (cp->ch_of_cons_CharsetEntry == str[i])
			 wcs[i] = cp->unich_of_cons_CharsetEntry;
	 }
      }
      free(cs1);
   }
   else
   {
      free(wcs);
      return -1;
   }

   wcp = wcs;
   total_len = 0;
   for (i = 0; i < wclen; i++)
   {
      wc = *wcp++;
      if (wc < 0x80)
	 total_len += 1;
      else if (wc < 0x800)
	 total_len += 2;
      else if (wc < 0x10000)
	 total_len += 3;
      else if (wc < 0x200000)
	 total_len += 4;
      else if (wc < 0x4000000)
	 total_len += 5;
      else
	 total_len += 6;
   }

   *result = calloc(total_len + 1, sizeof(char));
   res = *result;

   wcp = wcs;
   bp = res;
   for (i = 0; i < wclen; i++)
   {
      wc = *wcp++;

      if (wc < 0x80)
      {
	 first = 0;
	 clen = 1;
      }
      else if (wc < 0x800)
      {
	 first = 0xc0;
	 clen = 2;
      }
      else if (wc < 0x10000)
      {
	 first = 0xe0;
	 clen = 3;
      }
      else if (wc < 0x200000)
      {
	 first = 0xf0;
	 clen = 4;
      }
      else if (wc < 0x4000000)
      {
	 first = 0xf8;
	 clen = 5;
      }
      else
      {
	 first = 0xfc;
	 clen = 6;
      }

      switch (clen)
      {
      case 6:
	 bp[5] = (wc & 0x3f) | 0x80;
	 wc >>= 6;
      case 5:
	 bp[4] = (wc & 0x3f) | 0x80;
	 wc >>= 6;
      case 4:
	 bp[3] = (wc & 0x3f) | 0x80;
	 wc >>= 6;
      case 3:
	 bp[2] = (wc & 0x3f) | 0x80;
	 wc >>= 6;
      case 2:
	 bp[1] = (wc & 0x3f) | 0x80;
	 wc >>= 6;
      case 1:
	 bp[0] = wc | first;
      }

      bp += clen;
   }
   *bp = 0;

   free(wcs);

   return 0;
}
