int
clip_GLOB(ClipMachine * ClipMachineMemory)
{
   char *str = _clip_parc(ClipMachineMemory, 1);

   char *pattern = _clip_parc(ClipMachineMemory, 2);

   int caseflag = _clip_parl(ClipMachineMemory, 3);

   if (!str || !*pattern)
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, _clip_glob_match(str, pattern, caseflag) >= 0);

   return 0;
}
