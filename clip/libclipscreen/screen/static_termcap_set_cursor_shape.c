static void
termcap_set_cursor_shape(ScreenData * dp, int cursor)
{
   if (dp->oldcursor == cursor)
      return;
#if 0
   printf("VE: '%s'\n", dp->termcap_VE ? dp->termcap_VE + 1 : "");
   printf("VI: '%s'\n", dp->termcap_VI ? dp->termcap_VI + 1 : "");
   printf("VS: '%s'\n", dp->termcap_VS ? dp->termcap_VS + 1 : "");
   fflush(stdout);
#endif

   switch (cursor)
   {
   case 0:
   default:
      termcap_put_raw_str(dp, dp->termcap_VE);
      break;
   case 1:
      termcap_put_raw_str(dp, dp->termcap_VI);
      break;
   case 2:
      if (dp->termcap_VS)
	 termcap_put_raw_str(dp, dp->termcap_VS);
      else
	 termcap_put_raw_str(dp, dp->termcap_VE);
      break;
   }

   dp->oldcursor = cursor;
}
