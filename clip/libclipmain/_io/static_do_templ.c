static int
do_templ(char *str, char *templ, char *buf, int buflen, int ins, int left, int no_zero)
{
   char *t, *te, *s, *se, *d, *e, *p;

   int dot = 0;

   if (!templ)
   {
      int ret, l = strlen(str);

      if (l >= buflen)
	 ret = 0;
      else
	 ret = 1;

      strncpy(buf, str, buflen);
      return ret;
   }

   for (t = templ, s = str, d = buf, e = buf + buflen - 1; *t && d < e;)
   {
      int f_type, t_len, s_len, i, lead;

      /* find template and source field */
      lead = 0;
    next:
      switch (*t)
      {
      case 0:
	 break;
      case '$':
      case '*':
	 lead = *t;
	 ++t;
	 goto next;
      case '9':
      case '#':
      case 'N':
	 for (t_len = 1, f_type = *t, te = t + t_len; *te == f_type; ++te, ++t_len)
	    ;
	 while (*s && !is_num(*s))
	    ++s;
	 for (s_len = 0, se = s; is_num(*se); ++se, ++s_len)
	    ;
	 if (s_len <= t_len)
	 {
	    if (!dot && !left)
	       for (i = t_len - s_len; i && d < e; --i, ++d)
		  *d = ' ';
	    if (lead)
	       *d++ = lead;
	    if (no_zero && s_len == 1 && *s == '0')
	       *d++ = ' ';
	    else
	       for (i = 0; i < s_len && d < e; ++d, ++s, ++i)
		  *d = *s;
	    if (dot && !no_zero)
	       for (i = t_len - s_len; i && d < e; --i, ++d)
		  *d = '0';
	    else if (left)
	       for (i = t_len - s_len; i && d < e; --i, ++d)
		  *d = ' ';
	    t = te;
	    s = se;
	    *d = 0;
	    dot = 0;
	    continue;
	 }
	 else
	    /* overflow */
	 {
	    for (i = 0; i < t_len && d < e; ++i, ++d, ++s)
	       *d = *s;
	    *d = 0;
	    s = se;
	    t = te;
	    continue;
	 }
	 break;
      case 'A':
	 *d = *s;
	 break;
      case '.':
	 p = strchr(s, '.');
	 if (p)
	 {
	    s = p;
	    *d = *s;
	 }
	 else
	 {
	    *d = '.';
	    if (*s)
	       --s;
	 }
	 dot++;
	 break;
      case ',':
	 p = strchr(s, ',');
	 if (p)
	 {
	    s = p;
	    *d = *s;
	 }
	 else
	 {
	    *d = ',';
	    if (*s)
	       --s;
	 }
	 break;
      case '!':
	 *d = toupper(*s);
	 break;
      case 'Y':
	 switch (toupper(*s))
	 {
	 case 'T':
	 case 'Y':
	    *d = 'Y';
	    break;
	 case 'F':
	 case 'N':
	    *d = 'N';
	    break;
	 default:
	    *d = *s;
	    break;
	 }
	 break;
      case 'L':
	 switch (toupper(*s))
	 {
	 case 'T':
	 case 'Y':
	    *d = 'T';
	    break;
	 case 'F':
	 case 'N':
	    *d = 'F';
	    break;
	 default:
	    *d = *s;
	    break;
	 }
	 break;
      default:
	 *d = *t;
	 if (ins && *s)
	    --s;
	 break;
      }
      if (*s)
	 ++s;
      ++d;
      ++t;
   }

   /* just copy rest */
   for (; *s && d < e; ++s, ++d)
      *d = *s;

   *d = 0;
   if (d == e)
      return 0;
   else
      return 1;
}
