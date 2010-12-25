/* Closes all windows */
int
clip_WACLOSE(ClipMachine * ClipMachineMemory)
{
   int num, i;

   ClipWindow *wp, w;

   _clip_fullscreen(ClipMachineMemory);
   num = ClipMachineMemory->wnum;

   for (i = 0; i <= num; i++)
   {
      wp = ClipMachineMemory->windows + i;
      if (wp->no_of_ClipWindow == 0)
	 w = *wp;
      else
	 destroy_window(wp);
   }

   ClipMachineMemory->windows[0] = w;

   ClipMachineMemory->wnum = 0;

   select_window(ClipMachineMemory, 0);

   return 0;
}
