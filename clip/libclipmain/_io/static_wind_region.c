static void
wind_region(ClipMachine * ClipMachineMemory, int *ptop, int *pleft, int *pbottom, int *pright, int full)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int ftop = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect : wp->ClipRect_format_of_ClipWindow.top_of_ClipRect);

   int fleft = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect : wp->ClipRect_format_of_ClipWindow.left_of_ClipRect);

   if (ptop)
      *ptop += ftop;
   if (pleft)
      *pleft += fleft;
   if (pbottom)
      *pbottom += ftop;
   if (pright)
      *pright += fleft;
}
