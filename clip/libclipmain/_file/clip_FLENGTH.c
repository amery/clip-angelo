int
clip_FLENGTH(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1), *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   long ret = -1;

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   struct stat st;

   if (cf == NULL)
      *err = EBADF;
   else
   {
      if (fstat(cf->fileno_of_C_FILE, &st) == 0)
	 ret = st.st_size;
      else
	 *err = errno;
   }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
