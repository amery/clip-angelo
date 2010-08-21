int
clip_SECOND(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, _clip_seconds(), 10, 2);
   return 0;
}
