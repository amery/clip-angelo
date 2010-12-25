int
clip_LIKE(ClipMachine * ClipMachineMemory)
{
   int sl, l;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l);

   if (str == 0 || s == 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "LIKE");
   }
   _clip_retl(ClipMachineMemory, _clip_glob_match((char *) str, (char *) s, ClipMachineMemory->flags & TRANSLATE_FLAG) > 0);
   return 0;
}
