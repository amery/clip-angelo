int
clip_PID(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, getpid());
   return 0;
}
