/*
	   MLEFTDOWN() --> lIsPressed
*/
int
clip_MLEFTDOWN(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, (sp->mouse_buttons & MOUSE_BUTTONS_LEFT) ? 1 : 0);

   return 0;
}
