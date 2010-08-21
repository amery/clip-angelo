int
clip_ROW(ClipMachine * ClipMachineMemory)
{
   int r;

   int scr = _clip_parl(ClipMachineMemory, 1);

   if (ClipMachineMemory->fullscreen)
   {
      if (scr)
      {
	 Screen *sp = ClipMachineMemory->screen;

	 r = sp->y;
      }
      else
      {
	 ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	 r = ClipMachineMemory->screen->y - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
      }
   }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}
