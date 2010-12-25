/*
 *      STRFILE(<cCharacterstring>, <cFile>, [<lOverwrite>],
 *	  [<nOffset>], [<lCutOff>]) --> nRecordedByte
 */
int
clip_STRFILE(ClipMachine * ClipMachineMemory)	/* Writes a string to a file */
{
   const char *str = _clip_parc(ClipMachineMemory, 1);

   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2));

   int lowr = _clip_parl(ClipMachineMemory, 3);

   long n = 0, noff = _clip_parnl(ClipMachineMemory, 4);

   int lcut = _clip_parl(ClipMachineMemory, 5);

   int fd = -1, flags = O_WRONLY | O_CREAT;

   mode_t mode = ClipMachineMemory->fileCreateMode;

   int *fattr = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);

   if (lowr == 0)		/* create new file */
   {
      flags |= O_CREAT;
      unlink(uname);
   }
   else if (ClipMachineMemory->argc <= 3)
   {				/* nOffset - undefined */
      flags |= O_APPEND;
      noff = 0;
   }

   if (fattr && (*fattr & FA_READONLY))
      mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);

   if ((fd = open(uname, flags, mode)) < 0)
      goto end;

   if (ClipMachineMemory->argc > 3)
   {				/* nOffset - defined */
      if (lseek(fd, noff, SEEK_SET) < 0)
	 goto end;
   }

   n = write(fd, str, strlen(str));

 end:
   _clip_retnl(ClipMachineMemory, n);

   if (lcut)
      ftruncate(fd, noff + n);
   if (fd >= 0)
      close(fd);

   if (uname)
      free(uname);

   return 0;
}
