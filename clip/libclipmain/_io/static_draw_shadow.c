static void
draw_shadow(ClipMachine * ClipMachineMemory, ClipWindow * wp)
{
   int lines, columns, nrow, ncol;

   int top, left, bottom, right;

   int i, j, attr;

   Screen *sp;

   if (wp->shadow_of_ClipWindow == -1)
      return;

   sp = ClipMachineMemory->screen;
   lines = sp->base->Lines;
   columns = sp->base->Columns;
   attr = wp->shadow_of_ClipWindow;

   nrow = 1;
#if 0
   ncol = (columns - 20) / lines;
   if (ncol < 1)
      ncol = 1;
#else
   ncol = 2;
#endif

   top = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   left = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   bottom = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;
   right = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   for (i = top; i <= bottom + nrow; i++)
   {
      if (i < 0 || i >= lines)
	 continue;
      sp->touched[i] = 1;
      for (j = left + ncol; j <= right + ncol; j++)
      {
	 if (j < 0 || j >= columns)
	    continue;
	 if ((i >= (top + nrow) && j > right) || (j >= (left + ncol) && i > bottom))
	 {
	    /*int pg = sp->colors[i][j] & PG_ATTR; */

	    sp->colors[i][j] = attr /*| pg */ ;
	 }
      }
   }
}
