int
destroy_tty(struct ScreenBase *base)
{
   ScreenData *dp;

   if (!base)
      return -1;

   dp = (ScreenData *) base->data;
   if (!dp)
      return 0;

#ifdef _WIN32
   if (w32_console)
   {
      if (base->clear_on_exit)
	 w32_clear();
   }
   else
#endif
   {
      termcap_put_raw_str(dp, dp->termcap_ME);
      termcap_set_color(dp, COLOR_WHITE | COLOR_BACK_BLACK);
      if (base->clear_on_exit)
	 termcap_clear_screen(dp);
      termcap_set_cursor(dp, base->Lines - 1, 0);
      /*termcap_put_raw_str(dp, dp->termcap_TE); */
      termcap_put_raw_str(dp, dp->termcap_FS);
      termcap_put_raw_str(dp, dp->termcap_VE);
      termcap_put_raw_str(dp, dp->termcap_KE);
      termcap_flush(dp);
   }

   restore_tty(base);

   delete_Screen(base->realScreen);
   /*destroy_ScreenData(dp);
      free(dp); */
   free(base->terminalName);
   memset(base, 0, sizeof(ScreenBase));

   return 0;
}
