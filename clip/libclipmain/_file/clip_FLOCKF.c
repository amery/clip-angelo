int
clip_FLOCKF(ClipMachine * ClipMachineMemory)
{
   /*
      ( nFhandle, nLckType, nSize ) -> nRet
      where:
      nLckType
      0    FF_UNLCK        Unlock the whole file
      1    FF_RDLCK        Lock for reading by currrent process,
      prevents others from writing
      2    FF_WRLCK        Lock for writing by current porcess,
      prevents others from reading

      nSize
      is a positive byte count to be locked from the current position.
      If omitted, or zero is specified, the rest of the file is locked
      until end-of-file.

      nRet
      is set to zero, if the required locking is successful,
      nonzero otherwise

      (this syntax of FLOCKF is FlagShip compatible)
    */

   int ret, l_type = 0, l_cmd = 0;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   struct flock fl;

   int fd = _clip_parni(ClipMachineMemory, 1);

   int type = _clip_parni(ClipMachineMemory, 2);

   long size = _clip_parnl(ClipMachineMemory, 3);

   C_FILE *cf;

   _clip_retni(ClipMachineMemory, -1);
   if (_clip_parinfo(ClipMachineMemory, 0) < 2)
   {
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "FLOCKF");
   }

   cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL)
   {
      *err = EBADF;
      return 0;
   }
   else if (cf->type_of_C_FILE == FT_FIFO || cf->type_of_C_FILE == FT_SOCKET)
   {
      *err = ESPIPE;
      return 0;
   }

   l_cmd = (type == 0 ? F_UNLCK : F_SETLK);
   l_type = (type == 1 ? F_RDLCK : F_WRLCK);
   if (size == 0)
      size = 0xFFFFFFFF;

   fl.l_type = l_type;
   fl.l_whence = SEEK_SET;
   fl.l_start = lseek(cf->fileno_of_C_FILE, 0, SEEK_CUR);
   fl.l_len = size;
#ifndef OS_MINGW
   fl.l_pid = 0;
#endif
   ret = fcntl(cf->fileno_of_C_FILE, l_cmd, &fl);

   *err = ret == -1 ? errno : 0;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
