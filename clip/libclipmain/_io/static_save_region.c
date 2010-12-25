static char *
save_region(ClipMachine * ClipMachineMemory, char *mem, int top, int left, int bottom, int right, int *len)
{
   int i, j;

   unsigned char *s, *p;

   Screen *sp = ClipMachineMemory->screen;

#ifdef CLIP_DOS_SCRBUF
   i = 2;
#else
   i = 3;
#endif

   *len = (bottom - top + 1) * (right - left + 1) * i;
   s = (unsigned char *) realloc(mem, *len);

   for (i = top, p = s; i <= bottom; ++i)
      for (j = left; j <= right; ++j)
      {
	 *p++ = sp->chars[i][j];
	 *p++ = sp->colors[i][j];
#ifndef CLIP_DOS_SCRBUF
	 *p++ = sp->attrs[i][j];
#endif
      }
   return (char *) s;
}
