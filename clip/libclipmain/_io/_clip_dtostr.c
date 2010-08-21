int
_clip_dtostr(char *buf, int len, int dec, double d, int zero)
{
#if 1
   int im = 0, ie;

   char *s;

   double de;

   int neg = 0;

   int cnt = 0;

   int snp = 0;

   buf[len] = 0;

#ifdef CLIP_DEBUG
   printf("\n00dec=%d,len=%d", dec, len);
#endif
   if (dec == 0 ? dec >= len : dec >= len - 1)	/* (1) if not true dec + 1 < len */
      dec = 0;
   /*goto err;
      else
    */
   if (dec > 9)
      snp = 1;
   else
   {
      if (d < 0)
      {
	 d = -d;
	 neg = 1;
      }
      if (dec > 0)
      {
	 cnt = dec;
#ifdef ARCH_i386
	 (*(long long *) &d)++;
#endif
	 im = (int) (modf(d, &de) * _clip_deck[dec] + 0.5f);
	 /* chek if fractional part was rounded up */
	 /* commented out - 'ie += im' is faster
	    if (im >= _clip_deck[dec])
	    {
	    ++de;
	    im -= _clip_deck[dec];
	    }
	  */
      }
      else
	 modf(d + 0.5f, &de);

      if (de >= 1000000000)
	 snp = 1;
   }

#ifdef CLIP_DEBUG
   printf("\n01dec=%d,len=%d", dec, len);
#endif
   if (snp)
   {
#if 1				/* Uri added for str(-1234567890,10) */
      double diff;

      snprintf(buf, len + 1, zero ? "%0*.*f" : "%*.*f", len, dec, neg ? -d : d);
      diff = fabs(fabs(atof(buf)) - d);
      if (diff > (d * MIN_NUMERIC_DIFF))
	 goto err;
      /*
         if (fabs(atof(buf)) != d)
         goto err;
       */
#else
      if (snprintf(buf, len + 1, zero ? "%0*.*f" : "%*.*f", len, dec, neg ? -d : d) < 0)
	 goto err;
#endif
      s = strchr(buf, ',');
      if (s)
	 *s = '.';
      return 0;
   }

#ifdef CLIP_DEBUG
   printf("\n02dec=%d,len=%d", dec, len);
#endif
   ie = (int) de;
   s = buf + len;

   if (cnt)
   {
      while (cnt--)
      {
	 /* (1) is fales, dec + 1 < len always so s > b */
	 *(--s) = '0' + im % 10;
	 im /= 10;
      }
      *(--s) = '.';
      ie += im;
      /* im > 0 if fractional part was rounded up
         the case of *.999 with dec == 2 (im == 100 in that case)
       */
   }

#ifdef CLIP_DEBUG
   printf("\n03dec=%d,len=%d", dec, len);
#endif
   while (1)
   {
      if (--s < buf)
	 goto err;
      *s = '0' + ie % 10;
      ie /= 10;
      if (!ie)
	 break;
   }

#ifdef CLIP_DEBUG
   printf("\n04dec=%d,len=%d", dec, len);
#endif
   if (zero)
   {
      if (neg ? s <= buf : s < buf)
	 goto err;
#ifdef CLIP_DEBUG
      printf("\n05dec=%d,len=%d", dec, len);
#endif
      while (s > buf)
	 *(--s) = '0';
      if (neg)
	 *s = '-';
   }
   else
   {
#ifdef CLIP_DEBUG
      printf("\n06dec=%d,len=%d", dec, len);
#endif
      if (neg)
      {
	 if (--s < buf)
	    goto err;
	 *s = '-';
      }
      while (s > buf)
	 *(--s) = ' ';
#ifdef CLIP_DEBUG
      printf("\n07dec=%d,len=%d", dec, len);
#endif
   }

   return 0;

 err:
   memset(buf, '*', len);
   if (dec > 0 && dec < len)
      buf[len - dec - 1] = '.';

   return 1;
}
#else
   int im, ie;

   int l;

   char *s;

   double de;

   int neg = 0;

   int cnt;

   buf[len] = 0;
   if (dec < 0 || dec > 9 || dec >= len - 1)
   {
      if (snprintf(buf, len + 1, "%*.*f", len, dec, d) < 0)
	 goto err;
      s = strchr(buf, ',');
      if (s)
	 *s = '.';
      return 0;
   }

   if (d < 0)
   {
      d = -d;
      neg = 1;
   }
   im = (int) (modf(d, &de) * _clip_deck[dec] + 0.5f);
   if (de >= 1000000000)
   {
      if (snprintf(buf, len + 1, "%*.*f", len, dec, neg ? -d : d) < 0)
	 goto err;
      s = strchr(buf, ',');
      if (s)
	 *s = '.';
      return 0;
   }
   ie = (int) de;
   s = buf + len;
   if (dec)
   {
      cnt = 0;
      while (cnt++ < dec)
      {
	 l = im % 10;
	 s--;
	 if (s < buf)
	    goto err;
	 *s = '0' + l;
	 im = im / 10;
      }
      *(--s) = '.';
   }
   ie += im;			/* the case of *.999 with dec == 2 (im == 100 in that case) */
   while (1)
   {
      l = ie % 10;
      s--;
      if (s < buf)
	 goto err;
      *s = '0' + l;
      ie = ie / 10;
      if (!ie)
	 break;
   }
   if (zero)
   {
      while (s > buf)
	 *(--s) = '0';
      if (neg)
      {
	 if (s < buf)
	    goto err;
	 *s = '-';
      }
   }
   else
   {
      if (neg)
      {
	 s--;
	 if (s < buf)
	    goto err;
	 *s = '-';
      }
      while (s > buf)
	 *(--s) = ' ';
   }
   return 0;
 err:
   memset(buf, '*', len);
   if (dec > 0 && dec < len)
      buf[len - dec - 1] = '.';
   return 1;
}
#endif
