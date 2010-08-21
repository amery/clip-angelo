int
clip_FILEDATE(ClipMachine * ClipMachineMemory)	/* Determines the file date */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   16      FA_DIRECTORY        DIR (Directory)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   struct stat st;

   char *fname = NULL;

   char *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int fattr = _clip_parni(ClipMachineMemory, 2);

   struct tm *t = NULL;

   if (fname != NULL && (fattr == FA_NORMAL ||
			 ((fattr & FA_READONLY) && (st.st_mode & S_IRUSR)
			  && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN) && fname[0] == '.') || ((fattr & FA_DIRECTORY) && S_ISDIR(st.st_mode)) || ((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode))))
   {
      t = localtime(&st.st_mtime);
   }

   if (t == NULL)
      _clip_retdj(ClipMachineMemory, 0);
   else
      _clip_retdc(ClipMachineMemory, 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday);

   if (uname != NULL)
      free(uname);

   return 0;
}
