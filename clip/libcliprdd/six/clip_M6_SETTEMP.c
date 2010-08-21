int
clip_M6_SETTEMP(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   _clip_retc(ClipMachineMemory, ".");
   return 0;
}
