int
clip_PARSEFILENAME(ClipMachine * ClipMachineMemory)
{
   char *name;

   char path[PATH_MAX];

   char *fullpath = _clip_parc(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 1) != 1)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "PARSEFILENAME");
   }
   name = malloc(PATH_MAX);

   _clip_parse_path(fullpath, path, name);
   _clip_retcn_m(ClipMachineMemory, name, strlen(name));
   return 0;
}
