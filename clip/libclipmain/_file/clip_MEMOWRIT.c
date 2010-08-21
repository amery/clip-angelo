int
clip_MEMOWRIT(ClipMachine * ClipMachineMemory)
{
   mode_t mode = ClipMachineMemory->fileCreateMode;

   char buf[PATH_MAX];

   int len, fh = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char *fname = _clip_parc(ClipMachineMemory, 1);

   char *str = _clip_parcl(ClipMachineMemory, 2, &len);

   if (fname == NULL || str == NULL)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MEMOWRIT");
   }
   _clip_translate_path(ClipMachineMemory, fname, buf, sizeof(buf));

   fh = _clip_creat(ClipMachineMemory, buf, O_WRONLY, mode, 1);
   if (fh < 0)
   {
      *err = errno;
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   else
   {
      *err = 0;
   }
   len = write(fh, str, len);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (len < 0)
      *err = errno;
   else
      *err = 0;
   _clip_close(ClipMachineMemory, _clip_hashstr(buf), fh);
   close(fh);
   if (len < 0)
   {
      *err = errno;
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   _clip_retl(ClipMachineMemory, 1);
   return 0;
}
