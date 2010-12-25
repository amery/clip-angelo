void
_clip_locale_msg_plural(char *module, char *msgid, char *msgid_plural, long n, char **dst)
{
   Locale *lp;

   lp = find_locale(module);

   if (lp)
   {
      int i, l;

      unsigned long int nn = 0;

      char *ep;

      char *sp = 0;

#ifdef PO_COMPAT
      {
	 int l1, l2;

	 char *buf;

	 l1 = strlen(msgid);
	 l2 = strlen(msgid_plural);
	 buf = alloca(l1 + l2 + 2);

	 memcpy(buf, msgid, l1);
	 memcpy(buf + l1 + 1, msgid_plural, l2);
	 buf[l1] = PO_COMPAT_CHAR;
	 buf[l1 + l2 + 1] = 0;

	 sp = find_msg(lp, buf, &l);
      }
#endif
      if (!sp)
	 sp = find_msg(lp, msgid, &l);
      if (!sp)
	 goto ret;

      if (!lp->pd)
      {
	 if (n == 1)
	    goto retok;
	 sp = find_msg(lp, msgid_plural, &l);
	 if (sp)
	    goto retok;
	 else
	    goto ret;
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

    retok:
      if (sp)
      {
#ifndef PO_COMPAT
	 l = strlen(sp);
#endif
	 _clip_logg(4, "locale msg plural(%ld:%lu): %s -> %s: %s", n, nn, lp->charset, _clip_hostcs, sp);
	 if (lp->charset && strcasecmp(lp->charset, _clip_hostcs))
	 {
	    *dst = (char *) malloc(l + 1);
	    (*dst)[l] = 0;
	    _clip_translate_charset(lp->charset, _clip_hostcs, (unsigned char *) sp, (unsigned char *) (*dst), l);
	    _clip_logg(3, "localed msg: %s -> %s: %.*s -> %.*s", lp->charset, _clip_hostcs, l, sp, l, *dst);
	 }
	 else
	 {
	    *dst = (char *) malloc(l + 1);
	    (*dst)[l] = 0;
	    memcpy(*dst, sp, l);
	 }
	 return;
      }
   }

 ret:
   if (n == 1)
      *dst = strdup(msgid);
   else
      *dst = strdup(msgid_plural);
}
