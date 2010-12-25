int
clip_CHMOD(ClipMachine * ClipMachineMemory)
{
   int lp = 0, *err = NULL;

   char buf[PATH_MAX];

   char *fname = _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 1);
   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
   *err = 0;

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMachineMemory, 2));
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      lp = _clip_parni(ClipMachineMemory, 2);
   if (fname == NULL || lp == 0)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHMOD");
   }

   _clip_translate_path(ClipMachineMemory, fname, buf, sizeof(buf));
   if (chmod(buf, lp) == 0)
      return 0;

   _clip_retl(ClipMachineMemory, 0);
   *err = errno;

   return 0;
}
