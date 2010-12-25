static void
profile_enable(char *opt)
{
   char *s;

   s = strchr(opt, '=');
   if (s)
      _clip_profiler = atoi(s + 1);
   else
      _clip_profiler++;
/*_clip_logg(0, "set profiledebug level to %d", _clip_debuglevel);*/
}
