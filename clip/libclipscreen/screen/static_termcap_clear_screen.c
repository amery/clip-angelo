static void
termcap_clear_screen(ScreenData * dp)
{
   if (dp->lineDrawMode)
   {
      if (dp->termcap_GE)
	 termcap_put_raw_str(dp, dp->termcap_GE);
      else if (dp->termcap_AE)
	 termcap_put_raw_str(dp, dp->termcap_AE);
      dp->lineDrawMode = 0;
   }
   termcap_put_raw_str(dp, dp->termcap_ME);
   dp->boldMode = 0;
   termcap_set_cursor(dp, 0, 0);
   termcap_put_raw_str(dp, " ");
   termcap_put_raw_str(dp, dp->termcap_CL);
   termcap_set_cursor(dp, 0, 0);
}
