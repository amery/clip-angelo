int
clip_LASTKEY(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, ClipMachineMemory->lastkey, 10, 0);
   return 0;
}
