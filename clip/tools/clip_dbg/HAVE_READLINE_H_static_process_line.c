static void
process_line(char *bp)
{
   char obuf[1024];

   char **argv = 0;

   int argc = 0;

   int i, l;

   char *s;

   if (*bp)
      add_history(bp);
   else
   {
      HIST_ENTRY *p = previous_history();

      if (p)
	 bp = p->line;
   }

   if (bp[0] == '!')
   {
      system(bp + 1);
      return;
   }

   s = strchr(bp, '|');
   if (s && s[1])
   {
      start_pipe("|", s + 1);
      *s = 0;
   }
   else
   {
      s = strchr(bp, '>');
      if (s && s[1])
      {
	 start_pipe(">", s + 1);
	 *s = 0;
      }
   }

   split_vector(bp, &argv, &argc);

#if 0
   if (argc && !strcasecmp(argv[0], "quit"))
      cleanup(0);
#endif

   obuf[0] = 0;
   for (i = 0; i < argc; i++)
   {
      l = strlen(obuf);
      snprintf(obuf + l, sizeof(obuf) - l, "%s ", argv[i]);
   }

   l = strlen(obuf);

   snprintf(obuf + l, sizeof(obuf) - l, "\n");
   l = strlen(obuf);

   if (fwrite(obuf, l, 1, fin) != 1)
   {
      fprintf(stderr, "cannot write to PID %lu\n", (unsigned long) pid);
      cleanup(1);
   }
   fflush(fin);
#ifndef OS_MINGW
   if (kill(pid, SIGUSR1))
   {
      fprintf(stderr, "cannot send signal to PID %lu\n", (unsigned long) pid);
      cleanup(1);
   }
#endif

   free(argv);
#ifdef HAVE_READLINE_ALREADY_PROMPTED
   rl_already_prompted = 1;
#endif
}
