int
clip_LTOC(ClipMachine * ClipMachineMemory)
{
   if (_clip_parl(ClipMachineMemory, 1))
      _clip_retc(ClipMachineMemory, "T");
   else
      _clip_retc(ClipMachineMemory, "F");
   return 0;
}
