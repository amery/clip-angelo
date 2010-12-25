void
_clip_add_locale(char *locale)
{
   if (strcasecmp(locale, "C") && strcasecmp(locale, "POSIX"))
      setlocale(LC_ALL, locale);
   _clip_logg(3, "add locale '%s'", locale);
   append_Coll(&locale_names, strdup(locale));
}
