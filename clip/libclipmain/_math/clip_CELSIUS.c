int
clip_CELSIUS(ClipMachine * ClipMachineMemory)
{
   _clip_retnd(ClipMachineMemory, (_clip_parnd(ClipMachineMemory, 1) - 32) * 5 / 9);
   return 0;
}
