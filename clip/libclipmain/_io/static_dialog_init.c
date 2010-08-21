static void
dialog_init(ClipMachine * ClipMachineMemory)
{
   if (!ClipMachineMemory->fullscreen)
   {
      static int dialog = 0;

      if (!dialog)
      {
	 int r = _clip_init_tty(ClipMachineMemory);

	 if (!r)
	    dialog = 1;
	 else
	 {
	    int l;

	    char msg[256];

	    snprintf(msg, sizeof(msg), "\nrequest to switch into DIALOG mode failed: %s\n", ClipMachineMemory->syserr);
	    l = strlen(msg);
	    out_log(ClipMachineMemory, msg, l, 0);
	    out_err(ClipMachineMemory, msg, l, 0);
	    exit(2);
	 }
      }
/*_clip_restore_tty(ClipMachineMemory);*/
   }
}
