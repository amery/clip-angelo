int
clip_FFLUSH(ClipMachine * ClipMachineMemory)
{
   int ret = 0, fd = _clip_parni(ClipMachineMemory, 1);

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL)
      *err = EBADF;
   else
   {
      *err = 0;
      if (cf->file_of_C_FILE != NULL && (cf->stat_of_C_FILE & FS_BUFFERED) != 0)
      {
	 if (fflush(cf->file_of_C_FILE) == 0)
	    ret = 1;
	 else
	    *err = errno;
      }
      else
	 ret = 1;
   }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
