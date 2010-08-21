int
clip_PUTENV(ClipMachine * ClipMachineMemory)
{
   char *name = _clip_parc(ClipMachineMemory, 1);

   char *val = _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 0);
   if (name)
      _clip_retl(ClipMachineMemory, _clip_put_env(name, val) == 0);
   return 0;
}
