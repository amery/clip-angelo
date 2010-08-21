static void
termcap_put_raw_str(ScreenData * dp, const char *str)
{
   if (!str)
      return;
   scr_tputs(str, 1, termcap_put_raw_char, dp);
}
