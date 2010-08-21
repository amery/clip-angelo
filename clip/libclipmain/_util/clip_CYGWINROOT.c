int
clip_CYGWINROOT(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_cygwinroot());
   return 0;
}
