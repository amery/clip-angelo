static void
save_window(ClipMachine * ClipMachineMemory, ClipWindow * wp)
{
   int l;

   char *s;

   WindowSave *ws;

   ClipRect w;

   int lines = ClipMachineMemory->screen->base->Lines;

   int columns = ClipMachineMemory->screen->base->Columns;

   w = wp->ClipRect_rect_of_ClipWindow;
   if (w.top_of_ClipRect < 0)
      w.top_of_ClipRect = 0;
   if (w.bottom_of_ClipRect < 0)
      return;
   if (w.left_of_ClipRect < 0)
      w.left_of_ClipRect = 0;
   if (w.right_of_ClipRect < 0)
      return;
   if (w.top_of_ClipRect >= lines)
      return;
   if (w.bottom_of_ClipRect >= lines)
      w.bottom_of_ClipRect = lines - 1;
   if (w.left_of_ClipRect >= columns)
      return;
   if (w.right_of_ClipRect >= columns)
      w.right_of_ClipRect = columns - 1;

   s = save_region(ClipMachineMemory, wp->save_of_ClipWindow, w.top_of_ClipRect, w.left_of_ClipRect, w.bottom_of_ClipRect, w.right_of_ClipRect, &l);
   s = (char *) realloc(s, l + sizeof(WindowSave));
   memmove(s + sizeof(WindowSave), s, l);
   ws = (WindowSave *) s;
   ws->attr = ClipMachineMemory->attr;
   ws->cursor = ClipMachineMemory->cursor;
   ws->rect = w;
   ws->dr = w.top_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   ws->dc = w.left_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   ws->y = ClipMachineMemory->screen->y - wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   ws->x = ClipMachineMemory->screen->x - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   ws->l = l;

   wp->save_of_ClipWindow = s;
}
