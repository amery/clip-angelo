int
clip_FREAD(ClipMachine * ClipMachineMemory)
{
#if 0
   int ret, *err, bl;

   int fh = _clip_parni(ClipMachineMemory, 1);

   char *buf = _clip_parcl(ClipMachineMemory, 2, &bl);

   int kol = _clip_parni(ClipMachineMemory, 3);

   if (bl < kol)
      kol = bl;

   ret = read(fh, buf, kol);
   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (ret < 0)
      *err = errno;
   else
      *err = 0;
#else
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1), lenbuf = 0;

   char *buf = _clip_parcl(ClipMachineMemory, 2, &lenbuf);

   long ret = -1, nb = _clip_parnl(ClipMachineMemory, 3);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   _clip_retnl(ClipMachineMemory, -1);

   if (cf == NULL)
   {
      *err = EBADF;
      return 0;
   }

   if (lenbuf < nb)
      nb = lenbuf;

   if (nb > 0)
   {
      if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
	 ret = _clip_read(cf, buf, nb);
      else
	 ret = fread(buf, 1, nb, cf->file_of_C_FILE);
   }
   else
      ret = 0;

   *err = ret < 0 ? errno : 0;
#endif

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
