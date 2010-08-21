int
clip_FILETIME(ClipMachine * ClipMachineMemory)	/* Determines a file's time */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   16      FA_DIRECTORY        (Subdirectory)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   struct stat st;

   char *fname = NULL;

   char *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int fattr = _clip_parni(ClipMachineMemory, 2);

   struct tm *t = NULL;

   char tbuf[9];

   memset(tbuf, 0, 9);

   if (fname != NULL && (fattr == FA_NORMAL ||
			 ((fattr & FA_READONLY) && (st.st_mode & S_IRUSR)
			  && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN) && fname[0] == '.') || ((fattr & FA_DIRECTORY) && S_ISDIR(st.st_mode)) || ((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode))))
   {
      t = localtime(&st.st_mtime);
      snprintf(tbuf, 9, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
   }

   _clip_retc(ClipMachineMemory, tbuf);

   if (uname != NULL)
      free(uname);

   return 0;
}
