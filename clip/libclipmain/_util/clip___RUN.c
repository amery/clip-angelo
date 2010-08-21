int
clip___RUN(ClipMachine * ClipMachineMemory)
{
   char *com = _clip_parc(ClipMachineMemory, 1);

   int old_cursor = 0;

   if (com == NULL)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");
      _clip_retc(ClipMachineMemory, "");
      return 1;
   }

   if (ClipMachineMemory->fullscreen)
   {
      old_cursor = ClipMachineMemory->screen->cursor;
      restore_tty(ClipMachineMemory->screen_base);	/* restore start mode */
   }

   system(com);

   if (ClipMachineMemory->fullscreen)
      restart_tty(ClipMachineMemory->screen_base);	/* set work mode */
   if (ClipMachineMemory->fullscreen)
   {
      redraw_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
      ClipMachineMemory->screen->cursor = !ClipMachineMemory->screen->cursor;
      sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
      ClipMachineMemory->screen->cursor = old_cursor;
      sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
   }

   return 0;
}
