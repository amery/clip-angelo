int
clip_FTELL(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1), *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   long ret = -1;

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   _clip_retni(ClipMachineMemory, ret);
   if (cf == NULL)
      *err = EBADF;
   else
   {
      if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
	 ret = lseek(cf->fileno_of_C_FILE, 0, SEEK_CUR);
      else
	 ret = ftell(cf->file_of_C_FILE);

      *err = ret < 0 ? errno : 0;
   }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
