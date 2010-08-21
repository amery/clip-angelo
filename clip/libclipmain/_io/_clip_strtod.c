CLIP_DLLEXPORT double
_clip_strtod(char *str, int *decpos)
{
#if 1
   int ie = 0;

   int im = 0;

   double d;

   char *s = str, *e, *m;

   int ecnt, mcnt, cnt;

   struct lconv *lc;

   int neg = 0;

   while (*s == ' ')
      s++;
   if (*s == '+')
      s++;
   if (!(((*s >= '0') && (*s <= '9')) || (*s == '.') || (*s == ',') || (*s == '-') || (*s == '+')))
   {
      if (decpos)
	 *decpos = strlen(str);
      return 0;
   }

   if (*s == '-')
   {
      neg = 1;
      s++;
   }
   e = s;
   while (*s >= '0' && *s <= '9')
      s++;
   ecnt = s - e;

   m = s;
   if ((*m == '.') || (*m == ','))
   {
      if (decpos)
	 *decpos = (s == str) ? 1 : s - str;
      m++;
      while (*m >= '0' && *m <= '9')
	 m++;
      mcnt = m - s - 1;
   }
   else
   {
      mcnt = 0;
      if (decpos)
	 *decpos = 0;
   }

   if (ecnt > 9 || mcnt > 9)
   {
      if (mcnt > 0)
      {
	 lc = localeconv();
	 *s = *lc->decimal_point;
	 d = strtod(e, NULL);
	 *s = '.';
      }
      else
	 d = strtod(e, NULL);
   }
   else
   {
      for (m = s + 1, cnt = mcnt; cnt > 0; m++, cnt--)
	 im = im * 10 + (*m - '0');

      for (; ecnt > 0; e++, ecnt--)
	 ie = ie * 10 + (*e - '0');

      d = (double) ie;
      if (im > 0)
	 d += (double) (im) / _clip_deck[mcnt];
   }

   return neg ? -d : d;

#else
   int ie = 0;

   int im = 0;

   double d;

   char *s = str, *e;

   int cnt, dcnt = 0;

   struct lconv *lc;

   int neg = 0;

   while (*s == ' ')
      s++;
   if (!(((*s >= '0') && (*s <= '9')) || (*s == '.') || (*s == '-') || (*s == '+')))
   {
      if (decpos)
	 *decpos = strlen(str);
      return 0;
   }
   while ((*s >= '0' && *s <= '9') || *s == '-' || *s == '+')
      s++;

   e = s;
   while ((*e >= '0' && *e <= '9') || *e == '.')
      e++;
   if (*s != 0 && *s == '.')
   {
      if (decpos)
	 *decpos = s - str;
   }
   else
   {
      s = NULL;
      if (decpos)
	 *decpos = 0;
   }
   if (s)
   {
      if (s - str > 9 || e - s - 1 > 9)
      {
	 lc = localeconv();
	 *s = *lc->decimal_point;
	 if (decpos)
	    *decpos = s - str;
	 return strtod(str, NULL);
      }
   }
   else if (e - str > 9)
   {
      return strtod(str, NULL);
   }
   if (s)
   {
      for (dcnt = 0; (--e) > s; dcnt++)
      {
	 if (*e >= '0' && *e <= '9')
	    im += (*e - '0') * _clip_deck[dcnt];
	 else
	 {
	    if (*e == ' ')
	       dcnt--;
	    else
	       break;
	 }
      }
   }
   for (cnt = 0; (--e) >= str; cnt++)
   {
      if (*e == '-')
      {
	 neg = 1;
	 break;
      }
      if (*e >= '0' && *e <= '9')
	 ie += (*e - '0') * _clip_deck[cnt];
      else
      {
	 if (*e == ' ')
	    cnt--;
	 else
	    break;
      }
   }
   d = (double) ie;
   if (s)
      d += (double) (im) / _clip_deck[dcnt];
   if (neg)
      d = -d;
   return d;
#endif
}
