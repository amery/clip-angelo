int
clip_SCREENATTR(ClipMachine * ClipMachineMemory)
{
   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   Screen *sp = ClipMachineMemory->screen;

   int r;

   _clip_retni(ClipMachineMemory, 0);
   if (!ClipMachineMemory->fullscreen)
      return 0;

   if ((_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType) || (top < 0) || (top >= sp->base->Lines))
      top = sp->y;
   if ((_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType) || (left < 0) || (left >= sp->base->Columns))
      left = sp->x;

   r = (int) sp->colors[top][left];
   _clip_retni(ClipMachineMemory, r);
   return 0;
}
