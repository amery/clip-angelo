int
clip_FCREATE(ClipMachine * ClipMachineMemory)
{
   FILE *f = NULL;

   C_FILE *cf = NULL;

   int fd = -1, ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char buf[PATH_MAX];

   char *fname = _clip_parc(ClipMachineMemory, 1);

   int share_mode = _clip_parni(ClipMachineMemory, 2);

   int lbuf;			/*, lposix; */

   /* TODO: How to create file without locking?
      We need to extend last param in _clip_creat for new mode
    */
   /*
      lposix = ((share_mode & FO_POSIX) != 0 ||
      (FOPENMODE_FLAG & ClipMachineMemory->flags1) != 0);
    */

   if ((share_mode & FO_NOBUFF) != 0)
      lbuf = 1;			/*((share_mode & FO_NOBUFF) != 0); */
   else
   {
      if ((share_mode & FO_BUFFERED) != 0)
	 lbuf = 0;		/*((share_mode & FO_BUFFERED) != 0); */
      else
	 lbuf = ((BUFFERING_FLAG & ClipMachineMemory->flags1) != 0);
   }

   *err = 0;

   if (fname == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "FCREATE");
   }
   _clip_translate_path(ClipMachineMemory, fname, buf, sizeof(buf));

   if ((fd = _clip_creat(ClipMachineMemory, buf, O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode, 1)) != -1)
   {
      if (!lbuf || (f = fdopen(fd, "r+")) != NULL)
      {
	 cf = (C_FILE *) calloc(1, sizeof(C_FILE));
	 cf->type_of_C_FILE = _clip_ftype(fd);
	 cf->fileno_of_C_FILE = fd;
	 cf->file_of_C_FILE = f;
	 cf->stat_of_C_FILE = (lbuf ? FS_BUFFERED : 0);
	 cf->pid_of_C_FILE = -1;
	 cf->timeout_of_C_FILE = 0;
	 cf->hash_of_C_FILE = _clip_hashstr(buf);
	 cf->ClipMachineMemory_of_C_FILE = ClipMachineMemory;
	 ret = _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);
      }
   }
   if (ret == -1)
   {
      *err = errno;
      if (fd != -1)
	 close(fd);
   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
