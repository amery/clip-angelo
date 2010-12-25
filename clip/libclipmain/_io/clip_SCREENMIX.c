/*
   screenmix(cCharStr, cAttrStr [, nLine [,nColumn]]) ->Nil
 */
int
clip_SCREENMIX(ClipMachine * ClipMachineMemory)
{
   int len, alen;

   unsigned char *str;

   unsigned char *astr;

   int y, x, ey, ex, i, l;

   Screen *sp;

   ClipWindow *wp;

   str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &len);
   astr = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &alen);

   if (!str || !astr)
      return EG_ARG;

   _clip_fullscreen(ClipMachineMemory);

   sp = ClipMachineMemory->screen;

   wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   if (ClipMachineMemory->argc > 2)
      y = _clip_parnl(ClipMachineMemory, 3);
   else
      y = ClipMachineMemory->screen->y - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;

   if (ClipMachineMemory->argc > 3)
      x = _clip_parnl(ClipMachineMemory, 4);
   else
      x = ClipMachineMemory->screen->x - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;

   if (len < alen)
      l = alen;
   else
      l = len;

   ex = x + l;
   ey = y;

   clip_region(ClipMachineMemory, &y, &x, &ey, &ex, 1, -1);

   if (y <= ey)
   {
      sp->touched[y] = 1;
      for (i = 0; x <= ex && i <= l; ++i, ++x)
      {
	 if (i < len)
	    sp->chars[y][x] = str[i];
	 if (i < alen)
	    sp->colors[y][x] = astr[i];
      }
   }

   sync_mp(ClipMachineMemory);

   return 0;
}
