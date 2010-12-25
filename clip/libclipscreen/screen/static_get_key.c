static char *
get_key(ScreenData * dp, int no)
{
   char *rp;

   if (dp->terminfo.keys[no] == -1)
      return 0;

   rp = dp->terminfo.buf + dp->terminfo.keys[no];
   if (!*rp)
      return 0;
   else
      return rp;
}
