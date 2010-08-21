static void
init_mouse(ScreenBase * base, char **envp)
{
   char *term;

#ifndef _WIN32
   char *e;
#endif

   term = base->terminalName;

   if (base->fd < 0)
      return;

   base->mouse_top = 0;
   base->mouse_left = 0;
   base->mouse_bottom = base->Lines - 1;
   base->mouse_right = base->Columns - 1;
   base->mouse_visible = 1;
   base->mouse_auto_visible = 0;
   base->mouse_dclick_speed = 250;
   base->mouse_driver = "";

#ifdef _WIN32
   xterm_mouse = 1;
   base->mouse_present = 1;
   base->mouse_driver = "win32console";
   return;
#else

   e = get_env(envp, "XTERM_MOUSE");

   if ((e && (!strcasecmp(e, "yes") || !strcasecmp(e, "on"))) || !strncmp(term, "xterm", 5) || !strncmp(term, "rxvt", 4) || strstr(term, "xterm") || strstr(term, "rxvt"))
   {
      const char msg[] = "\033[?1001s\033[?1000h";

      xterm_mouse = 1;

      /* turn xterm mouse on */

      write(1, msg, sizeof(msg) - 1);
      base->mouse_present = 1;
      base->mouse_driver = "xterm";

      return;
   }
#endif

#ifdef HAVE_GPM_H
   {
      int r;

      memset(&conn, 0, sizeof(conn));
      conn.eventMask = GPM_MOVE | GPM_UP | GPM_DOWN | GPM_DRAG | GPM_DOUBLE;
      conn.defaultMask = 0;
      conn.minMod = 0;
      conn.maxMod = 0;
      gpm_zerobased = 1;
      r = Gpm_Open(&conn, 0);
      if (r >= 0)
      {
	 base->mouse_present = 1;
	 base->mouse_driver = "LinuxConsoleGPM";
      }
      _clip_logg(3, "Gpm_Open return %d", r);
   }
#endif
}
