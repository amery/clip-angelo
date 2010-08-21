int
clip_FILEDELETE(ClipMachine * ClipMachineMemory)	/* Deletes file(s) by name and attribute */
{
/*
	   0         FA_NORMAL
	   1         FA_READONLY       Read-only
	   2         FA_HIDDEN         HIDDEN (Concealed files)
	   4         FA_SYSTEM         SYSTEM (System files)
	   8         FA_VOLUME         VOLUME (Name of floppy/hard disk)
	   32        FA_ARCHIVE        ARCHIVE (Changed since last backup)
*/
   char *s = NULL, *uname = NULL;

   char path[MAXPATHLEN];

   char buf[MAXPATHLEN];

   int ret = 0, fattr = FA_ARCHIVE, plen = 0;

   DIR *dir = NULL;

   struct dirent *d;

   struct stat st;

   if (ClipMachineMemory->argc < 1)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
      return 1;
   }
   if ((uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1))) == NULL)
   {
      goto end;
   }

   if ((s = strrchr(uname, '/')) == NULL)
   {				/* current directory */
      if ((dir = opendir(".")) == NULL)
	 goto end;
      path[0] = '.';
      path[1] = '/';
      path[2] = 0;
      plen = 2;
   }
   else if (*(s + 1) == 0)
   {				/* file name/mask expected */
      goto end;
   }
   else
   {
      plen = ++s - uname;
      memcpy(path, uname, plen);
      path[plen] = 0;
      if ((dir = opendir(path)) == NULL)
	 goto end;
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      fattr = _clip_parni(ClipMachineMemory, 2);

   while ((d = readdir(dir)) != NULL)
   {

      if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
	 continue;
#ifdef _WIN32
      if (_clip_glob_match(d->d_name, s, 1) != strlen(d->d_name))
#else
      if (_clip_glob_match(d->d_name, s, ClipMachineMemory->flags & TRANSLATE_FLAG) != strlen(d->d_name))
#endif
	 continue;
      stat(d->d_name, &st);
      /*
         if (!(((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode)) ||
         ((fattr & FA_READONLY) &&
         (st.st_mode & S_IRUSR) && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN) && d->d_name[0] == '.')))
         {
         continue;
         }
       */
      strncpy(buf, path, MAXPATHLEN);
      strncpy(buf + plen, d->d_name, MAXPATHLEN - plen);
      if (unlink(buf) == 0)	/* success */
	 ret = 1;
   }

 end:
   _clip_retl(ClipMachineMemory, ret);

   if (uname != NULL)
      free(uname);
   if (dir)
      closedir(dir);
   return 0;
}
