static void
move_window(ClipMachine * ClipMachineMemory, ClipWindow * wp, int top, int left)
{
   int i, dv, dh;

   save_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);

   dv = top - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   dh = left - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;

   wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect += dv;
   wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect += dv;
   wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect += dh;
   wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect += dh;
   wp->ClipRect_format_of_ClipWindow.top_of_ClipRect += dv;
   wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect += dv;
   wp->ClipRect_format_of_ClipWindow.left_of_ClipRect += dh;
   wp->ClipRect_format_of_ClipWindow.right_of_ClipRect += dh;

   for (i = 0; i <= ClipMachineMemory->wnum; i++)
      rest_window(ClipMachineMemory, ClipMachineMemory->windows + i);

   sync_mp(ClipMachineMemory);
}
