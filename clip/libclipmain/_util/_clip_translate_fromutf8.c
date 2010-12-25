/* Translate_FromUtf8(string, [source codepage]) */
int
_clip_translate_fromutf8(char *p1, char *str, int len, char **result)
{
   int i, j, mask, clen, len1 = 0;

   unsigned long *wcs, *wcp;

   unsigned char *cp, *end, c;

   char *res;

   int n;

   cons_CharsetEntry *cs1 = 0;

   if (!p1 || !str)
      return EG_ARG;

   cp = (unsigned char *) str;
   end = cp + len;
   n = 0;
   wcs = calloc(len + 1, sizeof(unsigned long));
   wcp = wcs;
   while (cp != end)
   {
      mask = 0;
      c = *cp;
      if (c < 0x80)
      {
	 clen = 1;
	 mask = 0x7f;
      }
      else if ((c & 0xe0) == 0xc0)
      {
	 clen = 2;
	 mask = 0x1f;
      }
      else if ((c & 0xf0) == 0xe0)
      {
	 clen = 3;
	 mask = 0x0f;
      }
      else if ((c & 0xf8) == 0xf0)
      {
	 clen = 4;
	 mask = 0x07;
      }
      else if ((c & 0xfc) == 0xf8)
      {
	 clen = 5;
	 mask = 0x03;
      }
      else if ((c & 0xfc) == 0xfc)
      {
	 clen = 6;
	 mask = 0x01;
      }
      else
      {
	 free(wcs);
	 return -1;
      }

      if (cp + clen > end)
      {
	 free(wcs);
	 return -1;
      }

      *wcp = (cp[0] & mask);
      for (i = 1; i < clen; i++)
      {
	 if ((cp[i] & 0xc0) != 0x80)
	 {
	    free(wcs);
	    return -1;
	 }
	 *wcp <<= 6;
	 *wcp |= (cp[i] & 0x3f);
      }

      cp += clen;
      wcp++;
      n++;
   }
   if (cp != end)
   {
      free(wcs);
      return -1;
   }

   /* n is the number of wide chars constructed */

   *result = calloc(n + 1, sizeof(char));
   res = *result;
   res[n] = 0;

   if (load_charset_name(p1, &cs1, &len1))
   {
      _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p1, strerror(errno));
   }
   wcp = wcs;
   if (cs1)
   {
      for (i = 0; i < n; i++)
	 for (j = 0; j < len1; j++)
	 {
	    cons_CharsetEntry *cp;

	    cp = cs1 + j;
		 if (wcs[i] == cp->unich_of_cons_CharsetEntry)
	    {
			 res[i] = cp->ch_of_cons_CharsetEntry;
	       break;
	    }
	 }
   }
   else
      strcpy(res, str);
   free(cs1);
   free(wcs);

   return 0;
}
