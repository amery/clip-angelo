static void
out_scr(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int wrap)
{
   int i;

   unsigned char *s, ch;

   Screen *sp = ClipMachineMemory->screen;

   int x = sp->x;

   int y = sp->y;

   int top = -1024, left = -1024, bottom = 1024, right = 1024;

   if (ClipMachineMemory->obuf)
   {
      int i;

      for (i = 0; i < n; i++)
	 putByte_Buf(ClipMachineMemory->obuf, _clip_outtbl[(unsigned char) buf[i]]);
      return;
   }

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 1, -1);

   if (x > right || y > bottom || x < left || y < top)
      return;

   sp->touched[y] = 1;

   for (i = 0, s = (unsigned char *) buf; i < n; ++i, ++s)
   {
      switch ((ch = *s))
      {
      case '\n':
	 if (wrap)
	 {
	    ++y;
	    if (y <= bottom)
	       sp->touched[y] = 1;
	    x = left;
	 }
	 else
	    goto def;

	 break;
      case '\r':
	 if (wrap)
	 {
	    x = left;
	 }
	 else
	    goto def;
	 break;
      case '\b':
	 if (wrap)
	 {
	    --x;
	    if (x < left)
	    {
	       --y;
	       x = right;
	       sp->touched[y] = 1;
	    }
	 }
	 else
	    goto def;

	 break;
      case '\t':
	 if (wrap)
	 {
	    int i = x;

	    x = (x / 8 + 1) * 8;
	    for (; i < x; ++i)
	    {
	       if (i > right)
		  break;
	       sp->chars[y][i] = ' ';
	       sp->colors[y][i] = attr;
	       sp->attrs[y][i] = 0;
	    }
	 }
	 else
	    goto def;

	 break;
      default:
       def:
	 ch = ClipMachineMemory->term_xlat[ch];

	 if (ch < 32 && !is_pgch(ch))
	 {
	    sp->chars[y][x] = ch + 64;
	    sp->colors[y][x] = attr | 0x8;
	    sp->attrs[y][x] = 0;
	 }
	 else
	 {
	    sp->chars[y][x] = ch;
	    sp->colors[y][x] = attr;
	    sp->attrs[y][x] = 0;
	 }

	 ++x;
	 break;
      }

      if (wrap)
      {
	 if (x > right)
	 {
	    x = left;
	    y++;
	    if (y <= bottom)
	       sp->touched[y] = 1;
	 }
	 if (y > bottom)
	 {
	    top = 0;
	    clip_region(ClipMachineMemory, &top, 0, 0, 0, 0, -1);
	    while (y > bottom)
	    {
	       scrollw_Screen(sp, top, left, bottom, right, 1, attr);
	       --y;
	    }
	 }
	 if (y < top)
	 {
	    top = 0;
	    clip_region(ClipMachineMemory, &top, 0, 0, 0, 0, -1);
	    while (y < top)
	    {
	       scrollw_Screen(sp, top, left, bottom, right, -1, attr);
	       ++y;
	    }
	 }
      }
      else if (x > right)
	 break;
   }

   /*sp->touched[y] = 1; */
   sp->y = y;
   sp->x = x;
}
