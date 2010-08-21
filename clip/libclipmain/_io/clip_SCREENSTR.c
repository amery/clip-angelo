int
clip_SCREENSTR(ClipMachine * ClipMachineMemory)
{
   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int l = _clip_parni(ClipMachineMemory, 3);

   Screen *sp = ClipMachineMemory->screen;

   char *r, *p;

   int i, b;

#ifdef CLIP_DOS_SCRBUF
   b = 2;
#else
   b = 3;
#endif

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;
   if ((_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType) || (l < 1) || (left + l > sp->base->Columns))
      l = sp->base->Columns - left;

   r = calloc(1, l * b + 1);

   for (i = 0, p = r; i < l; i++)
   {
      *p++ = sp->chars[top][left + i];
      *p++ = sp->colors[top][left + i];
#ifndef CLIP_DOS_SCRBUF
      *p++ = sp->attrs[top][left + i];
#endif
   }
   _clip_retcn_m(ClipMachineMemory, r, l * b);
   return 0;
}
