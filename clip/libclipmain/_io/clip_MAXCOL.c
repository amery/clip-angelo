int
clip_MAXCOL(ClipMachine * ClipMachineMemory)
{
   int r;

   int scr = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->fullscreen)
   {
      if (scr)
      {
	 Screen *sp = ClipMachineMemory->screen;

	 r = sp->base->Columns - 1;
      }
      else
      {
	 ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

	 r = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
      }
   }
   else
      r = 0;
   _clip_retnl(ClipMachineMemory, r);
   return 0;
}
