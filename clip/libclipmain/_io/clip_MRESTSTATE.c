/*
	   MRESTSTATE( <cSaveState> ) --> NIL
*/
int
clip_MRESTSTATE(ClipMachine * ClipMachineMemory)
{
   ScreenBase *sp;

   int *save;

   char *s;

   int l;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen->base;

   s = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!s || l != (7 * sizeof(int)))
      return EG_ARG;

   save = (int *) s;

   sp->mouse_visible = save[0];
   sp->mouse_y = save[1];
   sp->mouse_x = save[2];
   sp->mouse_top = save[3];
   sp->mouse_left = save[4];
   sp->mouse_bottom = save[5];
   sp->mouse_right = save[6];

   return 0;
}
