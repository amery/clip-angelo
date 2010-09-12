static void
getrc(char *name)
{
   FILE *file;

   int argc = 0;

   char **argv = 0;

   char buf[256];

   file = fopen(name, "rt");
   if (!file)
      return;

   while (fgets(buf, sizeof(buf), file))
   {
      int l;

      char *s, *p;

      l = strlen(buf);
      if (l < 1)
	 continue;

      while ((--l >= 0) && (buf[l] == '\n' || buf[l] == '\r'))
	 buf[l] = 0;

      for (s = buf; *s && isspace(*s); s++, l--)
	 ;

      if (l < 1)
	 continue;

      if (*s == '#')
	 continue;

      for (p = s; *p && !isspace(*p); p++)
	 ;
      if (*p)
      {
	 *p = 0;
	 for (p++; *p && isspace(*p); p++)
	    ;
      }

      argv = (char **) realloc(argv, sizeof(char *) * (argc + 1));

      argv[argc++] = strdup(s);
      if (*p)
      {
	 argv = (char **) realloc(argv, sizeof(char *) * (argc + 1));

	 argv[argc++] = strdup(p);
      }
   }

   fclose(file);
   if (argc > 0)
      get_opt(argc, argv);
/*
   while(argc)
   free(argv[--argc]);
 */
   free(argv);
}
