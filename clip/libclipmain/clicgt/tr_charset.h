static char *
tr_charset(Locale * lp, char *msg)
{
   if (lp && lp->charset && out_charset && strcasecmp(lp->charset, out_charset))
    {
#ifdef HAVE_ICONV
       char     *rp, *ip, *op;

       int       l, il, ol, rl;

       if (!lp->tr_inited)
	{
	   lp->it = iconv_open(out_charset, lp->charset);
	   lp->tr_inited = 1;
	}
       if (lp->it == (iconv_t) - 1)
	  return msg;
       il = l = strlen(msg);
       rl = ol = l * 3;
       rp = (char *) malloc(ol);
       ip = msg;
       op = rp;
       iconv(lp->it, &ip, (size_t *) & il, &op, (size_t *) & ol);
       rl -= ol;
       rp = (char *) realloc(rp, rl + 1);
       rp[rl] = 0;
       return rp;
#else
       char     *rp;

       int       i, rl;

       rl = strlen(msg);
       if (!lp->tr_inited)
	{
	   char     *p1, *p2;

	   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

	   int       len1 = 0, len2 = 0;

	   int       r1, r2;

	   p1 = lp->charset;
	   p2 = out_charset;

	   if ((r1 = load_charset_name(p1, &cs1, &len1)))
	    {
	       yywarning("cannot load charset '%s': %s", p1, strerror(errno));
	    }
	   if ((r2 = load_charset_name(p2, &cs2, &len2)))
	    {
	       yywarning("cannot load charset '%s': %s", p2, strerror(errno));
	    }

	   if (!r1 && !r2)
	    {
	       make_translation(cs1, len1, cs2, len2, lp->trans_tbl);
	    }
	   else
	    {
	       for (i = 0; i < 256; i++)
		  lp->trans_tbl[i] = i;
	    }
	   lp->tr_inited = 1;
	}
       rp = (char *) malloc(rl + 1);
       for (i = 0; i < rl; i++)
	  rp[i] = lp->trans_tbl[((unsigned char *) msg)[i]];
       rp[rl] = 0;
       return rp;
#endif
    }
   else
      return msg;
}
