/*
	   MDBLCLK([<nNewSpeed>]) --> nSpeed
	   note: really not work
*/
int
clip_MDBLCLK(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retni(ClipMachineMemory, sp->mouse_dclick_speed);

   if (ClipMachineMemory->argc > 0)
      sp->mouse_dclick_speed = _clip_parni(ClipMachineMemory, 1);

   return 0;
}
