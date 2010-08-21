static void
destroy_window(ClipWindow * wp)
{
   if (wp->save_of_ClipWindow)
      free(wp->save_of_ClipWindow);
}
