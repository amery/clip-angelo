int
clip_FILE(ClipMachine * ClipMachineMemory)
{
   char **paths;

   const char *filename = _clip_parc(ClipMachineMemory, 1);

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char p[PATH_MAX], fn[PATH_MAX];

   char fullname[PATH_MAX + 1];

   const char *s, *s1 = NULL;

   int npaths = 0, i;

   int ret = 0;

   char *c, *c1;

   char *fname;

   struct dirent *dirp = NULL;

   struct stat statbuf;

   DIR *dirh;

   if (filename == NULL || *filename == 0)
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }

   paths = malloc(sizeof(char *));

   if (strlen(filename) > 2 && filename[1] == ':' && filename[2] != '/' && filename[2] != '\\')
   {
      /* drive with default path */
      char *def_path;

      def_path = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[toupper(*filename) - 65]);
      snprintf(fn, PATH_MAX, "%c:%s/%s", *filename, def_path, filename + 2);
   }
   else if ((ClipMachineMemory->flags & TRANSLATE_FLAG) && ((filename[0] == '\\') || (filename[0] == '/')))
   {
      char *dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

      if (dname == NULL)
	 dname = "C";
      snprintf(fn, PATH_MAX, "%c:%s", *dname, filename);
   }
   else
   {
      strncpy(fn, filename, PATH_MAX - 1);
   }
   if ((s = strrchr(fn, '\\')) || (s1 = strrchr(fn, '/')))
   {
      if (!s1)
	 s1 = strrchr(fn, '/');
      s = (s > s1) ? s : s1;
      paths[0] = malloc(s - fn + 1);
      memcpy(paths[0], fn, s - fn);
      paths[0][s - fn] = 0;
      npaths++;
      fname = strdup(s + 1);
   }
   else
   {
      paths[0] = strdup(ClipMachineMemory->defaul);
      npaths++;
      strncpy(p, ClipMachineMemory->path, sizeof(p) - 1);
      c1 = p;
      do
      {
	 c = strchr(c1, ';');
	 if (c)
	 {
	    *c = 0;
	 }
	 else
	 {
	    c = strchr(c1, ',');
	    if (c)
	       *c = 0;
	 }
	 paths = realloc(paths, sizeof(char *) * (npaths + 1));

	 paths[npaths] = strdup(c1);
	 npaths++;
	 c1 = c + 1;
      }
      while (c);
      fname = strdup(fn);
   }
   _clip_unix_path(fname, ClipMachineMemory->flags & TRANSLATE_FLAG);

   for (i = 0; i < npaths; i++)
   {
      _clip_absolute_path(ClipMachineMemory, paths[i], p, sizeof(p));

      dirh = opendir(p);

      if (dirh == NULL)
	 *err = errno;
      else
	 *err = 0;

      if (dirh != NULL)
	 dirp = readdir(dirh);
      while (dirh != NULL && dirp != NULL)
      {
#ifdef _WIN32
	 if (_clip_glob_match(dirp->d_name, fname, 1) > 0)
#else
	 if (_clip_glob_match(dirp->d_name, fname, 0) > 0)
#endif
	 {
	    snprintf(fullname, sizeof(fullname), "%s%s", p, dirp->d_name);
	    stat(fullname, &statbuf);
	    if (!(S_ISDIR(statbuf.st_mode)))
	    {
	       ret = 1;
	       break;
	    }
	 }
	 dirp = readdir(dirh);
      }
      if (dirh != NULL)
	 closedir(dirh);
      if (ret == 1)
	 break;
   }

   for (i = 0; i < npaths; i++)
      free(paths[i]);
   free(paths);
   free(fname);
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
