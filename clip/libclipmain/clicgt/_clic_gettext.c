char *
_clic_gettext(const char *s)
{
   char *sp;

   if (!lp && !inited)
   {
      inited = 1;
      lp = init_locale("clip");
   }

   if (!lp)
      return (char *) s;

   sp = find_msg(lp, s, 0);

   if (sp)
      return tr_charset(lp, sp);
   return (char *) s;
}
