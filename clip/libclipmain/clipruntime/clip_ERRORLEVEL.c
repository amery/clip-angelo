int
clip_ERRORLEVEL(ClipMachine * ClipMachineMemory)
{
   int level = errorlevel;

   if (ClipMachineMemory->argc > 0)
      errorlevel = _clip_parni(ClipMachineMemory, 1);

   _clip_retni(ClipMachineMemory, level);
   return 0;
}
