char *
_clip_gettext(const char *msg)
{
   Locale *locale;

   locale = find_locale("cliprt");

   if (locale)
   {
      char *s;

      s = find_msg(locale, msg, 0);
      if (s)
      {
	 if (locale->charset && strcasecmp(locale->charset, _clip_hostcs))
	 {
	    static char buf[1024];

	    int l = sizeof(buf) - 1;

	    buf[l] = 0;
	    _clip_translate_charset(locale->charset, _clip_hostcs, (unsigned char *) s, (unsigned char *) buf, l);
	    _clip_logg(3, "localed msg: %s -> %s: %.*s -> %.*s", locale->charset, _clip_hostcs, l, s, l, buf);
	    return buf;
	 }
	 else
	    return s;
      }
      else
	 return (char *) msg;
   }
   else
      return (char *) msg;
}
