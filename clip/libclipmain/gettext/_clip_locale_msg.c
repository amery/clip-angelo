void
_clip_locale_msg(char *module, char *msg, char **dst)
{
   Locale *locale;

   locale = find_locale(module);

   if (locale)
   {
      char *s;

      s = find_msg(locale, msg, 0);
      if (s)
      {
	 _clip_logg(4, "locale msg: %s -> %s: %s", locale->charset, _clip_hostcs, s);
	 if (locale->charset && strcasecmp(locale->charset, _clip_hostcs))
	 {
	    int l = strlen(s);

	    *dst = (char *) malloc(l + 1);
	    (*dst)[l] = 0;
	    _clip_translate_charset(locale->charset, _clip_hostcs, (unsigned char *) s, (unsigned char *) (*dst), l);
	    _clip_logg(3, "localed msg: %s -> %s: %.*s -> %.*s", locale->charset, _clip_hostcs, l, s, l, *dst);
	    return;
	 }
	 else
	    msg = s;
      }
   }
   *dst = strdup(msg);
}
