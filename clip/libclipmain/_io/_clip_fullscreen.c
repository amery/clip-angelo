void
_clip_fullscreen(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->fullscreen)
      return;
   ClipMachineMemory->fullscreen = 1;
   _clip_init_tty(ClipMachineMemory);
   if (!ClipMachineMemory->screen)
   {
      char msg[256];

      int l;

      ClipFrame *fp;

      snprintf(msg, sizeof(msg), "request to switch into FULLSCREEN mode failed: %s\n", ClipMachineMemory->syserr);
      l = strlen(msg);
      out_log(ClipMachineMemory, msg, l, 0);
      out_err(ClipMachineMemory, msg, l, 0);
      for (fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
      {
	 _clip_logg(0, "trace: file '%s' line %d", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
	 fprintf(stderr, "trace: file '%s' line %d\n", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
	 fflush(stderr);
      }

      exit(2);
   }
   /*clear_Screen(ClipMachineMemory->screen); */
}
