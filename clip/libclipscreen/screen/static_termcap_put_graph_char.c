static void
termcap_put_graph_char(ScreenData * dp, int ch)
{
   if (!dp->lineDrawMode && (dp->termcap_GS || dp->termcap_AS))
   {
      if (dp->termcap_GS)
	 termcap_put_raw_str(dp, dp->termcap_GS);
      else
	 termcap_put_raw_str(dp, dp->termcap_AS);
      dp->lineDrawMode = 1;
   }

   if (scr_scan_mode)
      termcap_put_raw_char(dp->outputTable[ch], dp);
   else
      termcap_put_raw_char(ch, dp);
}
