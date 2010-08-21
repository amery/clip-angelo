/*
	dbgcommand(cCommand) -> cResponce
*/
int
clip_DBGCOMMAND(ClipMachine * ClipMachineMemory)
{
#ifdef _WIN32
   return EG_ARG;
#else
   char *str = _clip_parc(ClipMachineMemory, 1);

   char *buf, *obuf = 0;

   if (!str)
      return EG_ARG;

   if (!fin)
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: no connection");
      return -1;
   }

   buf = (char *) malloc(4096);
   memset(buf, 0, 4096);

#if 0
   /* read all possible input */
   for (;;)
   {
      fd_set rfds;

      int r, fno = fileno(fout);

      struct timeval tv;

      FD_ZERO(&rfds);
      FD_SET(fno, &rfds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;

      r = select(fno + 1, &rfds, 0, 0, &tv);

      if (r <= 0)
	 break;

      for (;;)
      {
	 if (!fgets(buf, 4096, fout))
	 {
	    _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot read data");
	    cleanup();
	    free(buf);
	    return -1;
	 }
	 /*printf("got: %s", buf); */
	 if (!strcmp(buf, ".\n"))
	 {
	    fflush(fout);
	    break;
	 }
      }
   }
#endif

   fprintf(fin, "%s\n", str);
   fflush(fin);

   if (kill(pid, SIGUSR1))
   {
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot send signal to PID %lu\n", (unsigned long) pid);
      cleanup();
      free(buf);
      return -1;
   }

   usleep(1);

   /* read input up to '.' line */
   obuf = (char *) malloc(1);
   *obuf = 0;

   for (;;)
   {
      int l, ol;

      if (!fgets(buf, 4096, fout))
      {
	 _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot read data");
	 cleanup();
	 free(buf);
	 free(obuf);
	 return -1;
      }
      /*printf("got: %s", buf); */
      if (!strcmp(buf, ".\n"))
	 break;
      l = strlen(buf);
      ol = strlen(obuf);
      obuf = (char *) realloc(obuf, ol + l + 1);
      memcpy(obuf + ol, buf, l);
      obuf[ol + l] = 0;
   }

   _clip_retcn_m(ClipMachineMemory, obuf, strlen(obuf));
   free(buf);
   return 0;
#endif
}
