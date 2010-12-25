int
clip_FILEATTRS(ClipMachine * ClipMachineMemory)	/* Determines a file's attributes */
{
/*
   0       FA_NORMAL
   1       FA_READONLY         READ ONLY (Read-only)
   2       FA_HIDDEN           HIDDEN (Hidden files)
   4       FA_SYSTEM           SYSTEM (System files)
   8       FA_VOLUME           VOLUME (Name of a floppy/hard disk)
   16      FA_DIRECTORY        DIR (Directory)
   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
 */
   struct stat st;

   char fattr[8];

   char *fname = NULL;

   char *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int i = 0;

   if (fname != NULL)
   {
      if (S_ISREG(st.st_mode))
      {
	 fattr[i++] = 'A';
      }
      if (fname[0] == '.')
      {
	 fattr[i++] = 'H';
      }
      if ((st.st_mode & S_IRUSR) && !(st.st_mode & S_IWUSR))
      {
	 fattr[i++] = 'R';
      }
      if (S_ISDIR(st.st_mode))
      {
	 fattr[i++] = 'D';
      }
      fattr[i] = 0;
   }

   _clip_retc(ClipMachineMemory, fattr);

   if (uname != NULL)
      free(uname);

   return 0;
}
