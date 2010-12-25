int
clip_SCREENSTRING(ClipMachine * ClipMachineMemory)
{
   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int l = _clip_parni(ClipMachineMemory, 3);

   Screen *sp = ClipMachineMemory->screen;

   char *r;

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;
   if ((_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType) || (l < 1) || (left + l > sp->base->Columns))
      l = sp->base->Columns - left;

   r = calloc(1, l + 1);
   memcpy(r, sp->chars[top] + left, l);
   _clip_retcn_m(ClipMachineMemory, r, l);
   return 0;
}
