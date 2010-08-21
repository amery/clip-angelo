int
clip_HOST_CHARSET(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_hostcs);
   return 0;
}
