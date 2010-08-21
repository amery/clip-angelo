int
clip_FWRITE(ClipMachine * ClipMachineMemory)
{
#if 0
   int ret, sl, *err;

   int h = _clip_parni(ClipMachineMemory, 1);

   char *str = _clip_parcl(ClipMachineMemory, 2, &sl);

   int kol = _clip_parni(ClipMachineMemory, 3);

   if (kol <= 0)
      kol = sl;
   if (sl < kol)
      kol = sl;

   ret = write(h, str, kol);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (ret < 0)
      *err = errno;
   else
      *err = 0;
   _clip_retni(ClipMachineMemory, ret);
#else
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1), len = 0;

   char *buf = _clip_parcl(ClipMachineMemory, 2, &len);

   long ret = -1, n = _clip_parnl(ClipMachineMemory, 3);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      fd = 1;

   cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);
   if (cf == NULL)
      *err = EBADF;
   else
   {
      if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
	 n = len;
      if (n > len)
	 n = len;

      if (n <= 0)
	 ret = 0;
      else
      {
	 if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
	    ret = _clip_write(cf, buf, n);
	 else
	    ret = fwrite(buf, 1, n, cf->file_of_C_FILE);
      }
      *err = ret < 0 ? errno : 0;
   }
   _clip_retnl(ClipMachineMemory, ret);
#endif
   return 0;
}
