/*
	   MSETBOUNDS( [<nTop>], [<nLeft>], [<nBottom>],
				 [<nRight>] ) --> NIL
*/
int
clip_MSETBOUNDS(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   int top, bottom, left, right;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   top = 0;
   bottom = sp->Lines - 1;
   left = 0;
   right = sp->Columns - 1;

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
   {
      top = _clip_parni(ClipMachineMemory, 1);
      if (top < 0)
	 top = 0;
      if (top >= sp->Lines)
	 top = sp->Lines - 1;
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
   {
      left = _clip_parni(ClipMachineMemory, 1);
      if (left < 0)
	 left = 0;
      if (left >= sp->Columns)
	 left = sp->Columns - 1;
   }

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
   {
      bottom = _clip_parni(ClipMachineMemory, 1);
      if (bottom < 0)
	 bottom = 0;
      if (bottom >= sp->Lines)
	 bottom = sp->Lines - 1;
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
   {
      right = _clip_parni(ClipMachineMemory, 1);
      if (right < 0)
	 right = 0;
      if (right >= sp->Columns)
	 right = sp->Columns - 1;
   }

   if (left > right || top > bottom)
      return EG_ARG;

   sp->mouse_top = top;
   sp->mouse_left = left;
   sp->mouse_bottom = bottom;
   sp->mouse_right = right;

   return 0;
}
