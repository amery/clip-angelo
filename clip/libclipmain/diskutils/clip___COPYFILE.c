int
clip___COPYFILE(ClipMachine * ClipMachineMemory)
{
   char *src = _clip_parc(ClipMachineMemory, 1);

   char *dst = _clip_parc(ClipMachineMemory, 2);

   char *usrc = _get_unix_name(ClipMachineMemory, src);

   char *udst = _get_unix_name(ClipMachineMemory, dst);

   int fdsrc = -1, fddst = -1;

   FILE *fsrc = NULL;

   FILE *fdst = NULL;

   int c = 0, r = 0, ret = 0;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   const char *funcname = "__COPYFILE";

   *err = 0;
   if (src == NULL || dst == NULL || *src == 0 || *dst == 0 || usrc == NULL || udst == NULL)
   {
      _clip_retni(ClipMachineMemory, ret);
      r = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "invalid argument");
      goto end;
   }

   if ((fdsrc = open(usrc, O_RDONLY)) < 0)
      goto end;
   else if ((fddst = creat(udst, ClipMachineMemory->fileCreateMode)) < 0)
   {
      ret = 0;
      r = _clip_trap_err(ClipMachineMemory, EG_OPEN, 0, 0, __FILE__, __LINE__, funcname);
      *err = errno;
      goto end;
   }
   if (!_set_lock(fdsrc, F_RDLCK) /*|| !_set_lock(fddst, F_WRLCK) */ )
   {
      ret = 0;
      r = _clip_trap_err(ClipMachineMemory, EG_LOCK, 0, 0, __FILE__, __LINE__, funcname);
      *err = errno;
      goto end;
   }

   ftruncate(fddst, 0);

   if ((fsrc = fdopen(fdsrc, "r")) == NULL || (fdst = fdopen(fddst, "w")) == NULL)
   {
      ret = 0;
      r = _clip_trap_err(ClipMachineMemory, EG_OPEN, 0, 0, __FILE__, __LINE__, funcname);
      *err = errno;
      goto end;
   }

   while ((c = fgetc(fsrc)) != EOF)
   {
      if (fputc(c, fdst) == EOF)
      {
	 *err = errno;
	 r = _clip_trap_err(ClipMachineMemory, EG_WRITE, 0, 0, __FILE__, __LINE__, "__COPYFILE");
	 break;
      }
      ret++;
   }

 end:
   _clip_retni(ClipMachineMemory, ret);

   if (fsrc)
      fclose(fsrc);
   else if (fdsrc >= 0)
      close(fdsrc);

   if (fdst)
      fclose(fdst);
   else if (fddst >= 0)
      close(fddst);

   if (usrc)
      free(usrc);

   if (udst)
      free(udst);

   return r;
}
