static void
termcap_beep(ScreenData * dp)
{
   if (dp->lineDrawMode)
   {
      if (dp->termcap_GE)
	 termcap_put_raw_str(dp, dp->termcap_GE);
      else if (dp->termcap_AE)
	 termcap_put_raw_str(dp, dp->termcap_AE);
      dp->lineDrawMode = 0;
   }
   if (dp->termcap_BL)
      termcap_put_raw_str(dp, dp->termcap_BL);
   else
      termcap_put_raw_str(dp, "\007");
}
