/*
	   MSAVESTATE() --> cSaveState
*/
int
clip_MSAVESTATE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   int *save;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   save = (int *) malloc(7 * sizeof(int));

   save[0] = sp->mouse_visible;
   save[1] = sp->mouse_y;
   save[2] = sp->mouse_x;
   save[3] = sp->mouse_top;
   save[4] = sp->mouse_left;
   save[5] = sp->mouse_bottom;
   save[6] = sp->mouse_right;

   _clip_retcn_m(ClipMachineMemory, (char *) save, 7 * sizeof(int));

   return 0;
}
