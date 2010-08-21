int
clip_FILESIZE(ClipMachine * ClipMachineMemory)	/* Determines the size of a file */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup.)
*/
   struct stat st;

   char *fname = NULL;

   char *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int fattr = _clip_parni(ClipMachineMemory, 2);

   off_t fsize = -1;

   if (fname != NULL && (fattr == FA_NORMAL ||
			 ((fattr & FA_READONLY) && (st.st_mode & S_IRUSR)
			  && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN) && fname[0] == '.') || ((fattr & FA_DIRECTORY) && S_ISDIR(st.st_mode)) || ((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode))))
   {
      fsize = st.st_size;
   }

   _clip_retnl(ClipMachineMemory, fsize);

   if (uname != NULL)
      free(uname);

   return 0;
}
