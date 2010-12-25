int
_clip_init_tty(ClipMachine * ret)
{
   int r;

   char errbuf[128];

   if (ret->screen)
   {
      restart_tty(ret->screen->base);
      return 0;
   }

   r = init_tty(ret->screen_base, 0, _clip_envp, 0, (ScreenPgChars *) _clip_pgtbl, errbuf, sizeof(errbuf));

   if (r >= 0)
   {
      ret->screen = new_Screen(ret->screen_base);
      if (r > 0)
	 _clip_logg(0, "init screen: %s", errbuf);
      r = 0;
   }
   else
   {
      free(ret->screen_base);
      snprintf(ret->syserr, sizeof(ret->syserr), "%s", errbuf);
      ret->screen_base = 0;
      ret->screen = 0;
   }

   if (ret->screen)
   {
      ret->wboard.bottom_of_ClipRect = ret->windows->ClipRect_format_of_ClipWindow.bottom_of_ClipRect = ret->screen->base->Lines - 1;
      ret->wboard.right_of_ClipRect = ret->windows->ClipRect_format_of_ClipWindow.right_of_ClipRect = ret->screen->base->Columns - 1;
      ret->windows->ClipRect_format_of_ClipWindow = ret->windows->ClipRect_format_of_ClipWindow;
   }

   return r;
}
