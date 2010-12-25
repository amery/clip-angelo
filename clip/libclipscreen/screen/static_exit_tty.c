static void
exit_tty()
{
#if 0
   termcap_put_raw_str(termcap_ME);
   termcap_set_color(COLOR_WHITE | COLOR_BACK_BLACK);
   if (clear_on_exit)
      termcap_clear_screen();
   termcap_set_cursor(Lines - 1, 0);
   termcap_put_raw_str(termcap_TE);
   termcap_put_raw_str(termcap_KE);
   termcap_flush();
#endif
   stop_scan_mode(0);
#ifdef _WIN32
   if (w32_console)
      return;
#endif

#ifndef _WIN32
   if (xterm_mouse)		/* xterm */
   {
      const char msg[] = "\033[?1000l\033[?1001r";

      write(1, msg, sizeof(msg) - 1);
   }

   /* enable cursor */

   if (screen_data)
   {
      termcap_put_raw_str(screen_data, screen_data->termcap_VE);
      termcap_flush(screen_data);
   }

#endif

#ifdef HAVE_GPM_H
   if (gpm_fd >= 0)
      Gpm_Close();
#endif

   tcsetattr(0, TCSADRAIN, &start_mode);
}
