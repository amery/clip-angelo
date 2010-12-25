static char *
get_str(ScreenData * dp, int no)
{
   char *rp;

   if (dp->terminfo.strings[no] == -1)
      return 0;

   rp = dp->terminfo.buf + dp->terminfo.strings[no];
   if (!*rp)
      return 0;
   else
      return rp;
}
