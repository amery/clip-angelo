int
clip_PARSEFILEPATH(ClipMachine * ClipMachineMemory)
{
   char *path;

   char name[PATH_MAX];

   char *fullpath = _clip_parc(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 1) != 1)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "PARSEFILEPATH");
   }
   path = malloc(PATH_MAX);

   _clip_parse_path(fullpath, path, name);
   _clip_retcn_m(ClipMachineMemory, path, strlen(path));
   return 0;
}
