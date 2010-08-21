static int
make_dir(char *path)
{
   char dir[256];

   int r;

   snprintf(dir, sizeof(dir), path);
#ifdef OS_MINGW
   r = mkdir(dir);
#else
   r = mkdir(dir, 0775);
#endif
   if (r && errno == EEXIST)
      return 0;

   if (r)
   {
      char *s = strrchr(dir, '/');

      if (s)
      {
	 *s = 0;
	 r = make_dir(dir);
	 if (r)
	    yywarning("cannot create dir '%s': %s", dir, strerror(errno));
	 else
	 {
	    snprintf(dir, sizeof(dir), path);
#ifdef OS_MINGW
	    if ((r = mkdir(dir)))
#else
	    if ((r = mkdir(dir, 0775)))
#endif
	       yywarning("cannot create dir '%s': %s", dir, strerror(errno));
	 }
	 return r;
      }
      else
      {
	 return -1;
      }
   }
   return 0;
}
