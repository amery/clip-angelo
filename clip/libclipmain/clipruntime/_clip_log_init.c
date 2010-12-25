void
_clip_log_init(char *filename)
{
   static int inited = 0;

   char buf[256];

   if (filename == NULL)
   {
      if (inited)
	 return;
      inited = 1;
      sprintf(buf, "%s.log", _clip_progname);
      logg = fopen(buf, "at");
      if (!logg)
      {
	 fprintf(stderr, "cannot open log file '%s'", buf);
	 logg = stderr;
      }
   }
   else
   {
      inited = 1;
      if (logg != stderr)
	 fclose(logg);
      logg = fopen(filename, "at");
      if (!logg)
      {
	 fprintf(stderr, "cannot open log file '%s'", buf);
	 logg = stderr;
      }
   }
}
