/*
	   MCOL() --> nMouseCol
*/
int
clip_MCOL(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_x);

   return 0;
}
