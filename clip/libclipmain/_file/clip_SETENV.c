int
clip_SETENV(ClipMachine * ClipMachineMemory)
{
   char *name = _clip_parc(ClipMachineMemory, 1);

   char *val = _clip_parc(ClipMachineMemory, 2);

   _clip_retl(ClipMachineMemory, 0);
   if (!name)
      return 0;

   _clip_put_env(name, val);
   if (val)
      _clip_retl(ClipMachineMemory, setenv(name, val, 1) == 0);
   else
   {
      _clip_retl(ClipMachineMemory, 1);
      unsetenv(name);
   }
   return 0;
}
