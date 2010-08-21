static void
setColorMap(ScreenData * dp, char *fg, char *bg)
{
   short buf[16];

   int i, fglen = 0;

   int bglen = 0;

   if (dp->termcap_NF > 16 || dp->termcap_NF < 0)
      dp->termcap_NF = 16;
   if (dp->termcap_NB > 16 || dp->termcap_NB < 0)
      dp->termcap_NB = 16;
   if (!dp->termcap_MF)
   {
      if (!strncasecmp(dp->base->terminalName, "pc3", 3))
	 dp->termcap_MF = "0125436789ADCBEF";
      else
	 dp->termcap_MF = "0123456789ABCDEF";
   }
   if (!dp->termcap_MB)
   {
      if (!strncasecmp(dp->base->terminalName, "pc3", 3))
	 dp->termcap_MB = "0125436789ADCBEF";
      else
	 dp->termcap_MB = "0123456789ABCDEF";
   }
   for (i = 0; i < 16; ++i)
      dp->ctab[i] = dp->btab[i] = -1;

   if (fg)
      fglen = strlen(fg);
   if (bg)
      bglen = strlen(bg);

   for (i = 0; i < 16 && i < dp->termcap_NF; ++i)
   {
      if (!dp->termcap_MF[i])
	 break;
      else if (dp->termcap_MF[i] >= '0' && dp->termcap_MF[i] <= '9')
	 dp->ctab[dp->termcap_MF[i] - '0'] = i;
      else if (dp->termcap_MF[i] >= 'A' && dp->termcap_MF[i] <= 'F')
	 dp->ctab[dp->termcap_MF[i] - 'A' + 10] = i;
   }
   memcpy(buf, dp->ctab, sizeof(buf));
   for (i = 0; i < 16 && i < dp->termcap_NF && i < fglen; ++i)
   {
      if (fg[i] >= '0' && fg[i] <= '9')
	 dp->ctab[i] = buf[fg[i] - '0'];
      else if (fg[i] >= 'A' && fg[i] <= 'F')
	 dp->ctab[i] = buf[fg[i] - 'A' + 10];
   }
   for (i = 0; i < 16 && i < dp->termcap_NB; ++i)
   {
      if (!dp->termcap_MB[i])
	 break;
      else if (dp->termcap_MB[i] >= '0' && dp->termcap_MB[i] <= '9')
	 dp->btab[dp->termcap_MB[i] - '0'] = i;
      else if (dp->termcap_MF[i] >= 'A' && dp->termcap_MF[i] <= 'F')
	 dp->btab[dp->termcap_MB[i] - 'A' + 10] = i;
   }
   memcpy(buf, dp->btab, sizeof(buf));
   for (i = 0; i < 16 && i < dp->termcap_NB && i < bglen; ++i)
   {
      if (bg[i] >= '0' && bg[i] <= '9')
	 dp->btab[i] = buf[bg[i] - '0'];
      else if (bg[i] >= 'A' && bg[i] <= 'F')
	 dp->btab[i] = buf[bg[i] - 'A' + 10];
   }
   for (i = 1; i < 8; ++i)
   {
      if (dp->ctab[i] >= 0 && dp->ctab[i + 8] < 0)
	 dp->ctab[i + 8] = dp->ctab[i];
      if (dp->ctab[i + 8] >= 0 && dp->ctab[i] < 0)
	 dp->ctab[i] = dp->ctab[i + 8];
      if (dp->btab[i] >= 0 && dp->btab[i + 8] < 0)
	 dp->btab[i + 8] = dp->btab[i];
      if (dp->btab[i + 8] >= 0 && dp->btab[i] < 0)
	 dp->btab[i] = dp->btab[i + 8];
   }
}
