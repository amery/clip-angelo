int
clip_FTRUNC(ClipMachine * ClipMachineMemory)
{
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1), fno = -1;

   long len = _clip_parnl(ClipMachineMemory, 2);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL)
   {
      *err = EBADF;
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }

   if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
      fno = cf->fileno_of_C_FILE;
   else
      fno = fileno(cf->file_of_C_FILE);

   if (fno == -1)
   {
      *err = EBADF;
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }

   if (_clip_parinfo(ClipMachineMemory, 0) < 2)
      len = lseek(fno, 0, SEEK_CUR);

   //if ( len > 0 )
   len = ftruncate(fno, len);

   *err = len < 0 ? errno : 0;

   _clip_retl(ClipMachineMemory, len >= 0);
   return 0;
}
