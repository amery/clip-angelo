static void
clip_region(ClipMachine * ClipMachineMemory, int *ptop, int *pleft, int *pbottom, int *pright, int full, int wnum)
{
   ClipWindow *wp;

   int top = -1024, left = -1024, bottom = 1024, right = 1024;

   int Top, Bottom, Left, Right, ftop, fleft, columns, lines;

   if (wnum == -1)
      wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;
   else
      wp = ClipMachineMemory->windows + wnum;

   /*int Top = wp->rect.top; */
   Top = (full ? wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect : wp->ClipRect_format_of_ClipWindow.top_of_ClipRect);
   Bottom = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect : wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect);
   Left = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect : wp->ClipRect_format_of_ClipWindow.left_of_ClipRect);
   Right = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect : wp->ClipRect_format_of_ClipWindow.right_of_ClipRect);
   ftop = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect : wp->ClipRect_format_of_ClipWindow.top_of_ClipRect);
   fleft = (full == 2 ? wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect : wp->ClipRect_format_of_ClipWindow.left_of_ClipRect);
   columns = ClipMachineMemory->screen->base->Columns - 1;
   lines = ClipMachineMemory->screen->base->Lines - 1;

   if (Bottom > lines)
      Bottom = lines;
   if (Right > columns)
      Right = columns;

   if (ptop)
      top = *ptop;
   if (pleft)
      left = *pleft;
   if (pbottom)
      bottom = *pbottom;
   if (pright)
      right = *pright;

   top += ftop;
   bottom += ftop;
   left += fleft;
   right += fleft;

   if (top < Top)
      top = Top;
   else if (top > Bottom)
      top = Bottom;

   if (bottom < Top)
      bottom = Top;
//      else if (bottom == Top)
   //              bottom = Bottom;
   else if (bottom > Bottom)
      bottom = Bottom;

   if (bottom < top)
      bottom = top;

   if (left < Left)
      left = Left;
   else if (left > Right)
      left = Right;

   if (right < Left)
      right = Left;
/*	else if (right == Left)
		right = Right;*/
   else if (right > Right)
      right = Right;

   if (right < left)
      right = left;

   if (ptop)
      *ptop = top;
   if (pleft)
      *pleft = left;
   if (pbottom)
      *pbottom = bottom;
   if (pright)
      *pright = right;

}
