int
_clip_loglevel(int newlevel)
{
   int ret = log_level;

   if (newlevel >= 0)
      log_level = newlevel;
   return ret;
}
