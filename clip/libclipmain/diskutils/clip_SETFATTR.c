int
clip_SETFATTR(ClipMachine * ClipMachineMemory)	/* Sets a file's attributes */
{
/*
	   ������������ ��������:
	   ~~~~~~~~~~~~~~~~~~~~~~
	0      NO_DISK_ERR         No error found
	   -2      ER_FILE_NOT_FOUND   File not found
	   -3      ER_PATH_NOT_FOUND   Path not found
	   -5      ER_ACCESS_DENIED    Access denied (e.g., in network)

	   ��������������� ��������:
	   ~~~~~~~~~~~~~~~~~~~~~~~~~
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   int fattr = _clip_parni(ClipMachineMemory, 2);

   mode_t mode = 0;

   if (uname == NULL)
   {
      _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
      return 1;
   }
#ifdef OS_MINGW
   mode = S_IRUSR;
#else
   mode = S_IRUSR | S_IRGRP | S_IROTH;
#endif
   if (!(fattr & FA_READONLY))
   {
#ifdef OS_MINGW
      mode = S_IWUSR;
#else
      mode |= S_IWUSR | S_IWGRP | S_IWOTH;
#endif
   }
   if (chmod(uname, mode) != 0)
   {
      _check_error(ClipMachineMemory, uname, 0);	/* fail */
   }
   else
   {
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
   }
   free(uname);
   return 0;
}
