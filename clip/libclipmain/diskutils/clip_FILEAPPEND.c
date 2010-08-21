int
clip_FILEAPPEND(ClipMachine * ClipMachineMemory)	/* Appends data to a file */
{
   char *src = _clip_parc(ClipMachineMemory, 1);

   char *dst = _clip_parc(ClipMachineMemory, 2);

   char *usrc = _get_unix_name(ClipMachineMemory, src);

   char *udst = _get_unix_name(ClipMachineMemory, dst);

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int *fattr = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);

   mode_t mode = ClipMachineMemory->fileCreateMode;

   int fdsrc = -1, fddst = -1;

   FILE *fsrc = NULL;

   FILE *fdst = NULL;

   int c = 0;

   off_t i = 0;

   struct stat st;

   *err = 0;

   if (src == NULL || dst == NULL || *src == 0 || *dst == 0 || usrc == NULL || udst == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
      return 1;
   }

   if (fattr && !(*fattr & FA_READONLY))
   {
#ifdef OS_MINGW
      mode |= S_IWUSR;
#else
      mode |= S_IWUSR | S_IWGRP | S_IWOTH;
#endif
   }

   if ((fdsrc = open(usrc, O_RDONLY)) < 0)
   {
    err:
      _clip_retni(ClipMachineMemory, 0);
      *err = errno;
      goto end;
   }

   if (!_set_lock(fdsrc, F_RDLCK) || fstat(fdsrc, &st))
   {
    err1:
      close(fdsrc);
      goto err;
   }

   if (access(udst, F_OK))
   {
      if ((fddst = creat(udst, mode)) < 0)
	 goto err1;
   }
   else
   {
      if ((fddst = open(udst, O_WRONLY)) < 0)
	 goto err1;
      if (!_set_lock(fddst, F_WRLCK))
      {
       err2:
	 close(fddst);
	 goto err1;
      }
   }

   if (!(fsrc = fdopen(fdsrc, "r")))
   {
      goto err2;
   }
   if (!(fdst = fdopen(fddst, "a")))
   {
      close(fddst);
      _clip_retni(ClipMachineMemory, 0);
      *err = errno;
      goto end;
   }

   for (i = 0; (c = fgetc(fsrc)) != EOF && i < st.st_size; i++)
   {
      if (fputc(c, fdst) == EOF)
      {
	 *err = errno;
	 break;
      }
   }
   _clip_retnd(ClipMachineMemory, i);
 end:
   if (fsrc)
      fclose(fsrc);
   if (fdst)
      fclose(fdst);
   free(usrc);
   free(udst);
   return 0;
}
