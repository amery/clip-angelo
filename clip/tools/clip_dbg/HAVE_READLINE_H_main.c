int
main(int argc, char **argv)
{
   int ch;

   char prompt[96];

   while ((ch = getopt(argc, argv, "hv")) != EOF)
   {
      switch (ch)
      {
      case 'v':
	 log_level++;
	 break;
      case 'h':
	 usage();
      }
   }

   argc -= optind;
   argv += optind;

   if (argc < 1)
      usage();

   progname = argv[0];

   if (is_dig(progname))
   {
      pid = strtoul(progname, 0, 10);
   }
   else
   {
      char path[256];

      FILE *file;

      snprintf(path, sizeof(path), "%s.pid", progname);
      file = fopen(path, "r");
      if (file)
      {
	 unsigned long ul;

	 if (fscanf(file, "%lu", &ul) == 1)
	    pid = ul;
	 fclose(file);
      }

      if (pid == 0)
      {
	 snprintf(path, sizeof(path), "pidof -s %s", progname);
	 file = popen(path, "r");
	 if (file)
	 {
	    unsigned long ul;

	    if (fscanf(file, "%lu", &ul) == 1)
	       pid = ul;
	    pclose(file);
	 }
      }
   }

   if (pid == 0)
   {
      fprintf(stderr, "cannot determine PID of target process '%s'\n", progname);
      exit(1);
   }

   tcgetattr(0, &term);
   oterm = term;
   term.c_lflag &= ~ICANON;
   term.c_cc[VTIME] = 1;
   tcsetattr(0, TCSANOW, &term);

   signal(SIGINT, sigfunc);
   signal(SIGTERM, sigfunc);
#ifndef OS_MINGW
   signal(SIGHUP, sigfunc);
   signal(SIGPIPE, sigpipe);
   signal(SIGUSR1, sigusr);
#endif
   signal(SIGILL, sigfunc);
   signal(SIGSEGV, sigfunc);

   snprintf(in_path, sizeof(in_path), "/tmp/clip_dbg.%lu.in", (unsigned long) pid);
   remove(in_path);
   if (mkfifo(in_path, 0600))
   {
      fprintf(stderr, "clip_dbg: cannot create FIFO '%s': %s\n", in_path, strerror(errno));
      cleanup(3);
   }

   logg(1, "fifo %s created successfully", in_path);

   snprintf(out_path, sizeof(out_path), "/tmp/clip_dbg.%lu.out", (unsigned long) pid);
   remove(out_path);
   if (mkfifo(out_path, 0600))
   {
      fprintf(stderr, "clip_dbg: cannot create FIFO '%s': %s\n", out_path, strerror(errno));
      cleanup(4);
   }
   logg(1, "fifo %s created successfully", out_path);

#ifndef OS_MINGW
   if (kill(pid, SIGUSR1))
   {
      fprintf(stderr, "clip_dbg: cannot send signal SIGUSR1 to pid %lu: %s\n", (unsigned long) pid, strerror(errno));
      cleanup(7);
   }
#endif

   fin = fopen(in_path, "w");
   if (!fin)
   {
      fprintf(stderr, "clip_dbg: cannot open FIFO '%s': %s\n", in_path, strerror(errno));
      cleanup(5);
   }
   setvbuf(fin, 0, _IOLBF, 0);
   /*setvbuf(stdin, 0, _IOLBF, 0); */
   /*setvbuf(stdout, 0, _IOLBF, 0); */
   logg(1, "fifo %s opened for writing", in_path);

   fwrite("\n", 1, 1, fin);
   logg(1, "signal SIGUSR1 sent to pid %lu", (unsigned long) pid);

   fout = open(out_path, O_RDONLY, 0);
   if (fout < 0)
   {
      fprintf(stderr, "clip_dbg: cannot open FIFO '%s': %s\n", out_path, strerror(errno));
      cleanup(6);
   }
#ifndef OS_MINGW
   fcntl(fout, F_SETFL, O_NONBLOCK);
#endif

   logg(1, "fifo %s opened for reading", out_path);
   mpid = getpid();

   write(1, usage_str, sizeof(usage_str) - 1);

   snprintf(prompt, sizeof(prompt), "clip_dbg %s > ", progname);
   rl_callback_handler_install(prompt, process_line);
   write(1, "?\n", 2);

   while (1)
   {
      fd_set rfs;

      int r, l;

      char buf[1024];

      FD_ZERO(&rfs);
      FD_SET(fout, &rfs);
      FD_SET(fileno(stdin), &rfs);

      r = select(fout + 1, &rfs, 0, 0, 0);
      if (r < 0)
	 break;

      if (FD_ISSET(fout, &rfs))
      {
	 l = read(fout, buf, sizeof(buf));
	 if (l <= 0)
	    break;
	 if (pipe_flag)
	 {
	    int i;

	    for (i = 0; i < l; i++)
	       char_pipe(buf[i]);
	 }
	 else
	 {
	    if (write(1, buf, l) < l)
	       break;
	    rl_forced_update_display();
	    continue;
	 }
      }
      if (FD_ISSET(fileno(stdin), &rfs))
      {
	 if (!pipe_flag)
	 {
	    rl_callback_read_char();
	 }
      }
   }

   cleanup(0);

   return 0;
}
