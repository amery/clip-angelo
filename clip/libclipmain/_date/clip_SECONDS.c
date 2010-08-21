int
clip_SECONDS(ClipMachine * ClipMachineMemory)
{
   _clip_retndp(ClipMachineMemory, _clip_seconds(), 10, 2);
   return 0;
}
