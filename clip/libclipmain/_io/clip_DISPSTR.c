/*
   dispstr(nStr,nCol,cStr);
 */
int
clip_DISPSTR(ClipMachine * ClipMachineMemory)
{
   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int len;

   char *str = _clip_parcl(ClipMachineMemory, 3, &len);

   int bottom, right, i, k;

   Screen *sp;

   if (!str)
      return 0;
   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   bottom = top;
   right = left + len - 1;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);
   sp->touched[top] = 1;
   for (k = 0, i = left; i <= right; ++i, ++k)
      sp->chars[top][i] = str[k];

   sync_mp(ClipMachineMemory);

   return 0;
}
