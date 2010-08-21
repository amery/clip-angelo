char *
_clic_ngettext(const char *msg, const char *nmsg, int n)
{
   char *sp = 0;

   int l = 0;

   if (!lp && !inited)
   {
      inited = 1;
      lp = init_locale("clip");
   }

   if (!lp)
      goto ret;

#ifdef PO_COMPAT
   {
      int l1, l2;

      char *buf;

      l1 = strlen(msg);
      l2 = strlen(nmsg);
#ifdef OS_MINGW
      buf = malloc(l1 + l2 + 2);
#else
      buf = alloca(l1 + l2 + 2);
#endif

      memcpy(buf, msg, l1);
      memcpy(buf + l1 + 1, nmsg, l2);
      buf[l1] = PO_COMPAT_CHAR;
      buf[l1 + l2 + 1] = 0;

      sp = find_msg(lp, buf, &l);
#ifdef OS_MINGW
      free(buf);
#endif
   }
#endif

   if (!sp)
      sp = find_msg(lp, msg, &l);

   if (sp)
   {
      int i;

      unsigned long int nn;

      char *ep;

      if (!lp->pd)
      {
	 if (n == 1)
	    return tr_charset(lp, sp);
	 sp = find_msg(lp, nmsg, &l);
	 if (sp)
	    return tr_charset(lp, sp);
	 else
	    return (char *) nmsg;
      }

      nn = plural_eval(lp->pd, n);
      ep = sp + l;
      for (i = 0; i < lp->nplural && sp < ep; i++)
      {
	 char *p;

#ifdef PO_COMPAT
	 p = strchr(sp, PO_COMPAT_CHAR);
	 if (!p)
	 {
	    l = strlen(sp);
	    break;
	 }
	 else
	 {
	    l = p - sp;
	    p++;
	 }
#else
	 p = sp + strlen(sp) + 1;
#endif
	 if (p >= ep || i >= nn)
	    break;
	 sp = p;
      }

#ifdef PO_COMPAT
      {
	 /* this is a memleak!!! but... */
	 char *rp;

	 rp = (char *) malloc(l + 1);
	 memcpy(rp, sp, l);
	 rp[l] = 0;
	 return tr_charset(lp, rp);
      }
#else
      return tr_charset(lp, sp);
#endif
   }

 ret:
   return n > 1 ? (char *) nmsg : (char *) msg;
}
