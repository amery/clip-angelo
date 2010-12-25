/*
	   MMIDDLEDOWN() --> lIsPressed
*/
int
clip_MMIDDLEDOWN(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   _clip_retl(ClipMachineMemory, (sp->mouse_buttons & MOUSE_BUTTONS_MIDDLE) ? 1 : 0);

   return 0;
}
