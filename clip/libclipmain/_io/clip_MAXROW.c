int
clip_MAXROW(ClipMachine * ClipMachineMemory)
{
   int r;

   int scr = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->fullscreen)
   {
      if (scr)
      {
	 Screen *sp = ClipMachineMemory->screen;

	 r = sp->base->Lines - 1;
      }
      else
      {
	 ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	 r = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
      }
   }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}
