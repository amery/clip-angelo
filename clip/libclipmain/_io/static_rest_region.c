static void
rest_region(ClipMachine * ClipMachineMemory, int top, int left, int bottom, int right, char *s, int l)
{
   int i, j;

   unsigned char *p, *e;

   Screen *sp = ClipMachineMemory->screen;

   int lines = sp->base->Lines;

   int columns = sp->base->Columns;

   for (i = top, p = (unsigned char *) s, e = p + l; i <= bottom; ++i)
   {
      if (i < 0 || i >= lines)
	 continue;
      sp->touched[i] = 1;
      for (j = left; j <= right; ++j)
      {
	 if (j < 0 || j >= columns)
	    continue;
	 if (p >= e)
	    return;
	 sp->chars[i][j] = *p++;
	 if (p >= e)
	    return;
	 sp->colors[i][j] = *p++;
#ifndef CLIP_DOS_SCRBUF
	 if (p >= e)
	    return;
	 sp->attrs[i][j] = *p++;
#endif
      }
   }
}
