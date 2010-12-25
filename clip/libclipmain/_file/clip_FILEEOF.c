int
clip_FILEEOF(ClipMachine * ClipMachineMemory)
{
   int ret = 1, fd = _clip_parni(ClipMachineMemory, 1);

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   *err = 0;
   if (cf == NULL)
      *err = EBADF;
   else if (cf->type_of_C_FILE == FT_FIFO || cf->type_of_C_FILE == FT_SOCKET)
      ret = (cf->stat_of_C_FILE & FS_EOF) ? 1 : 0;
   else
   {
      if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
      {
	 int i, j;

	 i = lseek(cf->fileno_of_C_FILE, 0, SEEK_CUR);
	 j = lseek(cf->fileno_of_C_FILE, 0, SEEK_END);
	 ret = (i >= j);
	 lseek(cf->fileno_of_C_FILE, i, SEEK_SET);
      }
      else
	 ret = feof(cf->file_of_C_FILE) ? 1 : 0;
   }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
