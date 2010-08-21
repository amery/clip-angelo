/*
   Allocates allowable screen area for windows
 */
int
clip_WBOARD(ClipMachine * ClipMachineMemory)
{
   int top, left, bottom, right;

   _clip_fullscreen(ClipMachineMemory);

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      Screen *sp = ClipMachineMemory->screen;

      top = left = 0;
      bottom = sp->base->Lines - 1;
      right = sp->base->Columns - 1;
   }
   else
   {
      top = _clip_parni(ClipMachineMemory, 1);
      left = _clip_parni(ClipMachineMemory, 2);
      bottom = _clip_parni(ClipMachineMemory, 3);
      right = _clip_parni(ClipMachineMemory, 4);
   }

   ClipMachineMemory->wboard.top_of_ClipRect = top;
   ClipMachineMemory->wboard.left_of_ClipRect = left;
   ClipMachineMemory->wboard.bottom_of_ClipRect = bottom;
   ClipMachineMemory->wboard.right_of_ClipRect = right;

   _clip_retni(ClipMachineMemory, 0);

   return 0;
}
