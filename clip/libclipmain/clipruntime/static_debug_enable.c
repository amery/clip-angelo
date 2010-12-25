static void
debug_enable(char *opt)
{
   char *s;

   s = strchr(opt, '=');
   if (s)
      _clip_debuglevel = atoi(s + 1);
   else
      _clip_debuglevel++;
/*_clip_logg(0, "set debug level to %d", _clip_debuglevel);*/
}
