int
_clip_init(ClipMachine * ClipMachineMemory, int Argc, char **Argv, char **envp)
{
   char *e;

   ClipFunction **np, ****npp;

   const char **sp;

   ClipFile **fp, ****fpp;

   int ret;

   if (ClipMachineMemory->inited)
      return 0;

   if (!init_struct)
      exit(100);

   ClipMachineMemory->inited = 1;

   if (!logg)
      logg = stderr;

   if (!_clip_progname)
   {
      int arg_c = 0;

      char **arg_v = 0;

      char *p1, *p2;

      _clip_put_env("CLIP_HOSTCS", init_struct->CLIP_HOSTCS);

      _clip_raw_argc = Argc;
      _clip_raw_argv = Argv;

      process_args(Argc, Argv, &arg_c, &arg_v);

      _clip_argc = arg_c;
      _clip_argv = arg_v;

      e = strrchr(arg_v[0], '/');

      if (e)
	 _clip_progname = e + 1;
      else
	 _clip_progname = arg_v[0];

      for (; (e = *envp); ++envp)
	 put_env(&_clip_envp, e);

      atexit(_clip_exit);

#ifndef OS_MINGW
      signal(SIGHUP, _clip_signal);
      signal(SIGQUIT, _clip_signal);
      signal(SIGKILL, _clip_signal);
      signal(SIGPIPE, _clip_signal);
      signal(SIGALRM, _clip_signal);
      if (_clip_debuglevel)
	 signal(SIGUSR1, _clip_sigdebug);
      else
	 signal(SIGUSR1, SIG_IGN);
#endif

      signal(SIGINT, _clip_sigint);
      signal(SIGILL, _clip_signal);
      signal(SIGABRT, _clip_signal);
      signal(SIGFPE, _clip_signal);
      signal(SIGSEGV, _clip_signal);
      signal(SIGTERM, _clip_signal);

      set_sysvars();

      _clip_logg(2, "start");

      /* read CLIPROOT/environment */
      e = get_env(_clip_envp, "CLIPROOT");
      if (e && *e)
	 CLIPROOT = strdup(e);

      process_environ(CLIPROOT, "etc/environment");
      e = get_env(_clip_envp, "CLIP_LANG");
      if (e == NULL)
	 e = get_env(_clip_envp, "LANG");
      if (e && *e)
      {
	 char buf[256];

	 snprintf(buf, sizeof(buf), "%s/lang", CLIPROOT);
	 _clip_logg(3, "process LANG environ '%s/%s'", buf, e);
	 process_environ(buf, e);
      }
      e = get_env(_clip_envp, "CHARSET");
      if (e && *e)
      {
	 char buf[256];

	 snprintf(buf, sizeof(buf), "%s/lang", CLIPROOT);
	 _clip_logg(3, "process CHARSET environ '%s/%s'", buf, e);
	 process_environ(buf, e);
      }
      e = get_env(_clip_envp, "TERM");
      if (e && *e)
      {
	 char buf[256];

	 snprintf(buf, sizeof(buf), "%s/term", CLIPROOT);
	 _clip_logg(3, "process TERM environ '%s/%s'", buf, e);
	 process_environ(buf, e);
      }

      e = _clip_getenv("CLIP_LOCALE");
      if (!e)
      {
	 e = _clip_getenv("CLIP_LANG");
	 if (e == NULL)
	    e = _clip_getenv("LANG");
	 if (e && !strcmp(e, "C"))
	    e = 0;
	 if (!e)
	 {
	    e = _clip_getenv("LC_ALL");
	    if (e && !strcmp(e, "C"))
	       e = 0;
	 }
	 if (!e)
	 {
	    e = _clip_getenv("LC_MESSAGES");
	    if (e && !strcmp(e, "C"))
	       e = 0;
	 }
	 if (e)
	    _clip_put_env("CLIP_LOCALE", e);
      }

      e = _clip_getenv("CLIP_CLIENTCS");
      if (!e || !*e)
      {
	 e = _clip_getenv("CLIP_LOCALE");
	 if (e)
	 {
	    char *s;

	    s = strrchr(e, '.');
	    if (s)
	       e = s + 1;
	    else
	       e = 0;
	 }
	 if (!e)
	    e = _clip_getenv("CLIP_CHARSET");
	 if (!e)
	    e = _clip_getenv("CHARSET");
	 if (e)
	 {
	    char *s;

	    s = strdup(e);
	    for (e = s; *e; e++)
	       *e = tolower(*e);
	    _clip_put_env("CLIP_CLIENTCS", s);
	    free(s);
	 }
      }
      p1 = _clip_getenv("CLIP_HOSTCS");
      p2 = _clip_getenv("CLIP_CLIENTCS");
      if (p1 && !strcasecmp(p1, "c") && p2 && strcasecmp(p2, "c"))
	 _clip_put_env("CLIP_HOSTCS", p2);
      e = _clip_getenv("CLIP_HOSTCS");
      if (e && *e)
      {
	 _clip_logg(3, "load charset tables '%s'", e);
	 load_charset_tables(e, _clip_cmptbl, _clip_uptbl, _clip_lowtbl, _clip_isalpha_tbl, _clip_pgtbl);
	 _clip_hostcs = strdup(e);
      }

      p1 = _clip_getenv("CLIP_HOSTCS");
      p2 = _clip_getenv("CLIP_CLIENTCS");
      if (p1 && p2 && strcmp(p1, p2))
      {
	 cons_CharsetEntry *cs1 = 0, *cs2 = 0;

	 int len1 = 0, len2 = 0;

	 _clip_logg(2, "set stream io translation  %s -> %s", p1, p2);

	 if (load_charset_name(p1, &cs1, &len1))
	 {
	    _clip_logg(0, "cannot load charset file '%s': %s", p1, strerror(errno));
	 }

	 if (load_charset_name(p2, &cs2, &len2))
	 {
	    _clip_logg(0, "cannot load charset file '%s': %s", p2, strerror(errno));
	 }

	 if (cs1 && cs2)
	 {
	    make_translation(cs1, len1, cs2, len2, _clip_outtbl);
	    make_translation(cs2, len2, cs1, len1, _clip_intbl);
	 }

	 free(cs1);
	 free(cs2);
      }

      e = get_env(_clip_envp, "CLIP_LOCALE_ROOT");
      if (e && *e)
	 CLIP_LOCALE_ROOT = strdup(e);

      e = _clip_getenv("CLIP_LOCALE");
      if (e && *e)
      {
	 _clip_add_locale(e);
      }
   }

   for (npp = init_struct->_libinits; *npp; ++npp)
   {
      for (np = **npp; *np; ++np)
	 if ((ret = _clip_main_func(ClipMachineMemory, *np, _clip_argc, _clip_argv, _clip_envp)))
	    return ret;
   }
   for (np = init_struct->_inits; *np; ++np)
      if ((ret = _clip_main_func(ClipMachineMemory, *np, _clip_argc, _clip_argv, _clip_envp)))
	 return ret;

   for (fpp = init_struct->_libcpfiles; *fpp; ++fpp)
   {
      for (fp = **fpp; *fp; ++fp)
	 _clip_register_file(ClipMachineMemory, *fp);
   }
   for (fp = init_struct->_cpfiles; *fp; ++fp)
   {
      ClipFile *file = *fp;

      _clip_register_file(ClipMachineMemory, file);
   }

   for (fpp = init_struct->_libcpfiles; *fpp; ++fpp)
   {
      for (fp = **fpp; *fp; ++fp)
	 _clip_load_inits(ClipMachineMemory, *fp);
   }
   for (fp = init_struct->_cpfiles; *fp; ++fp)
   {
      ClipFile *file = *fp;

      _clip_load_inits(ClipMachineMemory, file);
   }

   for (sp = init_struct->_pfunctions_of_ClipInitStruct; *sp; ++sp)
      if ((ret = _clip_load(ClipMachineMemory, *sp, 0, 0)))
	 return ret;

   if (need_stop)
   {
      need_stop = 0;
      _clip_in_breakpoint();
   }

   return 0;
}
