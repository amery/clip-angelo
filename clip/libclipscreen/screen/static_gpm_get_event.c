static int
gpm_get_event(Gpm_Event * event)
{
   int r;

   if (gpm_fd < 0)
      return 0;

   r = Gpm_GetEvent(event);
   return r;
}
