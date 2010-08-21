int
clip_SCREENCHAR(ClipMachine * ClipMachineMemory)
{
   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   Screen *sp = ClipMachineMemory->screen;

   char *r = calloc(1, 2);

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;

   r[0] = sp->chars[top][left];
   _clip_retcn_m(ClipMachineMemory, r, 1);
   return 0;
}
