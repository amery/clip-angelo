/*
	   MHIDE() --> nOldVisibleStatus
	   note: all parameters are ignored
*/
int
clip_MHIDE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_visible);
   sp->mouse_visible = 0;

   return 0;
}
