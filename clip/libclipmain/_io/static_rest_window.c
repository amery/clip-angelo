static void
rest_window(ClipMachine * ClipMachineMemory, ClipWindow * wp)
{
   int l;

   char *s;

   WindowSave *ws;

   int nr, nc, top, left, y, x;

   if (!(s = wp->save_of_ClipWindow))
      return;

   ws = (WindowSave *) s;
   s += sizeof(WindowSave);
   nr = ws->rect.bottom_of_ClipRect - ws->rect.top_of_ClipRect;
   nc = ws->rect.right_of_ClipRect - ws->rect.left_of_ClipRect;
   l = ws->l;
   top = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect + ws->dr;
   left = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect + ws->dc;

   rest_region(ClipMachineMemory, top, left, top + nr, left + nc, s, l);

   ClipMachineMemory->attr = ws->attr;
   ClipMachineMemory->cursor = ws->cursor;
   y = ws->y /*+ wp->rect.top */ ;
   x = ws->x /*+ wp->rect.left */ ;
   clip_region(ClipMachineMemory, &y, &x, 0, 0, 2, -1);
   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;

   draw_shadow(ClipMachineMemory, wp);
}
