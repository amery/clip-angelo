static void
termcap_set_cursor(ScreenData * dp, int y, int x)
{
   char buf[32];

   scr_tgoto(dp->termcap_CM, x, y, buf, sizeof(buf));
   termcap_put_raw_str(dp, buf);
}
