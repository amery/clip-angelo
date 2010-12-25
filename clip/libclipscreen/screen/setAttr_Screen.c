void
setAttr_Screen(Screen * scr, int *pblink, int *pbold)
{
   ScreenData *dp = (ScreenData *) scr->base->data;

   int blink, bold;

   blink = dp->useBlinkAttr;
   bold = dp->useBoldAttr;

   if (pblink)
   {
      if (*pblink >= 0)
	 dp->useBlinkAttr = *pblink ? 1 : 0;
      *pblink = blink;
   }
   if (pbold)
   {
      if (*pbold >= 0)
	 dp->useBoldAttr = *pbold ? 1 : 0;
      *pbold = bold;
   }
}
