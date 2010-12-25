/*
	   MSETCURSOR( [<lVisible>] ) --> lIsVisible
*/
int
clip_MSETCURSOR(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_visible);

   if (ClipMachineMemory->argc > 0)
      sp->mouse_visible = _clip_parl(ClipMachineMemory, 1);

   return 0;
}
