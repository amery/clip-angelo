static void
termcap_set_fgbg(ScreenData * dp, int fg, int bg)
{
   /* This should be optimized later. */
   /* For example, we could have an array of 128 color */
   /* switching escape strings, precompiled during Init phase. */
   char buf[32];

   int bold, blink;

   fg = dp->ctab[fg];
   if (fg < 0)
      fg = 7;			/* bg = ctab [bg]; */
   bg = dp->btab[bg];
   if (bg < 0)
      bg = 0;

   bold = (fg & 0x8 && dp->useBoldAttr ? 1 : 0);
   blink = (bg & 0x8 && dp->useBlinkAttr ? 1 : 0);

   if ((bold ^ dp->boldMode) || (blink ^ dp->blinkMode))
   {
      termcap_put_raw_str(dp, dp->termcap_ME);
      dp->boldMode = 0;
      dp->blinkMode = 0;
   }

   if (bold && !dp->boldMode)
   {
      termcap_put_raw_str(dp, dp->termcap_MD);
      dp->boldMode = 1;
   }
   if (blink && !dp->blinkMode)
   {
      termcap_put_raw_str(dp, dp->termcap_mb);
      dp->blinkMode = 1;
   }

   if (dp->termcap_AF && dp->termcap_AB)
   {
      scr_tgoto(dp->termcap_AF, 0, /*fg, */ cga_to_ansi[fg], buf,
		sizeof(buf));
      termcap_put_raw_str(dp, buf);
      scr_tgoto(dp->termcap_AB, 0, /*bg, */ cga_to_ansi[bg], buf,
		sizeof(buf));
      termcap_put_raw_str(dp, buf);
   }
   else if (dp->termcap_SETF && dp->termcap_SETB)
   {
      scr_tgoto(dp->termcap_SETF, 0, fg, /*cga_to_ansi[fg], */ buf,
		sizeof(buf));
      termcap_put_raw_str(dp, buf);
      scr_tgoto(dp->termcap_SETB, 0, bg, /*cga_to_ansi[bg], */ buf,
		sizeof(buf));
      termcap_put_raw_str(dp, buf);
   }
   else if (dp->termcap_C2 && dp->termcap_CF)
   {
      scr_tgoto(dp->termcap_CF, bg, fg,
		/*cga_to_ansi[bg], cga_to_ansi[fg], */ buf, sizeof(buf));
      termcap_put_raw_str(dp, buf);
   }
   else if (dp->termcap_CF && dp->termcap_CB)
   {
      scr_tgoto(dp->termcap_CF, 0, fg /* cga_to_ansi[fg] */ , buf,
		sizeof(buf));
      termcap_put_raw_str(dp, buf);
      scr_tgoto(dp->termcap_CB, 0, bg /* cga_to_ansi[bg] */ , buf,
		sizeof(buf));
      termcap_put_raw_str(dp, buf);
   }
}
