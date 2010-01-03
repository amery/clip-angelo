static void
get_env(char *name, char *val)
{
   if (!name || !val || !*val)
      return;

   if (!strcmp(name, "CC"))
      CC = val;
   else if (!strcmp(name, "CLIPROOT"))
      CLIPROOT = strdup(val);
   else if (!strcmp(name, "CLIP_LOCALE_ROOT"))
      CLIP_LOCALE_ROOT = strdup(val);
   else if (!strcmp(name, "CLIP_MODULE"))
      CLIP_MODULE = strdup(val);
   else if (!strcmp(name, "CFLAGS"))
      CFLAGS = strdup(val);
   else if (!strcmp(name, "COPT"))
      COPT = strdup(val);
   else if (!strcmp(name, "CDBG"))
      CDBG = strdup(val);
   else if (!strcmp(name, "OUT_FLAG"))
      OUT_FLAG = strdup(val);
   else if (!strcmp(name, "INCLUDE_FLAG"))
      INCLUDE_FLAG = strdup(val);
   else if (!strcmp(name, "OBJSUF"))
      OBJSUF = strdup(val);
   else if (!strcmp(name, "SOBJSUF"))
      SOBJSUF = strdup(val);
   else if (!strcmp(name, "LIBSUF"))
      LIBSUF = strdup(val);
   else if (!strcmp(name, "SLIBSUF"))
      SLIBSUF = strdup(val);
   else if (!strcmp(name, "COMPILE_FLAG"))
      COMPILE_FLAG = strdup(val);
   else if (!strcmp(name, "SFLAGS"))
      SFLAGS = strdup(val);
   else if (!strcmp(name, "CLIPLIB"))
      CLIPLIB = strdup(val);
   else if (!strcmp(name, "CLIPLIBS"))
      CLIPLIBS = strdup(val);
   else if (!strcmp(name, "CLIPSLIB"))
      CLIPSLIB = strdup(val);
   else if (!strcmp(name, "CLIP_NAMES"))
    {
       if (!strcasecmp(val, "yes") || !strcasecmp(val, "on"))
	  names_flag = 1;
       else if (!strcasecmp(val, "no") || !strcasecmp(val, "off"))
	  names_flag = 0;
       else
	  names_flag = atoi(val);
    }
   else if (!strcmp(name, "CLIP_CMDSTR"))
    {
       int       argc;

       char     *str = strdup(val);

       char     *argv[32], *s;

       for (argc = 0; argc < 32 && (s = strtok(str, ",")) != NULL; ++argc)
	{
	   argv[argc] = s;
	   str = NULL;
	}
       get_opt(argc, argv);
    }
#ifdef USE_AS
   else if (!strcmp(name, "CLIP_ASM"))
    {
       if (!strcasecmp(val, "yes") || !strcasecmp(val, "on"))
	  use_asm = 1;
       else if (!strcasecmp(val, "no") || !strcasecmp(val, "off"))
	  use_asm = 0;
    }
#endif

}
