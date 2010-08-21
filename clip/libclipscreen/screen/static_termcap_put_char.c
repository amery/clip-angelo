static void
termcap_put_char(ScreenData * dp, int ch, int utf8term)
{
   if (dp->lineDrawMode)
   {
      if (dp->termcap_GE)
	 termcap_put_raw_str(dp, dp->termcap_GE);
      else if (dp->termcap_AE)
	 termcap_put_raw_str(dp, dp->termcap_AE);
      dp->lineDrawMode = 0;
   }

   if ((ch & 0x80) || (ch < 32))
   {
      int pg;

      if (dp->utf8_mode /*&& utf8term */ )
      {
	 char utfch[7];

	 int n;

	 n = u32toutf8(utfch, dp->uniTable[ch]);
	 utfch[n] = 0;
	 termcap_put_raw_str(dp, utfch);
      }
      else if (scr_scan_mode && ch > 32)
	 termcap_put_raw_char(dp->outputTable[ch], dp);
      else if (dp->pg_mode && (pg = dp->pgTable[ch]))
	 termcap_put_graph_char(dp, (pg >= PG_SIZE) ? pg : dp->base->pg_chars[pg]);
      else
	 termcap_put_raw_char(dp->outputTable[ch], dp);
   }
   else
      termcap_put_raw_char(ch, dp);
}
