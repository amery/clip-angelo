int
clip_FRENAME(ClipMachine * ClipMachineMemory)
{
   int ret, *err;

   char buf1[PATH_MAX];

   char buf2[PATH_MAX];

   char *fname1 = _clip_parc(ClipMachineMemory, 1);

   char *fname2 = _clip_parc(ClipMachineMemory, 2);

   if (fname1 == NULL || fname2 == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "FRENAME");
   }
   _clip_translate_path(ClipMachineMemory, fname1, buf1, sizeof(buf1));
   _clip_translate_path(ClipMachineMemory, fname2, buf2, sizeof(buf1));
   ret = rename(buf1, buf2);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (ret < 0)
      *err = errno;
   else
      *err = 0;

   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
