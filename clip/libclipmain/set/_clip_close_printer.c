int
_clip_close_printer(ClipMachine * ClipMachineMemory)
{
   FILE *printer = (FILE *) ClipMachineMemory->printer;

   if (!printer)
      return 0;

   fclose(printer);
   ClipMachineMemory->printer = 0;
   if (ClipMachineMemory->real_prfile != ClipMachineMemory->prfile)
   {
      char buf[256], *sp = ClipMachineMemory->prfile;

      int prno = 0;

      char *p_prog;

      /*setenv("CLIP_PRINT_PROG","lpr",0); */
      p_prog = getenv("CLIP_PRINT_PROG");
      if (!p_prog || !*p_prog)
	 p_prog = "lpr";

#if 0
      if (!strcasecmp(sp, "lpt2") || !strcasecmp(sp, "lpt2:"))
	 prno = 2;
      else if (!strcasecmp(sp, "lpt3") || !strcasecmp(sp, "lpt3:"))
	 prno = 3;
/*
		elseif (!strcasecmp(sp, "lpt1") || !strcasecmp(sp, "lpt1:"))
			prno = 1;
*/
#else
      if (!strncasecmp(sp, "lpt", 3) && strlen(sp) > 3 && sp[3] >= '1' && sp[3] <= '9')
	 prno = atoi(sp + 3);
      if (prno <= 1)
	 prno = 0;
#endif
#ifndef _WIN32
      if (prno)
	 snprintf(buf, sizeof(buf), "%s -P lp%d %s 2>&1 >/dev/null", p_prog, prno, ClipMachineMemory->real_prfile);
      else
	 snprintf(buf, sizeof(buf), "%s %s 2>&1 >/dev/null", p_prog, ClipMachineMemory->real_prfile);
#else
      snprintf(buf, sizeof(buf), "copy /b %s %s >null", ClipMachineMemory->real_prfile, ClipMachineMemory->prfile);
#endif
      system(buf);
      _clip_logg(2, "close printer:%s", buf);
      remove(ClipMachineMemory->real_prfile);
      free(ClipMachineMemory->real_prfile);
   }
   free(ClipMachineMemory->prfile);

   ClipMachineMemory->prfile = 0;
   ClipMachineMemory->real_prfile = 0;
   return 0;
}
