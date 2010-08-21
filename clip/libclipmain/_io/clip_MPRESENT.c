/*
	   MPRESENT() --> lIsPresent
*/
int
clip_MPRESENT(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, sp->mouse_present);

   return 0;
}
