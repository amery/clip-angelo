/*
	   MSETPOS( <nRow>, <nCol> ) --> NIL
*/
int
clip_MSETPOS(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   sp->mouse_y = _clip_parni(ClipMachineMemory, 1);
   sp->mouse_x = _clip_parni(ClipMachineMemory, 2);

   return 0;
}
