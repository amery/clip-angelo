int
clip_FSEEK(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1);

   long nb = _clip_parnl(ClipMachineMemory, 2);

   int whence = SEEK_SET, w = _clip_parni(ClipMachineMemory, 3), *err, ret = -1;

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
   *err = 0;
   _clip_retnl(ClipMachineMemory, -1);

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

   switch (w)
   {
   case 0:
      whence = SEEK_SET;
      break;
   case 1:
      whence = SEEK_CUR;
      break;
   case 2:
      whence = SEEK_END;
      break;
   }
   if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
   {
      if ((ret = lseek(cf->fileno_of_C_FILE, nb, whence)) == -1)
	 *err = errno;
   }
   else
   {
      if ((ret = fseek(cf->file_of_C_FILE, nb, whence)) == -1)
	 *err = errno;
      else
	 ret = ftell(cf->file_of_C_FILE);
   }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
