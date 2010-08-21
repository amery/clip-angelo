int
clip_FREADSTR(ClipMachine * ClipMachineMemory)
{
#if 0
   int *err;

   int fh = _clip_parni(ClipMachineMemory, 1);

   int kol = _clip_parni(ClipMachineMemory, 2);

   char *buf = malloc(kol + 1);

   int ret = read(fh, buf, kol);

   if (ret >= 0)
   {
      buf[ret] = 0;
      _clip_retcn_m(ClipMachineMemory, buf, ret);
   }
   else
      _clip_retc(ClipMachineMemory, "");

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (ret < 0)
      *err = errno;
   else
      *err = 0;
#else
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1);

   long ret = -1, nb = _clip_parnl(ClipMachineMemory, 2);

   char *buf;

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   _clip_retc(ClipMachineMemory, "");

   if (cf == NULL)
   {
      *err = EBADF;
      return 0;
   }

   buf = malloc(nb + 1);
   if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
      ret = _clip_read(cf, buf, nb);
   else
      ret = fread(buf, 1, nb, cf->file_of_C_FILE);

   if (ret >= 0)
   {
      buf[ret] = 0;
      _clip_retc(ClipMachineMemory, buf);
   }
   free(buf);

   *err = ret < 0 ? errno : 0;
#endif
   return 0;
}
