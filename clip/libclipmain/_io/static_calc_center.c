static void
calc_center(ClipMachine * ClipMachineMemory, ClipWindow * wp, int *top, int *left, int *bottom, int *right)
{
   int sh = ClipMachineMemory->screen->base->Lines;

   int sw = ClipMachineMemory->screen->base->Columns;

   int h = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect + 1;

   int w = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect + 1;

   int t, l, b, r;

   t = (sh - h) / 2;
   b = t + h - 1;
   l = (sw - w) / 2;
   r = l + w - 1;

   if (top)
      *top = t;
   if (left)
      *left = l;
   if (bottom)
      *bottom = b;
   if (right)
      *right = r;
}
