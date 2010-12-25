/*
   Closes the active window
 */
int
clip_WCLOSE(ClipMachine * ClipMachineMemory)
{
   int num, i;

   ClipWindow *wp;

   _clip_fullscreen(ClipMachineMemory);

   num = ClipMachineMemory->wnum;
   wp = ClipMachineMemory->windows + num;

   if (wp->no_of_ClipWindow == 0)
      return 0;

   destroy_window(wp);

   num = --(ClipMachineMemory->wnum);
   wp = ClipMachineMemory->windows + num;
   if (wp->no_of_ClipWindow == 0 && num > 0)
   {
      ClipWindow wp2;

      wp2 = ClipMachineMemory->windows[num - 1];
      ClipMachineMemory->windows[num - 1] = ClipMachineMemory->windows[num];
      ClipMachineMemory->windows[num] = wp2;
   }
   wp = ClipMachineMemory->windows + num;

   for (i = 0; i <= ClipMachineMemory->wnum; i++)
      if ((ClipMachineMemory->windows + i)->no_of_ClipWindow == 0)
	 rest_window(ClipMachineMemory, ClipMachineMemory->windows + i);
   for (i = 0; i <= ClipMachineMemory->wnum; i++)
      if ((ClipMachineMemory->windows + i)->no_of_ClipWindow != 0)
	 rest_window(ClipMachineMemory, ClipMachineMemory->windows + i);

   sync_mp(ClipMachineMemory);

   return 0;
}
