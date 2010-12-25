int
clip_FWRITELN(ClipMachine * ClipMachineMemory)
{
#if 0
   int ret = -1, sl, *err;

   int h = _clip_parni(ClipMachineMemory, 1);

   char *str = _clip_parcl(ClipMachineMemory, 2, &sl);

   int kol = _clip_parni(ClipMachineMemory, 3);

   if (sl < kol)
      kol = sl;

   ret = write(h, str, kol);
   ret += write(h, "\n", 1);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (ret < 0)
      *err = errno;
   else
      *err = 0;
#else
   int ret = -1, len = 0, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1);

   char *buf = _clip_parcl(ClipMachineMemory, 2, &len);

   int n = _clip_parni(ClipMachineMemory, 3);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL)
      *err = EBADF;
   else
   {
      if (n <= 0)
	 n = len;
      if (len < n)
	 n = len;

      if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
      {
	 ret = _clip_write(cf, buf, n);
	 if (ret == n)
	    if (_clip_write(cf, "\n", 1) == 1)
	       ++ret;
      }
      else
      {
	 ret = fwrite(buf, 1, n, cf->file_of_C_FILE);
	 if (ret == n)
	    if (fwrite("\n", 1, 1, cf->file_of_C_FILE) == 1)
	       ++ret;
      }
      *err = ret < 0 ? errno : 0;
   }
#endif
   _clip_retni(ClipMachineMemory, ret);

   return 0;
}
